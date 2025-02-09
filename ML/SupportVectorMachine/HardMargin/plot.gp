reset

set size ratio -1

set xrange [-1:1]
set yrange [-1:1]

set style line 1 lc rgb '#ff0000'
set style line 2 lc rgb '#0000ff'
set style line 3 lc rgb '#000000'
set style line 4 lc rgb '#00ff00'

set key outside

plot \
  'data/training_dataset_0.dat' u 1:2 t 'category 0'         ls 1 pt 7 ps 1 w p, \
  'data/training_dataset_1.dat' u 1:2 t 'category 1'         ls 2 pt 5 ps 1 w p, \
  'data/specified_boundary.dat' u 1:2 t 'specified boundary' ls 3 dt 1 w l, \
  'data/trained_boundary.dat'   u 1:2 t 'trained boundary'   ls 3 dt 3 w l, \
  'data/support_vectors.dat'    u 1:2 t 'support vectors'    ls 4 pt 8 ps 2 w p
