--[[
	desc:起服关服流程管理
	author:wuyinjie
	since:2016-11-27
]]

local class = require "dev.common.core.class"
local flow = require "dev.common.flow.base"

local slaveflow = class(flow)

function slaveflow:ctor()
end

function slaveflow:onEnter()
end

function slaveflow:onExitNotify(serviceAddr)
end

return slaveflow