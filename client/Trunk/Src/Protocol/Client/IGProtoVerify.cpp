#include "Common/IGHeader.h"
#include "Protocol/IIGProtoHelper.h"
#include "IGProtoVerify.h"


IGProtoVerify::IGProtoVerify()
{
}


IGProtoVerify::~IGProtoVerify()
{
}

void IGProtoVerify::init()
{

}

std::string IGProtoVerify::pack(IGClientProtoType protoType, int sessionID, const void* data)
{
	IGVerifyMessage* realData = (IGVerifyMessage*)data;

	cJSON* root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "protoType", (int)protoType);
	cJSON_AddNumberToObject(root, "sessionID", sessionID);
	cJSON_AddStringToObject(root, "user", realData->user.c_str());
	cJSON_AddStringToObject(root, "verify", realData->verify.c_str());
	cJSON_AddStringToObject(root, "response", "");

	char* out = cJSON_Print(root);
	string serializeStream(out);

	cJSON_Delete(root);
	free(out);

	return serializeStream;
}

void IGProtoVerify::parse(const string& data)
{
	cJSON* root = cJSON_Parse(data.c_str());
	cJSON* responseObj = cJSON_GetObjectItem(root, "response");
	cJSON* msgcodeObj = cJSON_GetObjectItem(responseObj, "msgcode");

	IGVerifyMessage info;
	info.response.msgcode = msgcodeObj->valueint;

	m_callback((void*)&info);
}

void IGProtoVerify::registerCallback(const std::function<void(const void*)>& callback)
{
	m_callback = callback;
}
