#!/bin/bash

echo "spamming $1:$2 with udp packets";

COUNTER=0;

while true; do
	echo "message $COUNTER"
	((COUNTER+=1))
	sleep $3
done | nc -u $1 $2;

