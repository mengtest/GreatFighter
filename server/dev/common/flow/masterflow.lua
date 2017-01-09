--[[
	desc:起服关服流程管理
	author:wuyinjie
	since:2016-11-27
]]

local igskynet = require "common.core.igskynet"
local class = require "common.core.class"
local flow = require "common.flow.base"
local const = require "common.const"

local masterflow = class(flow)

function masterflow:ctor()
end

function masterflow:onEnter()
	igskynet.name(const.MASTER_FLOW, igskynet.self())
end

function masterflow:onStopFinish(serviceAddr)
	if self.exitNotifyCount >= #self.addrs then
		-- TODO:notify gm process to kill all logic process
	end
end

return masterflow