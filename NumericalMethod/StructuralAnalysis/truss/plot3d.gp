reset

set xyplane at 0
set view equal xyz

set style line 1 lc rgb '#000000' lw 1 dt 3
set style line 2 lc rgb '#000000' lw 3 dt 1

file_name = 'positions.dat'

splot \
  file_name u 1:2:3:7 notitle ls 1 lc rgb variable w l, \
  file_name u ($1 + $4):($2 + $5):($3 + $6):8 notitle ls 2 lc rgb variable w l, \
  NaN t 'BEF' ls 1 w l, \
  NaN t 'AFT' ls 2 w l
