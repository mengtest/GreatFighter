--[[
	desc:玩家类
	author:wuyinjie
	since:2016-4-9
]]

local class = require "dev.core.class"
local base = require "game.battle.entity.role"

local player = class(base)

function player:getPlayerId()
end

return player