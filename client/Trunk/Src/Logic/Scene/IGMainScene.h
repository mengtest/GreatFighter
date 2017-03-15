/////////////////////////////////////////////
// desc:ÓÎÏ·Ö÷³¡¾°
// auth:wuyinjie
// since:2017-03-6
/////////////////////////////////////////////

#pragma once

class IGMainScene : public cocos2d::Scene
{
public:
	IGMainScene();
	~IGMainScene();

	static cocos2d::Scene* createScene(const string& user, const string& secret, const string& ip, int port);

	virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void update(float delta);
	CREATE_FUNC(IGMainScene);

	void assignServerInfo(const string& user, const string& secret, const string& ip, int port);
private:
	void onTips(int msgcode);
	void onAppShutDown();

	IGPlayer* m_player = nullptr;
	IGMainUI* m_mainUI = nullptr;

	IGMainSceneEventListener m_eventListener;
};

