--[["
    desc: 错误码
    author: wuyinjie
    since: 2017-2-27
"]]

local msgcode = {}

msgcode.SUCCESS = 0

local LOGIN_START_IDX = 100
msgcode.USER_NAME_ILLEGAL = LOGIN_START_IDX + 1
msgcode.IN_CAPTCHA_CD = LOGIN_START_IDX + 2
msgcode.CAPTCHA_INCORRECT = LOGIN_START_IDX + 3
msgcode.WAITTING_FOR_REGISTER = LOGIN_START_IDX + 4 -- 等待注册
msgcode.USER_NAME_EXISTED = LOGIN_START_IDX + 5     -- 用户名已经被注册
msgcode.ACCOUNT_NOT_EXIST = LOGIN_START_IDX + 6     -- 用户不存在
msgcode.PWD_INCORRECT = LOGIN_START_IDX + 7
msgcode.CAPTCHA_EXPIRED = LOGIN_START_IDX + 8

return msgcode