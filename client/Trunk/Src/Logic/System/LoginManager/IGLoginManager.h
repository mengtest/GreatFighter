// desc: 登陆管理器,MVC架构中的Model
// author : wuyinjie
// since : 2017-02-16

#pragma once

class IGProtoManager;

class IGLoginManager
{
public:
	IGLoginManager();
	~IGLoginManager();

	bool init();
	void uninit();

	void onEnter(const IGLoginSceneEventListener& eventListener);
	void onExit();
	void update(int interval);

	bool doRequestCaptcha();
	bool doRegisterAccount(const string& userName, const string& pwd, const string& captcha);
	bool doLogin(const string& userName, const string& pwd, const string& captcha);
private:
	void onRecvCaptcha(const void* data);
	void onRegisterAccount(const void* data);
	void onRegisterAccountNotify(const void* data);
	void onLogin(const void* data);

	void connectToServerThread();
	void onMsgTips(int msgcode);

	IGProtoManager* m_protoManager = nullptr;
	IGLoginSceneEventListener m_eventListener;

	string m_userName;
	string m_pwd;

	std::list < std::function<void()>> m_tipsList;
};

