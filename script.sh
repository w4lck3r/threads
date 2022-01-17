rm -f res.dat
#remplir les resultats dans res.dat
for((i=2; i<10; i++)){
    ./aleazard 10**$i > genome.txt #exe la prod des genomes
    for((j = 0; j < 6; j++)){
        # echo "10**$i    2**$j   `./compteur-gc genome.txt 2**$j | awk -e (?<=\Durée des calculs: ).*`" >> res.dat;
        echo "$((10**$i))    $((2**$j))   `./compteur-gc genome.txt 2**$j`" >> res.dat; #10*10*10...    nb-threads     calcul-gc
    }
}

