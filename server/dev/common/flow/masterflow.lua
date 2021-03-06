--[[
	desc:起服关服流程管理
	author:wuyinjie
	since:2016-11-27
]]

local igskynet = require "common.core.igskynet"
local class = require "common.core.class"
local base = require "common.flow.base"
local const = require "common.const"
local cluster = require "cluster"
local log = require "common.core.log"

local masterflow = class(base)

function masterflow:ctor()
    base.ctor(self)

    self.slaveflowInfo = {}
    self.exitSlaveCount = 0
end

function masterflow:onEnter()
	igskynet.name("masterflow", igskynet.self())
end

function masterflow:dostop()
    base.dostop(self)

    for k, v in ipairs(self.slaveflowInfo) do 
        local proxy = cluster.proxy(v.nodeName, v.slaveflowName)
        igskynet.send(proxy, "dostop")
    end

    if #self.addrs <= 0 and #self.slaveflowInfo <= 0 then
        self:onStopFinish()
    end
end

function masterflow:registerSlave(nodeName, slaveflowName)
    local info = {}
    info.nodeName = nodeName
    info.slaveflowName = slaveflowName

    table.insert(self.slaveflowInfo, info)

    log.info("gamecenter masterflow registerSlave nodeName(%s) slaveflowName(%s)", nodeName, slaveflowName)
end

function masterflow:onSlaveExitNotify(nodeName)
    log.info("masterflow|slave(%s) exit notify", nodeName)

    local hasFind = false
    for idx, info in ipairs(self.slaveflowInfo) do 
        if info.nodeName == nodeName then
            hasFind = true
            break
        end
    end

    if hasFind then
        self.exitSlaveCount = self.exitSlaveCount + 1
        self:onStopFinish()
    end
end

function masterflow:onStopFinish()
	if self.exitServiceCount >= #self.addrs and self.exitSlaveCount >= #self.slaveflowInfo then
		log.info("masterflow|notify web process to kill all logic process")
	end
end

return masterflow