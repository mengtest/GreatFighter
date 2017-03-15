// desc: ��������
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
	Sucess = 0,					// �ɹ������ǲ�ִ���κ���ʾ
	LostConnectToServer = 110,
	Connecting = 111,
	RegisterInfoIncomplete = 112,
	LoginInfoIncomplete = 113,
	RegisterPwdNotMatch = 114,  // ע�������ȷ�����벻ƥ��
	ConnectGameServerSuccess = 115, // �ɹ�������Ϸ������
	ConnectServerFailure = 116,
	ConnectLoginServerSucess = 117,
};