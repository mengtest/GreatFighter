#pragma once

#include <thread>
#include <queue>
#include <string>
#include <functional>
using namespace std;

#include <winsock.h>
#include <errno.h>

#include "NetworkError.h"
#include <stdlib.h>

const int MAX_BUFFER_SIZE = 4096;
const int RECV_LEN = 255;

class NetworkClient
{
public:
    ~NetworkClient();

    static NetworkClient* getInstance();

    void start(const string& serverIP, int port, const std::function<void(NetworkError)>& errorFunction);
    void sendRequest(const string& request);
    queue<string> popRecvQueue();

private:
    bool init();
    void sendThreadLoop();
    void recvThreadLoop();
    void waitSendLockRelease();
    void waitRecvLockRelease();
    void lockSend();
    void unlockSend();
    void lockRecv();
    void unlockRecv();

    SOCKET m_socket;

    queue<string> m_sendQueue;
    bool m_sendLock = false;

    queue<string> m_recvQueue;
    bool m_recvLock = false;

    std::thread m_sendThread;
    std::thread m_recvThread;

    std::function<void(NetworkError)> m_errorFunction;

    NetworkClient();
    NetworkClient(const NetworkClient&);
    NetworkClient& operator() (const NetworkClient&);
};

