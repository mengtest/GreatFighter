// desc: 常量定义
// author : wuyinjie
// since : 2017-02-13
#pragma once

enum class IGClientProtoType
{
	Default = 0,
	RequestCaptcha = 1,
	RegisterAccount = 2,
	Login = 3,
};

enum class IGServerNotifyType
{
	Default = 0,
	RegisterAccount = 1,
};

enum class IGMsgcode
{
	Sucess = 0,
	LostConnectToServer = 110,
	Connecting = 111,
	RegisterInfoIncomplete = 112,
	LoginInfoIncomplete = 113,
	RegisterPwdNotMatch = 114,  // 注册密码和确认密码不匹配
};