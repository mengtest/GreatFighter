--[["
    desc: loginsvr启动入口lua文件
    author: wuyinjie
    since: 2017-01-08
 "]]

 local igskynet = require "common.core.igskynet"
 local log = require "common.core.log"
 local cluster = require "cluster"
 local const = require "common.const"

 igskynet.start(function()
    log.info("================================")
    log.info("server|loginsvr|start...")

    igskynet.newservice("debug_console", igskynet.getenv("debug_console_port"))
    local flowAddr = igskynet.uniqueservice("slaveflow", const.NODE_LOGIN_SVR)

    local redisMgrAddr = igskynet.uniqueservice("redismgr")
    igskynet.send(flowAddr, "registerLocal", redisMgrAddr)

    local registerAddr = igskynet.uniqueservice("register")
    igskynet.send(flowAddr, "registerLocal", registerAddr)

    local gateway = igskynet.uniqueservice("gateway")
    igskynet.send(flowAddr, "registerLocal", gateway)

    igskynet.call(flowAddr, "dostart")

    cluster.open(const.NODE_LOGIN_SVR)

    log.info("server|loginsvr|booted")
    igskynet.exit()
 end)