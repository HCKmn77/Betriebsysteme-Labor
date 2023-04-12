#!/bin/bash

#########################################################################
#									#
#	last_analysis.sh						#
#	Beschreibung: Auflisten von Uptime- & Login-Informationen	#
#									#
#	Betriebssystem Labor Aufgabe 1					#
#	Autoren: Fabian Schneider, Jeremia Haackmann			#
#									#
#########################################################################


function runtimeFunc()
{
	# Listet die Uptime seit dem letzten Reboot auf.
	
	last | grep -E "^reboot" | grep -v "still running" | cut -d" " -f7,15 > test.txt
}

function userFunc()
{
	# Zählt die Anzahl an Logins jedes einzelnen Users auf
	
	# Alle jemals angemeldeten User in einer temp. Datei speichern
	last | cut -d" " -f1,2 | head -n -2 | sort -u > /tmp/user
	
	# Für jeden User aus der vorherigen Datei zählen, wie oft er sich am System angemeldet hat
	while read LINE ; do 
		(last | cut -d" " -f1,2 | head -n -2 | grep -c $LINE | tr -d "\n" && echo " $LINE") >> /tmp/user1; 
	done < /tmp/user
	
	# Ausgabe einer sortierten Liste 
	cat /tmp/user1 | sort -n
	
	# Entfernen der temp. Dateien
	rm /tmp/user /tmp/user1
}

function helpFunc()
{
	# Help Message
	echo "Usage: [-r|--runtime] listet die Uptime seit dem letzten Reboot auf"
	echo "       [-u|--user] zählt die Anzahl an Logins jedes einzelnen Users auf"
	echo "       [-h|--help] zeigt diese Hilfe an"
}

# Alle übergebenen Parameter nacheinander abarbeiten
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
