--[[
	desc:状态机基类
	author:wuyinjie
	since:2016-4-9
]]

local class = require "dev.core.class"

local fsm = class()

function fsm:ctor()
end

function fsm:init()
end

function fsm:onEnter()
end

function fsm:onExit()
end

function fsm:isComplete()
end

function fsm:canInterruptTo()
end

function fsm:onMove()
end

function fsm:onSkill()
end

function fsm:onBeAttack()
end

return fsm