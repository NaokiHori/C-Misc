reset

set ytics nomirror
set y2tics nomirror

plot \
  'data/training.log' u 1:3 t 'objective' w l axes x1y1, \
  'data/training.log' u 1:4 t 'tolerance' w l axes x1y2

