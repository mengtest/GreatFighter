#include "Base.h"
#include "GameClient.h"


GameClient::GameClient()
{
	init();
}


GameClient::~GameClient()
{
	uninit();
}

bool GameClient::init()
{
	bool result = false;

	WSADATA dllData;
	int error = WSAStartup(MAKEWORD(2, 0), &dllData);
	if (error != 0)
	{
		return false;
	}

	m_clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_clientSocket == INVALID_SOCKET)
	{
		return false;
	}

	return result;
}

void GameClient::uninit()
{
	if (m_clientSocket != INVALID_SOCKET)
	{
		closesocket(m_clientSocket);
	}

	WSACleanup();
}

bool GameClient::connectToServer(const string& ipAddress, int port)
{
	if (m_clientSocket == INVALID_SOCKET)
	{
		return false;
	}

	sockaddr_in serverAddress;
	serverAddress.sin_family = AF_INET;
	serverAddress.sin_addr.s_addr = inet_addr(ipAddress.c_str());
	serverAddress.sin_port = htons(port);

	int connectResult = connect(m_clientSocket, (sockaddr*)&serverAddress, sizeof(serverAddress));
	if (connectResult == SOCKET_ERROR)
	{
		return false;
	}

    //u_long ulongValue = 1;
    //ioctlsocket(m_clientSocket, FIONBIO, (unsigned long*)&ulongValue);

	return true;
}

ExecuteResult GameClient::receiveFromServer(string& receiveString, int packageCount)
{
    if (m_clientSocket == INVALID_SOCKET)
    {
        return ExecuteResult::Invalid;
    }

    char receiveBuffer[4096] = {};
    int result = recv(m_clientSocket, receiveBuffer, packageCount, 0);
    if (result == 0)
    {
        return ExecuteResult::LostConnectServer;
    }
    else if (result < 0) // ���ݰ����͹����г���
    {
        return ExecuteResult::Error;
    }

    receiveString.assign(receiveBuffer);

    return ExecuteResult::Success;
}

ExecuteResult GameClient::sendToServer(const string& sendString)
{
    if (m_clientSocket == INVALID_SOCKET)
    {
        return ExecuteResult::Invalid;
    }

    int sendSize = send(m_clientSocket, (const char FAR*)sendString.data(), sendString.length(), 0);
    if (sendSize < (int)sendString.length())
    {
        return ExecuteResult::CacheNotEnough;
    }
    else if (sendSize < 0)
    {
        return ExecuteResult::Error;
    }
    else if (sendSize == 0)
    {
        return ExecuteResult::LostConnectServer;
    }

    cout << "send size = " << sendSize << endl;

    return ExecuteResult::Success;
}
