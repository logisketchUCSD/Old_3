set output 'error_time_b3.eps'
set terminal postscript eps color enhanced 22

#set title "Accurary vs. Number of Randomization"
set xlabel "Number of Randomization"
set ylabel "Time (ms)"

plot [0:5][0:60] \
 '50.tmp' with linespoints lw 10 title "Top-3 Error  50", \
'100.tmp' with linespoints lw 10 title "Top-3 Error 100", \
'200.tmp' with linespoints lw 10 title "Top-3 Error 200"