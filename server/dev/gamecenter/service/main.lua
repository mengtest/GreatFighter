--[["
    desc: gamecenter启动入口lua文件
    author: wuyinjie
    since: 2017-01-08
 "]]

 local igskynet = require "common.core.igskynet"
 local log = require "common.core.log"
 local cluster = require "cluster"

 igskynet.start(function()
    log.info("server|gamecenter|start...")

    igskynet.newservice("debug_console", 8001)
    local flowAddr = igskynet.uniqueservice("gamecenter/masterflow")

    igskynet.call(flowAddr, "lua", "dostart")

    cluster.open(igskynet.getenv("node_name"))

    log.info("server|gamecenter|booted")
 end)