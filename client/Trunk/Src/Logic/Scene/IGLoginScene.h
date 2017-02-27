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
    CREATE_FUNC(IGLoginScene);

private:
	void onRequestRegisterAccount(const string& userName, const string& pwd, const string& captcha);
	void onRegisterAccountNotify(int msgcode);
	void onRequestCaptcha();
	void onRequestCaptchaNotify(int msgcode, const string& imageString);

	IGLoginUI* m_loginUI = nullptr;
	IGLoginManager* m_loginManager = nullptr;
	IGLoginSceneEventListener m_eventListener;
};

#endif // __LOGINSCENE_SCENE_H__
