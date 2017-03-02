#include "Common/IGHeader.h"
#include "Protocol/IIGProtoHelper.h"
#include "IGProtoRequestCaptcha.h"


IGProtoRequestCaptcha::IGProtoRequestCaptcha()
{
}

IGProtoRequestCaptcha::~IGProtoRequestCaptcha()
{
}

void IGProtoRequestCaptcha::init()
{

}

std::string IGProtoRequestCaptcha::pack(IGClientProtoType protoType, int sessionID, const void* data)
{
	cJSON* root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "protoType", (int)protoType);
	cJSON_AddNumberToObject(root, "sessionID", sessionID);
	cJSON_AddItemToObject(root, "response", cJSON_CreateString(""));

	char* out = cJSON_Print(root);

	string serializeStream(out);

	cJSON_Delete(root);
	free(out);

	return serializeStream;
}

void IGProtoRequestCaptcha::parse(const string& data)
{
	cJSON* root = cJSON_Parse(data.c_str());

	string responseKey = string("response");
	cJSON* responseObj = cJSON_GetObjectItem(root, responseKey.c_str());
	cJSON* msgCodeObj = cJSON_GetObjectItem(responseObj, "msgcode");
	cJSON* imageStreamObj = cJSON_GetObjectItem(responseObj, "imageStream");

	IGCaptcha info;
	info.response.msgcode = msgCodeObj->valueint;

	if (msgCodeObj == nullptr)
	{
		info.response.imageStream = "";
	}
	else
	{
		info.response.imageStream = imageStreamObj->valuestring;
	}

	m_callback((void*)&info);

	cJSON_Delete(root);
}

void IGProtoRequestCaptcha::registerCallback(const std::function<void(const void*)>& callback)
{
	m_callback = callback;
}
