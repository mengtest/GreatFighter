--[["
    desc: gamecenter启动入口lua文件
    author: wuyinjie
    since: 2017-01-08
 "]]

 local igskynet = require "common.core.igskynet"
 local log = require "common.core.log"
 local cluster = require "cluster"
 local const = require "common.const"

 igskynet.start(function()
    log.info("server|gamecenter|start...")

    igskynet.newservice("debug_console", igskynet.getenv("debug_console_port"))
    local flowAddr = igskynet.uniqueservice("masterflow")

    igskynet.uniqueservice("gm")
    igskynet.uniqueservice("telnetlistener", "telnetcontroller", igskynet.getenv("gm_addr"))

    igskynet.call(flowAddr, "dostart")
    cluster.open(const.NODE_GAME_CENTER)
    log.info("server|gamecenter|booted")

    igskynet.exit()
 end)