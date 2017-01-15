--[["
    desc: gm指令
    author: wuyinjie
    since: 2017-01-15
 "]]

 local log = require "common.core.log"
 local igskynet = require "common.core.igskynet"
 local const = require "common.const"

 local cmd = {}

 function cmd.stopServer()
    igskynet.send(const.MASTER_FLOW, "dostop")
 end

 return cmd