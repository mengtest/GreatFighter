/////////////////////////////////////////////
// desc:登录场景管理，MVC架构中的Controller
// auth:wuyinjie
// since:2017-02-26
/////////////////////////////////////////////

#ifndef __LOGINSCENE_SCENE_H__
#define __LOGINSCENE_SCENE_H__

#include "cocos2d.h"
#include "Logic/UI/Login/IGLoginUI.h"
#include "Logic/System/LoginManager/IGLoginManager.h"

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
	IGLoginUI* m_loginUI = nullptr;
	IGLoginManager* m_loginManager = nullptr;
};

#endif // __LOGINSCENE_SCENE_H__
