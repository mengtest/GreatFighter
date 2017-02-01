--[["
    desc: 自定义定时器
    author: wuyinjie
    since: 2017-2-1
"]]

local igskynet = require "common.core.igskynet"
local log = require "common.core.log"
local tableinsert = table.insert

-- 时间事件类型
local TIME_OF_INTER     = 0     -- 每隔多久
local TIME_OF_DAY       = 1     -- 每天
local TIME_OF_WEEK      = 2     -- 每周
local TIME_OF_MONTH     = 3     -- 每月
local TIME_OF_SPECIFIC  = 4     -- 特定时间
local TIME_OF_HOUR      = 5     -- 整数点
local INVALID_NUMBER    = -1    -- 无效数字

local igtimer = {}

--------- timeout ---------
local timeList = {}

-- desc: 每隔多久(精度为秒)
-- param: interval,间隔秒数
-- param: func,执行函数
function igtimer.timeout(interval, func)
    local timeObj = {}
    timeObj.type     = TIME_OF_INTER
    timeObj.interval = interval             
    timeObj.func     = func                 
    timeObj.doTime   = os.time() + interval 
    tableinsert(timeList, timeObj) 
end

-- 每天(指定时,分,秒,精度为秒)
function igtimer.timeday(hour, min, sec, func)
    local timeObj = {}
    timeObj.type     = TIME_OF_DAY  
    timeObj.func     = func         

    local nowTime = os.date("*t")
    timeObj.time = {}
    timeObj.time.year   = nowTime.year
    timeObj.time.month  = nowTime.month
    timeObj.time.day    = nowTime.day
    timeObj.time.hour   = hour
    timeObj.time.min    = min
    timeObj.time.sec    = sec

    if os.time(nowTime) >= os.time(timeObj.time) then
        timeObj.time.day = timeObj.time.day + 1 
    end

    timeObj.doTime = os.time(timeObj.time)  
    tableinsert(timeList, timeObj)
end

-- 每周(1表示周日,指定周几,时,分,秒,精度为秒)
function igtimer.dayweek(wday, hour, min, sec, func)
    local timeObj = {}
    timeObj.type     = TIME_OF_WEEK     
    timeObj.func     = func             

    local nowTime = os.date("*t")
    timeObj.time = {}
    timeObj.time.year   = nowTime.year
    timeObj.time.month  = nowTime.month
    timeObj.time.hour   = hour
    timeObj.time.min    = min
    timeObj.time.sec    = sec
    if nowTime.wday > wday then
        timeObj.time.day = nowTime.day + (5 + nowTime.wday - wday)
    elseif nowTime.wday == wday then
        timeObj.time.day = nowTime.day
        if os.time(nowTime) > os.time(timeObj.time) then
            timeObj.time.day = timeObj.time.day + 7
        end
    else
        timeObj.time.day = nowTime.day + (wday - nowTime.wday)
    end

    timeObj.doTime = os.time(timeObj.time) 
    
    tableinsert(timeList, timeObj) 
end

-- 每月(天,时,分,秒,精度为秒)
function igtimer.daymonth(day, hour, min, sec, func)
    local timeObj = {}
    timeObj.type        = TIME_OF_MONTH  
    timeObj.func        = func          

    local nowTime = os.date("*t")
    timeObj.time = {}
    timeObj.time.year   = nowTime.year
    timeObj.time.month  = nowTime.month
    timeObj.time.day    = day
    timeObj.time.hour   = hour
    timeObj.time.min    = min
    timeObj.time.sec    = sec

    if os.time(nowTime) >= os.time(timeObj.time) then
        timeObj.time.month = timeObj.time.month + 1
    end

    timeObj.doTime = os.time(timeObj.time)
    tableinsert(timeList, timeObj) 
end

-- 特定时间(指定年月日,时分秒,精度为秒)
function igtimer.spectime(year, month, day, hour, min, sec, func)
    local timeObj = {}
    timeObj.type    = TIME_OF_SPECIFIC  
    timeObj.func    = func              

    timeObj.time = {}
    timeObj.time.year   = year
    timeObj.time.month  = month
    timeObj.time.day    = day
    timeObj.time.hour   = hour
    timeObj.time.min    = min
    timeObj.time.sec    = sec

    local nowTime = os.time()
    local start_time = os.time(timeObj.time)
    if nowTime >= start_time then
        timeObj.doTime  = INVALID_NUMBER
    else
        timeObj.doTime  = start_time
    end
    
    tableinsert(timeList, timeObj)
end

-- 整点时间
function igtimer.hourtime(func)
    local timeObj = {}
    timeObj.type = TIME_OF_HOUR     
    timeObj.func = func             

    local nowTime = os.date("*t")
    timeObj.time = {}
    timeObj.time.year   = nowTime.year
    timeObj.time.month  = nowTime.month
    timeObj.time.day    = nowTime.day
    timeObj.time.hour   = nowTime.hour
    timeObj.time.min    = 0
    timeObj.time.sec    = 0

    if os.time(nowTime) >= os.time(timeObj.time) then
        timeObj.time.hour = timeObj.time.hour + 1
    end

    timeObj.doTime = os.time(timeObj.time)
    tableinsert(timeList, timeObj)
end

function igtimer.init()
    local function nextTime(timeObj)
        if timeObj.type == TIME_OF_INTER then       -- 间隔时间
            timeObj.doTime = timeObj.doTime + timeObj.interval
        elseif timeObj.type == TIME_OF_DAY then     -- 每天
            timeObj.time.day = timeObj.time.day + 1
            timeObj.doTime = os.time(timeObj.time)
        elseif timeObj.type == TIME_OF_WEEK then    -- 每周
            timeObj.time.day = timeObj.time.day + 7
            timeObj.doTime = os.time(timeObj.time)
        elseif timeObj.type == TIME_OF_MONTH then   -- 每月
            timeObj.time.month = timeObj.time.month + 1
            timeObj.doTime = os.time(timeObj.time)
        elseif timeObj.type == TIME_OF_SPECIFIC then -- 特定时间
            timeObj.doTime = INVALID_NUMBER
        elseif timeObj.type == TIME_OF_HOUR then    -- 整数点
            timeObj.time.hour = timeObj.time.hour + 1
            timeObj.doTime = os.time(timeObj.time)
        end
    end
    local function init()       
        local nowTime = os.time()
        for _, timeObj in pairs(timeList) do
            if timeObj.doTime ~= INVALID_NUMBER and nowTime >= timeObj.doTime then
                timeObj.func()
                nextTime(timeObj)           
            end
        end
        nowTime = nil
        -- timeout每500ms执行一次检测
        igskynet.timeout(50, init)
    end

    init()
end
--------- timeout ---------

--------- frame ---------
local frameList = {}
    
-- desc: 每隔多久(精度为10毫秒,skynet的内部时钟精度为1/100秒)
-- param: interval,间隔interval*10毫秒数,取值范围从1到100间,如果大于100则直接使用timeout即可,这个执行比timeout要更耗
-- param: func,执行函数
function igtimer.frame(interval, func)
    local frameObj = {interval = interval, func = func}
    tableinsert(frameList, frameObj)
end

function igtimer.initFrame()
    local count = 0
    local function init()
        local nowTime = os.time()
        for _, frameObj in pairs(frameList) do
            if frameObj.func and (count % frameObj.interval) == 0 then
                frameObj.func()
            end
        end

        nowTime = nil
        count = count + 1
        -- frame每10ms执行一次检测
        igskynet.timeout(1, init)
    end

    init()
end
--------- frame ---------

return igtimer