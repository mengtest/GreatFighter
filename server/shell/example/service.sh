#!/bin/bash
# /***
# * 3D进程管理脚本(start|stop|restart|check)
# * @author Wangcheng
# * @since 2015-12-30
# */

#=============================================================================================
## 全局变量和方法
RedEcho () { echo -e "\033[0;31;1m`date +"%F %T"`: $* \e[0m" ; }
YellowEcho () { echo -e "\033[0;33;1m`date +"%F %T"`: $* \e[0m" ; }
 
function SetGlobal() {
	tempPath=$(dirname "$0")
	cd $tempPath
	realPath=$(pwd)
	parentPath=$(dirname $(pwd))
	configFile="$(pwd)/../config/machine.conf"
	[ ! -f $ConfigFile ] && RedEcho "配置文件$ConfigFile不存在,退出脚本" && exit

	echo "本脚本文件所在目录路径是：$realPath"
	startTime=`date +"%F_%T"`

	ulimit -c unlimited
    ulimit -n 65534
    /sbin/sysctl -w kernel.core_pattern="$(pwd)/../dump/core_%e_%t"
}


#=============================================================================================
## 读取配置文件,通过"="来分隔
function GetConfig() {
	arg1=$1
	grep -q "$arg1=" $configFile || (RedEcho "文件$configFile中不存在$arg1配置" && exit)
	GetValue=`grep "$arg1=" $configFile | head -n1 | cut -d= -f2`
	echo $GetValue
}


#=============================================================================================
## start函数用来启动服务端进程,带有参数则启动参数进程,没带参数则启动所有进程
function start_process() {
	ps -o command -C skynet | grep "$parentPath/skynet/skynet $parentPath/dev/$1/conf" &> /dev/null
	[ $? -eq 0 ] && RedEcho "进程$1已经存在,禁止重复启动" && return
	YellowEcho "$parentPath/skynet/skynet $parentPath/dev/$1/conf &> /dev/null &"
	nohup $parentPath/skynet/skynet $parentPath/dev/$1/conf &> /dev/null &
}
function start() {
	## 获取配置
	configProcesses=`GetConfig start_processeslist`
	if [[ ! -z $1 && "$1" != "all" ]] ; then
		## 关于=~man手册: LANG=C PAGER='less -iRsp=~' man bash
		if [[ $configProcesses =~ $1 ]] ; then
			start_process $1
		else
			RedEcho "按照配置$1不能在本机启动" && exit
		fi
	else
		## 如果$1为空,或者为all,都启动所有
		for processName in ${configProcesses[@]} ; do
			start_process $processName
			sleep 1
		done
	fi
}


#=============================================================================================
## 停止进程,带有参数则停止参数进程,没带参数则停止所有进程
function stop_process() {
	YellowEcho "killing $1 ..."
	res=`ps aux | grep "$1" | grep -v tail | grep -v grep | awk '{print $2}'`
	[ "$res" != "" ] && kill $res
}

function stop() {
	## 获取配置
	configProcesses=`GetConfig start_processeslist`
	if [[ ! -z $1 && "$1" != "all" ]] ; then
		if [[ $configProcesses =~ $1 ]] ; then
			stop_process "$parentPath/skynet/skynet $parentPath/dev/$1/conf"
		else
			RedEcho "本机没有配置启动该进程,无法停止"
		fi
	else
		## 如果$1为空,或者为all,停止本机所有进程
		for processName in ${configProcesses[@]} ; do
			stop_process $processName
			sleep 1
		done
	fi
}


#=============================================================================================
## 脚本默认参数,check: 打印出正在运行的进程,进程名有颜色显示
function check() {
	## 获取配置
	configProcesses=`GetConfig start_processeslist`
	for processName in ${configProcesses[@]} ; do
		filter="$parentPath/skynet/skynet $parentPath/dev/$processName/conf|${filter}"
	done

	ps axo pid,rss,vsize,size,%cpu,command --sort=size | head -1
	ps axo pid,rss,vsize,size,%cpu,command --sort=size | egrep "${filter%?}" | grep -v grep | egrep --color=auto "${configProcesses[@]//\ /|}"
}


#=============================================================================================
## Main方法,脚本入口,目前该脚本支持start|stop|check参数

case $1 in
	check|"")
		SetGlobal
		check
			;;
	start)
		SetGlobal
		start $2
			;;
	stop)
		SetGlobal
		stop $2
			;;
	*)
		RedEcho "Usage: sh $0 {start|stop|checkex|check(default)}"
esac
