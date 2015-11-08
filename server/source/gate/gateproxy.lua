local skynet = require "skynet"
local util = require "lib.util"

local SOCKET = {}
local CMD = {}
local gate

local agents = {}

local function close_client(fd)
	if not agents[fd] then
		util.log_info("fd = %d is not exist", fd)
		return
	end

	util.log_info("close cliend fd = %d", fd)

	skynet.call(gate, "lua", "kick", fd)
	skynet.send(agents[fd], "lua", "kill")

	agents[fd] = nil
end

function SOCKET.open(fd, addr)
	util.log_info("fd = %d addr = %s new agent", fd, addr)

	if agents[fd] then
		util.log_info("fd = %d addr = %s is already exist", fd, addr)
	else
		agents[fd] = skynet.newservice("agent")
		skynet.call(agents[fd], "lua", "start", { gate = gate, client = fd, gateproxy = skynet.self() })
	end
end

function SOCKET.data(fd, data)
end

function SOCKET.error(fd, msg)
	close_client(fd)
end

function SOCKET.warning(fd, size)
end

function SOCKET.close(fd)
	close_client(fd)
end

function CMD.start(conf)
	util.log_info("gateproxy start")

	skynet.call(gate, "lua", "open", conf)
end

function CMD.close(fd)
	close_client(fd)
end

skynet.start(function()
	skynet.dispatch("lua", function(session, source, cmd, subcmd, ...)
		if cmd == "socket" then
			local functor = assert(SOCKET[subcmd], subcmd)
			functor(...)
		else
			local functor = assert(CMD[cmd], cmd)
			skynet.ret(skynet.pack(functor(subcmd, ...)))
		end
	end)

	gate = skynet.newservice("gate")
end)