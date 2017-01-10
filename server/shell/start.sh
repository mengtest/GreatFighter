#!/bin/bash

# /***
# * GreatFighter服务端启动脚本
# * @author wuyinjie
# * @since 2017-01-11
# */
·
# 启动脚本
parentPath=$(dirname $(pwd))

function start_process() {
	ps -o command -C skynet | grep "$parentPath/skynet/skynet $parentPath/dev/$1/conf$2" &> /dev/null
	[ $? -eq 0 ] && echo "进程$1已经存在,禁止重复启动" && return
	echo "$parentPath/skynet/skynet $parentPath/dev/$1/conf$2 &> /dev/null &"
	nohup $parentPath/skynet/skynet $parentPath/dev/$1/conf$2 &> /dev/null &
}

start_process gamecenter
sleep 10

start_process loginsvr
start_process gm

start_process gamesvr
start_process battle
start_process verify