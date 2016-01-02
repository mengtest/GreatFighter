local skynet = require "skynet"
local util = require "lib.util"
local socket = require "socket"
local netpack = require "netpack"

local CMD = {}
local gateproxy
local client_id

function CMD.start(conf)
	local fd = conf.client
	client_id = fd

	gateproxy = conf.gateproxy

	skynet.call(conf.gate, "lua", "forward", fd)

	-- util.log_info("send data to client")
	-- socket.write(client_id, "welcome!")
end

function CMD.kill()
	skynet.exit()
end

local function on_recv(msg)
	util.log_info("recv from client msg = %s", msg)
	-- socket.write(client_id, msg)
end

skynet.register_protocol {
	name = "client",
	id = skynet.PTYPE_CLIENT,
	unpack = function (msg, sz)
		-- util.log_info("agent unpacking...")
		return skynet.tostring(msg, sz)
	end,
	dispatch = function (_, _, msg)
		on_recv(msg)
	end
}

skynet.start(function()
	skynet.dispatch("lua", function(session, source, cmd, ...)
		local functor = assert(CMD[cmd])
		skynet.ret(skynet.pack(functor(...)))
	end)
end)