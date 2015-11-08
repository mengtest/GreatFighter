#pragma once

enum class ExecuteResult
{
    Invalid = 0,

    LostConnectServer,
    Error,
    Success,
    CacheNotEnough,
};

class GameClient
{
public:
	GameClient();
	~GameClient();

	bool connectToServer(const string& ipAddress, int port);
    ExecuteResult receiveFromServer(string& receiveString, int packageCount);
    ExecuteResult sendToServer(const string& sendString);
private:
	bool init();
	void uninit();

	SOCKET m_clientSocket = INVALID_SOCKET;
};

