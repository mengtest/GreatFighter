--[["
    desc: 数据库代理
    author: wuyinjie
    since: 2017-2-18
"]]

local igskynet = require "common.core.igskynet"
local igtimer = require "common.core.igtimer"
local class = require "common.core.class"
local base = require "common.core.svrbase"
local log = require "common.core.log"
local const = require "common.const"
local reidslib = require "redis"
local dbconfig = require "dbconfig"
local cjson = require "cjson"

local redismgr = class(base)

function redismgr:ctor()
    igskynet.name(".redismgr", igskynet.self())
    self.dbObjs = {}
end

function redismgr:establishConnect()
    for k, v in pairs(dbconfig) do 
        if k == const.PLAYER_DB then
            for idx, cfg in ipairs(v) do 
                self.dbObjs[k .. idx] = reidslib.connect(cfg)
            end
        else
            self.dbObjs[k] = reidslib.connect(v)
        end
    end
end

function redismgr:dostart()
end

function redismgr:dostop()
    igskynet.send("slaveflow", "onLocalExitNotify", igskynet.self())
end

function redismgr:add(dbName, key, value)
    local obj = self.dbObjs[dbName]
    if not obj then
        log.info("redismgr|add|can not find db obj by name(%s)", dbName)
        return false
    end

    obj:set(key, cjson.encode(value))
    return true
end

function redismgr:del(dbName, key)
    local obj = self.dbObjs[dbName]
    if not obj then
        log.info("redismgr|del|can not find db obj by name(%s)", dbName)
        return false
    end    
    
    return obj:del(key) >= 1
end

function redismgr:query(dbName, key)
    local obj = self.dbObjs[dbName]
    if not obj then
        log.info("redismgr|query|can not find db obj by name(%s)", dbName)
        return nil
    end

    local jsonString = obj:get(key)
    if jsonString then
        return cjson.decode(jsonString)
    else
        return nil
    end
end

function redismgr:modify(dbName, key, value)
    local obj = self.dbObjs[dbName]
    if not obj then
        log.info("redismgr|modify|can not find db obj by name(%s)", dbName)
        return false
    end    

    obj:set(key, cjson.encode(value))
    return true
end

igskynet.start(function()
    local redisObj = redismgr.new()
    igskynet.create(redisObj)

    redisObj:establishConnect()
end)