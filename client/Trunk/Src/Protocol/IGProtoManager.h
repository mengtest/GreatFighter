#pragma once

class IGProtoManager
{
public:
	IGProtoManager();
	~IGProtoManager();

	bool init();
	bool sendData(const string& protoName, const void* data);
	void update();

private:
	NetworkClient* m_network = nullptr;
};

