local skynet = require "skynet"

local util = {}

-- format:
-- hour    
-- min     
-- wday   
-- year    
-- yday    
-- month   
-- sec     
-- day     
-- isdst
function util.convert_timestamp_to_date(timestamp)
	local date = os.date("*t", timestamp)
	return date
end

function util.log_info(formatstring, ...)
	local message = string.format(formatstring, ...)

	skynet.error(message)
end

return util