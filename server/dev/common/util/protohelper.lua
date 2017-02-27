--[["
    desc: 协议层管理
    author: wuyinjie
    since: 2017-2-14
"]]

local client2server = require "res.client2server"
local server2client = require "res.server2client"

local protohelper = {}

function protohelper.pack(protoType, params)
    params.protoType = server2client[protoType]
    params.sessionID = 0

    return params
end

function protohelper.parse(protoType)
    return client2server[protoType]
end

return protohelper