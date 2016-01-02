local util = require "lib.util"
local skynet = require "skynet"
require "skynet.manager"

local is_lock = false
local function log_printer(msg)
	while is_lock do 
		-- just block
	end

	is_lock = true

	local timestamp, _ = math.floor(skynet.time())
	local date = util.convert_timestamp_to_date(timestamp)

	local file = io.open(string.format("../../../server-log/GreatFighter/logic.%d-%.2d-%.2d.txt", date.year, date.month, date.day), "a")
	file:write(string.format("%d-%.2d-%.2d %.2d:%.2d:%.2d: %s \n", date.year, date.month, date.day, date.hour, date.min, date.sec, msg))
	file:close()

	is_lock = false
end

skynet.register_protocol {
	name = "text",
	id = skynet.PTYPE_TEXT,
	unpack = skynet.tostring,
	dispatch = function(_, address, msg)
		log_printer(msg)
	end
}

skynet.start(function()
	skynet.register(".logger")
end)