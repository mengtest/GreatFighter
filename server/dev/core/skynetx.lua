--[[
	desc:服务功能管理类
	author:wuyinjie
	since:2016-4-9
]]

local skynet = require "skynet"
local log = require "dev.core.log"

local PROTO_NAME = "lua"
local PROTO_TYPE = {}
PROTO_TYPE.CALL = 1
PROTO_TYPE.SEND = 2

for k, v in pairs(skynet) do 
	skynetx[k] = v
end

function skynetx.create(svrmgr)
	skynet.dispatch(PROTO_NAME, function(session, source, command, protoType, ...)
		if protpType == PROTO_TYPE.CALL then
			return skynet.retpack(svrmgr:run(command, ...))
		elseif protpType == PROTO_TYPE.SEND then
			return svrmgr:run(command, ...)
		else
			log.error("unknow protoType %s", protoType)
		end
	end)
end

function skynetx.send(name, command, ...)
	skynet.send(name, PROTO_NAME, command, PROTO_TYPE.SEND, ...)
end

function skynetx.call(name, command, ...)
	return skynet.call(name, command, PROTO_TYPE.CALL, ...)
end

return skynetx