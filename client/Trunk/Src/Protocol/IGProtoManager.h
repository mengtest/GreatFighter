/////////////////////////////////
// desc: –≠“È≤„mgr
// author : wuyinjie
// since : 2017-02-13
/////////////////////////////////

#pragma once

class IGProtoManager
{
public:
	~IGProtoManager();
	IGProtoManager* getInstance();
	void destroyInstance();

	bool init();
	void uninit();
	bool sendData(const string& protoName, const void* data);
	void registerClientProto(IGClientProtoType ptype, const std::function<const void*>& callback);
	void registerServerNotify(IGServerNotifyType stype, const std::function<const void*>& callback);
	void update();

	void startNetwork(const string& ip, int port);
	void stopNetwork();
private:
	void onRecvMessage(const list<string>& msgs);
	void onNetworkError(NetworkError error);
	bool canFindSessionID(int sessionID);

	int m_sessionID = 0;
	std::list<int> m_sessionList;

	NetworkClient* m_network = nullptr;
	std::map<string, IIGProtoHelper*> m_clientProtos;
	std::map<string, IIGProtoHelper*> m_serverNotify;
	std::list<string> m_messageList;

	IGProtoManager();
	IGProtoManager(const IGProtoManager&);
	IGProtoManager& operator() (const IGProtoManager&);
	IGProtoManager& operator= (const IGProtoManager&);
};

