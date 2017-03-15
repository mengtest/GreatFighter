--[["
    desc: 玩家游戏逻辑代理
    author: wuyinjie
    since: 2017-3-1
"]]

local igskynet = require "common.core.igskynet"
local igtimer = require "common.core.igtimer"
local class = require "common.core.class"
local base = require "common.core.svrbase"
local log = require "common.core.log"
local socket = require "socket"
local const = require "common.const"
local json = require "cjson"
local date = require "common.util.date"
local protohelper = require "common.util.protohelper"
local msgcode = require "common.msgcode"
local crypt = require "crypt"
local player = require "logic.agent.player"

local agentObj = nil

local agent = class(base)

function agent:ctor()
    self.msgList = {}
    self.fd = 0
    self.gate = nil
    self.watchdog = nil
    self.player = nil
end

function agent:verify(params)
    local authInfo = igskynet.call("loginmgr", "getAuthInfo", params.user)
    if not authInfo then
        return { msgcode = msgcode.LOGIN_SERVER_NOT_AUTH }
    end

    local clientSecret = crypt.base64decode(params.secret)
    local serverSecret = crypt.base64decode(authInfo.secret)

    if clientSecret == serverSecret then
        igskynet.send("loginmgr",  "dologin", authInfo.playerUuid, igskynet.self())
        igskynet.send("loginmgr",  "clearAuthInfo", params.user)

        self.player = player.new()

        return { msgcode = msgcode.SUCCESS }
    else
        igskynet.send("loginmgr",  "clearAuthInfo", params.user)
        igtimer.timeout(10, function() self:onDelayExit() end)

        return { msgcode = msgcode.LOGIN_SERVER_NOT_AUTH }
    end
end

function agent:onDelayExit()
    igskynet.send(self.watchdog, "kick", self.fd)
end

function agent:isPlayerExist()
    return self.player ~= nil
end

function agent:onPush()
    if #self.msgList <= 0 then
        return
    end

    for idx, msg in ipairs(self.msgList) do 
        socket.write(self.fd, msg)
    end
    self.msgList = {}
end

function agent:push(params)
    local msg = json.encode(params)
    local package = string.pack(">s4", msg)
    table.insert(self.msgList, package)
end

function agent:update()
    self:onPush()

    if self:isPlayerExist() then
        self.player:update()
    end
end

function agent:onRecv(params)
    local protoType = assert(params.protoType)
    local protoName = protohelper.parse(protoType)

    if protoName ~= "verify" then
        assert(self:isPlayerExist(), "player try request before init player obj")
    end

    local request = self[protoName]
    if request then
        local ret = request(self, params)
        if params.response then
            params.response = assert(ret)
        end

        return params
    else
        return nil
    end
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

    -- save data
    igskynet.send("loginmgr", "dologout", self.playerUuid)

    igskynet.exit()
end

igskynet.register_protocol {
    name = "client",
    id = igskynet.PTYPE_CLIENT,
    unpack = function (msg, sz)
        return igskynet.tostring(msg, sz)
    end,
    dispatch = function (_, _, msg)
        local ret = agentObj:onRecv(json.decode(msg))
        if ret then
            local msg = json.encode(ret)
            local package = string.pack(">s4", msg)
            socket.write(self.fd, package)
        end
    end
}

igskynet.start(function()
    agentObj = agent.new()
    igskynet.create(agentObj)

    igtimer.init()
    igtimer.initFrame()
    igtimer.frame(5, function() agentObj:update() end)
end)