--[[
	desc:日志输出
	author:wuyinjie
	since:2016-4-9
]]

local skynet = require "skynet"
local log = {}

local function print(typeStr, format, ...)
	local message = string.format("[" .. typeStr .. "] " .. format, ...)
	skynet.error(message)
end

function log.info(format, ...)
	print("info", format, ...)
end

function log.error(format, ...)
	print("error", format, ...)
end

return log