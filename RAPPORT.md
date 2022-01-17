# Rapport threads
## Binomes :
* SALMI Badr-eddine
* EL ANSARI Fayssal

### Questions traitées :
Toutes les questions sont traitées .
### Questions non-traitée :
None.
### Notions : 
* L'idée en générale de ce Tp est d'avoir utiliser la notions des threads et surtout d'utiliser plusieurs threads a la fois, pour avoir la possibilité de la multi-taches ou multi-fonctionement et pour faire ça on doit bien metrisé trois choses en gros :
    * Creation de la fonction wrapper 'cpt_wrapper' ; qui est une copie de notre fonction mais avec une structure acceptable a utiliser dans les threads, on est besoin d'ajouter une structure 'compteur-gc-struct' qui conntient les args de notre fonction 'compteur-gc' .
    * Creation des threads : On a creer n_threads `pthread_t threads[n_threads];`qui est recuperer par l'arg de la fonction 'main' `int n_threads =  atoi(argv[2]);` est puisqu'on a plusieur threads on est besoin de plusieurs structure de 'compteur_gc_struct' `struct compteur_gc_struct structs[n_threads];` chaque structure aura la taille de `taille/n_threads` seule la dernier qui aura `taille/n_threads + taille%n_threads` pour avoir tous le reste dans la derniere.
`pthread_create(&threads[i], NULL, cpt_wrapper, &structs[i]);` ; creations des threads avec la fonctions cpt_wrapper et les args de compteur_gc en passant par les données des structures .
    * Après On a fait un thread_join `for(int i = 0; i < n_threads; i++){ pthread_join(threads[i],NULL;cptr += structs[i].resultat;} ` pour attendre tous les threads a finir et bien evidament faire une somme des resultats obtenus par chaque thread .
* Dans ce Tp on a pas seuelement traiter juste la notion des threads mais bien aussi autre fonctionalité comme:
    * l'entrée et sortie des fichiers ; ouvrir un fichier : `fd = open(argv[1], O_RDONLY);` , lu un fichier : `read(fd, tampon + taille, st.st_size - taille);` , fermer un fichier : `close(fd);`
    * recuperer les données dans un fichier et le passer dans la zone memoire pour efectuer des traitements plus rapide , avec un choix de tampon bien precis pour profiter le max de notre memoire sans causé des problemes technique et dans notre cas on a choisi la taille du tampo, et la taille du fichier traiter `tampon = malloc(st.st_size);`
    * Dans ce Tp on a utiliser la notion du time , qui nous permet de recuperer le temps d'exec d'une partie de notre prog on lancent un debut et fin du compteur avec la ligne `clock_gettime(CLOCK_MONOTONIC, &debut)` , `clock_gettime(CLOCK_MONOTONIC, &fin)` aprés il faut que faire la difference entre les deux var debut et fin `fin.tv_sec  -= debut.tv_sec;`
    * Pour visualiser les resultats dans une diagrame on utilise Gnuplot avec des setters vous le trouverez dans le fichier `run.gnu`

## Rendez-vous 
### remarques:
* Avant l'utilisation des semaphores il n'est pas suffisant d'importer la biblitheque: `<semaphore.h>` , mais il faut aussi initialiser chaque object semaphore avant de pouvoir l'utiliser avec `sem_init`.
* Dans le cas general de l'utilisation des semaphores on definit par la valeur de `value` dans la signature: `int sem_init(sem_t *sem, int pshared, unsigned int value);`, le nombre maximal des des threads qui peuvent acceder a un block d'instructions.
* Il existe une autre maniere d'utiliser les semaphores qui nous permet d'empecher le thread actuel d'acceder a un block d'instructions sauf s'il y avait un nombre definit de thread qui ont terminer un autre block d'instructions. Dans l'exercice rendez-vous par exemple on ne passe pas a l'execution des `b()` que lorsque tout les `a()` dans touts les threads sont execute.
    * Dans cette deuxieme maniere d'utiliser les semaphores on commence toujours par un `value = 0` et donc on execute toujours `sem_post(&sem2);` pour les autres sem avant le `sem_wait(&sem1);` pour le sem actuel.
    * Meme si on peut dire que la commande: `sem_wait` des semaphores empeche le passage a la prochaine ligne de programme, mais pour le programme la notion des lignes  n'existe pas c'est juste des instructions ordonnes. Et donc chaque semaphore a une file de threads libres.
    * Donc la valeur de semaphore correspond a ce nombre des threads libres qu'il peut utiliser. Si elle est egale a 0 il est impossible d'ajouter un autre thread. 
    * Rien ne nous empeche de faire appele a `sem_post` plusieurs fois et donc augmenter la valeur du semaphore c'est le cas de l'exclusion mutuelle.
* On verra plus sur `int pshared` de sem_init dans `PDM`. 

* Pour le Tp rendez-vous on reste toujours avec les threads mais en plus les semaphores , l'idée de ce Tp est de realiser des compteurs simples dans les threads , ce qui veut dire lancer plusieurs compteurs a la fois et pour ne pas confondre les resultats on a utiliser les semaphores. Pour faire ça on est besoin de :
    * Une fonction de comptage : `void a(int i);`
    * Une structure si il y a plusieurs variables : `struct p_arg_s; ` 
    * La fonction du thread : 
        * `void *p(void *arg);`  pour syncroniser ou bloquer le processuce avec les deux commandes:
            * `sem_post(&structure->sem);` sync ou attendre .
            * `sem_wait(&structure->sem);` bloquer le simaphore . 
    * Initialiser le semaphore : `assert(sem_init(&sem[i], 0, 0) == 0);` On initialise le semaphore avec chaque semaphore = `&sem[i]` , on utilisent les threads donc le deuxieme arg est `0` et une valeur init de `0`. 
    * On rempli les structures par ses données `args[i].i = i; args[i].sem = sem[i];` 
    * finalement il reste que créer les threads et les joindre . 

### Execution/Compilation
* Pour compiler tout les fichier `*.c` veuillez executer la commande: `make`
* Pour tester le prmier programme avec seulement deux threads: `./rdv`
* Pour tester le deuxieme programme avec 3 threads: `./rdv3`
* Pour tester le programme general avec N threads et N semaphores: `./rdvN n` avec n le nombre de threads/sem desiree par exemple: `./rdvN 101`
* Pour executer le dernier programme avec N threads, fonction comptuer et seuelement 2 semaphores: `./rdvN_2sem`

### Resulats
* 2 sems; 2 threads:
```
a1
a2
b1
b2
```

* 3 sems; 3 threads:
```
a1
a3
a2
b3
b1
b2
```

* N threads: `./rdvN 11` N = 11 
```
a3
a1
a6
a8
a4
a5
a9
a0
a7
a10
a2
b3
b8
b1
b9
b7
b5
b0
b4
b6
b10
b2
```
