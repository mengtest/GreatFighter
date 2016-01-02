#include <iostream>
#include "NetworkClient.h"

void networkError(NetworkError error)
{
    cout << "error" << endl;
}

void onRecv(const string& value)
{
    cout << value << endl;
}

void main()
{
    auto network = NetworkClient::getInstance();
    network->start("192.168.1.110", 8888, std::bind(onRecv, std::placeholders::_1), std::bind(networkError, std::placeholders::_1));

    while (true)
    {
        network->sendRequest("Hello World");

        Sleep(1000);
    }
}