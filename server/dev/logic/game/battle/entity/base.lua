--[[
	desc:实体基类
	author:wuyinjie
	since:2016-4-9
]]

local class = require "dev.core.class"
local entity = class()

function entity:ctor()
end

function entity:init()
end

function entity:getInstanceId()
end

function entity:getForward()
end

function entity:setForward()
end

function entity:getPosition()
end

function entity:setPosition()
end

function entity:update()
end

function entity:onAttack()
end

function entity:onBeAttack()
end

function entity:canAttack()
end

function entity:canBeAttack()
end

function entity:getGangId()
end

function entity:setGangId()
end

function entity:getType()
end

function entity:getAttribute()
end

function entity:setAttribute()
end

return entity