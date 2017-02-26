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
	void requestCaptcha();
	void registerAccount(const string& userName, const string& pwd, const string& captcha);
	void login();
private:
	void onRecvCaptcha(const void* data);
	void onRegisterAccount(const void* data);
	void onLogin(const void* data);

	IGProtoManager* m_protoManager = nullptr;
	IGLoginSceneEventListener m_eventListener;

	string m_userName;
	string m_pwd;
};

