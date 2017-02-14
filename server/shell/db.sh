#!/bin/bash

# /***
# * GreatFighter数据库进程启动关闭脚本
# * @author wuyinjie
# * @since 2017-02-14
# */

parentPath=$(dirname $(pwd))

if [[ "$1" == "start" ]]; then
	nohup $parentPath/3rd/redis-3.2.8/src/redis-server $parentPath/db/login.conf
elif [[ "$1" == "stop" ]]; then
	nohup $parentPath/3rd/redis-3.2.8/src/redis-cli -p 6379 shutdown
else
	echo "不存在$1指令"
fi