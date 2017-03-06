#include "Common/IGHeader.h"
#include "Common/IGConst.h"
#include "Logic/UI/IGTipsLayer.h"
#include "Logic/EventListener/IGMainSceneEventListener.h"
#include "IGMainUI.h"


IGMainUI::IGMainUI()
{
}


IGMainUI::~IGMainUI()
{
}

bool IGMainUI::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::createNode("MainUI.csb");
	addChild(rootNode);

	m_exitButton = static_cast<Button*>(rootNode->getChildByName("ExitButton"));

	m_tipsLayer = IGTipsLayer::create();
	addChild(m_tipsLayer);

	return true;
}

void IGMainUI::onEnter()
{
	Layer::onEnter();

	m_exitButton->addTouchEventListener(CC_CALLBACK_2(IGMainUI::onExitButtonTouched, this));
}

void IGMainUI::registerEventListener(const IGMainSceneEventListener& eventListener)
{
	m_eventListener = eventListener;
}

void IGMainUI::showTips(int msgcode)
{
	m_tipsLayer->showTips(msgcode);
}

void IGMainUI::onExitButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType)
{
	if (touchType == ui::Widget::TouchEventType::ENDED)
	{
		m_eventListener.onAppShutDown();
	}
}
