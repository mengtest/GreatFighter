--[[
	desc:起服关服流程管理
	author:wuyinjie
	since:2016-11-27
]]

local class = require "dev.common.core.class"
local svrbase = require "dev.common.core.svrbase"
local flow = class(svrbase)

function flow:ctor()
end

function flow:onEnter()
end

function flow:register()
end

function flow:dostart()
end

function flow:dostop()
end

function flow:onExitNotify(serviceAddr)
end

return flow