--[[
	desc:起服关服流程管理
	author:wuyinjie
	since:2016-11-27
]]

local slaveflow = require "dev.common.flow.slaveflow"
local igskynet = require "dev.common.core.igskynet"

igskynet.start(function()
	local slaveflowObj = slaveflow.new()
	igskynet.create(slaveflowObj)
	slaveflowObj:onEnter()
end)