//
// Created by HP on 24. 11. 2021.
//


/* vyberie z pameti polynomi a socekt (pre informaciu)
 * zavolanim funkcii bude robit operacie s polynommi
 * ulozi do pameti vysledok polynomov a priradi mu povodny socket
 *
 * potreba 2 vlakien
 * 1. komunikacia s klientom
 * 2. spracovanie polynomov
*/


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <errno.h>

#include "polynom.h"

// obsahuje dat pre vytvorenie servra argcD, *sockfd, *newsockfd, ***argvD, *mutex;
typedef struct serData{
    int argcD, *sockfd; //, *newsockfd
    char ***argvD;
    pthread_mutex_t *mutex;
}SDT;

// *mutex, sockklient, koniec, program[10], vykonanie[10], klient_index, pocitanie[10], polyD[10][3]
typedef struct komData{
    pthread_mutex_t *mutex;
    int sockserver, koniec, progKod[10], vykonanie[10], klient_index;//, sockklient, klient_max;
   // char **zbernica;
    bool pocitanie[10];

    // zasobnik
    POLYNOM polyD[10][3];
}KDT;

int vytvor_servra(void * data, bool vytvor) {
    SDT *sd = data;
    int sockfd, argc; // newsockfd,
    char **argv;

    if (vytvor != false) {

        pthread_mutex_lock(sd->mutex);
        argc = sd->argcD;
        argv = *sd->argvD;
        pthread_mutex_unlock(sd->mutex);

        // inicailizácia adresy na korej sa budu primat spojenia
        struct sockaddr_in serv_addr; // , cli_addr

        if (argc < 2) {
            fprintf(stderr, "usage %s port\n", argv[0]);
            return 1;
        }

        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(atoi(argv[1]));
        // vytvornie socketu server
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
        if (sockfd < 0) {
            perror("Error creating socket");
            return 1;
        }
        if(sockfd > 0) {
            pthread_mutex_lock(sd->mutex);
            *sd->sockfd= sockfd;
            pthread_mutex_unlock(sd->mutex);
            printf("Socket servra prideleny: %d \n", *sd->sockfd);
        }
        // priradenie adresi k socketu
        if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
            perror("Error binding socket address");
            return 2;
        }
    }
    if (vytvor == false) {
        close(sockfd);
    }
    return 0;
}

