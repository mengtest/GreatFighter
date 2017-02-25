--[["
    desc: 玩家登陆验证服务
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

local auth = class(base)

function auth:ctor()
end

function auth:verify()
end

igskynet.create(function()
    local authObj = auth.new()
    igskynet.create(authObj)
end)