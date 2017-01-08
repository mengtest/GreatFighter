--[[
	desc:起服关服流程管理
	author:wuyinjie
	since:2016-11-27
]]

local igskynet = require "common.core.igskynet"
local class = require "common.core.class"
local flow = require "common.flow.base"
local cluster = require "cluster"
local const = require "common.const"

local slaveflow = class(flow)

function slaveflow:ctor()
end

function slaveflow:onEnter()
    local proxy = cluster.proxy(const.NODE_GAME_CENTER, const.MASTER_FLOW)
	igskynet.send(proxy, "register", igskynet.self())
end

function slaveflow:onStopFinish(serviceAddr)
	if self.exitNotifyCount >= #self.addrs then
		igskynet.send("masterflow", "onExitNotify", igskynet.self())
	end
end

return slaveflow