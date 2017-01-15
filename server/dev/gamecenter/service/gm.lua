--[["
    desc: gm服务,暂时简陋实现关服处理
    author: wuyinjie
    since: 2017-01-15
 "]]

 local igskynet = require "common.core.igskynet"
 local log = require "common.core.log"
 local const = require "common.const"
 local class = require "common.core.class"
 local gmcmd = require "gm.command"

 local gm = class()

 function gm:ctor()
    self.commands = {}
    igskynet.name(const.GM_SERVICE, igskynet.self())
 end

 function gm:init()
    for cmdname, func in pairs(gmcmd) do 
        self:register(cmdname, func)
    end
 end

 function gm:register(cmd, func)
    self.commands[cmd] = func
 end

 function gm:run(cmd, params)
    local func = self.commands[cmd]
    if func then
        func(params)
        log.info("gm|execute command(%s)", cmd)
    else
        log.error("gm|can not find cmd(%s)", cmd)
    end
 end

 function gm:parseCommand(cmdline)
    local cmdname = cmdline -- TODO
    local params = {}

    return cmdname, params
 end

 function gm:executeCommand(cmdline)
    local cmd, params = self:parseCommand(cmdline)
    self:run(cmd, params)
 end

 igskynet.start(function() 
    local gmobj = gm.new()
    gmobj:init()
    igskynet.create(gmobj)
 end)