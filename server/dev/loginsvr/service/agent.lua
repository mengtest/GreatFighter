--[["
    desc: 代理服务
    author: wuyinjie
    since: 2017-2-1
"]]

local igskynet = require "common.core.igskynet"
local igtimer = require "common.core.igtimer"
local class = require "common.core.class"
local base = require "common.core.svrbase"
local log = require "common.core.log"
local socketdriver = require "socketdriver"

local agent = class(base)
local agentObj = nil

function agent:ctor()
    self.msgList = {}
    self.fd = 0
    self.gate = nil
    self.watchdog = nil
end

function agent:dostart(config)
    log.info("agent|dostart")

    self.fd = config.fd 
    self.gate = config.gate
    self.watchdog = config.watchdog

    igskynet.originCall(self.gate, "lua", "forward", self.fd)
end

function agent:dostop()
    log.info("agent|dostop")
    igskynet.exit()
end

function agent:update()
    self:onPush()
end

function agent:onRecv(msg)
    log.info("agent|onRecv|msg = %s", msg)
    self:push(msg)
    log.info("agent|onRecv|msgList len = %d", #self.msgList)
end

function agent:onPush()
    if #self.msgList <= 0 then
        return
    end

    for idx, msg in ipairs(self.msgList) do 
        socketdriver.send(self.fd, msg)
    end
    self.msgList = {}
end

function agent:push(msg)
    -- TODO:protobuf encode

    local package = string.pack(">s2", msg)
    table.insert(self.msgList, package)
end

igskynet.register_protocol {
    name = "client",
    id = igskynet.PTYPE_CLIENT,
    unpack = function (msg, sz)
        return igskynet.tostring(msg, sz)
    end,
    dispatch = function (_, _, msg)
        -- TODO:protobuf decode

        agentObj:onRecv(msg)
    end
}

igskynet.start(function()
    agentObj = agent.new()
    igskynet.create(agentObj)

    igtimer.init()
    igtimer.timeout(1, function() agentObj:update() end)
end)