/////////////////////////////////////////////
// desc:主场景事件监听器
// auth:wuyinjie
// since:2017-03-6
/////////////////////////////////////////////

#pragma once

#include "Common/IGHeader.h"

struct IGMainSceneEventListener
{
	std::function<void(int)> onError;
	std::function<void()> onAppShutDown;
};