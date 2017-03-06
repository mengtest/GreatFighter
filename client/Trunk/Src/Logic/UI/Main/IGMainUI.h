/////////////////////////////////////////////
// desc:游戏主界面
// auth:wuyinjie
// since:2017-03-6
/////////////////////////////////////////////

#pragma once
class IGMainUI : public cocos2d::Layer
{
public:
	IGMainUI();
	~IGMainUI();

	virtual bool init();
	virtual void onEnter();
	CREATE_FUNC(IGMainUI);

	void registerEventListener(const IGMainSceneEventListener& eventListener);
	void showTips(int msgcode);

private:
	void onExitButtonTouched(Ref* sender, ui::Widget::TouchEventType touchType);

	IGMainSceneEventListener m_eventListener;
	Button* m_exitButton = nullptr;
	IGTipsLayer* m_tipsLayer = nullptr;
};

