--[[
	desc:起服关服流程管理
	author:wuyinjie
	since:2016-11-27
]]

local igskynet = require "common.core.igskynet"
local class = require "common.core.class"
local base = require "common.flow.base"
local cluster = require "cluster"
local const = require "common.const"
local log = require "common.core.log"

local slaveflow = class(base)

function slaveflow:ctor(nodeName)
    base.ctor(self)
    self.nodeName = nodeName
end

function slaveflow:onEnter()
    igskynet.name(".slaveflow", igskynet.self())

    local proxy = cluster.proxy(const.NODE_GAME_CENTER, "masterflow")
	igskynet.send(proxy, "registerSlave", self.nodeName, "slaveflow")
end

function slaveflow:dostop()
    base.dostop(self)

    if #self.addrs <= 0 then
        self:onStopFinish()
    end
end

function slaveflow:onStopFinish()
	if self.exitServiceCount >= #self.addrs then
        local proxy = cluster.proxy(const.NODE_GAME_CENTER, "masterflow")
		igskynet.send(proxy, "onSlaveExitNotify", self.nodeName)
	end
end

return slaveflow