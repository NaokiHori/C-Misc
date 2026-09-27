reset

set xrange [-1:1]
set yrange [-1:1]

set size ratio -1

set palette defined (0 'red', 1 'blue')

unset colorbox

plot \
  'test_set.dat' u 1:2:3 notitle lc palette z pt 8 ps 1 w p, \
  'training_set.dat' u 1:2:3 every :::0::0 notitle lc palette z pt 7 ps 2 w p, \
  'training_set.dat' u 1:2:3 every :::1::1 notitle lc palette z pt 7 ps 1 w p
