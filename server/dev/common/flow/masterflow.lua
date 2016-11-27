--[[
	desc:起服关服流程管理
	author:wuyinjie
	since:2016-11-27
]]

local class = require "dev.common.core.class"
local flow = require "dev.common.flow.base"

local masterflow = class(flow)

function masterflow:ctor()
end

function masterflow:onEnter()
end

function masterflow:onExitNotify(serviceAddr)
end

return masterflow