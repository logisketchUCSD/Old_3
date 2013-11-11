#!/bin/sh

gen_one() {
	out=$1.tmp
	echo "0 `./result.sh user_study_error_${1}r0 | ./average.sh 3`" >  $out
	echo "1 `./result.sh user_study_error_${1}r1 | ./average.sh 3`" >> $out
	echo "2 `./result.sh user_study_error_${1}r2 | ./average.sh 3`" >> $out
	echo "5 `./result.sh user_study_error_${1}r5 | ./average.sh 3`" >> $out
}

gen_one 50
gen_one 100
gen_one 200

gnuplot plot_error_time.gp

rm 50.tmp 100.tmp 200.tmp