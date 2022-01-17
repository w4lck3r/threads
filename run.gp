#!/usr/bin/gnuplot
set title "Taux d'exec de compteur en fonction de la taille et le nombre des threads"
set output 'compteur_gc.png'
set term png size 1024,768
set logscale x
set dgrid3d 30,30
set pm3d
splot "res.dat" using 1:2:3 with lines
