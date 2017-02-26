#pragma once

/////////////////////////////////////////////
// desc:◊¢≤·’À∫≈Õ∆ÀÕ
// author:wuyinjie
// since:2017-02-26
/////////////////////////////////////////////

struct IGRegisterAccountNotify
{
	int msgCode = 0;
	string userName;
	string pwd;
};

class IGProtoRegisterAccountNotify : public IIGProtoHelper
{
public:
	IGProtoRegisterAccountNotify();
	~IGProtoRegisterAccountNotify();

	virtual void init();
	virtual void parse(const string& data);
	virtual void registerCallback(const std::function<void(const void*)>& callback);

private:
	std::function<void(const void* data)> m_callback;
};

