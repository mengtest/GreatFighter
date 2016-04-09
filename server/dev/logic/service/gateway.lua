--[[
	desc:网关
	author:wuyinjie
	since:2016-4-9
]]

local skynetx = require "dev.core.skynetx"
local gate = require "dev.core.svrmgr"

function gate:ctor()
end

function gate:init()
	self:register("connect", self.connect)
	self:register("message", self.message)
	self:register("error", self.error)
	self:register("disconnect", self.disconnect)
end

function gate:connect()
end

function gate:message()
end

function gate:error()
end

function gate:disconnect()
end

function gate:push()
end

skynetx.start(function()
	skynetx.create(gate.new())
end)