void * komunikacia(void * data) {
    KDT *kd = data;

    int sockfd, newsockfd, sock_klient[10] = {0}, max_klientov = 10, max_sd, sd, aktivita,
        valcit, cis;
    char zberP[64], *ukaz;

    pthread_mutex_lock(kd->mutex);
    sockfd = kd->sockserver;
   // kd->klient_max = max_klientov;
    pthread_mutex_unlock(kd->mutex);
    for (int i = 0; i < max_klientov; ++i) {
        pthread_mutex_lock(kd->mutex);
        kd->pocitanie[i] = false;
        pthread_mutex_unlock(kd->mutex);
    }

    // cli_len, cli_addr upravit na pole pre napr.: 8 klientov
    socklen_t cli_len;
    struct sockaddr_in cli_addr;
    fd_set citaFds;

    // priprava socketu na priatie spojeni. mozu cakat 5
    listen(sockfd, 3);
    cli_len = sizeof(cli_addr);
    printf("Caka na spojenia.\n");

    for (int i = 0; i < max_klientov; ++i) {
        pthread_mutex_lock(kd->mutex);
        kd->vykonanie[i] = 0;
        pthread_mutex_unlock(kd->mutex);
    }

    while(1) {
        FD_ZERO(&citaFds);

        FD_SET(sockfd, &citaFds);
        max_sd = sockfd;

        for (int i = 0; i < max_klientov; ++i) {

            sd = sock_klient[i];
            if(sd > 0)
               FD_SET(sd, &citaFds);

            if(sd > max_sd)
                max_sd = sd;
        }
        // caka na aktivitu na jednom zo soketov
        aktivita = select(max_sd + 1, &citaFds, NULL, NULL, NULL);

        if(aktivita < 0 && errno!=EINTR) {
            perror("select error");
        }
        // zmeny na servery, prijima nove spojenia
        if(FD_ISSET(sockfd, &citaFds)) {
            // priatie spojenia od klienta. spojenie s klientom, socket klienta
            if((newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len)) < 0) {
                perror("ERROR on accept");
                exit(3); //return 3;
            }
            printf("Nove spojenie, socket %d , port %d .\t", newsockfd, ntohs(cli_addr.sin_port));

            strcpy(zberP,"Spojenie so servrom\n");
            // moze poslat spravu klietovy o chybnom nadviazani spojenia
            if( (write(newsockfd, zberP, sizeof(zberP)) ) !=  sizeof(zberP)){
                perror("Posielanie 1. spojenie");
            }
            bzero(zberP,64);

            for (int i = 0; i < max_klientov; ++i) {
                if( sock_klient[i] == 0) {
                    sock_klient[i] = newsockfd;
                    printf("Pridane do zoznamu socketov klientov: %d\n", i);
                    break;
                }
            }
        }

        // nejaka komunikacia na klientskom sockete, zistuje ci sa nieco deje na klientskom sokete
        for (int i = 0; i < max_klientov; ++i) {

           // bool zapis= false;
            sd = sock_klient[i];
            // zmeny u klintov
            if(sd > 0) {
                if (FD_ISSET(sd, &citaFds)) {
                    bzero(zberP, 64);
                    if (kd->vykonanie[i] == 0) {
                        printf("Socket klienta prideleny: %d \n", sd);
                        printf("Caka na zadanie hodnot.\n");
                    }

                    // niekto sa odpojil
                    if ((valcit = (int) read(sd, zberP, 63)) == 0) {
                        getpeername(sd, (struct sockaddr *) &cli_addr, (socklen_t *) &cli_len);
                        printf("Host odpojeny, klient %d port %d \n", sd,
                               ntohs(cli_addr.sin_port)); // , ip %s iner_ntoa(cli_addr.sin_addr),

                        close(sd);
                        sock_klient[i] = 0;
                        pthread_mutex_lock(kd->mutex);
                        kd->vykonanie[i] = 0;
                        pthread_mutex_unlock(kd->mutex);
                        // chyba pri prijme
                    } else if (valcit < 0) {
                        perror("Sprava od kienta");
                        // zberP[valcit] = '\0';
                        // write(sd, zberP, sizeof(zberP));
                    }

                    // prijatie spravy
                    if (valcit > 0) {
                        // printf("\t server prial od klintlist %d, %d : %s \n", i, sd, zberP); // velkost: %ld , sizeof(zberP)
                        pthread_mutex_lock(kd->mutex);
                        kd->klient_index = i;
                        kd->pocitanie[i] = false;
                        pthread_mutex_unlock(kd->mutex);

                        switch (kd->vykonanie[i]) {
                            case 0:
                                pthread_mutex_lock(kd->mutex);
                                pridajPolynom(&(kd->polyD[i][0]), zberP);
                                // 1 nacitany 1. polynom
                                pthread_mutex_unlock(kd->mutex);
                                printf("Vykonavanie %d , pridany 1.poly klienta %d ,index %d \n %s \n",
                                       kd->vykonanie[i], sd, i, zberP);
                                break;
                            case 1:
                                pthread_mutex_lock(kd->mutex);
                                pridajPolynom(&(kd->polyD[i][1]), zberP);
                                // 2 nacitany 2. polynom
                                pthread_mutex_unlock(kd->mutex);
                                printf("Vykonavanie %d , pridany 2.poly klienta %d ,index %d \n %s \n",
                                       kd->vykonanie[i], sd, i, zberP);
                                break;
                            case 4:
                                ukaz = strchr(zberP, '\0');
                                --ukaz;
                                cis = (int) strtol(zberP, &ukaz, 0);// vykonavanie na 6 nastavi 2. valko po vypocitani
                                pthread_mutex_lock(kd->mutex);
                                kd->progKod[i] = cis;
                                kd->pocitanie[i] = true;
                                pthread_mutex_unlock(kd->mutex);
                                printf("Vykonavanie %d , pridany prikaz %d , klienta %d ,index %d \n",
                                       kd->vykonanie[i], cis, sd, i);
                                while(1) {
                                    usleep(400);
                                    if (kd->vykonanie[i] == 7 && kd->pocitanie[i] == false) {
                                        pthread_mutex_lock(kd->mutex);
                                        kd->vykonanie[i] = -1;
                                        pthread_mutex_unlock(kd->mutex);
                                        printf("nove hodnoty od klienta %d s index %d  vykonavanie %d \n", sd, i, kd->vykonanie[i]);
                                        break;
                                    }
                                    if (kd->vykonanie[i] == 6 && kd->pocitanie[i] == false) {
                                        printf("Klient %d s index %d sa odpojil. \n", sd, i);
                                        break;
                                    }
                                    if (kd->vykonanie[i] == 5 && kd->pocitanie[i] == false ) {
                                        pthread_mutex_lock(kd->mutex);
                                        vypisPolynom(&(kd->polyD[i][2]), zberP);
                                        // 7 vypis pre klinta 3. polynom, vysledok
                                        //kd->vykonanie[i]--;
                                        pthread_mutex_unlock(kd->mutex);
                                        if ((valcit = (int) write(sd, zberP, strlen(zberP))) < 0) {
                                            perror("Error writing to socket, 3.poly klienta");
                                            exit(5);
                                        } if(valcit > 0) {
                                            printf("Vykonavanie %d , vypis 3.poly, vysledok klienta %d ,index %d \n %s \n",
                                                   kd->vykonanie[i], sd, i, zberP);
                                            pthread_mutex_lock(kd->mutex);
                                            kd->vykonanie[i]--;
                                            pthread_mutex_unlock(kd->mutex);
                                            break;
                                        }
                                    }
                                }
                                break; // prida
                            default:
                                break;
                        }
                        if (kd->vykonanie[i] != 4) {
                            pthread_mutex_lock(kd->mutex);
                            kd->vykonanie[i]++;
                            pthread_mutex_unlock(kd->mutex);
                        }
                    }
                }
                if ( kd->pocitanie[i] == false ) {
                    switch (kd->vykonanie[i]) {

                        case 2:
                            bzero(zberP, 64);
                            pthread_mutex_lock(kd->mutex);
                            vypisPolynom(&(kd->polyD[i][0]), zberP);
                            // 3 vypis pre klinta 1. polynom
                            pthread_mutex_unlock(kd->mutex);
                            if ((valcit = (int) write(sd, zberP, strlen(zberP))) < 0) {
                                perror("Error writing to socket, 1.poly klienta");
                                exit(5);
                            } if(valcit > 0) {
                                printf("Vykonavanie %d , vypis 1.poly klienta %d ,index %d \n %s \n", kd->vykonanie[i], sd,
                               i, zberP);
                            }
                            break;
                        case 3:
                            bzero(zberP, 64);
                            pthread_mutex_lock(kd->mutex);
                            vypisPolynom(&(kd->polyD[i][1]), zberP);
                            // 4 vypis pre klinta 2. polynom
                            pthread_mutex_unlock(kd->mutex);
                            if ((valcit = (int) write(sd, zberP, strlen(zberP))) < 0) {
                                perror("Error writing to socket, 2.poly klienta");
                                exit(5);
                            } if(valcit > 0) {
                                printf("Vykonavanie %d , vypis 2.poly klienta %d ,index %d \n %s \n", kd->vykonanie[i], sd,
                               i, zberP);
                            }
                            break;
                        default:
                            break;
                    }
                }
            }
        }
    }


