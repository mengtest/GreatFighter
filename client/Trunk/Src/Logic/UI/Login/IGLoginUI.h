/////////////////////////////////////////////
// desc:��¼����UI��MVC�ܹ��е�View
// auth:wuyinjie
// since:2017-02-26
/////////////////////////////////////////////

#pragma once

#include "Common/IGHeader.h"

class IGLoginUI : public cocos2d::Layer
{
public:
	IGLoginUI();
	~IGLoginUI();

	virtual bool init();
	virtual void onEnter();
	CREATE_FUNC(IGLoginUI);

private:
	// SAP->Select Account Panel
	void registerSAPEvents();
	void onSAPRegisterButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType);
	void onSAPLoginButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType);

	// RP->Register Panel
	void registerRPEvents();
	void onRPBackButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType);
	void onRPConfirmButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType);
	void onRPCancelButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType);
	void clearRPInfo();
	ui::TextField* m_rpUserNameTextFiled = nullptr;
	ui::TextField* m_rpPwdTextField = nullptr;
	ui::TextField* m_rpConfirmPwdTextField = nullptr;
	ui::TextField* m_rpVerifyCodeTextField = nullptr;

	// LP->Login Panel
	void registerLPEvents();
	void onLPBackButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType);
	void onLPConfirmButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType);
	void onLPCancelButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType);
	void clearLPInfo();
	ui::TextField* m_lpUserNameTextFiled = nullptr;
	ui::TextField* m_lpPwdTextField = nullptr;
	ui::TextField* m_lpVerifyCodeTextField = nullptr;

	ui::Layout* m_selectAccountPanel = nullptr;
	ui::Layout* m_registerPanel = nullptr;
	ui::Layout* m_loginPanel = nullptr;
};

