#!/bin/bash

# /***
# * GreatFighter编译、清理脚本
# * @author wuyinjie
# * @since 2017-01-11
# */

parentPath=$(dirname $(pwd))

function build() {
	# 编译skynet
	echo "====================="
	echo "start build skyent..."
	cd $parentPath/skynet
	make linux MALLOC_STATICLIB= SKYNET_DEFINES=-DNOUSE_JEMALLOC

	# 编译log日志服务
	echo "====================="
	echo "start build service log..."
	cd $parentPath/3rd/service-log
	make

	# 编译telnet服务
	echo "====================="
	echo "start build service telnet..."
	cd $parentPath/3rd/service-telnetd
	make

	# 编译cjson库
	echo "====================="
	echo "start build lua-cjson..."
	cd $parentPath/3rd/lua-cjson
	make

	# 编译redis
	echo "====================="
	echo "start unpack redis..."
	cd $parentPath/3rd
	tar -zxvf redis-3.2.8.tar.gz
	echo "start build redis..."
	cd $parentPath/3rd/redis-3.2.8
	make
}

function clean() {
	# 清理skynet
	echo "====================="
	echo "start clean skyent..."
	cd $parentPath/skynet
	make clean

	# 清理log日志服务
	echo "====================="
	echo "start clean service log..."
	cd $parentPath/3rd/service-log
	make clean

	# 清理telnet服务
	echo "====================="
	echo "start clean service telnet..."
	cd $parentPath/3rd/service-telnetd
	make clean

	# 清理cjson库
	echo "====================="
	echo "start clean cjson..."
	cd $parentPath/3rd/lua-cjson
	make clean

	# 清理redis
	echo "====================="
	echo "start clean redis..."
	cd $parentPath/3rd/redis-3.2.8
	make clean
	cd $parentPath/3rd
	rm -rf redis-3.2.8
}

if [[ "$1" == "all" ]]; then
	build
elif [[ "$1" == "clean" ]]; then
	clean
else
	echo "不存在$1指令"
fi