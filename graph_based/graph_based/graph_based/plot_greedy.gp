set output 'greedy.eps'
set terminal postscript eps color enhanced 22

#set title "Accurary vs. Number of Randomization"
set xlabel "Number of Randomization"
set ylabel "Accuracy (%)"

plot \
 'greedy.tmp' with linespoints lw 10 title "Greedy"
