--[["
    desc: 客户端协议ID转服务端协议名
    author: wuyinjie
    since: 2017-2-26
"]]

local client2server = {}

client2server[1] = "requestCaptcha"
client2server[2] = "registerAccount"
client2server[3] = "tryLogin"

return client2server