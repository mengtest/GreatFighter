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
local captcha = require "captcha"
local date = require "common.util.date"
local protohelper = require "common.util.protohelper"
local msgcode = require "common.msgcode"
local crypt = require "crypt"

local CAPTCHA_CREATE_INTERVAL = 30
local CAPTCHA_NUM_LEN = 6

local agent = class(base)
local agentObj = nil

function agent:ctor()
    self.msgList = {}
    self.fd = 0
    self.gate = nil
    self.watchdog = nil

    self.captchaNumber = 0
    self.captchaValidTime = 0

    math.randomseed(date.now())
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
    local request = self[protohelper.parse(protoType)]
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

function agent:onRegisterNotify(type, info)
    if type == const.REGISTER_NOTIFY_SUCCESS then
        info.msgcode = msgcode.SUCCESS
        local notifyInfo = protohelper.pack("registerAccountNotify", info)
        self:push(notifyInfo)

    elseif type == const.REGISTER_NOTIFY_USER_EXIST then
        info.msgcode = msgcode.USER_NAME_EXISTED
        local notifyInfo = protohelper.pack("registerAccountNotify", info)
        self:push(notifyInfo)
    end
end

function agent:onLoginNotify()
end

------------client request-----------------
function agent:requestCaptcha()
    if date.now() < self.captchaValidTime then
        return { msgcode = msgcode.IN_CAPTCHA_CD }
    end

    local captchaStr = ""
    for i = 1, CAPTCHA_NUM_LEN do 
        captchaStr = captchaStr .. math.random(0, 9)
    end
    self.captchaNumber = tonumber(captchaStr)

    local filename = string.format("captcha_%d.jpg", igskynet.self())
    local cap = captcha.new()

    cap:font("../dev/res/font/Vera.ttf")
    cap:string(captchaStr)
    cap:bgcolor(61, 174, 233)
    cap:fgcolor(49, 54, 59)
    cap:line(true)

    cap:generate()
    cap:write(filename, 70);

    self.captchaValidTime = date.now() + CAPTCHA_CREATE_INTERVAL

    return { msgcode = msgcode.SUCCESS, imageStream = cap:jpegStr(70) }
end

local function isUserNameLegal(userName)
    -- TODO
    return true
end

function agent:registerAccount(params)
    if not isUserNameLegal(params.userName) then
        return { msgcode = msgcode.USER_NAME_ILLEGAL }
    end

    if date.now() > self.captchaValidTime then
        return { msgcode = msgcode.CAPTCHA_EXPIRED }
    end

    if self.captchaNumber ~= tonumber(params.captcha) then
        return { msgcode = msgcode.CAPTCHA_INCORRECT }
    end

    igskynet.send("register", "insertRegisterQueue", params.userName, params.pwd, igskynet.self())

    return { msgcode = msgcode.WAITTING_FOR_REGISTER }
end

function agent:tryLogin(params)
    local userName = params.userName
    local pwd = params.pwd

    if not userName then
        return { msgcode = msgcode.ACCOUNT_NOT_EXIST }
    end

    if not pwd then
        return { msgcode = msgcode.PWD_INCORRECT }
    end

    if date.now() > self.captchaValidTime then
        return { msgcode = msgcode.CAPTCHA_EXPIRED }
    end

    if self.captchaNumber ~= tonumber(params.captcha) then
        return { msgcode = msgcode.CAPTCHA_INCORRECT }
    end

    local dbInfo = igskynet.call("redismgr", "query", const.LOGIN_DB_ACCOUNT, userName)
    if not dbInfo then
        return { msgcode = msgcode.ACCOUNT_NOT_EXIST }
    end

    if pwd ~= dbInfo.pwd then
        return { msgcode = msgcode.PWD_INCORRECT }
    end

    local user = userName .. "@" .. date.now()
    local secret = crypt.base64encode(pwd .. "@" .. date.now()) -- TODO 以后要用真正的secret生成算法，暂时写死
    local loginNode = igskynet.call("loginmgr", "getBestLoginNode")

    local authInfo = {}
    authInfo.user = user
    authInfo.secret = secret
    authInfo.dbInfo = dbInfo
    igskynet.send("loginmgr", "registerAuthInfo", dbInfo.playerUuid, authInfo)

    return { msgcode = msgcode.SUCCESS, user = user, secret = secret, loginNode = loginNode }
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