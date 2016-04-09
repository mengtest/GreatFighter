--[[
	desc:游戏大厅
	author:wuyinjie
	since:2016-4-9
]]

local skynetx = require "dev.core.skynetx"
local lobby = require "dev.core.svrmgr"

function lobby:ctor()
end

function lobby:init()
end

function lobby:onEnter()
end

function lobby:onExit()
end

function lobby:dologin()
end

function lobby:dologout()
end

function lobby:doQueueUp()
end

function lobby:cancelQueueUp()
end

function lobby:exitWaittingRoom()
end

function lobby:update()
end

skynetx.start(function()
	skynetx.create(lobby.new())
end)