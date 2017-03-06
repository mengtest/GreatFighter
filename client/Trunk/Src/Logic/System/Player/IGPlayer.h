/////////////////////////////////////////////
// desc:�����
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

	std::list<std::function<void()>> m_workThreadEvents; // ���ڷ��ù����̵߳��¼����������߳��д������ڱ��⹤���߳̽���ui��������崻�

	IGMainSceneEventListener m_eventListener;
	IGProtoManager* m_protoManager = nullptr;

	IGPlayer();
	IGPlayer& operator=(const IGPlayer&);
	IGPlayer(const IGPlayer&);
};

