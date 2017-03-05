#pragma once

/////////////////////////////////////////////
// desc:��ʾ��Ϣ��
// author:wuyinjie
// since:2017-03-4
/////////////////////////////////////////////

struct IGTipsMessage
{
	string msg;
	int duration = 0;
};

class IGTipsLayer : public cocos2d::Layer
{
public:
	IGTipsLayer();
	~IGTipsLayer();

	virtual bool init();
	virtual void onEnter();
	CREATE_FUNC(IGTipsLayer);

	bool showTips(int msgcode);
private:
	void loadConfig();
	void hideTips();

	Text* m_tipsText = nullptr;
	int m_defaultDuration = 0;
	std::map<int, IGTipsMessage> m_errorMsg;
};

