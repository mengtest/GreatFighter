#include "Common/IGHeader.h"
#include "Protocol/IIGProtoHelper.h"
#include "IGProtoRegisterAccountNotify.h"


IGProtoRegisterAccountNotify::IGProtoRegisterAccountNotify()
{
}


IGProtoRegisterAccountNotify::~IGProtoRegisterAccountNotify()
{
}

void IGProtoRegisterAccountNotify::init()
{

}

void IGProtoRegisterAccountNotify::parse(const string& data)
{
	cJSON* root = cJSON_Parse(data.c_str());
	cJSON* msgCodeObj = cJSON_GetObjectItem(root, "msgCode");
	cJSON* userNameObj = cJSON_GetObjectItem(root, "userName");
	cJSON* pwdObj = cJSON_GetObjectItem(root, "pwd");

	IGRegisterAccountNotify info;
	info.msgCode = msgCodeObj->valueint;
	info.userName = msgCodeObj->valuestring;
	info.pwd = msgCodeObj->valuestring;

	m_callback((void*)&info);

	cJSON_Delete(root);
}

void IGProtoRegisterAccountNotify::registerCallback(const std::function<void(const void*)>& callback)
{
	m_callback = callback;
}
