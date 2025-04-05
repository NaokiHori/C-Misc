reset

set size ratio -1;

set style line 1 lc rgb '#FF0000'
set style line 2 lc rgb '#0000FF'

plot \
  'points.dat'    u 1:2 notitle ls 1 pt 7 ps 2 w p, \
  'triangles.dat' u 1:2 notitle ls 2 w l

