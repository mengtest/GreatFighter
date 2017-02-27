--[["
    desc: 数据库配置
    author: wuyinjie
    since: 2017-2-18
"]]

local const = require "common.const"

local config = {}

config[const.LOGIN_DB_ACCOUNT] = { host = "127.0.0.1", port = 6379, db = 1, auth = "foobared" }
config[const.LOGIN_DB_SERVER] = { host = "127.0.0.1", port = 6379, db = 2, auth = "foobared" }
config[const.PLAYER_DB] = {
    [1] = { host = "127.0.0.1", port = 6379, db = 3, auth = "foobared" },
    [2] = { host = "127.0.0.1", port = 6379, db = 4, auth = "foobared" },
    [3] = { host = "127.0.0.1", port = 6379, db = 5, auth = "foobared" },
}

return config