// GameClient

#include "Base.h"
#include "GameClient.h"
#include <windows.h>
#include <timeapi.h>

const string g_serverAddress = "192.168.1.110";
const int g_serverPort = 8888;

bool isTimeForReconnectAgain()
{
	static float lastTime = ::timeGetTime() / 1000.0f;
	static float currentTime = lastTime;

	currentTime = ::timeGetTime() / 1000.0f;

	if (currentTime - lastTime > 5.0f)
	{
		lastTime = currentTime;

		return true;
	}

	return false;
}

void main()
{
	GameClient client;
    bool connectSuccess = client.connectToServer(g_serverAddress, g_serverPort);

	cout << "Connect to server..." << endl;
	while(true)
	{
		if (!connectSuccess && isTimeForReconnectAgain())
		{
            connectSuccess = client.connectToServer(g_serverAddress, g_serverPort);

			cout << "Try connect server agian..." << endl;
		}

        string receiveString;
        auto receiveResult = client.receiveFromServer(receiveString, 255);
        if (receiveResult == ExecuteResult::Success)
        {
            cout << "Server: " << endl;
            cout << receiveString << endl;
        }

        cout << "Please input command :" << endl;
        string sendToServerString;
        cin >> sendToServerString;

        client.sendToServer(sendToServerString);
	}
}