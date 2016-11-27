--[[
	desc:起服关服流程管理
	author:wuyinjie
	since:2016-11-27
]]

local igskynet = require "dev.common.core.igskynet"
local class = require "dev.common.core.class"
local flow = require "dev.common.flow.base"

local masterflow = class(flow)

function masterflow:ctor()
end

function masterflow:onEnter()
	igskynet.name(igskynet.self(), "masterflow")
end

function masterflow:onStopFinish(serviceAddr)
	if self.exitNotifyCount >= #self.addrs then
		-- TODO:notify gm process to kill all logic process
	end
end

return masterflow