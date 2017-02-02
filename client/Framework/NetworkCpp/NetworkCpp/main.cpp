#include <iostream>
#include "NetworkClient.h"

void networkError(NetworkError error)
{
    cout << "error" << endl;
}

void onRecv(const list<string>& recvQueue)
{
	string result = "";
    for (auto& data : recvQueue)
    {
		result += data;
    }
	
	cout << "recv data len " << result.length() << endl;
	// cout << "recv data " << result << endl;
}

void main()
{
    auto network = NetworkClient::getInstance();
    network->start("10.80.4.24", 8888, std::bind(onRecv, std::placeholders::_1), std::bind(networkError, std::placeholders::_1));

    while (true)
    {
		int byteNum = 0;
		cin >> byteNum;

        string request;
		for (int idx = 0; idx < byteNum; idx++)
		{
			char buffer[255] = {};
			sprintf(buffer, "%d", idx % 10);
			request += string(buffer);
		}

        network->sendRequest(request);
		// cout << "send request " << request << endl;
    }
}