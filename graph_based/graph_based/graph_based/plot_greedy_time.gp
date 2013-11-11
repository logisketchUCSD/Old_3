set output 'greedy_time.eps'
set terminal postscript eps color enhanced 22

#set title "Accurary vs. Number of Randomization"
set xlabel "Number of Randomization"
set ylabel "Time (ms)"

plot [0:5][0:60] \
 'greedy_time.tmp' with linespoints lw 10 title ""
