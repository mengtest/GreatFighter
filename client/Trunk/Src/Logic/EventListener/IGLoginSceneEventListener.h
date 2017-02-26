#pragma once

#include "Common/IGHeader.h"

struct IGLoginSceneEventListener
{
	std::function<void(const string&, const string&, const string&)> requestRegisterAccount;
	std::function<void(int)> onRegisterAccountNotify;
	std::function<void()> requestCaptcha;
	std::function<void(int, const string&)> onRequestCaptchaNotify;
};

