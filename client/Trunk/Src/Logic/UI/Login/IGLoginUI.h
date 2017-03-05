/////////////////////////////////////////////
// desc:登录场景UI，MVC架构中的View
// auth:wuyinjie
// since:2017-02-26
/////////////////////////////////////////////

#pragma once

class IGTipsLayer;

enum class LSPanelType
{
	SelectAccountPanel = 0,
	RegisterAccountPanel,
	LoginPanel,
};

class IGLoginUI : public cocos2d::Layer
{
public:
	IGLoginUI();
	~IGLoginUI();

	virtual bool init();
	virtual void onEnter();
	CREATE_FUNC(IGLoginUI);

	void registerEventListener(const IGLoginSceneEventListener& eventListener);
	void switchTo(LSPanelType panelType);
	void refreshCaptcha(const string& imageStream);
	bool showTips(int msgcode);
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
	ui::TextField* m_rpCapthaTextField = nullptr;
	ImageView* m_rpCaptcha = nullptr;

	// LP->Login Panel
	void registerLPEvents();
	void onLPBackButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType);
	void onLPConfirmButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType);
	void onLPCancelButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType);
	void clearLPInfo();
	ui::TextField* m_lpUserNameTextFiled = nullptr;
	ui::TextField* m_lpPwdTextField = nullptr;
	ui::TextField* m_lpCaptchaTextField = nullptr;
	ImageView* m_lpCaptcha = nullptr;

	ui::Layout* m_selectAccountPanel = nullptr;
	ui::Layout* m_registerPanel = nullptr;
	ui::Layout* m_loginPanel = nullptr;

	IGTipsLayer* m_tipsLayer = nullptr;

	IGLoginSceneEventListener m_eventListener;
};

