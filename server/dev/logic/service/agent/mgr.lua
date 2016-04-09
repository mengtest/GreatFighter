--[[
	desc:agent管理
	author:wuyinjie
	since:2016-4-9
]]

local skynetx = require "dev.core.skynetx"
local agent = require "dev.core.svrmgr"

function agent:ctor()
end

function agent:init()
	self:register("loginPlayer", self.loginPlayer)
	self:register("logoutPlayer", self.logoutPlayer)
	self:register("dosave", self.dosave)
	self:register("dodaily", self.dodaily)
	self:register("doweekly", self.doweekly)
	self:register("pushMsg", self.pushMsg)
	self:register("startGame", self.startGame)
end

-- login和logout事件是由agent发起的
function agent:loginPlayer()
end

function agent:logoutPlayer()
end

function agent:dosave()
end

function agent:dodaily()
end

function agent:doweekly()
end

function agent:createPlayer()
end

-- 将消息推送给客户端
function agent:sendToClient()
end

-- 接收来自其他服务的推送消息
function agent:pushMsg()
end

function agent:startGame()
end

function agent:update()
end

skynetx.start(function()
	skynetx.create(agent.new())
end)