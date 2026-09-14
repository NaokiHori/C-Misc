reset

set size ratio -1

set style line 1 lc rgb '#000000' lw 1 dt 3
set style line 2 lc rgb '#000000' lw 3 dt 1

file_name = 'positions.dat'

plot \
  file_name u 1:2:5 notitle ls 1 lc rgb variable w l, \
  file_name u ($1 + $3):($2 + $4):6 notitle ls 2 lc rgb variable w l, \
  NaN t 'BEF' ls 1 w l, \
  NaN t 'AFT' ls 2 w l
