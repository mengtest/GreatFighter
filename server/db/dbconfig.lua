--[["
    desc: 数据库配置
    author: wuyinjie
    since: 2017-2-18
"]]

local const = require "common.const"

local config = {}

config[const.LOGIN_DB_PLAYER] = { host = "127.0.0.1", port = 6379, db = 1, auth = "foobared" }

return config