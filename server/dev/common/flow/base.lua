--[[
	desc:起服关服流程管理
	author:wuyinjie
	since:2016-11-27
]]

local igskynet = require "common.core.igskynet"
local class = require "common.core.class"
local svrbase = require "common.core.svrbase"
local log = require "common.core.log"
local flow = class(svrbase)

function flow:ctor()
	self.addrs = {} -- 被管理的服务地址
	self.exitServiceCount = 0
end

function flow:onEnter()
end

function flow:registerLocal(addr)
	local hasFind = false
	for idx, serviceAddr in ipairs(self.addrs) do 
		if serviceAddr == addr then
			hasFind = true
			break
		end
	end

	if hasFind then
		-- log.error
		return 
	end

	table.insert(self.addrs, addr)
end

function flow:dostart()
	for idx, addr in ipairs(self.addrs) do 
		igskynet.send(addr, "dostart")
	end
end

function flow:dostop()
	for idx, addr in ipairs(self.addrs) do 
		igskynet.send(addr, "dostop")
	end
end

function flow:onLocalExitNotify(serviceAddr)
	local hasFind = false 
	for idx, addr in ipairs(self.addrs) do 
		if addr == serviceAddr then
			hasFind = true
			break
		end
	end

	if hasFind then
		self.exitServiceCount = self.exitServiceCount + 1
		self:onStopFinish()
	end
end

function flow:onStopFinish()
end

return flow