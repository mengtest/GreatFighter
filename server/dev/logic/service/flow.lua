--[[
	desc:流程管理服务
	author:wuyinjie
	since:2016-4-9
]]

local skynetx = require "dev.core.skynetx"
local flow = require "dev.core.svrmgr"

function flow:ctor()
end

function flow:init()
end

function flow:dostart()
end

function flow:dostop()
end

function flow:dologin()
end

function flow:dologout()
end

function flow:dodaily()
end

function flow:doweekly()
end

function flow:dosave()
end

skynetx.start(function()
	skynetx.create(flow.new())
end)