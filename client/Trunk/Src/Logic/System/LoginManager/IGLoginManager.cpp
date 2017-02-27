#include "Common/IGHeader.h"
#include "Common/IGConst.h"
#include "Protocol/IGProtoManager.h"
#include "Logic/EventListener/IGLoginSceneEventListener.h"
#include "IGLoginManager.h"
#include "Protocol/IIGProtoHelper.h"
#include "Protocol/Client/IGProtoRequestCaptcha.h"
#include "Protocol/Client/IGProtoRegisterAccount.h"
#include "Protocol/Server/IGProtoRegisterAccountNotify.h"


IGLoginManager::IGLoginManager()
{
	init();
}


IGLoginManager::~IGLoginManager()
{
	uninit();
}

bool IGLoginManager::init()
{
	m_protoManager = IGProtoManager::getInstance();
	return true;
}

void IGLoginManager::uninit()
{
	m_protoManager = nullptr;
}

void IGLoginManager::onEnter(const IGLoginSceneEventListener& eventListener)
{
	m_eventListener = eventListener;

	m_protoManager->registerClientProto(IGClientProtoType::RequestCaptcha, std::bind(&IGLoginManager::onRecvCaptcha, this, std::placeholders::_1));
	m_protoManager->registerClientProto(IGClientProtoType::RegisterAccount);
	m_protoManager->registerClientProto(IGClientProtoType::Login, std::bind(&IGLoginManager::onLogin, this, std::placeholders::_1));

	m_protoManager->registerServerNotify(IGServerNotifyType::RegisterAccount, std::bind(&IGLoginManager::onRegisterAccount, this, std::placeholders::_1));

	// 暂时写死,以后要放在配置表中配置
	m_protoManager->startNetwork("192.168.1.103", 8888);
}

void IGLoginManager::onExit()
{
	m_protoManager->stopNetwork();
}

void IGLoginManager::update(int interval)
{

}

void IGLoginManager::requestCaptcha()
{

}

void IGLoginManager::onRecvCaptcha(const void* data)
{
	IGCaptcha* realData = (IGCaptcha*)data;

	m_eventListener.onRequestCaptchaNotify(realData->response.msgcode, realData->response.imageStream);
}

void IGLoginManager::registerAccount(const string& userName, const string& pwd, const string& captcha)
{
	IGRegisterInfo info;
	info.userName = userName;
	info.pwd = pwd;
	info.captcha = captcha;

	m_userName = userName;
	m_pwd = pwd;

	m_protoManager->sendData(IGClientProtoType::RegisterAccount, (void*)&info);
}

void IGLoginManager::onRegisterAccount(const void* data)
{
	IGRegisterAccountNotify* info = (IGRegisterAccountNotify*)data;
	
	if (info->msgcode != 0)
	{
		m_userName = "";
		m_pwd = "";
	}

	m_eventListener.onRegisterAccountNotify(info->msgcode);
}

void IGLoginManager::login()
{

}

void IGLoginManager::onLogin(const void* data)
{

}