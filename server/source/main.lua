local skynet = require "skynet"
local util = require "lib.util"

skynet.start(function()
	util.log_info("gate server test")

	local gateproxy = skynet.newservice("gateproxy")
	skynet.call(gateproxy, "lua", "start", {
			address = "192.168.1.110",
			port = 8888,
			maxclient = 1024,
			nodelay = true,
		})
end)