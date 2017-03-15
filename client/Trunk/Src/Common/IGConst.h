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
	Verify = 4,
};

enum class IGServerNotifyType
{
	Default = 0,
	RegisterAccount = 1,
};

enum class IGMsgcode
{
	Sucess = 0,					// 成功，但是不执行任何提示
	LostConnectToServer = 110,
	Connecting = 111,
	RegisterInfoIncomplete = 112,
	LoginInfoIncomplete = 113,
	RegisterPwdNotMatch = 114,  // 注册密码和确认密码不匹配
	ConnectGameServerSuccess = 115, // 成功连接游戏服务器
	ConnectServerFailure = 116,
	ConnectLoginServerSucess = 117,
};