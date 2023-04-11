#!/bin/bash

function runtimeFunc()
{
	last | grep -E "^reboot" | grep -v "still running" | cut -d" " -f7,15 > test.txt
}

function userFunc()
{
	last | cut -d" " -f1,2 | head -n -2 | sort -u > /tmp/user
	while read LINE ; do (last | cut -d" " -f1,2 | head -n -2 | grep -c $LINE | tr -d "\n" && echo " $LINE") >> /tmp/user1; done < /tmp/user
	cat /tmp/user1 | sort -n
	rm /tmp/user /tmp/user1
}

function helpFunc()
{
	echo "Usage: [-r|--runtime] listet die Uptime von Reboots auf"
	echo "       [-u|--user] sortiert nach Anzahl Logins von Nutzern und dem Nutzernamen"
	echo "       [-h|--help] zeigt diese Hilfe an"
}

while [[ $# -gt 0 ]] ; do
	case "$1" in
		-r|--runtime)
			runtimeFunc
			shift
			;;
		-u|--user)
			userFunc
			shift
			;;
		-h|--help)
			helpFunc
			shift
			;;
		*)
			helpFunc
			shift
			;;
	esac
done
