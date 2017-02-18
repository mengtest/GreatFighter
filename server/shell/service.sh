#!/bin/bash

# /***
# * GreatFighter服务端启动关闭脚本
# * @author wuyinjie
# * @since 2017-01-11
# */

# 启动脚本
parentPath=$(dirname $(pwd))

function start_process() {
	ps -o command -C skynet | grep "$parentPath/skynet/skynet $parentPath/dev/$1/conf$2" &> /dev/null
	[ $? -eq 0 ] && echo "进程$1已经存在,禁止重复启动" && return
	echo "$parentPath/skynet/skynet $parentPath/dev/$1/conf$2 &> /dev/null &"
	nohup $parentPath/skynet/skynet $parentPath/dev/$1/conf$2 &> /dev/null &
}

function launch_all() {
	start_process gamecenter
	sleep 10

	start_process loginsvr
	start_process gm

	start_process gamesvr
	start_process battle
	start_process verify
}

function stop_process() {
	echo "killing $1 ..."
	res=`ps aux | grep "$1" | grep -v tail | grep -v grep | awk '{print $2}'`
	[ "$res" != "" ] && kill $res
}

function stop_all() {
	stop_process loginsvr
	stop_process gm

	stop_process gamesvr
	stop_process battle
	stop_process verify

	stop_process gamecenter
}

if [[ "$1" == "start" ]]; then
	launch_all
elif [[ "$1" == "stop" ]]; then
	stop_all
else
	echo "不存在$1指令"
fi