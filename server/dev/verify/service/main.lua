--[["
    desc: verify启动入口lua文件
    author: wuyinjie
    since: 2017-01-08
 "]]

 local igskynet = require "common.core.igskynet"
 local log = require "common.core.log"
 local cluster = require "cluster"
 local const = require "common.const"

 igskynet.start(function()
    log.info("================================")
    log.info("server|verify" .. igskynet.getenv("node_idx") .. "|start...")

    igskynet.newservice("debug_console", igskynet.getenv("debug_console_port"))
    local flowAddr = igskynet.uniqueservice("slaveflow", const.NODE_VERIFY .. igskynet.getenv("node_idx"))

    igskynet.call(flowAddr, "dostart")

    local node_name = assert(const.NODE_VERIFY .. igskynet.getenv("node_idx"))
    log.info(node_name)
    cluster.open(node_name)

    log.info("server|verify" .. igskynet.getenv("node_idx") .. "|booted")
    igskynet.exit()
 end)