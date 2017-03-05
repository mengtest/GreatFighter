#include "Common/IGHeader.h"
#include "Common/IGConst.h"
#include "Protocol/IGProtoManager.h"
#include "Logic/EventListener/IGLoginSceneEventListener.h"
#include "IGLoginManager.h"
#include "Protocol/IIGProtoHelper.h"
#include "Protocol/Client/IGProtoRequestCaptcha.h"
#include "Protocol/Client/IGProtoRegisterAccount.h"
#include "Protocol/Server/IGProtoRegisterAccountNotify.h"
#include "Protocol/Client/IGProtoLogin.h"


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
	m_protoManager = new IGProtoManager();
	return true;
}

void IGLoginManager::uninit()
{
	delete m_protoManager;
	m_protoManager = nullptr;
}

void IGLoginManager::onEnter(const IGLoginSceneEventListener& eventListener)
{
	m_eventListener = eventListener;

	m_protoManager->registerClientProto(IGClientProtoType::RequestCaptcha, std::bind(&IGLoginManager::onRecvCaptcha, this, std::placeholders::_1));
	m_protoManager->registerClientProto(IGClientProtoType::RegisterAccount);
	m_protoManager->registerClientProto(IGClientProtoType::Login, std::bind(&IGLoginManager::onLogin, this, std::placeholders::_1));

	m_protoManager->registerServerNotify(IGServerNotifyType::RegisterAccount, std::bind(&IGLoginManager::onRegisterAccountNotify, this, std::placeholders::_1));

	std::thread connectThread = std::thread(&IGLoginManager::connectToServerThread, this);
	connectThread.detach();
}

void IGLoginManager::onExit()
{
	m_protoManager->stopNetwork();
}

void IGLoginManager::update(int interval)
{
	if (!m_tipsList.empty())
	{
		auto msgfunc = m_tipsList.front();
		msgfunc();
		m_tipsList.pop_front();
	}
}

bool IGLoginManager::doRequestCaptcha()
{
	IGCaptcha info;
	return m_protoManager->sendData(IGClientProtoType::RequestCaptcha, (void*)&info);
}

void IGLoginManager::onRecvCaptcha(const void* data)
{
	IGCaptcha* realData = (IGCaptcha*)data;

	m_eventListener.onRequestCaptchaNotify(realData->response.msgcode, realData->response.imageStream);
}

bool IGLoginManager::doRegisterAccount(const string& userName, const string& pwd, const string& captcha)
{
	IGRegisterInfo info;
	info.userName = userName;
	info.pwd = pwd;
	info.captcha = captcha;

	m_userName = userName;
	m_pwd = pwd;

	return m_protoManager->sendData(IGClientProtoType::RegisterAccount, (void*)&info);
}

void IGLoginManager::onRegisterAccount(const void* data)
{
	IGRegisterInfo* realData = (IGRegisterInfo*)data;
	m_eventListener.onRegisterAccount(realData->response.msgcode);
}

void IGLoginManager::onRegisterAccountNotify(const void* data)
{
	IGRegisterAccountNotify* info = (IGRegisterAccountNotify*)data;
	
	if (info->msgcode != 0)
	{
		m_userName = "";
		m_pwd = "";
	}

	m_eventListener.onRegisterAccountNotify(info->msgcode);
}

bool IGLoginManager::doLogin(const string& userName, const string& pwd, const string& captcha)
{
	IGLoginMessage info;
	info.userName = userName;
	info.pwd = pwd;
	info.captcha = captcha;

	return m_protoManager->sendData(IGClientProtoType::Login, (void*)&info);
}

void IGLoginManager::onLogin(const void* data)
{
	IGLoginMessage* realData = (IGLoginMessage*)data;
	m_eventListener.onRequestLogin(realData->response.msgcode, realData->response.user, realData->response.secret, realData->response.ip);
}

void IGLoginManager::connectToServerThread()
{
	// 暂时写死,以后要放在配置表中配置
	if (m_protoManager->startNetwork("192.168.1.103", 8888))
	{
		m_tipsList.push_back(std::bind(&IGLoginManager::onMsgTips, this, (int)IGMsgcode::Sucess));
	}
	else
	{
		m_tipsList.push_back(std::bind(&IGLoginManager::onMsgTips, this, (int)IGMsgcode::LostConnectToServer));
	}
}

void IGLoginManager::onMsgTips(int msgcode)
{
	m_eventListener.onError(msgcode);
}
