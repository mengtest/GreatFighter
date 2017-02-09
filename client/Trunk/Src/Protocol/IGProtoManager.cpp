#include "Network/NetworkClient.h"
#include "Protocol/IGProtoManager.h"


IGProtoManager::IGProtoManager()
{
}


IGProtoManager::~IGProtoManager()
{
}

bool IGProtoManager::init()
{
	m_network = NetworkClient::getInstance();
	return true;
}

bool IGProtoManager::sendData(const string& protoName, const void* data)
{
	return true;
}

void IGProtoManager::update()
{

}
