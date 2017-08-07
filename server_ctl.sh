#!/bin/bash
function usage(){
	echo "Usage: $0 start|stop|restart"	
}
pid=""
function service_is_start(){
	pid=`ps -ef | grep "httpd" | grep -v "grep httpd" | awk '{print $2}'`
	if [ "$pid" = "" ]; then
		return 1
	fi
	return 0
}
function start_the_service(){
	if service_is_start ;then
		echo "the service is already started"
		return 
	fi
	./httpd 80
	echo "the service is starting...done!"
}
function stop_the_service(){
	if service_is_start ; then
		kill -9 $pid
		echo "the service is stoping...done!"
	else
		echo "the service is not start"
	fi
}
if [ $# -ne 1 ]; then
	usage
	exit 
fi

case $@ in
	start )
		start_the_service
	;;
	restart )
		stop_the_service
		start_the_service
	;;
	stop )
		stop_the_service
	;;
	* )
		usage
		exit
	;;	
esac
