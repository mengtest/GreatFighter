/////////////////////////////////////////////
// desc:µÇÂ½³¡¾°ÊÂ¼þ¼àÌýÆ÷
// auth:wuyinjie
// since:2017-02-26
/////////////////////////////////////////////

#pragma once

#include "Common/IGHeader.h"

struct IGLoginSceneEventListener
{
	std::function<void(const string&, const string&, const string&)> requestRegisterAccount;
	std::function<void(int)> onRegisterAccount;
	std::function<void(int)> onRegisterAccountNotify;

	std::function<void()> requestCaptcha;
	std::function<void(int, const string&)> onRequestCaptchaNotify;

	std::function<void(const string&, const string&, const string&)> requestLogin;
	std::function<void(int, const string&, const string&, const string&, int)> onRequestLogin;

	std::function<void(int)> onError;
};