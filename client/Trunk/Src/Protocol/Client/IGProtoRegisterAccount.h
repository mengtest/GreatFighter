#pragma once

/////////////////////////////////////////////
// desc:’À∫≈◊¢≤·–≠“È
// author:wuyinjie
// since:2017-02-26
/////////////////////////////////////////////

struct IGRegisterInfo : public IGProtoMessage
{
	string userName;
	string pwd;
	string captcha;

	struct Response
	{
		int msgcode = 0;
	}
	response;
};


class IGProtoRegisterAccount : public IIGProtoHelper
{
public:
	IGProtoRegisterAccount();
	~IGProtoRegisterAccount();

	virtual void init();
	virtual string pack(IGClientProtoType protoType, int sessionID, const void* data);
	virtual void parse(const string& data);
	virtual void registerCallback(const std::function<void(const void*)>& callback);
};

