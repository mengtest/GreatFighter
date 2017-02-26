/////////////////////////////////
// desc: ����㣬�������ݰ��շ�
// author : wuyinjie
// since : 2017-02-13
/////////////////////////////////

#pragma once

#include <thread>
#include <queue>
#include <string>
#include <functional>
#include <list>
using namespace std;

#ifdef WINDOWS_DEV
#include <winsock.h>
#else
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/ioctl.h>
#endif

#include <errno.h>

#include "NetworkError.h"
#include <stdlib.h>
#include <mutex>

const unsigned int MAX_BUFFER_SIZE = 65535;
const unsigned int RECV_LEN = 65535;

class NetworkClient
{
public:
	NetworkClient();
    ~NetworkClient();

    void start(
        const string& serverIP, 
        int port, 
        const std::function<void(const list<string>&)>& recvFunction,
        const std::function<void(NetworkError)>& errorFunction
        );
	void stop();
    void sendRequest(const string& request);

private:
    bool init();
    void sendThreadLoop();
    void recvThreadLoop();
    void lockSend();
    void unlockSend();
    void lockRecv();
    void unlockRecv();

    int m_socket = 0;

	bool m_isRecvThreadRunning = false;
	bool m_isSendThreadRunning = false;

    queue<string> m_sendQueue;
    std::mutex m_sendLock;

    list<string> m_recvQueue;
    std::mutex m_recvLock;

    std::thread m_sendThread;
    std::thread m_recvThread;

    std::function<void(NetworkError)> m_errorFunction;
    std::function<void(const list<string>&)> m_recvFunction;
};