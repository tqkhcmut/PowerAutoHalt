#! /bin/sh
### BEGIN INIT INFO
# Provides:          kieutq
# Required-Start:    $remote_fs $syslog
# Required-Stop:     $remote_fs $syslog
# Default-Start:     2 3 4 5
# Default-Stop:      0 1 6
# Short-Description: Auto halt scripts
# Description:       This file should be used to construct scripts to be
#                    placed in /etc/init.d.
### END INIT INFO


case "$1" in
	start)
		echo "Start Power Automatic Halt."
		sudo /home/pi/Power/./Power
		;;
	stop) 
		echo "Force Stop Power Halt."
		pkill Power
		;;
	*)
		echo "Usage: /etc/init.h/power.sh {start|stop}"
		exit 1
		;;
esac

:
