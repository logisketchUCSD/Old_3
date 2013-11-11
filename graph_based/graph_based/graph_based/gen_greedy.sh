#!/bin/sh

gen_one() {
	out=greedy.tmp
	echo "0 `./result.sh user_study_greedy_r0 | ./average.sh 2`" >  $out
	echo "1 `./result.sh user_study_greedy_r1 | ./average.sh 2`" >> $out
	echo "2 `./result.sh user_study_greedy_r2 | ./average.sh 2`" >> $out
	echo "5 `./result.sh user_study_greedy_r5 | ./average.sh 2`" >> $out
}

gen_one

gnuplot plot_greedy.gp

rm greedy.tmp