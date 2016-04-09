--[[
	desc:服务功能管理类
	author:wuyinjie
	since:2016-4-9
]]

local class = require "dev.core.class"
local log = require "dev.core.log"

local svrmgr = class()

function svrmgr:ctor()
	self.commands = {}
end

function svrmgr:init()
end

function svrmgr:register(command, func)
	self.commands[command] = function(...)
		func(self, ...)
	end
end

function svrmgr:run(command, param)
	local func = self.commands[command]
	if func then
		return func(param)
	else
		log.error("unknow cammand %s", command)
	end
end

function svrmgr:dostart()
end

function svrmgr:dostop()
end

function svrmgr:dologin()
end

function svrmgr:dologout()
end

function svrmgr:dodaily()
end

function svrmgr:doweekly()
end

function svrmgr:domouthly()
end

return svrmgr