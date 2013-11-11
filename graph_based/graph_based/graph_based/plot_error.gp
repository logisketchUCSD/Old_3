set output 'error.eps'
set terminal postscript eps color enhanced 22

#set title "Accurary vs. Number of Randomization"
set xlabel "Number of Randomization"
set ylabel "Accuracy (%)"

plot \
 '50.tmp' with linespoints lw 10 title "Error  50", \
'100.tmp' with linespoints lw 10 title "Error 100", \
'200.tmp' with linespoints lw 10 title "Error 200"