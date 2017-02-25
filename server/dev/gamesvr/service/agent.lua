--[["
    desc: 玩家游戏逻辑代理
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

local agent = class(base)

function agent:ctor()
end

function agent:update()
end

function agent:dologin()
end

function agent:dologout()
end

igskynet.start(function()
    local agentObj == agent.new()
    igskynet.create(agentObj)

    igtimer.init()
    igtimer.timeout(1, function() agentObj:update() end)
end)