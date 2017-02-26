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
local socket = require "socket"
local const = require "common.const"
local json = require "cjson"
local client2server = require "res.client2server"
local captcha = require "captcha"

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

function agent:onRecv(params)
    local protoType = assert(params.protoType)
    local request = self[client2server[protoType]]
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

function agent:onRegisterNotify()
end

function agent:onLoginNotify()
end

------------client request-----------------
function agent:requestCaptcha()
    local filename = string.format("captcha_%d.jpg", os.time())
    local cap = captcha.new()

    cap:font("res/font/Vera.ttf")
    cap:string("123456")
    cap:bgcolor(61, 174, 233)
    cap:fgcolor(49, 54, 59)
    cap:line(true)

    cap:generate()
    cap:write(filename, 70);

    return { msgCode = 0, imageStream = cap:jpegStr(70) }
end

function agent:registerAccount()
end

function agent:tryLogin()
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
    igtimer.timeout(1, function() agentObj:update() end)
end)