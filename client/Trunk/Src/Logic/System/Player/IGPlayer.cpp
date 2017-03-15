#include "Common/IGHeader.h"
#include "Common/IGConst.h"
#include "Protocol/IGProtoManager.h"
#include "Logic/EventListener/IGMainSceneEventListener.h"
#include "IGPlayer.h"
#include "Protocol/IIGProtoHelper.h"
#include "Protocol/Client/IGProtoVerify.h"

static IGPlayer* s_player = nullptr;

IGPlayer::IGPlayer()
{
	init();
}


IGPlayer::~IGPlayer()
{
	uninit();
}

IGPlayer* IGPlayer::getInstance()
{
	if (s_player == nullptr)
	{
		s_player = new IGPlayer();
	}

	return s_player;
}

void IGPlayer::destoryInstance()
{
	if (s_player != nullptr)
	{
		delete s_player;
		s_player = nullptr;
	}
}

bool IGPlayer::init()
{
	m_protoManager = new IGProtoManager();
	m_protoManager->registerClientProto(IGClientProtoType::Verify, std::bind(&IGPlayer::onVerify, this, std::placeholders::_1));

	return true;
}

void IGPlayer::uninit()
{
	delete m_protoManager;
	m_protoManager = nullptr;
}

void IGPlayer::initServerInfo(const string& user, const string& secret, const string& ip, int port)
{
	m_user = user;
	m_secret = secret;
	m_ip = ip;
	m_port = port;
}

void IGPlayer::connectToServer()
{
	std::thread threadObj = std::thread(&IGPlayer::connectThread, this);
	threadObj.detach();
}

void IGPlayer::disconnect()
{
	m_protoManager->stopNetwork();
}

void IGPlayer::registerEventListener(const IGMainSceneEventListener& eventListener)
{
	m_eventListener = eventListener;
}

void IGPlayer::update(int interval)
{
	while (!m_workThreadEvents.empty())
	{
		auto event = m_workThreadEvents.front();
		event();
		m_workThreadEvents.pop_front();
	}
}

void IGPlayer::connectThread()
{
	if (m_protoManager->startNetwork(m_ip, m_port))
	{
		m_workThreadEvents.push_back(std::bind(&IGPlayer::onTips, this, (int)IGMsgcode::ConnectGameServerSuccess));
		m_workThreadEvents.push_back(std::bind(&IGPlayer::doVerify, this));
	}
	else
	{
		m_workThreadEvents.push_back(std::bind(&IGPlayer::onTips, this, (int)IGMsgcode::ConnectServerFailure));
	}
}

void IGPlayer::onTips(int msgcode)
{
	m_eventListener.onError(msgcode);
}

void IGPlayer::doVerify()
{
	IGVerifyMessage info;
	info.user = m_user;
	info.secret = m_secret;

	if (!m_protoManager->sendData(IGClientProtoType::Verify, (void*)&info))
	{
		m_eventListener.onError((int)IGMsgcode::LostConnectToServer);
	}
}

void IGPlayer::onVerify(const void* data)
{
	IGVerifyMessage* realData = (IGVerifyMessage*)data;
	m_eventListener.onError(realData->response.msgcode);
}