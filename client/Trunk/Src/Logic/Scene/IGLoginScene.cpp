#include "Common/IGHeader.h"
#include "Logic/EventListener/IGLoginSceneEventListener.h"
#include "Logic/UI/Login/IGLoginUI.h"
#include "Logic/System/LoginManager/IGLoginManager.h"
#include "IGLoginScene.h"

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

	m_eventListener.requestRegisterAccount = std::bind(&IGLoginScene::onRequestRegisterAccount, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	m_eventListener.onRegisterAccountNotify = std::bind(&IGLoginScene::onRegisterAccountNotify, this, std::placeholders::_1);
	m_eventListener.requestCaptcha = std::bind(&IGLoginScene::onRequestCaptcha, this);
	m_eventListener.onRequestCaptchaNotify = std::bind(&IGLoginScene::onRequestCaptchaNotify, this, std::placeholders::_1, std::placeholders::_2);

	m_loginUI->registerEventListener(m_eventListener);
	m_loginManager->onEnter(m_eventListener);
}

void IGLoginScene::onExit()
{
	Scene::onExit();

	m_loginManager->onExit();
}

void IGLoginScene::onRequestRegisterAccount(const string& userName, const string& pwd, const string& captcha)
{
	m_loginManager->registerAccount(userName, pwd, captcha);
}

void IGLoginScene::onRegisterAccountNotify(int msgcode)
{
	if (msgcode == 0)
	{
		m_loginUI->switchTo(LSPanelType::LoginPanel);
	}
	else
	{

	}
}

void IGLoginScene::onRequestCaptcha()
{
	m_loginManager->requestCaptcha();
}

void IGLoginScene::onRequestCaptchaNotify(int msgcode, const string& imageString)
{
	if (msgcode == 0)
	{
		m_loginUI->refreshCaptcha(imageString);
	}
	else
	{

	}
}
