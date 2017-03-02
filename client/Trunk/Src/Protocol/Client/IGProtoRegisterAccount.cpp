#include "Common/IGHeader.h"
#include "Protocol/IIGProtoHelper.h"
#include "IGProtoRegisterAccount.h"


IGProtoRegisterAccount::IGProtoRegisterAccount()
{
}

IGProtoRegisterAccount::~IGProtoRegisterAccount()
{
}

void IGProtoRegisterAccount::init()
{

}

std::string IGProtoRegisterAccount::pack(IGClientProtoType protoType, int sessionID, const void* data)
{
	IGRegisterInfo* realData = (IGRegisterInfo*)data;

	cJSON* root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "protoType", (int)protoType);
	cJSON_AddNumberToObject(root, "sessionID", sessionID);
	cJSON_AddItemToObject(root, "userName", cJSON_CreateString(realData->userName.c_str()));
	cJSON_AddItemToObject(root, "pwd", cJSON_CreateString(realData->pwd.c_str()));
	cJSON_AddItemToObject(root, "captcha", cJSON_CreateString(realData->captcha.c_str()));
	char* out = cJSON_Print(root);

	string serializeStream(out);

	cJSON_Delete(root);
	free(out);

	return serializeStream;
}

void IGProtoRegisterAccount::parse(const string& data)
{
	cJSON* root = cJSON_Parse(data.c_str());

	string responseKey = string("response");
	cJSON* responseObj = cJSON_GetObjectItem(root, responseKey.c_str());
	cJSON* msgCodeObj = cJSON_GetObjectItem(responseObj, "msgcode");

	IGRegisterInfo info;
	info.response.msgcode = msgCodeObj->valueint;

	m_callback((void*)&info);

	cJSON_Delete(root);
}

void IGProtoRegisterAccount::registerCallback(const std::function<void(const void*)>& callback)
{
	m_callback = callback;
}
