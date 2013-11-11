#!/bin/sh

if [ $# -eq 0 ]; then
	echo "Usage: $0 dir_b1.txt|dir_b3.txt"
	exit 1
fi

for i in `cat $1`; do
	#echo "sh result.sh $i "
	echo $i
	./result.sh $i
done