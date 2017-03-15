#include "Common/IGHeader.h"
#include "Common/IGConst.h"
#include "Protocol/IGProtoManager.h"
#include "Logic/EventListener/IGMainSceneEventListener.h"
#include "Logic/System/Player/IGPlayer.h"
#include "Logic/UI/IGTipsLayer.h"
#include "Logic/UI/Main/IGMainUI.h"
#include "IGMainScene.h"


IGMainScene::IGMainScene()
{
}


IGMainScene::~IGMainScene()
{
}

cocos2d::Scene* IGMainScene::createScene(const string& user, const string& secret, const string& ip, int port)
{
	auto scene = Scene::create();
	auto layer = IGMainScene::create();
	scene->addChild(layer);

	layer->assignServerInfo(user, secret, ip, port);

	return scene;
}

bool IGMainScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	m_mainUI = IGMainUI::create();
	addChild(m_mainUI);

	m_player = IGPlayer::getInstance();

	return true;
}

void IGMainScene::assignServerInfo(const string& user, const string& secret, const string& ip, int port)
{
	m_player->initServerInfo(user, secret, ip, port);
}

void IGMainScene::onEnter()
{
	Scene::onEnter();

	m_eventListener.onError = std::bind(&IGMainScene::onTips, this, std::placeholders::_1);
	m_eventListener.onAppShutDown = std::bind(&IGMainScene::onAppShutDown, this);

	m_player->registerEventListener(m_eventListener);
	m_mainUI->registerEventListener(m_eventListener);

	m_player->connectToServer();
	m_mainUI->showTips((int)IGMsgcode::Connecting);

	scheduleUpdate();
}

void IGMainScene::onExit()
{
	Scene::onExit();
}

void IGMainScene::update(float delta)
{
	m_player->update((int)(delta * 1000));
}

void IGMainScene::onTips(int msgcode)
{
	m_mainUI->showTips(msgcode);
}

void IGMainScene::onAppShutDown()
{
	Director::getInstance()->end();
}