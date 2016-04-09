--[[
	desc:实体管理类
	author:wuyinjie
	since:2016-4-9
]]

local class = require "dev.core.class"

local entitymgr = class(base)

function entitymgr:calcAoiMsg()
end

function entitymgr:calcPushMsg()
end

function entitymgr:addEntity()
end

function entitymgr:deleteEntity()
end

function getEntities()
end

function getEntityBy(instanceId)
end

function deleteAll()
end

return entitymgr