set output 'stochastic.eps'
set terminal postscript eps color enhanced 22

#set title "Accurary vs. Random Number"
set xlabel "Number of Randomization"
set ylabel "Accuracy (%)"

plot \
'100.tmp' with linespoints lw 10 title "Stochastic 100", \
'200.tmp' with linespoints lw 10 title "Stochastic 200", \
'300.tmp' with linespoints lw 10 title "Stochastic 300"