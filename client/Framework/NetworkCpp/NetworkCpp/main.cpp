#include <iostream>
#include "NetworkClient.h"

void networkError(NetworkError error)
{
    cout << "error" << endl;
}

void onRecv(const list<string>& recvQueue)
{
    for (auto& data : recvQueue)
    {
        cout << "recv data " << data << endl;
    }
}

void main()
{
    auto network = NetworkClient::getInstance();
    network->start("192.168.1.110", 8888, std::bind(onRecv, std::placeholders::_1), std::bind(networkError, std::placeholders::_1));

    while (true)
    {
        string request;

        cin >> request;

        network->sendRequest(request);
    }
}