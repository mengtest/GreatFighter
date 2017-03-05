#include "Common/IGHeader.h"
#include "IGTipsLayer.h"


IGTipsLayer::IGTipsLayer()
{
}


IGTipsLayer::~IGTipsLayer()
{
}

bool IGTipsLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::createNode("TipsLayer.csb");
	addChild(rootNode);

	// auto layer = static_cast<ui::Layout*>(rootNode->getChildByName("Layer"));
	m_tipsText = static_cast<ui::Text*>(rootNode->getChildByName("TipsText"));

	auto visibleSize = Director::getInstance()->getVisibleSize();
	m_tipsText->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

	return true;
}

void IGTipsLayer::onEnter()
{
	Layer::onEnter();

	loadConfig();
	setCascadeOpacityEnabled(true);
}

void IGTipsLayer::loadConfig()
{
	auto jsonString = FileUtils::getInstance()->getStringFromFile("msgcode.json");
	cJSON* root = cJSON_Parse(jsonString.c_str());
	cJSON* msgArray = cJSON_GetObjectItem(root, "msgcodeArray");
	cJSON* defaultDurationObj = cJSON_GetObjectItem(root, "defaultDuration");

	assert(msgArray);
	assert(defaultDurationObj);

	m_defaultDuration = defaultDurationObj->valueint;

	cJSON* queryItem = msgArray->child;
	while (queryItem)
	{
		cJSON* msgcodeObj = cJSON_GetObjectItem(queryItem, "code");
		assert(msgcodeObj);

		cJSON* tipsObj = cJSON_GetObjectItem(queryItem, "msg");
		assert(tipsObj);

		cJSON* durationObj = cJSON_GetObjectItem(queryItem, "duration");

		IGTipsMessage tipsMsg;
		tipsMsg.msg = string(tipsObj->valuestring);

		if (durationObj)
		{
			tipsMsg.duration = durationObj->valueint;
		}
		else
		{
			tipsMsg.duration = m_defaultDuration;
		}

		m_errorMsg[msgcodeObj->valueint] = tipsMsg;
		queryItem = queryItem->next;
	}

	cJSON_Delete(root);
}

bool IGTipsLayer::showTips(int msgcode)
{
	auto tipsMsgIter = m_errorMsg.find(msgcode);
	if (tipsMsgIter == m_errorMsg.end())
	{
		return false;
	}

	int duration = tipsMsgIter->second.duration;
	if (duration == 0)
	{
		duration = INT_MAX;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	m_tipsText->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	m_tipsText->setVisible(true);
	m_tipsText->setScale(1.0f);
	m_tipsText->setText(tipsMsgIter->second.msg);
	m_tipsText->stopAllActions();

	auto sequenceAction = Sequence::create(
		ScaleTo::create(0.1f, 1.3f),
		ScaleTo::create(0.3f, 1.0f),
		DelayTime::create(duration),
		CallFunc::create(CC_CALLBACK_0(IGTipsLayer::hideTips, this)),
		nullptr);

	m_tipsText->runAction(sequenceAction);

	return true;
}

void IGTipsLayer::hideTips()
{
	m_tipsText->setVisible(false);
}
