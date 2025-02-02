reset

set grid
set size ratio -1

set format x ''
set format y ''

set xtics 1. / 128.
set ytics 1. / 128.

plot 'contours-gnuplot.dat' u 1:2 notitle w l
