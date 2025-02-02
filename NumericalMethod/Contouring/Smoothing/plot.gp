reset

set size ratio -1

set format x ''
set format y ''

set style line 2 lw 5
set style line 3 lw 5 lc rgb '#000000'

factor = 1e-2

plot \
  'before.dat' u 1:2 notitle ls 1 pt 5 ps 2 w p, \
  'after.dat'  u 1:2 notitle ls 2 pt 7 ps 1 w l, \
  'after.dat'  u 1:2:(factor * -$4 / sqrt($3 * $3 + $4 * $4)):(factor * $3 / sqrt($3 * $3 + $4 * $4)) notitle ls 3 w vectors
