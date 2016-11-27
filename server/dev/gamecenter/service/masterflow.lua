--[[
	desc:起服关服流程管理
	author:wuyinjie
	since:2016-11-27
]]

local masterflow = require "dev.common.flow.masterflow"
local igskynet = require "dev.common.core.igskynet"

igskynet.start(function()
	local masterflowObj = masterflow.new()
	igskynet.create(masterflowObj)
	masterflowObj:onEnter()
end)