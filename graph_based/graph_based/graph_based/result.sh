#!/bin/sh

if [ $# -eq 0 ]; then
	echo "Usage: $0 user_study_dir"
	exit 1
fi

for i in $1/*/??_summary.txt; do
	grep Ave $i
done