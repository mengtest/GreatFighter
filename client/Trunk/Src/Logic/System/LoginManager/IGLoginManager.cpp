#include "Common/IGHeader.h"
#include "Protocol/IGProtoManager.h"
#include "IGLoginManager.h"


IGLoginManager::IGLoginManager()
{
	init();
}


IGLoginManager::~IGLoginManager()
{
	uninit();
}

bool IGLoginManager::init()
{
	m_protoManager = IGProtoManager::getInstance();
	return true;
}

void IGLoginManager::uninit()
{
	m_protoManager = nullptr;
}

void IGLoginManager::onEnter()
{
	// ��ʱд��,�Ժ�Ҫ�������ñ�������
	m_protoManager->startNetwork("", 8001);
}

void IGLoginManager::onExit()
{
	m_protoManager->stopNetwork();
}

void IGLoginManager::update(int interval)
{

}

void IGLoginManager::requestVerifyCode()
{

}

void IGLoginManager::onRecvVerifyCode()
{

}

void IGLoginManager::registerAccount()
{

}

void IGLoginManager::onRegisterAccount()
{

}

void IGLoginManager::login()
{

}

void IGLoginManager::onLogin()
{

}