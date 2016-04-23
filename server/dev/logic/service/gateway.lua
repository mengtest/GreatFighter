--[[
	desc:网关
	author:wuyinjie
	since:2016-4-9
]]

local skynet = require "skynet"
local log = require "dev.core.log"

local socket = {}
local commands = {}
local gate

local agents = {}

local function closeSocket(fd)
	if not agents[fd] then
		log.info("fd = %d is not exist", fd)
		return
	end

	log.info("close cliend fd = %d", fd)

	skynet.call(gate, "lua", "kick", fd)
	skynet.send(agents[fd], "lua", "kill")

	agents[fd] = nil
end

function socket.open(fd, addr)
	log.info("fd = %d addr = %s new agent", fd, addr)

	if agents[fd] then
		log.info("fd = %d addr = %s is already exist", fd, addr)
	else
		agents[fd] = skynet.newservice("agent")
		skynet.call(agents[fd], "lua", "start", { gate = gate, client = fd, gateproxy = skynet.self() })
	end
end

function socket.data(fd, data)
	log.info("socket data")
end

function socket.error(fd, msg)
	closeSocket(fd)
end

function socket.warning(fd, size)
end

function socket.close(fd)
	closeSocket(fd)
end

function commands.start(conf)
	log.info("gateproxy start")

	skynet.call(gate, "lua", "open", conf)
end

function commands.close(fd)
	closeSocket(fd)
end

skynet.start(function()
	skynet.dispatch("lua", function(session, source, cmd, subcmd, ...)
		if cmd == "socket" then
			local functor = assert(socket[subcmd], subcmd)
			functor(...)
		else
			local functor = assert(commands[cmd], cmd)
			skynet.ret(skynet.pack(functor(subcmd, ...)))
		end
	end)

	gate = skynet.newservice("gate")
end)