reset

set xrange [-0.55:0.55]
set yrange [-0.55:0.55]

set size ratio -1

plot \
  'nodes.dat' u 1:2 notitle pt 7 ps 1 w p, \
  'convex_hull.dat' u 1:2 notitle w l
