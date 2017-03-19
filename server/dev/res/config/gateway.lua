--[["
    desc: 网关配置
    author: wuyinjie
    since: 2017-3-7
"]]

local gateway = {}

gateway.loginsvr = {
	ip = "192.168.0.104",
	port = 8888,
	maxClient = 1024,
}

gateway.gamesvr1 = {
    ip = "192.168.0.104",
    port = 8889,
    maxClient = 1024,
}

return gateway