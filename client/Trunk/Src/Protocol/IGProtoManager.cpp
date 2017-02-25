#include "Common/IGHeader.h"
#include "Network/NetworkClient.h"
#include "Protocol/IIGProtoHelper.h"
#include "Protocol/IGProtoManager.h"

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

bool IGProtoManager::sendData(const string& protoName, const void* data)
{
	m_sessionID ++;

	auto iter = m_clientProtos.find(protoName);
	IIGProtoHelper* helper = iter->second;
	string msg = helper->pack(protoName, m_sessionID, data);

	m_sessionList.push_back(m_sessionID);
	m_network->sendRequest(msg);

	return true;
}

void IGProtoManager::registerClientProto(IGClientProtoType ptype, const std::function<const void*>& callback)
{

}

void IGProtoManager::registerServerNotify(IGServerNotifyType stype, const std::function<const void*>& callback)
{

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

		string typeKey = string("protoName");
		cJSON* protoNameObj = cJSON_GetObjectItem(root, typeKey.c_str());
		string protoName = string(protoNameObj->valuestring);

		string sessionKey = string("sessionID");
		cJSON* sessionObj = cJSON_GetObjectItem(root, sessionKey.c_str());
		int sessionID = sessionObj->valueint;

		if (canFindSessionID(sessionID))
		{
			auto iter = m_clientProtos.find(protoName);
			IIGProtoHelper* helper = iter->second;
			helper->parse(msg);

			m_sessionList.remove(sessionID);
		}
		else
		{
			auto iter = m_serverNotify.find(protoName);
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
