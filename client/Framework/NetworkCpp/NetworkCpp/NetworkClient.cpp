#include "NetworkClient.h"

static NetworkClient* s_networkClient = nullptr;

NetworkClient::NetworkClient()
{
}


NetworkClient::~NetworkClient()
{
    closesocket(m_socket);

    WSACleanup();
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
    const std::function<void(const string&)>& recvFunction, 
    const std::function<void(NetworkError)>& errorFunction)
{
    m_errorFunction = errorFunction;
    m_recvFunction = recvFunction;

    // only for windows
    WSADATA dllData;
    int error = WSAStartup(MAKEWORD(2, 0), &dllData);
    if (error != 0)
    {
        m_errorFunction(NetworkError::CannotCreateSocket);

        return;
    }

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
        default:    break;
        }

        return;
    }

    m_sendThread = std::thread(&NetworkClient::sendThreadLoop, this);
    m_recvThread = std::thread(&NetworkClient::recvThreadLoop, this);

    m_sendThread.detach();
    m_recvThread.detach();
}

void NetworkClient::sendRequest(const string& request)
{
    waitSendLockRelease();

    lockSend();
    m_sendQueue.push(request);
    unlockSend();
}

void NetworkClient::sendThreadLoop()
{
    while (true)
    {
        waitSendLockRelease();

        if (m_sendQueue.empty())
        {
            continue;
        }

        lockSend();
        string sendString = m_sendQueue.front();
        m_sendQueue.pop();

        string sendData = "00" + sendString;
        int sendLen = (int)sendString.length();
        sendData[0] = (char)((sendLen >> 8) & 0xff);
        sendData[1] = (char)(sendLen & 0xff);
        unlockSend();

        const char* buff = sendData.c_str();
        int totalLen = (int)sendData.length();
        int sendSize = 0;
        while (sendSize < totalLen)
        {
            int size = send(m_socket, buff + sendSize, totalLen - sendSize, 0);

            sendSize += size;

            if (sendSize == 0)
            {
                m_errorFunction(NetworkError::LostConnection);
                break;
            }
            else if (sendSize < 0)
            {
                m_errorFunction(NetworkError::Unknow);
                break;
            }
        }
    }
}

void NetworkClient::recvThreadLoop()
{
    while (true)
    {
        char recvBuffer[MAX_BUFFER_SIZE] = {};
        int result = recv(m_socket, recvBuffer, RECV_LEN, 0);
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

        waitRecvLockRelease();

        lockRecv();

        string buffer(recvBuffer);
        int receiveLength = (int)buffer.length();
        int read = 0;

        while (receiveLength > read)
        {
            if (read >= (int)buffer.length() - 1)
            {
                break;
            }

            int size = (buffer[read] | buffer[read + 1] << 8);
            read += 2;

            if (receiveLength >= read + size)
            {
                m_recvQueue.push(buffer);
            }

            read += size;
        }
        
        int queueLen = (int)m_recvQueue.size();
        for (int i = 0; i < queueLen; i++)
        {
            auto value = m_recvQueue.front();
            m_recvQueue.pop();

            m_recvFunction(value);
        }

        unlockRecv();
    }
}

void NetworkClient::waitSendLockRelease()
{
    while (m_sendLock)
    {
        // just block
    }
}

void NetworkClient::waitRecvLockRelease()
{
    while (m_recvLock)
    {
        // just block
    }
}

void NetworkClient::lockSend()
{
    m_sendLock = true;
}

void NetworkClient::unlockSend()
{
    m_sendLock = false;
}

void NetworkClient::lockRecv()
{
    m_recvLock = true;
}

void NetworkClient::unlockRecv()
{
    m_recvLock = false;
}
