/////////////////////////////////////////////
// desc:登录场景管理，MVC架构中的Controller
// auth:wuyinjie
// since:2017-02-26
/////////////////////////////////////////////

#ifndef __LOGINSCENE_SCENE_H__
#define __LOGINSCENE_SCENE_H__

class IGLoginScene : public cocos2d::Scene
{
public:
	~IGLoginScene();
    static cocos2d::Scene* createScene();

    virtual bool init();
	virtual void onEnter();
	virtual void onExit();
	virtual void update(float delta);
    CREATE_FUNC(IGLoginScene);

private:
	void doRequestRegisterAccount(const string& userName, const string& pwd, const string& captcha);
	void onRequestRegisterAccount(int msgcode);
	void onRegisterAccountNotify(int msgcode);

	void doRequestCaptcha();
	void onRequestCaptchaNotify(int msgcode, const string& imageString);

	void doRequestLogin(const string& userName, const string& pwd, const string& captcha);
	void onRequestLogin(int msgcode, const string& user, const string& secret, const string& ip, int port);

	void onError(int msgcode);

	IGLoginUI* m_loginUI = nullptr;
	IGLoginManager* m_loginManager = nullptr;
	IGLoginSceneEventListener m_eventListener;
};

#endif // __LOGINSCENE_SCENE_H__
