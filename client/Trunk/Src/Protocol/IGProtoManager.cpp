#include "Common/IGHeader.h"
#include "Network/NetworkClient.h"
#include "Protocol/IIGProtoHelper.h"
#include "Protocol/IGProtoManager.h"
#include "Protocol/Client/IGProtoRequestCaptcha.h"
#include "Protocol/Client/IGProtoRegisterAccount.h"
#include "Protocol/Server/IGProtoRegisterAccountNotify.h"

IGProtoManager* s_protoManager = nullptr;

IGProtoManager::IGProtoManager()
{
	init();
}


IGProtoManager::~IGProtoManager()
{
	uninit();
}

IGProtoManager* IGProtoManager::getInstance()
{
	if (s_protoManager == nullptr)
	{
		s_protoManager = new IGProtoManager();
	}
	
	return s_protoManager;
}

void IGProtoManager::destroyInstance()
{
	if (s_protoManager != nullptr)
	{
		delete s_protoManager;
		s_protoManager = nullptr;
	}
}

bool IGProtoManager::init()
{
	m_network = new NetworkClient();
	return true;
}

void IGProtoManager::uninit()
{
	delete m_network;
	m_network = nullptr;
}

bool IGProtoManager::sendData(IGClientProtoType protoType, const void* data)
{
	m_sessionID ++;

	auto iter = m_clientProtos.find(protoType);
	IIGProtoHelper* helper = iter->second;
	string msg = helper->pack(protoType, m_sessionID, data);

	m_sessionList.push_back(m_sessionID);
	m_network->sendRequest(msg);

	return true;
}

void IGProtoManager::registerClientProto(IGClientProtoType ptype)
{
	registerClientProto(ptype, std::bind(&IGProtoManager::onEmptyCallback, this, std::placeholders::_1));
}

void IGProtoManager::registerClientProto(IGClientProtoType ptype, const std::function<void(const void*)>& callback)
{
	auto iter = m_clientProtos.find(ptype);
	if (iter != m_clientProtos.end())
	{
		return;
	}

	switch (ptype)
	{
	case IGClientProtoType::RequestCaptcha:
	{
		auto helper = new IGProtoRequestCaptcha();
		helper->registerCallback(callback);
		m_clientProtos[IGClientProtoType::RequestCaptcha] = helper;
	}
		break;
	case IGClientProtoType::RegisterAccount:
	{
		auto helper = new IGProtoRegisterAccount();
		helper->registerCallback(callback);
		m_clientProtos[IGClientProtoType::RegisterAccount] = helper;
	}
		break;
	case IGClientProtoType::Login:
		break;
	default:break;
	}
}

void IGProtoManager::registerServerNotify(IGServerNotifyType stype, const std::function<void(const void*)>& callback)
{
	auto iter = m_serverNotify.find(stype);
	if (iter != m_serverNotify.end())
	{
		return;
	}

	switch (stype)
	{
	case IGServerNotifyType::RegisterAccount:
	{
		auto helper = new IGProtoRegisterAccountNotify();
		helper->registerCallback(callback);
		m_serverNotify[IGServerNotifyType::RegisterAccount] = helper;
	}
		break;
	default:
		break;
	}
}

void IGProtoManager::update()
{
	while (!m_messageList.empty())
	{
		string msg = m_messageList.front();

		cJSON* root = cJSON_Parse(msg.c_str());
		if (root == nullptr)
		{
			continue;
		}

		string typeKey = string("protoType");
		cJSON* protoTypeObj = cJSON_GetObjectItem(root, typeKey.c_str());
		int protoType = protoTypeObj->valueint;

		string sessionKey = string("sessionID");
		cJSON* sessionObj = cJSON_GetObjectItem(root, sessionKey.c_str());
		int sessionID = sessionObj->valueint;

		if (canFindSessionID(sessionID))
		{
			auto iter = m_clientProtos.find((IGClientProtoType)protoType);
			IIGProtoHelper* helper = iter->second;
			helper->parse(msg);

			m_sessionList.remove(sessionID);
		}
		else
		{
			auto iter = m_serverNotify.find((IGServerNotifyType)protoType);
			IIGProtoHelper* helper = iter->second;
			helper->parse(msg);
		}

		cJSON_Delete(root);

		m_messageList.pop_front();
	}
}

void IGProtoManager::startNetwork(const string& ip, int port)
{
	m_network->start(ip, port, std::bind(&IGProtoManager::onRecvMessage, this, std::placeholders::_1), std::bind(&IGProtoManager::onNetworkError, this, std::placeholders::_1));
}

void IGProtoManager::stopNetwork()
{
	m_network->stop();
	m_messageList.clear();
	m_sessionID = 0;
	m_sessionList.clear();
}

// 工作线程中，不能进行ui操作，否则会宕机，因此先将数据插入消息队列，再在主线程中进行对应处理
void IGProtoManager::onRecvMessage(const list<string>& msgs)
{
	for (auto message : msgs)
	{
		m_messageList.push_back(message);
	}
}

void IGProtoManager::onNetworkError(NetworkError error)
{

}

bool IGProtoManager::canFindSessionID(int sessionID)
{
	bool result = false;

	for (int sid : m_sessionList)
	{
		if (sid == sessionID)
		{
			result = true;
			break;
		}
	}

	return result;
}

void IGProtoManager::onEmptyCallback(const void* data)
{

}
