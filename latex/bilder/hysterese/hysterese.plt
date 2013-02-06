set encoding utf8
set term postscript color eps enhanced size 3,2
set output 'hysterese.eps'
set xrange [-2:2]
set yrange [-1:1]
set title "Simulierte Hysterese eines 2D-Ferromagneten\nbei J=2, beta=1/3 mit 100^2 Spins"
set xlabel "Angelegtes Magnetfeld B"
set ylabel "Resultierende Magnetisierung M"
plot "hysterese.dat" using 1:2 w l title ""
