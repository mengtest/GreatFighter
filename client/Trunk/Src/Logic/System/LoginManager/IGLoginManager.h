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

	void onEnter();
	void onExit();
	void update(int interval);
	void requestVerifyCode();
	void registerAccount();
	void login();
private:
	void onRecvVerifyCode();
	void onRegisterAccount();
	void onLogin();

	IGProtoManager* m_protoManager = nullptr;
};

