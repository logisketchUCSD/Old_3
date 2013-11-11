set output 'greedy_b3.eps'
set terminal postscript eps color enhanced 22

#set title "Accurary vs. Number of Randomization"
set xlabel "Number of Randomization"
set ylabel "Accuracy (%)"

plot \
 'greedy_b3.tmp' with linespoints lw 10 title "Top-3 Greedy"
