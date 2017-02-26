#pragma once

#include "Common/IGHeader.h"

struct IGLoginSceneEventListener
{
	std::function<void(const string&, const string&, const string&)> requestRegisterAccount;
};

