--[[
	desc:起服关服流程管理
	author:wuyinjie
	since:2016-11-27
]]

local masterflow = require "common.flow.masterflow"
local igskynet = require "common.core.igskynet"
local log = require "common.core.log"

igskynet.start(function()
	local masterflowObj = masterflow.new()
	igskynet.create(masterflowObj)
	masterflowObj:onEnter()
end)