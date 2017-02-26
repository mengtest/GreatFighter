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