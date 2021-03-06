--[["
    desc: gamesvr启动入口lua文件
    author: wuyinjie
    since: 2017-01-08
 "]]

 local igskynet = require "common.core.igskynet"
 local log = require "common.core.log"
 local cluster = require "cluster"
 local const = require "common.const"

 igskynet.start(function()
    log.info("================================")
    log.info("server|gamesvr" .. igskynet.getenv("node_idx") .. "|start...")

    igskynet.newservice("debug_console", igskynet.getenv("debug_console_port"))
    local flowAddr = igskynet.uniqueservice("slaveflow", const.NODE_GAME_SVR .. igskynet.getenv("node_idx"))

    local redisMgrAddr = igskynet.uniqueservice("redismgr")
    igskynet.send(flowAddr, "registerLocal", redisMgrAddr)

    local gateway = igskynet.uniqueservice("gateway")
    igskynet.send(flowAddr, "registerLocal", gateway)

    igskynet.call(flowAddr, "dostart")

    cluster.open(const.NODE_GAME_SVR .. igskynet.getenv("node_idx"))

    log.info("server|gamesvr" .. igskynet.getenv("node_idx") .. "|booted")
    igskynet.exit()
 end)