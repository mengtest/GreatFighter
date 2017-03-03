#include "Common/IGHeader.h"
#include "Protocol/IIGProtoHelper.h"
#include "IGProtoLogin.h"


IGProtoLogin::IGProtoLogin()
{
}


IGProtoLogin::~IGProtoLogin()
{
}

void IGProtoLogin::init()
{

}

std::string IGProtoLogin::pack(IGClientProtoType protoType, int sessionID, const void* data)
{
	IGLoginMessage* realData = (IGLoginMessage*)data;

	cJSON* root = cJSON_CreateObject();
	cJSON_AddNumberToObject(root, "protoType", (int)protoType);
	cJSON_AddNumberToObject(root, "sessionID", sessionID);
	cJSON_AddItemToObject(root, "userName", cJSON_CreateString(realData->userName.c_str()));
	cJSON_AddItemToObject(root, "pwd", cJSON_CreateString(realData->pwd.c_str()));
	cJSON_AddItemToObject(root, "captcha", cJSON_CreateString(realData->captcha.c_str()));
	cJSON_AddItemToObject(root, "response", cJSON_CreateString(""));

	char* out = cJSON_Print(root);

	string serializeStream(out);

	cJSON_Delete(root);
	free(out);

	return serializeStream;
}

void IGProtoLogin::parse(const string& data)
{
	cJSON* root = cJSON_Parse(data.c_str());

	cJSON* responseObj = cJSON_GetObjectItem(root, "response");
	cJSON* msgcodeObj = cJSON_GetObjectItem(responseObj, "msgcode");
	cJSON* ipObj = cJSON_GetObjectItem(responseObj, "ip");
	cJSON* userObj = cJSON_GetObjectItem(responseObj, "user");
	cJSON* secretObj = cJSON_GetObjectItem(responseObj, "secret");

	IGLoginMessage info;
	if (ipObj == nullptr)
	{
		info.response.ip = "";
	}
	else
	{
		info.response.ip = ipObj->valuestring;
	}

	if (userObj == nullptr)
	{
		info.response.user = "";
	}
	else
	{
		info.response.user = userObj->valuestring;
	}

	if (secretObj == nullptr)
	{
		info.response.secret = "";
	}
	else
	{
		info.response.secret = secretObj->valuestring;
	}

	m_callback((void*)&info);

	cJSON_Delete(root);
}

void IGProtoLogin::registerCallback(const std::function<void(const void*)>& callback)
{
	m_callback = callback;
}
