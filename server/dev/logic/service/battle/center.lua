--[[
	desc:战斗服务管理中心
	author:wuyinjie
	since:2016-4-9
]]

local skynetx = require "dev.core.skynetx"
local battlecenter = require "dev.core.svrmgr"

function battlecenter:ctor()
end

function battlecenter:init()
end

function battlecenter:dologout()
end

function battlecenter:reservation()
end

function battlecenter:enterBattleField()
end

function battlecenter:exitBattleField()
end

function battlecenter:onRecvRequest()
end

function battlecenter:pushMsg()
end

function battlecenter:update()
end

skynetx.start(function()
	skynetx.create(battlecenter.new())
end)