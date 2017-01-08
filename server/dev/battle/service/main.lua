--[["
    desc: battle启动入口lua文件
    author: wuyinjie
    since: 2017-01-08
 "]]

 local igskynet = require "common.core.igskynet"
 local log = require "common.core.log"
 local cluster = require "cluster"
 local const = require "common.const"

 igskynet.start(function()
    log.info("server|battle" .. igskynet.getenv("node_idx") .. "|start...")

    igskynet.newservice("debug_console", igskynet.getenv("debut_console_port"))
    local flowAddr = igskynet.uniqueservice("battle/service/slaveflow")

    igskynet.call(flowAddr, "lua", "dostart")

    cluster.open(const.NODE_BATTLE .. igskynet.getenv("node_idx"))

    log.info("server|battle" .. igskynet.getenv("node_idx") .. "|booted")
    igskynet.exit()
 end)