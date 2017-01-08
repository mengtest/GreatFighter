--[[
	desc:起服关服流程管理
	author:wuyinjie
	since:2016-11-27
]]

local igskynet = require "common.core.igskynet"
local class = require "common.core.class"
local flow = require "common.flow.base"

local slaveflow = class(flow)

function slaveflow:ctor()
end

function slaveflow:onEnter()
	igskynet.send("masterflow", "register", igskynet.self())
end

function slaveflow:onStopFinish(serviceAddr)
	if self.exitNotifyCount >= #self.addrs then
		igskynet.send("masterflow", "onExitNotify", igskynet.self())
	end
end

return slaveflow