/*   // debataSkli(kd);

    // ukoncenie spojenia so soketom kienta
    //close(newsockfd);
    pthread_mutex_lock(kd->mutex);
    kd->koniec = -1;
    pthread_mutex_unlock(kd->mutex);
    sleep(4);
    // ukonceie soketu servra
    // close(sockfd);
    return NULL; */
}


void * pocitanie(void * data){
    KDT* kd = data;
    int n, newsockfd, koniec, progKod, cis, i, m;
   // char  buffer[64], *ukaz;

    while (true) {

        pthread_mutex_lock(kd->mutex);
        i = kd->klient_index;
        pthread_mutex_unlock(kd->mutex);
        if (kd->progKod[i] > 0) {
        if (kd->pocitanie[i] == true && kd->vykonanie[i] > 3) {
            koniec = 1;
            switch (kd->progKod[i]) {
                //. scitaj polynomi
                case 1:
                    printf("Scitanie polynomov:\n");
                    pthread_mutex_lock(kd->mutex);
                    kd->polyD[i][2] = scitajPolynomi(&(kd->polyD[i][0]), &(kd->polyD[i][1]));
                    kd->progKod[i] = 0;
                    pthread_mutex_unlock(kd->mutex);
                    break;
                    // odcitaj polynomi
                case 2:
                    printf("Odcitanie polynomov:\n");
                    pthread_mutex_lock(kd->mutex);
                    kd->polyD[i][2] = odcitajPolynomi(&(kd->polyD[i][0]), &(kd->polyD[i][1]));
                    kd->progKod[i] = 0;
                    pthread_mutex_unlock(kd->mutex);
                    break;
                    // nasob polynomi
                case 3:
                    printf("Nasobenie polynomov:\n");
                    pthread_mutex_lock(kd->mutex);
                    kd->polyD[i][2] = nasobPolynomi(&(kd->polyD[i][0]), &(kd->polyD[i][1]));
                    kd->progKod[i] = 0;
                    pthread_mutex_unlock(kd->mutex);
                    break;
                    // del polynomi
                case 4:
                    printf("Delenie polynomov:\n");
                    pthread_mutex_lock(kd->mutex);
                    kd->polyD[i][2] = delPolynomi(&(kd->polyD[i][0]), &(kd->polyD[i][1]));
                    kd->progKod[i] = 0;
                    pthread_mutex_unlock(kd->mutex);
                    break;
                    // zvisok po deleny
                case 5:
                    printf("Zvisok po deleni polynomov:\n");
                    pthread_mutex_lock(kd->mutex);
                    kd->polyD[i][2] = zvisokDelPolynomi(&(kd->polyD[i][0]), &(kd->polyD[i][1]));
                    kd->progKod[i] = 0;
                    pthread_mutex_unlock(kd->mutex);
                    break;
                    // koniec programu
                case 6:
                    pthread_mutex_lock(kd->mutex);
                    koniec = 0;
                    kd->progKod[i] = 0;
                    kd->vykonanie[i] = 5;
                    pthread_mutex_unlock(kd->mutex);
                    printf("\n__________________koniec___________________\n");
                    break;
                    // zadanie novych hodnot
                case 7:
                    pthread_mutex_lock(kd->mutex);
                    koniec = 2;
                    kd->progKod[i] = 0;
                    kd->vykonanie[i] = 6;
                    pthread_mutex_unlock(kd->mutex);
                    printf("\n______________________\tZadajte nove hodnoty\n");
                    break;
                default:
                    // printf("Zadanu moznost nieje mozne vykonat.\n");
                    pthread_mutex_lock(kd->mutex);
                    kd->progKod[i] = 0;
                    pthread_mutex_unlock(kd->mutex);
                    break;
            }
            if(koniec == 0 || koniec == 2) {
                m = 0;
                while (m < 3) {
                    pthread_mutex_lock(kd->mutex);
                    vymazPolynom(&(kd->polyD[i][m]));
                    pthread_mutex_unlock(kd->mutex);
                    ++m;
                }
                printf("hodnoty klienta s indexom %d boly odstranen \n", i);
            }
            // printf("cislo vykonavania %d \n", kd->vykonanie[i]);
            pthread_mutex_lock(kd->mutex);
            kd->vykonanie[i]++; // na 5
            kd->pocitanie[i] = false;
            pthread_mutex_unlock(kd->mutex);
            printf("cislo vykonavania po %d \n", kd->vykonanie[i]);
        }
    }
        if(kd->koniec == -1) {break;}
    }
    return NULL;
}


int main(int argc, char *argv[])
{
   // POLYNOM poly[3];
    int kodPr, koniec = 1, sockfd_m, newsockfd_m;
    // char *vyp;


    pthread_t prostrednik, konzument;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    SDT sd = {argc, &sockfd_m, &argv, &mutex}; //, &newsockfd_m

    vytvor_servra(&sd, true);
    //vytvor_spojenia(&sd);

    KDT kd =  { &mutex, sockfd_m}; // , &poly[0], &poly[1],&poly[2]

    pthread_create(&prostrednik, NULL, &komunikacia, &kd);
    pthread_create(&konzument, NULL, &pocitanie, &kd);

    pthread_join(prostrednik, NULL);
    pthread_join(konzument, NULL);

    pthread_mutex_destroy(&mutex);

    vytvor_servra(&sd, false);
    printf("Koniec spojenia.\n"); // newsockfd_m
    return 0;
}