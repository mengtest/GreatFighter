--[["
    desc: telnet监听服务
    author: wuyinjie
    since: 2017-01-15
 "]]

 local igskynet = require "common.core.igskynet"
 local log = require "common.core.log"
 local const = require "common.const"
 local class = require "common.core.class"

 local watcher, addr = ...

 igskynet.register_protocol {
    name = "text",
    id = igskynet.PTYPE_TEXT,
    pack = function (...)
        local n = select("#" , ...)
        if n == 0 then
            return ""
        elseif n == 1 then
            return tostring(...)
        else
            return table.concat({...}," ")
        end
    end,
    unpack = igskynet.tostring
 }

 local listener = class()

 function listener:ctor(address)
    local ip = nil
    local port = nil

    local pos = string.find(address, ":")
    if pos then
        ip = string.sub(address, 1, pos-1)
        port = assert(tonumber(string.sub(address, pos+1)))
    else
        port = assert(tonumber(address))
    end
    
    igskynet.launch("telnetd", igskynet.self(), ip, port)
    log.info("telnetd booted")
 end

 function listener:createController(watcher, message)
    igskynet.newservice(watcher, message)
 end

 igskynet.start(function()
    local telnetListener = listener.new(addr)
    igskynet.dispatch("text", function(session, address, message) 
        telnetListener:createController(watcher, message)
    end)
 end)