#include "NetworkClient.h"
#include <iostream>

static NetworkClient* s_networkClient = nullptr;

NetworkClient::NetworkClient()
{
}


NetworkClient::~NetworkClient()
{
	stop();
}

NetworkClient* NetworkClient::getInstance()
{
    if (s_networkClient == nullptr)
    {
        s_networkClient = new NetworkClient();
        s_networkClient->init();
    }

    return s_networkClient;
}

bool NetworkClient::init()
{
    return true;
}

void NetworkClient::start(
    const string& serverIP, 
    int port, 
    const std::function<void(const list<string>&)>& recvFunction,
    const std::function<void(NetworkError)>& errorFunction)
{
    m_errorFunction = errorFunction;
    m_recvFunction = recvFunction;

#ifdef WINDOWS_DEV
    // only for windows
    WSADATA dllData;
    int error = WSAStartup(MAKEWORD(2, 0), &dllData);
    if (error != 0)
    {
        m_errorFunction(NetworkError::CannotCreateSocket);

        return;
    }
#endif

    m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (m_socket == -1)
    {
        m_errorFunction(NetworkError::CannotCreateSocket);
        return;
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = inet_addr(serverIP.c_str());
    serverAddress.sin_port = htons(port);

    int connectResult = connect(m_socket, (sockaddr*)&serverAddress, sizeof(serverAddress));
    if (connectResult == -1)
    {
        switch (errno)
        {
        case ETIMEDOUT:
            m_errorFunction(NetworkError::ConnectTimeOut);
            break;
        case ECONNREFUSED:
            m_errorFunction(NetworkError::ServerProcessNotExist);
            break;
        case EHOSTUNREACH:
        case ENETUNREACH:
            m_errorFunction(NetworkError::CannotFindRoute);
            break;
        default:    
			m_errorFunction(NetworkError::Unknow);
			break;
        }

        return;
    }

	m_isSendThreadRunning = true;
	m_isRecvThreadRunning = true;

    m_sendThread = std::thread(&NetworkClient::sendThreadLoop, this);
    m_recvThread = std::thread(&NetworkClient::recvThreadLoop, this);

    m_sendThread.detach();
    m_recvThread.detach();
}

void NetworkClient::stop()
{
	m_socket = 0;
	m_isSendThreadRunning = false;
	m_isRecvThreadRunning = false;

	m_sendQueue = {};
	m_recvQueue.clear();

#ifdef WINDOWS_DEV
	closesocket(m_socket);
	WSACleanup();
#else
	close(m_socket);
#endif
}

void NetworkClient::sendRequest(const string& request)
{
    lockSend();
    m_sendQueue.push(request);
    unlockSend();
}

void NetworkClient::sendThreadLoop()
{
    while (m_isSendThreadRunning)
    {
        lockSend();
        if (m_sendQueue.empty())
        {
            unlockSend();
            continue;
        }

        string sendString = m_sendQueue.front();
        m_sendQueue.pop();

        string sendData = "00" + sendString;
        int sendLen = (int)sendString.length();
        sendData[0] = (unsigned char)((sendLen >> 8) & 0xff);
		sendData[1] = (unsigned char)(sendLen & 0xff);
        unlockSend();

        const char* buff = sendData.c_str();
        int totalLen = (int)sendData.length();
        int sendSize = 0;
        while (sendSize < totalLen)
        {
            int size = send(m_socket, buff + sendSize, totalLen - sendSize, 0);

            sendSize += size;

            if (size == 0)
            {
                m_errorFunction(NetworkError::LostConnection);
                break;
            }
            else if (size < 0)
            {
                m_errorFunction(NetworkError::Unknow);
                break;
            }
        }
    }
}

void NetworkClient::recvThreadLoop()
{
	string tempBuffer;
	int unreceiveSize = 0;

    while (m_isRecvThreadRunning)
    {
        char buffer[MAX_BUFFER_SIZE] = {};
        int result = recv(m_socket, buffer, RECV_LEN, 0);
        if (result == 0)
        {
            m_errorFunction(NetworkError::LostConnection);
            break;
        }
        else if (result == -1)
        {
            m_errorFunction(NetworkError::RecvEmpty);
            break;
        }
        else if (result < 0)
        {
            m_errorFunction(NetworkError::Unknow);
            break;
        }

        lockRecv();

        int receiveLength = result;
        int read = 0;

        while (receiveLength > read)
        {
            if (read >= result - 1)
            {
                break;
            }

			if (unreceiveSize <= 0)
			{
				unsigned int size = ((unsigned char)buffer[read] << 24 | (unsigned char)buffer[read + 1] << 16 | (unsigned char)buffer[read + 2] << 8 | (unsigned char)buffer[read + 3]);
				read += 4;

				if (receiveLength >= read + size)
				{
					char temp[MAX_BUFFER_SIZE + 1] = {};
					memcpy(temp, buffer + read, size);

					m_recvQueue.push_back(temp);
					read += size;
				}
				else
				{
					char temp[MAX_BUFFER_SIZE + 1] = {};
					memcpy(temp, buffer + read, receiveLength - read);

					tempBuffer += string(temp);
					unreceiveSize = size - (receiveLength - read);
					read += receiveLength - read;
				}
			}
			else
			{
				if (unreceiveSize <= receiveLength)
				{
					char temp[MAX_BUFFER_SIZE + 1] = {};
					memcpy(temp, buffer, unreceiveSize);
					tempBuffer += string(temp);

					m_recvQueue.push_back(tempBuffer);

					tempBuffer.clear();
					read += unreceiveSize;
					unreceiveSize = 0;
				}
				else
				{
					char temp[MAX_BUFFER_SIZE + 1] = {};
					memcpy(temp, buffer, receiveLength);
					tempBuffer += string(temp);

					unreceiveSize -= receiveLength;
					read += receiveLength;
				}
			}
        }

        m_recvFunction(m_recvQueue);
        m_recvQueue.clear();

        unlockRecv();
    }
}

void NetworkClient::lockSend()
{
    m_sendLock.lock();
}

void NetworkClient::unlockSend()
{
    m_sendLock.unlock();
}

void NetworkClient::lockRecv()
{
    m_recvLock.lock();
}

void NetworkClient::unlockRecv()
{
    m_recvLock.unlock();
}
