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
end

function loginmgr:registerAuthInfo()
end

function loginmgr:getAuthInfo()
end

function loginmgr:getOnlinePlayerCount()
end

-- 根据负载均衡的算法，选出最佳登陆节点
function loginmgr:getLoginNodeInfo()
end

function loginmgr:dologin()
end

function loginmgr:dologout()
end

igskynet.start(function()
    local loginmgrObj = loginmgr.new()
    igskynet.create(loginmgrObj)
end)