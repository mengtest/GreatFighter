--[[
	desc:战斗服务
	author:wuyinjie
	since:2016-4-9
]]

local skynetx = require "dev.core.skynetx"
local battlefield = require "dev.core.svrmgr"

function battlefield:ctor()
end

function battlefield:init()
end

function battlefield:dologout()
end

function battlefield:skill()
end

function battlefield:move()
end

function battlefield:onEnter()
end

function battlefield:onExit()
end

function battlefield:pushMsg()
end

function battlefield:update()
end

skynetx.start(function()
	skynetx.create(battlefield.new())
end)