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

-- 数据库名称
const.LOGIN_DB_SERVER = "loginServer"   -- 登录服数据
const.LOGIN_DB_ACCOUNT = "loginAccount"  -- 登陆账号数据
const.PLAYER_DB = "playerDB" 

-- 注册、登陆相关
local LOGIN_START_IDX = 100
const.REGISTER_NOTIFY_USER_EXIST = LOGIN_START_IDX + 1
const.REGISTER_NOTIFY_SUCCESS = LOGIN_START_IDX + 2

return const