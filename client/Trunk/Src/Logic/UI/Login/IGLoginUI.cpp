#include "Common/IGHeader.h"
#include "Logic/EventListener/IGLoginSceneEventListener.h"
#include "IGLoginUI.h"


IGLoginUI::IGLoginUI()
{
}


IGLoginUI::~IGLoginUI()
{
}

bool IGLoginUI::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto rootNode = CSLoader::createNode("LoginUI.csb");
	addChild(rootNode);

	m_selectAccountPanel = static_cast<ui::Layout*>(rootNode->getChildByName("SelectAccountPanel"));

	m_registerPanel = static_cast<ui::Layout*>(rootNode->getChildByName("RegisterPanel"));
	m_rpUserNameTextFiled = static_cast<ui::TextField*>(m_registerPanel->getChildByName("UserNameTextField"));
	m_rpPwdTextField = static_cast<ui::TextField*>(m_registerPanel->getChildByName("PwdTextField"));
	m_rpConfirmPwdTextField = static_cast<ui::TextField*>(m_registerPanel->getChildByName("ConfirmPwdTextField"));
	m_rpcaptchaTextField = static_cast<ui::TextField*>(m_registerPanel->getChildByName("captchaTextField"));

	m_loginPanel = static_cast<ui::Layout*>(rootNode->getChildByName("LoginPanel"));
	m_lpUserNameTextFiled = static_cast<ui::TextField*>(m_loginPanel->getChildByName("UserNameTextField"));
	m_lpPwdTextField = static_cast<ui::TextField*>(m_loginPanel->getChildByName("PwdTextField"));
	m_lpcaptchaTextField = static_cast<ui::TextField*>(m_loginPanel->getChildByName("captchaTextField"));

	return true;
}

void IGLoginUI::onEnter()
{
	Layer::onEnter();

	m_selectAccountPanel->setVisible(true);
	m_registerPanel->setVisible(false);
	m_loginPanel->setVisible(false);

	registerSAPEvents();
	registerRPEvents();
	registerLPEvents();
}

void IGLoginUI::registerEventListener(const IGLoginSceneEventListener& eventListener)
{
	m_eventListener = eventListener;
}

void IGLoginUI::registerSAPEvents()
{
	auto registerButton = static_cast<ui::Button*>(m_selectAccountPanel->getChildByName("RegisterButton"));
	registerButton->addTouchEventListener(CC_CALLBACK_2(IGLoginUI::onSAPRegisterButtonTouched, this));

	auto loginButton = static_cast<ui::Button*>(m_selectAccountPanel->getChildByName("LoginButton"));
	loginButton->addTouchEventListener(CC_CALLBACK_2(IGLoginUI::onSAPLoginButtonTouched, this));
}

void IGLoginUI::onSAPRegisterButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType)
{
	if (touchType == ui::Widget::TouchEventType::ENDED)
	{
		m_selectAccountPanel->setVisible(false);
		m_loginPanel->setVisible(false);

		m_registerPanel->setVisible(true);
	}
}

void IGLoginUI::onSAPLoginButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType)
{
	if (touchType == ui::Widget::TouchEventType::ENDED)
	{
		m_selectAccountPanel->setVisible(false);
		m_registerPanel->setVisible(false);

		m_loginPanel->setVisible(true);
	}
}

void IGLoginUI::registerRPEvents()
{
	auto confirmButton = static_cast<ui::Button*>(m_registerPanel->getChildByName("ConfirmButton"));
	confirmButton->addTouchEventListener(CC_CALLBACK_2(IGLoginUI::onRPConfirmButtonTouched, this));

	auto cancelButton = static_cast<ui::Button*>(m_registerPanel->getChildByName("CancelButton"));
	cancelButton->addTouchEventListener(CC_CALLBACK_2(IGLoginUI::onRPCancelButtonTouched, this));

	auto backButton = static_cast<ui::Button*>(m_registerPanel->getChildByName("BackButton"));
	backButton->addTouchEventListener(CC_CALLBACK_2(IGLoginUI::onRPBackButtonTouched, this));
}

void IGLoginUI::onRPBackButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType)
{
	if (touchType == ui::Widget::TouchEventType::ENDED)
	{
		m_loginPanel->setVisible(false);
		m_registerPanel->setVisible(false);

		m_selectAccountPanel->setVisible(true);

		clearRPInfo();
	}
}

void IGLoginUI::onRPConfirmButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType)
{
	if (touchType == ui::Widget::TouchEventType::ENDED)
	{
		if (m_rpUserNameTextFiled->getString().empty() ||
			m_rpPwdTextField->getString().empty() ||
			m_rpConfirmPwdTextField->getString().empty() ||
			m_rpcaptchaTextField->getString().empty())
		{
			CCLOG("%s", "----------------信息不完整");
			return;
		}
		else if (m_rpPwdTextField->getString() != m_rpConfirmPwdTextField->getString())
		{
			CCLOG("%s", "----------------密码和验证密码不正确");
			return;
		}


	}
}

void IGLoginUI::onRPCancelButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType)
{
	if (touchType == ui::Widget::TouchEventType::ENDED)
	{
		clearRPInfo();
	}
}

void IGLoginUI::clearRPInfo()
{
	m_rpUserNameTextFiled->setText("");
	m_rpPwdTextField->setText("");
	m_rpConfirmPwdTextField->setText("");
	m_rpcaptchaTextField->setText("");
}


void IGLoginUI::registerLPEvents()
{
	auto confirmButton = static_cast<ui::Button*>(m_loginPanel->getChildByName("ConfirmButton"));
	confirmButton->addTouchEventListener(CC_CALLBACK_2(IGLoginUI::onLPConfirmButtonTouched, this));

	auto cancelButton = static_cast<ui::Button*>(m_loginPanel->getChildByName("CancelButton"));
	cancelButton->addTouchEventListener(CC_CALLBACK_2(IGLoginUI::onLPCancelButtonTouched, this));

	auto backButton = static_cast<ui::Button*>(m_loginPanel->getChildByName("BackButton"));
	backButton->addTouchEventListener(CC_CALLBACK_2(IGLoginUI::onLPBackButtonTouched, this));
}

void IGLoginUI::onLPBackButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType)
{
	if (touchType == ui::Widget::TouchEventType::ENDED)
	{
		m_loginPanel->setVisible(false);
		m_registerPanel->setVisible(false);

		m_selectAccountPanel->setVisible(true);

		clearLPInfo();
	}
}

void IGLoginUI::onLPConfirmButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType)
{
	if (touchType == ui::Widget::TouchEventType::ENDED)
	{

	}
}

void IGLoginUI::onLPCancelButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType)
{
	if (touchType == ui::Widget::TouchEventType::ENDED)
	{
		clearLPInfo();
	}
}

void IGLoginUI::clearLPInfo()
{
	m_lpUserNameTextFiled->setText("");
	m_lpPwdTextField->setText("");
	m_lpcaptchaTextField->setText("");
}
