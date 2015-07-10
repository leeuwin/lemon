#!/bin/sh

[ $# != 1 ] && echo "Usage: $0 <start/stop/restart/status/reload>" && exit 1

timestamp=`date "+%m/%d %H:%M:%S"`

conf_file="../conf/lemon.xml"

function _start()
{
	nohup ./lemon $conf_file &
}

function _stop()
{
	pids=`ps aux|grep "lemon $conf_file"|grep -v grep|awk '{printf $2}'`
	for pid in $pids
	do
		kill -9 $pid
		echo "stop $pid"
	done
}

function _status()
{
	#set -x
	pids=`ps aux|grep "lemon $conf_file"|grep -v grep|awk '{printf $2}'`
	if [ "$pids" == "" ]; then
		echo "lemon is stopped"
		return 1
	else
        	for pid in $pids
        	do
        	        echo "$pid is running..."
        	done
	fi
	return 0
}

function _reload()
{
	pids=`ps aux|grep "lemon $conf_file"|grep -v grep|awk '{printf $2}'`
	[ "$pids" == "" ] && echo "lemon is stopped" && return
        for pid in $pids
        do
                kill -HUP $pid
        done
}

case $1 in
start)
	echo "starting..."
	_start
	_status
	if [ $? -eq 0 ]; then
		echo "start                            [ok]"
	else
		echo "start                            [failed]"
	fi
	;;
stop)
	echo "stoping..."
	_stop
	echo "stop                             [ok]"
	;;
restart)
	echo "restarting..."
	_stop
	_start
	;;
status)
	_status	
	;;
reload)
	echo "reloading..."
	_reload
	echo "reloading...                     [ok]"
	;;
*)
	echo "Usage: $0 <start/stop/restart/status/reload>" && exit 1
	;;
esac

