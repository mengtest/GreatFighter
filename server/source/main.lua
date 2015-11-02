local skynet = require "skynet"
local util = require "lib.util"

skynet.start(function()
	skynet.fork(function()
			while true do
				util.log_info("log test : hello skynet")
				skynet.sleep(100)
			end
		end)
end)