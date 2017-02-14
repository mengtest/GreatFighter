#pragma once
#include "Common/IGHeader.h"


class IIGProtoHelper
{
public:
	IIGProtoHelper();
	~IIGProtoHelper();

	virtual void init() = 0;
	virtual string pack(const string& protoName, int sessionID, const void* data) = 0;
	virtual void parse(const string& data) = 0;
	virtual void registerCallback(const std::function<void(string, const void* data)>& callback) = 0;
};

