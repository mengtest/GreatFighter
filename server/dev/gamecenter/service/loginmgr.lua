--[["
    desc: 玩家登陆管理中心
    author: wuyinjie
    since: 2017-2-26
"]]

local igskynet = require "common.core.igskynet"
local igtimer = require "common.core.igtimer"
local class = require "common.core.class"
local base = require "common.core.svrbase"
local log = require "common.core.log"
local socket = require "socket"
local const = require "common.const"
local json = require "cjson"

local loginmgr = class(base)

function loginmgr:ctor()
    igskynet.name("loginmgr", igskynet.self())

    self.onlinePlayers = {}
    self.authInfo = {}
end

function loginmgr:registerAuthInfo(playerUuid, authInfo)
    self.authInfo[playerUuid] = authInfo
end

function loginmgr:clearAuthInfo(playerUuid)
    self.authInfo[playerUuid] = nil
end

function loginmgr:getAuthInfo(playerUuid)
    return self.authInfo[playerUuid]
end

function loginmgr:getOnlinePlayerCount()
    local count = 0
    for k, v in pairs(self.onlinePlayers) do 
        count = count + 1
    end
    return count
end

-- 根据负载均衡的算法，选出最佳登陆节点
function loginmgr:getBestLoginNode()
end

function loginmgr:dostart()
end

function loginmgr:dostop()
    for k, v in pairs(self.onlinePlayers) do 
        igskynet.call(k, "kick")
    end

    igskynet.send("masterflow", "onLocalExitNotify", igskynet.self())
end

function loginmgr:dologin(playerUuid, agentAddr)
    self.onlinePlayers[playerUuid] = agentAddr
end

function loginmgr:dologout(playerUuid)
    self.onlinePlayers[playerUuid] = nil
end

igskynet.start(function()
    local loginmgrObj = loginmgr.new()
    igskynet.create(loginmgrObj)
end)