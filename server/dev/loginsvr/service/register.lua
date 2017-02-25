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
local socket = require "socket"
local const = require "common.const"
local json = require "cjson"

local register = class(base)

function register:ctor()
end

function register:update()
end

function register:addRegisterQueue()
end

igskynet.start(function()
    local registerObj = register.new()
    igskynet.create(registerObj)

    igtimer.init()
    igtimer.timeout(1, function() registerObj:update() end)
end)