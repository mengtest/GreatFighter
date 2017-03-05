#include "Common/IGHeader.h"
#include "Logic/EventListener/IGLoginSceneEventListener.h"
#include "Logic/UI/IGTipsLayer.h"
#include "Logic/UI/Login/IGLoginUI.h"
#include "Logic/System/LoginManager/IGLoginManager.h"
#include "IGLoginScene.h"
#include "Common/IGConst.h"

using namespace cocostudio::timeline;


IGLoginScene::~IGLoginScene()
{
	delete m_loginManager;
	m_loginManager = nullptr;
}

Scene* IGLoginScene::createScene()
{
    auto scene = Scene::create();

    auto layer = IGLoginScene::create();

    scene->addChild(layer);

    return scene;
}

bool IGLoginScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

	m_loginUI = IGLoginUI::create();
	addChild(m_loginUI);

	m_loginManager = new IGLoginManager();

    return true;
}

void IGLoginScene::onEnter()
{
	Scene::onEnter();

	m_eventListener.requestRegisterAccount = std::bind(&IGLoginScene::doRequestRegisterAccount, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_eventListener.onRegisterAccount = std::bind(&IGLoginScene::onRequestRegisterAccount, this, std::placeholders::_1);
	m_eventListener.onRegisterAccountNotify = std::bind(&IGLoginScene::onRegisterAccountNotify, this, std::placeholders::_1);

	m_eventListener.requestCaptcha = std::bind(&IGLoginScene::doRequestCaptcha, this);
	m_eventListener.onRequestCaptchaNotify = std::bind(&IGLoginScene::onRequestCaptchaNotify, this, std::placeholders::_1, std::placeholders::_2);

	m_eventListener.requestLogin = std::bind(&IGLoginScene::doRequestLogin, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_eventListener.onRequestLogin = std::bind(&IGLoginScene::onRequestLogin, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, std::placeholders::_4);

	m_eventListener.onError = std::bind(&IGLoginScene::onError, this, std::placeholders::_1);

	m_loginUI->registerEventListener(m_eventListener);
	m_loginUI->showTips((int)IGMsgcode::Connecting);

	m_loginManager->onEnter(m_eventListener);

	scheduleUpdate();
}

void IGLoginScene::onExit()
{
	Scene::onExit();

	m_loginManager->onExit();
}

void IGLoginScene::update(float delta)
{
	m_loginManager->update((int)delta);
}

void IGLoginScene::doRequestRegisterAccount(const string& userName, const string& pwd, const string& captcha)
{
	if (!m_loginManager->doRegisterAccount(userName, pwd, captcha))
	{
		m_loginUI->showTips((int)IGMsgcode::LostConnectToServer);
	}
}

void IGLoginScene::onRequestRegisterAccount(int msgcode)
{
	m_loginUI->showTips(msgcode);
}

void IGLoginScene::onRegisterAccountNotify(int msgcode)
{
	if (msgcode == 0)
	{
		m_loginUI->switchTo(LSPanelType::LoginPanel);
	}
	else
	{
		m_loginUI->showTips(msgcode);
	}
}

void IGLoginScene::doRequestCaptcha()
{
	if (!m_loginManager->doRequestCaptcha())
	{
		m_loginUI->showTips((int)IGMsgcode::LostConnectToServer);
	}
}

void IGLoginScene::onRequestCaptchaNotify(int msgcode, const string& imageString)
{
	if (msgcode == 0)
	{
		m_loginUI->refreshCaptcha(imageString);
	}
	else
	{
		m_loginUI->showTips(msgcode);
	}
}

void IGLoginScene::doRequestLogin(const string& userName, const string& pwd, const string& captcha)
{
	if (!m_loginManager->doLogin(userName, pwd, captcha))
	{
		m_loginUI->showTips((int)IGMsgcode::LostConnectToServer);
	}
}

void IGLoginScene::onRequestLogin(int msgcode, const string& user, const string& secret, const string& ip)
{
	if (msgcode == 0)
	{

	}
	else
	{
		m_loginUI->showTips(msgcode);
	}
}

void IGLoginScene::onError(int msgcode)
{
	m_loginUI->showTips(msgcode);
}
