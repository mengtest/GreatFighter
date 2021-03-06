#include <iostream>
#include "NetworkClient.h"
#include "cJSON.h"

void networkError(NetworkError error)
{
    cout << "error��" << (int)error << endl;
}

void onRecv(const list<string>& recvQueue)
{
	string result = "";
    for (auto& data : recvQueue)
    {
		cJSON* parseObj = cJSON_Parse(data.c_str());
		if (parseObj != nullptr)
		{
			char* out = cJSON_Print(parseObj);
			cout << "recv msg:" << out << endl;

			cJSON_Delete(parseObj);
			free(out);
		}
		else
		{
			cout << "parse json string error" << endl;
		}
    }
	// cout << "recv data " << result << endl;
}

void main()
{
    auto network = NetworkClient::getInstance();
    network->start("192.168.0.104", 8888, std::bind(onRecv, std::placeholders::_1), std::bind(networkError, std::placeholders::_1));

	cJSON* root = cJSON_CreateObject();
	cJSON_AddItemToObject(root, "typeName", cJSON_CreateString("helloWorld"));
	cJSON_AddItemToObject(root, "sessionId", cJSON_CreateNumber(1));

	char* bytes = cJSON_Print(root);
	string networkBytes(bytes);
	free(bytes);
	cJSON_Delete(root);

	network->sendRequest(networkBytes);
    while (true)
    {
		cout << "intput 1 to close the console." << endl;
		int cmd = 0;
		cin >> cmd;

		if (cmd == 1)
		{
			cout << "finish!" << endl;
			break;
		}
    }
}