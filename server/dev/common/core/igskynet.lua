--[[
	desc:服务功能管理类
	author:wuyinjie
	since:2016-11-27
]]

local skynet = require "skynet"

local PROTO_NAME = "lua"
local PROTO_TYPE = {}
PROTO_TYPE.CALL = 1
PROTO_TYPE.SEND = 2

local igskynet = {}

for k, v in pairs(skynet) do 
	igskynet[k] = v
end

function igskynet.create(svrobj)
	skynet.dispatch(PROTO_NAME, function(session, source, command, protoType, ...)
		local func = svrobj[command]
		if not func then
			skynet.error("unknow command %s", command)
			return
		end

		if protoType == PROTO_TYPE.CALL then
			return skynet.retpack(func(svrobj, ...))
		elseif protoType == PROTO_TYPE.SEND then
			return func(svrobj, ...)
		else
			skynet.error("unknow protoType %s", protoType)
		end
	end)
end

function igskynet.send(name, command, ...)
	skynet.send(name, PROTO_NAME, command, PROTO_TYPE.SEND, ...)
end

function igskynet.call(name, command, ...)
	return skynet.call(name, command, PROTO_TYPE.CALL, ...)
end

return igskynet