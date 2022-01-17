#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <pthread.h>

// fonction qu'on aura utiliser dans le threads
unsigned long compteur_gc(char *bloc, unsigned long taille) {
    unsigned long i, cptr = 0;

    for (i = 0; i < taille; i++)
        if (bloc[i] == 'G' || bloc[i] == 'C') // on calcul les genomes G ou C 
            cptr++;

    return cptr;
}

struct compteur_gc_struct { // puisque le tread a besoin d'une fonction qui prends un pointeur et renvoi un pointeur on creer cette structure pour stocker les données

    char* block;

    unsigned long taille;

    unsigned int resultat;
};


void* cpt_wrapper(void* arg){ //fonction qui remplace le compteur_gc pour pouvoir l'utiliser dans le ptread
    struct compteur_gc_struct* structure  = (struct compteur_gc_struct*) arg; //remplir les données d'args dans structure

    structure->resultat = compteur_gc(structure->block, structure->taille); // recuperer la resultat
    return NULL;
}


int main(int argc, char *argv[]) {
    struct stat st;
    int fd;
    char *tampon;
    int lus;
    unsigned long cptr = 0;
    off_t taille = 0;
    struct timespec debut, fin;

    assert(argc > 2);

    /* Quelle taille on prend pour le tampon ? */
    assert(stat(argv[1], &st) != -1);
    tampon = malloc(st.st_size);
    assert(tampon != NULL);

    /* Chargement en mémoire */
    fd = open(argv[1], O_RDONLY); //ouvrir le fichier en parametre ordinaire
    assert(fd != -1);
    while ((lus = read(fd, tampon + taille, st.st_size - taille)) > 0) //tq le fichier n'est pas vide
        taille += lus; 
    assert(lus != -1);
    assert(taille == st.st_size); //verifier que tous les données on etais lu
    close(fd);


    int n_threads =  atoi(argv[2]); //recuperer nbthreads par l'argument 2
    

    pthread_t threads[n_threads]; //On creer n threads
    struct compteur_gc_struct structs[n_threads]; //  On declare les n_threads structures pour stocker les données pour chaque thread
    char* tmp = tampon;

    assert(clock_gettime(CLOCK_MONOTONIC, &debut) != -1); //declancher un miniteur? pour calculer le temps d'op de la fonction
//  <Multi threads>
    for(int i = 0; i < n_threads; i++){
        if(i < n_threads - 1){
            structs[i].taille = taille/n_threads; 
        } else {
            structs[i].taille = taille/n_threads + taille%n_threads; //pour la derniere structure la taille doit avoir tous le reste
        }
        structs[i].block = tmp;
        tmp += structs[i].taille;
        pthread_create(&threads[i], NULL, cpt_wrapper, &structs[i]); //creactions des threads avec la fonctions pct_wrapper et les args de structs
    }

    for(int i = 0; i < n_threads; i++){
        pthread_join(threads[i], NULL);// pour attendre le calcul du prochain thread
        cptr += structs[i].resultat; 
    }

    assert(clock_gettime(CLOCK_MONOTONIC, &fin) != -1); //le calcul du temps a fini

    /* Affichage des résultats */
    // printf("Nombres de GC:   %ld\n", cptr);
    // printf("Taux de
    static int count = 0; /* varia
    fin.tv_sec  -= debut.tv_sec;
    fin.tv_nsec -= debut.tv_nsec;
    if (fin.tv_nsec < 0) {
        fin.tv_sec--;
        fin.tv_nsec += 1000000000;
    }
    printf("%ld.%09ld\n", fin.tv_sec, fin.tv_nsec);
    // printf("(Attention: très peu de chiffres après la virgule sont réellement significatifs !)\n");

    // printf("------------\n"); 

    cptr = 0;

    /* Calcul proprement dit */
    assert(clock_gettime(CLOCK_MONOTONIC, &debut) != -1);
    cptr = compteur_gc(tampon, taille);
    assert(clock_gettime(CLOCK_MONOTONIC, &fin) != -1);

    /* Affichage des résultats */
    // printf("Nombres de GC:   %ld\n", cptr);
    // printf("Taux de GC:      %lf\n", ((double) cptr) / ((double) taille));
    /*
        On a deja declarer le timespace :
            struct timespec debut, fin;
    */
    fin.tv_sec  -= debut.tv_sec; 
    fin.tv_nsec -= debut.tv_nsec;
    if (fin.tv_nsec < 0) {
        fin.tv_sec--;
        fin.tv_nsec += 1000000000;
    }
    // printf("Durée de calcul: %ld.%09ld\n", fin.tv_sec, fin.tv_nsec);
    // printf("(Attention: très peu de chiffres après la virgule sont réellement significatifs !)\n");

    return 0;
}


/**
 * creation d'une fonction qui prend un fd et un nombre de thread
 * ca peut prendre n = 0 threads, pour chaque n ca cree un p_thread (id)
 * le fichier lu sera decoupe en n; le premier thread (n=0?) commence par 0 et termine
 * par length de fd/n; le deuixieme thread commence par legnth de fd/n; 
 * pour le n eme thread il commence par la case de memoire de: n * size;
 * et termine par size/n; la fonction retourne le taux de g/c (unsigned Int)
*/
// unsigned int threader(int fd, int n_threads){
//     for(int i = 0; i < n_threads; i++){

//         structs[i].taille = ;
//         pthread_create(&threads[i], NULL, compteur_gc, &compteur_gc_struct[i]);

//     }
// }