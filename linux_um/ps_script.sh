#!/bin/bash

echo "PID      TTY      TIME          CMD"
BASE_PROC_DIR="/proc/"
MY_UID=$(id -u)
MY_TTY=$(tty)

for PID in "$BASE_PROC_DIR"/*; do
    # Check if it's a directory and its name is entirely numbers
    if [ -d "$PID" ] && [[ "$(basename "$PID")" =~ ^[0-9]+$ ]]; then
    	#check status, stat, fd and cmdline exsists
    	if [ -r "$PID/status" -a -r "$PID/stat" -a -r "$PID/fd" -a -r "$PID/cmdline" ]; then
        	proc_uid=$(awk '/^Uid:/ { print $2 }' "$PID/status")
        	proc_state=$(awk '/^State:/ {print $2}' "$PID/status")
        	proc_tty=$(readlink "$PID/fd/0")

        	#check uid match and R, S and D status and tty
        	if [[ "$proc_uid" == "$MY_UID" && ( "$proc_state" == "R" || "$proc_state" == "S" || "$proc_state" == "D" ) && $proc_tty == $MY_TTY ]] ; then

        		echo -ne "$(basename "$PID") \t"
        		
        		#add tty
        		echo -ne "${proc_tty#/dev/} \t"

        		#add time
				utime=$(awk '{print $14}' "$PID/stat")
				stime=$(awk '{print $15}' "$PID/stat")
				clk_tck=$(getconf CLK_TCK)
				cpu_time=$(( (utime + stime) / clk_tck ))

				#convert to 00:00:00 format
				h=$(( cpu_time / 3600 ))
				m=$(( (cpu_time % 3600) / 60 ))
				s=$(( cpu_time % 60 ))

				#manually padd with zero
				if [ $h -lt 10 ]; then h="0$h"; fi
				if [ $m -lt 10 ]; then m="0$m"; fi
				if [ $s -lt 10 ]; then s="0$s"; fi

				echo -ne "$h:$m:$s \t"

            	#add cmdline
            	strings "$PID/cmdline"
        	fi
        fi
    fi
done
