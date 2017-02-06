#!/bin/bash

# /***
# * GreatFighter清理编译文件
# * @author wuyinjie
# * @since 2017-01-11
# */

parentPath=$(dirname $(pwd))

# 编译skynet
echo "====================="
echo "start clean skyent..."
cd $parentPath/skynet
make clean

# 编译log日志服务
echo "====================="
echo "start clean service log..."
cd $parentPath/3rd/service-log
make clean

echo "====================="
echo "start clean service telnet..."
cd $parentPath/3rd/service-telnetd
make clean

echo "====================="
echo "start clean cjson..."
cd $parentPath/3rd/lua-cjson
make clean