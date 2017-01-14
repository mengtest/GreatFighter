--[[
	desc:起服关服流程管理
	author:wuyinjie
	since:2016-11-27
]]

local slaveflow = require "common.flow.slaveflow"
local igskynet = require "common.core.igskynet"
local log = require "common.core.log"

local nodeName = ...

igskynet.start(function()
	local slaveflowObj = slaveflow.new(nodeName)
	igskynet.create(slaveflowObj)
	slaveflowObj:onEnter()
end)