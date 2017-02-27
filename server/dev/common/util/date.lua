--[["
    desc: 时间和日期库
    author: wuyinjie
    since: 2017-2-27
"]]

local igskynet = require "common.core.igskynet"

local date = {}

function date.now()
    return igskynet.now()
end

return date