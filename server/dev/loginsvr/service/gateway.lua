--[["
    desc: gate服务
    author: wuyinjie
    since: 2017-02-01
 "]]

 local igskynet = require "common.core.igskynet"
 local log = require "common.core.log"

 local SOCKET = {}
 local CMD = {}

 local agents = {}
 local gate = nil

 local function closeClient(fd)
    if not agents[fd] then
        log.info("gateway|closeClient|can not find agent for fd = %d", fd)
    end

    igskynet.originCall(gate, "lua", "kick", fd)
    igskynet.send(agents[fd], "dostop")

    agents[fd] = nil
 end

 function SOCKET.open(fd, addr)
    log.info("gateway|open|new agent fd = %d, addr = %s", fd, addr)

    if agents[fd] then
        log.info("gateway|open|fd = %d has got agent", fd)
    else
        agents[fd] = igskynet.newservice("agent")
        igskynet.send(agents[fd], "dostart", { gate = gate, fd = fd, watchdog = igskynet.self() })
    end
 end

 function SOCKET.close(fd)
    closeClient(fd)
 end

 function SOCKET.data(fd, data)
 end

 function SOCKET.error(fd, msg)
    closeClient(fd)
 end

 function SOCKET.warning(fd, size)
 end

 function CMD.dostart()
    log.info("gateway dostart")

    assert(gate)

    local config = {}
    config.address = igskynet.getenv("gateway_ip") 
    config.port = igskynet.getenv("gateway_port")
    config.max_client = igskynet.getenv("max_client")
    config.nodelay = true
    igskynet.originCall(gate, "lua", "open", config)
 end

 function CMD.dostop()
    log.info("gateway dostop")
 end

 igskynet.start(function()
    igskynet.dispatch("lua", function(session, source, cmd, subcmd, ...)
        if cmd == "socket" then
            local functor = assert(SOCKET[subcmd], subcmd)
            functor(...)
        else
            local functor = assert(CMD[cmd], cmd)
            functor(subcmd, ...)
        end
    end)

    gate = igskynet.newservice("gate")
end)