set encoding utf8
set term postscript color eps enhanced size 3,2
set output 'critical.eps'
set xrange [0.3:0.5]
set yrange [-1:1]
set title "Phasenübergang eines Ferromagneten\nbei J=1, B=0 mit 50^2 Spins"
set xlabel "Temperatur beta=1/T"
set ylabel "Resultierende Magnetisierung M"
plot "critical.dat" using 1:2 w l title ""
