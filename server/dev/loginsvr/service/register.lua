--[["
    desc: 注册服务
    author: wuyinjie
    since: 2017-2-26
"]]

local igskynet = require "common.core.igskynet"
local igtimer = require "common.core.igtimer"
local class = require "common.core.class"
local base = require "common.core.svrbase"
local log = require "common.core.log"
local const = require "common.const"
local json = require "cjson"
local date = require "common.util.date"
local dbconfig = require "dbconfig"

local register = class(base)

function register:ctor()
    self.playerUuid = igskynet.call(".redismgr", "query", const.LOGIN_DB_SERVER, "playerUuid") or 10000

    self.accountQueue = {}
    igskynet.name(".register", igskynet.self())
end

function register:save()
    igskynet.send(".redismgr", "modify", const.LOGIN_DB_SERVER, self.playerUuid)
end

function register:getDataBaseIndex(playerUuid)
    local playerDB = dbconfig[const.PLAYER_DB]
    local idx = playerUuid % #playerDB + 1

    return idx
end

function register:update()
    local queueLength = #self.accountQueue
    for i = 1, queueLength do 
        local info = table.remove(self.accountQueue, 1)
        local userInfo = igskynet.call(".redismgr", "query", const.LOGIN_DB_ACCOUNT, info.userName)
        if userInfo then
            igskynet.send(info.agentAddr, "onRegisterNotify", const.REGISTER_NOTIFY_USER_EXIST, { userName = userInfo.userName, pwd = userInfo.pwd })
        else
            self.playerUuid = self.playerUuid + 1

            local playerData = {}
            playerData.playerUuid = self.playerUuid
            playerData.userName = info.userName
            playerData.pwd = info.pwd
            playerData.dbIdx = self:getDataBaseIndex(self.playerUuid)

            igskynet.send(".redismgr", "modify", const.LOGIN_DB_SERVER, self.playerUuid)
            igskynet.send(".redismgr", "add", const.LOGIN_DB_ACCOUNT, playerData.userName, playerData)
            igskynet.send(info.agentAddr, "onRegisterNotify", const.REGISTER_NOTIFY_SUCCESS, { userName = playerData.userName, pwd = playerData.pwd })
        end
    end

    -- 5分钟存一次库
    if date.now() % 300 == 0 then
        self:save()
    end
end

function register:dostop()
    self:save()
end

function register:insertRegisterQueue(userName, pwd, agentAddr)
    local info = {}
    info.userName = userName
    info.pwd = pwd
    info.agentAddr = agentAddr

    table.insert(self.accountQueue, info)
end

igskynet.start(function()
    local registerObj = register.new()
    igskynet.create(registerObj)

    igtimer.init()
    igtimer.timeout(1, function() registerObj:update() end)
end)