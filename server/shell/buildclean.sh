#!/bin/bash

# /***
# * GreatFighter清理编译文件
# * @author wuyinjie
# * @since 2017-01-11
# */

parentPath=$(dirname $(pwd))

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