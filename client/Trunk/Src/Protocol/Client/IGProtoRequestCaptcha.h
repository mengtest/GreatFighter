#pragma once

/////////////////////////////////////////////
// desc:验证码协议解析
// author:wuyinjie
// since:2017-02-26
/////////////////////////////////////////////

struct IGCaptcha : public IGProtoMessage
{
	struct Ressponse
	{
		int msgCode = 0;
		string imageStream;
	} 
	response;
};

class IGProtoRequestCaptcha : public IIGProtoHelper
{
public:
	IGProtoRequestCaptcha();
	~IGProtoRequestCaptcha();

	virtual void init();
	virtual string pack(IGClientProtoType protoType, int sessionID, const void* data);
	virtual void parse(const string& data);
	virtual void registerCallback(const std::function<void(const void*)>& callback);

private:
	std::function<void(const void* data)> m_callback;
};

