#!/bin/bash

# /***
# * GreatFighter服务端杀进程脚本, 注意要先执行关服指令，才能强杀进程，否则数据会丢失，后果很严重！！
# * @author wuyinjie
# * @since 2017-01-11
# */

function stop_process() {
	echo "killing $1 ..."
	res=`ps aux | grep "$1" | grep -v tail | grep -v grep | awk '{print $2}'`
	[ "$res" != "" ] && kill $res
}

stop_process loginsvr
stop_process gm

stop_process gamesvr
stop_process battle
stop_process verify

stop_process gamecenter