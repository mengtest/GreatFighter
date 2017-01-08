#!/bin/bash

if [ $# -ne 1 ] ;then
	echo "Usage:$0 all|clean" && exit
fi

root_path=$(pwd)

# compile skynet
echo "start compile skynet..."

cd $root_path/../skynet

if [ "$1" == "all" ] ;then
	make linux MALLOC_STATICLIB= SKYNET_DEFINES=-DNOUSE_JEMALLOC
else
	make $1
fi

echo -e "\n"

# compile 3rd
echo "start compile 3rd..."

cd $root_path/../3rd

sh build.sh $1
