/////////////////////////////////////////////
// desc:协议序列化、反序列化辅助类
// author:wuyinjie
// since:2017-02-26
/////////////////////////////////////////////

#pragma once
#include "Common/IGHeader.h"
#include "Common/IGConst.h"

struct IGProtoMessage
{
	int protoType = 0;
	int sessionID = 0;
};

class IIGProtoHelper
{
public:
	IIGProtoHelper(){};
	~IIGProtoHelper(){};

	virtual void init() = 0;
	virtual string pack(IGClientProtoType ptype, int sessionID, const void* data){ return ""; };
	virtual void parse(const string& data) = 0;
	virtual void registerCallback(const std::function<void(const void*)>& callback) = 0;
};

