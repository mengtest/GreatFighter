/////////////////////////////////
// desc: –≠“È≤„mgr
// author : wuyinjie
// since : 2017-02-13
/////////////////////////////////

#pragma once

class NetworkClient;
enum class NetworkError;
class IIGProtoHelper;

class IGProtoManager
{
public:
	IGProtoManager();
	~IGProtoManager();

	bool init();
	void uninit();
	bool sendData(IGClientProtoType protoType, const void* data);
	void registerClientProto(IGClientProtoType ptype);
	void registerClientProto(IGClientProtoType ptype, const std::function<void(const void*)>& callback);
	void registerServerNotify(IGServerNotifyType stype, const std::function<void(const void*)>& callback);
	void update();

	bool startNetwork(const string& ip, int port);
	void stopNetwork();
private:
	void onRecvMessage(const list<string>& msgs);
	void onNetworkError(NetworkError error);
	bool canFindSessionID(int sessionID);
	void onEmptyCallback(const void* data);

	int m_sessionID = 0;
	std::list<int> m_sessionList;

	bool m_isConnect = false;

	NetworkClient* m_network = nullptr;
	std::map<IGClientProtoType, IIGProtoHelper*> m_clientProtos;
	std::map<IGServerNotifyType, IIGProtoHelper*> m_serverNotify;
	std::list<string> m_messageList;
};

