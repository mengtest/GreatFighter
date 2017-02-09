#pragma once
#include "Common/IGHeader.h"


class IIGProtoParser
{
public:
	IIGProtoParser();
	~IIGProtoParser();

	virtual string pack(const void* data) = 0;
	virtual void parse(const string& data) = 0;
	virtual void registerCallback(const std::function<void(string, const void* data)>& callback) = 0;
};

