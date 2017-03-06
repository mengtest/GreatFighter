#pragma once

/////////////////////////////////////////////
// desc:µÇÂ½Ð­Òé
// author:wuyinjie
// since:2017-03-3
/////////////////////////////////////////////

struct IGLoginMessage : IGProtoMessage
{
	string userName;
	string pwd;
	string captcha;

	struct Response
	{
		int msgcode = 0;
		string ip;
		string user;
		string secret;
		int port = 0;
	}
	response;
};

class IGProtoLogin : public IIGProtoHelper
{
public:
	IGProtoLogin();
	~IGProtoLogin();

	virtual void init();
	virtual string pack(IGClientProtoType protoType, int sessionID, const void* data);
	virtual void parse(const string& data);
	virtual void registerCallback(const std::function<void(const void*)>& callback);
private:
	std::function<void(const void* data)> m_callback;
};

