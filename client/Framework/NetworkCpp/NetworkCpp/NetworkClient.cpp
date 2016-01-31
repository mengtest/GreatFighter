#include "NetworkClient.h"

static NetworkClient* s_networkClient = nullptr;

NetworkClient::NetworkClient()
{
}


NetworkClient::~NetworkClient()
{

#ifdef WINDOWS_DEV
    closesocket(m_socket);
    WSACleanup();
#else
    close(m_socket);
#endif

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

        waitRecvLockRelease();

        lockRecv();

        int receiveLength = result;
        int read = 0;

        while (receiveLength > read)
        {
            if (read >= result - 1)
            {
                break;
            }

            int size = (buffer[read] << 8 | buffer[read + 1]);
            read += 2;

            if (receiveLength >= read + size)
            {
                m_recvQueue.push_back(buffer + 2);
            }

            read += size;
        }

        m_recvFunction(m_recvQueue);
        m_recvQueue.clear();

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
