lx = 20.
ly = 30.
lz = 25.

reset

set xyplane at 0
set view equal xyz

unset xtics
unset ytics
unset ztics
unset border

splot \
  'trajectories.out' u ($2 / lx): ($3 / ly): ($4 / lz - 1.) notitle w l, \
  'trajectories.out' u ($5 / lx): ($6 / ly): ($7 / lz - 1.) notitle w l
