/////////////////////////////////////////////
// desc:玩家类
// auth:wuyinjie
// since:2017-03-6
/////////////////////////////////////////////

#pragma once
class IGPlayer
{
public:
	~IGPlayer();
	static IGPlayer* getInstance();
	static void destoryInstance();

	void initServerInfo(const string& user, const string& secret, const string& ip, int port);
	void connectToServer();
	void disconnect();

	void registerEventListener(const IGMainSceneEventListener& eventListener);
	void update(int interval);
private:
	bool init();
	void uninit();

	void connectThread();

	void onTips(int msgcode);

	void doVerify();
	void onVerify(const void* data);

	string m_user;
	string m_secret;
	string m_ip;
	int m_port = 0;

	std::list<std::function<void()>> m_workThreadEvents; // 用于放置工作线程的事件，并在主线程中处理，用于避免工作线程进行ui操作导致宕机

	IGMainSceneEventListener m_eventListener;
	IGProtoManager* m_protoManager = nullptr;

	IGPlayer();
	IGPlayer& operator=(const IGPlayer&);
	IGPlayer(const IGPlayer&);
};

