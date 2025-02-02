reset

set size ratio -1

set format x ''
set format y ''

set style line 2 lw 5

plot \
  'before.dat' u 1:2 notitle ls 1 pt 5 ps 2 w p, \
  'after.dat'  u 1:2 notitle ls 2 pt 7 ps 1 w l
