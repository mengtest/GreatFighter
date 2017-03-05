#pragma once

/////////////////////////////////////////////
// desc:µ«¬ººÏ—È–≠“È
// author:wuyinjie
// since:2017-03-3
/////////////////////////////////////////////

struct IGVerifyMessage : IGProtoMessage
{
	string user;
	string verify;

	struct Response
	{
		int msgcode = 0;
	}
	response;
};

class IGProtoVerify : public IIGProtoHelper
{
public:
	IGProtoVerify();
	~IGProtoVerify();

	virtual void init();
	virtual string pack(IGClientProtoType protoType, int sessionID, const void* data);
	virtual void parse(const string& data);
	virtual void registerCallback(const std::function<void(const void*)>& callback);
private:
	std::function<void(const void* data)> m_callback;
};

