--[["
    desc: telnet指令接收和解析服务
    author: wuyinjie
    since: 2017-01-15
 "]]

 local igskynet = require "common.core.igskynet"
 local log = require "common.core.log"
 local const = require "common.const"
 local class = require "common.core.class"
 local socket = require "socket"

 local sock = ...

 local controller = class()

 function controller:ctor(sock)
    self.fd = socket.bind(sock)
    self.sock = sock
 end

 function controller:readline()
    return socket.readline(self.fd, "\n")
 end

 function controller:close()
    socket.close(self.fd)
 end

 local function dispatchCmd(cmdline)
    if cmdline:sub(-1) == "\r" then
        cmdline = cmdline:sub(1,-2)
    end

    if cmdline == "" then
        return
    end

    log.info("exec cmd:", cmdline)
    igskynet.call("gm", "lua", "executeCommand", cmdline)
 end

 function controller:mainLoop()
    log.info("launch message loop for socket:%d", sock)
    while true do 
        local cmd = self:readline()
        if not cmd then
            log.info("read cm failed!!")
            break
        end

        log.info("read cmd==============+:", cmd)
        igskynet.fork(dispatchCmd, cmd)
    end
    log.info("stop message loop for socket:%d", sock)
 end

 igskynet.start(function()
    local ctrl = controller.new(sock)
    ctrl:mainLoop()
    igskynet.exit()
 end)