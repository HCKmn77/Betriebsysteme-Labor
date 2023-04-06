#!/bin/bash


echo Hello!

function runtimeFunc()
{
	echo "In runtime function"
	last | grep -E "^reboot" | cut -c23-39,58-72 > test.txt
}

function userFunc()
{
	echo "In User function"
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
			echo "runtime"
			runtimeFunc
			shift
			;;
		-u|--user)
			echo "user"
			userFunc
			shift
			;;
		-h|--help)
			echo "help"
			helpFunc
			shift
			;;
		*)
			echo "default"
			helpFunc
			shift
			;;
	esac
done
