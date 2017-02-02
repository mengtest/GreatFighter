--[[
    desc:常量
    author:wuyinjie
    since:2017-01-08
]]

local const = {}

-- 进程节点名称
const.NODE_GAME_CENTER = "gamecenter"
const.NODE_GAME_SVR = "gamesvr"
const.NODE_LOGIN_SVR = "loginsvr"
const.NODE_BATTLE = "battle"
const.NODE_GM = "gm"
const.NODE_VERIFY = "verify"

const.MASTER_FLOW = ".masterflow" -- 启服、关服控制中心名称
const.SLAVE_FLOW = ".slaveflow"
const.GM_SERVICE = ".gm"

const.MAX_NETWORK_BYTES = 65535

return const