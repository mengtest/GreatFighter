#include "Common/IGHeader.h"
#include "Logic/EventListener/IGLoginSceneEventListener.h"
#include "Logic/UI/IGTipsLayer.h"
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
	m_rpCapthaTextField = static_cast<ui::TextField*>(m_registerPanel->getChildByName("CaptchaTextField"));
	m_rpCaptcha = static_cast<ui::ImageView*>(m_registerPanel->getChildByName("CaptchaImage"));

	m_loginPanel = static_cast<ui::Layout*>(rootNode->getChildByName("LoginPanel"));
	m_lpUserNameTextFiled = static_cast<ui::TextField*>(m_loginPanel->getChildByName("UserNameTextField"));
	m_lpPwdTextField = static_cast<ui::TextField*>(m_loginPanel->getChildByName("PwdTextField"));
	m_lpCaptchaTextField = static_cast<ui::TextField*>(m_loginPanel->getChildByName("CaptchaTextField"));
	m_lpCaptcha = static_cast<ui::ImageView*>(m_loginPanel->getChildByName("CaptchaImage"));

	m_tipsLayer = IGTipsLayer::create();
	addChild(m_tipsLayer);

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

void IGLoginUI::switchTo(LSPanelType panelType)
{
	switch (panelType)
	{
	case LSPanelType::SelectAccountPanel:
	{
		m_selectAccountPanel->setVisible(true);
		m_registerPanel->setVisible(false);
		m_loginPanel->setVisible(false);
	}
		break;
	case LSPanelType::RegisterAccountPanel:
	{
		m_selectAccountPanel->setVisible(false);
		m_registerPanel->setVisible(true);
		m_loginPanel->setVisible(false);
	}
		break;
	case LSPanelType::LoginPanel:
	{
		m_selectAccountPanel->setVisible(false);
		m_registerPanel->setVisible(false);
		m_loginPanel->setVisible(true);
	}
		break;
	default:
		break;
	}
}

void IGLoginUI::refreshCaptcha(const string& imageStream)
{
	string captchaName("captch.jpg");

	auto imageObj = new Image();
	imageObj->initWithImageData((const unsigned char*)(imageStream.c_str()), imageStream.length());
	imageObj->saveToFile(captchaName);
	delete imageObj;
	imageObj = nullptr;

	m_lpCaptcha->loadTexture(captchaName);
	m_rpCaptcha->loadTexture(captchaName);
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
		m_eventListener.requestCaptcha();
	}
}

void IGLoginUI::onSAPLoginButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType)
{
	if (touchType == ui::Widget::TouchEventType::ENDED)
	{
		m_selectAccountPanel->setVisible(false);
		m_registerPanel->setVisible(false);

		m_loginPanel->setVisible(true);
		m_eventListener.requestCaptcha();
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
			m_rpCapthaTextField->getString().empty())
		{
			showTips((int)IGMsgcode::RegisterInfoIncomplete);
			return;
		}
		else if (m_rpPwdTextField->getString() != m_rpConfirmPwdTextField->getString())
		{
			showTips((int)IGMsgcode::RegisterPwdNotMatch);
			return;
		}

		m_eventListener.requestRegisterAccount(m_rpUserNameTextFiled->getString(), m_rpPwdTextField->getString(), m_rpCapthaTextField->getString());
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
	m_rpCapthaTextField->setText("");
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
		if (m_lpUserNameTextFiled->getString().empty() ||
			m_lpPwdTextField->getString().empty() ||
			m_lpCaptchaTextField->getString().empty())
		{
			showTips((int)IGMsgcode::LoginInfoIncomplete);
			return;
		}

		m_eventListener.requestLogin(m_lpUserNameTextFiled->getString(), m_lpPwdTextField->getString(), m_lpCaptchaTextField->getString());
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
	m_lpCaptchaTextField->setText("");
}

bool IGLoginUI::showTips(int msgcode)
{
	return m_tipsLayer->showTips(msgcode);
}
