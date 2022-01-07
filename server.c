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

#include "polynom.h"

// obsahuje dat pre vytvorenie servra argcD, *sockfd, *newsockfd, ***argvD, *mutex;
typedef struct serData{
    int argcD, *sockfd, *newsockfd;
    char ***argvD;
    pthread_mutex_t *mutex;
}SDT;

// sockklient, progKod, *koniec, *mutex, *polyD[3]
typedef struct komData{
    int sockklient, progKod, *koniec;

    pthread_mutex_t *mutex;
    POLYNOM * polyD[3];
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

        // inicailizácia adresy na korej sa bud primat spojenia
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

int vytvor_spojenia(void * data) {
    SDT *sd = data;
    int sockfd, newsockfd;

    pthread_mutex_lock(sd->mutex);
    sockfd = *sd->sockfd;
    pthread_mutex_unlock(sd->mutex);

    // cli_len, cli_addr upravit na pole pre napr.: 8 klientov
    socklen_t cli_len;
    struct sockaddr_in cli_addr;

    // priprava socketu na priatie spojeni. mozu cakat 5
    listen(sockfd, 5);
    cli_len = sizeof(cli_addr);
    // priatie spojenia od klienta. spojenie s klientom, socket klienta
    newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &cli_len);
    if (newsockfd < 0) {
        perror("ERROR on accept");
        return 3;
    }
    if(newsockfd > 0) {
        pthread_mutex_lock(sd->mutex);
        *sd->newsockfd= newsockfd;
        pthread_mutex_unlock(sd->mutex);
        printf("Socket klienta prideleny: %d \n", *sd->newsockfd);
    }
    return 0;
}

void * komunikacia(void * data) {
    KDT *kd = data;
    int n, newsockfd;
    char  buffer[64], *ukaz;

    // komunikacia s klientom
    bzero(buffer, 64);
    int cis, i;
    pthread_mutex_lock(kd->mutex);
    newsockfd = kd->sockklient;
    pthread_mutex_unlock(kd->mutex);

    do {
        pthread_mutex_lock(kd->mutex);
        *kd->koniec = 1;
        pthread_mutex_unlock(kd->mutex);

        printf("Caka na zadanie hodnot.\n");
        i = 0;
        bzero(buffer, 64);
        while (i < 2) { // server nacita polynomi
            n = (int) read(newsockfd, buffer, 63);
            if (n < 0) {
                perror("Error reading from socket");
                break;// return 4;
            } else if (n > 0) {
                printf("Server prial %s \n", buffer);
                pthread_mutex_lock(kd->mutex);
                pridajPolynom(kd->polyD[i], buffer);
                pthread_mutex_unlock(kd->mutex);
                ++i;
            }
            bzero(buffer, 64);
        }

        // ----------------------------------------------------------------------------------
        i = 0;
        while (i < 2) { // server posle ako kotrolu polynomi
            printf("Poslanie polynomu klintovy.\n");
            bzero(buffer, 64);
            pthread_mutex_lock(kd->mutex);
            vypisPolynom(kd->polyD[i], buffer); // &poly[i]
            pthread_mutex_unlock(kd->mutex);

            n = (int) write(newsockfd, buffer, strlen(buffer));
            if (n < 0) {
                perror("Error writing to socket");
                break; // return 5;
            }
            if (n > 0) {
                printf("%d. %s \n", i + 1, buffer);
                ++i; }
            usleep(50);
        }

        do{ // server nacita prikaz
           // printf("Zadanie prikzu na vykonavanie operacie.\n");
            bzero(buffer, 64);
            n = (int) read(newsockfd, buffer, 63);
            if (n < 0) {
                perror("Error reading from socket");
                break; // return 4;
            }
            ukaz = strchr(buffer, '\0');
            --ukaz;
            cis = (int) strtol(buffer, &ukaz, 0);

            if (cis > 0) {
                printf("Zadanie prikzu na vykonavanie operacie: %d\n", cis);
                pthread_mutex_lock(kd->mutex);
                kd->progKod = cis;
                pthread_mutex_unlock(kd->mutex);

                printf("premiesnene %d \n", kd->progKod);

                sleep(2);
                bzero(buffer,64);  // server posiela vyslekod
                if(*kd->koniec == 1) {
                    if(kd->polyD[2]->polePrvk[0].nasobnost == 0) {
                        strcpy(buffer, " operacie nieje mozne vypocitat pre rovnost stupna polynomov!\0");
                    } else {
                        pthread_mutex_lock(kd->mutex);
                        vypisPolynom(kd->polyD[2], buffer);
                        pthread_mutex_unlock(kd->mutex);
                        printf("Vysleok polynomu %s \n", buffer);
                    }
                    n = (int) write(newsockfd, buffer, strlen(buffer));
                    if (n < 0)
                    {
                        perror("Error writing to socket");
                        break; // return 5;
                    }
                }
            }
            bzero(buffer,64);

        } while (*kd->koniec == 1);
        i = 0;
        while (i < 3) {
            pthread_mutex_lock(kd->mutex);
            vymazPolynom(kd->polyD[i]);
            pthread_mutex_unlock(kd->mutex);
            ++i;
        }
    } while (*kd->koniec > 1);

    // ukoncenie spojenia so soketom kienta
    close(newsockfd);
    pthread_mutex_lock(kd->mutex);
    *kd->koniec = -1;
    pthread_mutex_unlock(kd->mutex);
    sleep(4);
    // ukonceie soketu servra
    // close(sockfd);
    return NULL;
}


void * pocitanie(void * data){
    KDT* kd = data;

    char buffer[64];
    // int newsockfd, n;
    while (true) {
        if (kd->progKod > 0  && kd->progKod < 8 ) {//
            bzero(buffer,64);
            switch (kd->progKod) {
                //. scitaj polynomy
                case 1:
                    printf("Scitanie polynomov:\n");
                    pthread_mutex_lock(kd->mutex);
                    *kd->polyD[2] = scitajPolynomi(kd->polyD[0], kd->polyD[1]);
                    kd->progKod = 0;
                    pthread_mutex_unlock(kd->mutex);
                    break;
                    // odcitaj polynomy
                case 2:
                    printf("Odcitanie polynomov:\n");
                    pthread_mutex_lock(kd->mutex);
                    *kd->polyD[2] = odcitajPolynomi(kd->polyD[0], kd->polyD[1]);
                    kd->progKod = 0;
                    pthread_mutex_unlock(kd->mutex);
                    break;
                    // nasob polynomy
                case 3:
                    printf("Nasobenie polynomov:\n");
                    pthread_mutex_lock(kd->mutex);
                    *kd->polyD[2] = nasobPolynomi(kd->polyD[0], kd->polyD[1]);
                    kd->progKod = 0;
                    pthread_mutex_unlock(kd->mutex);
                    break;
                    // del polynomy
                case 4:
                    printf("Delenie polynomov:\n");
                    pthread_mutex_lock(kd->mutex);
                    *kd->polyD[2] = delPolynomi(kd->polyD[0], kd->polyD[1]);
                    kd->progKod = 0;
                    pthread_mutex_unlock(kd->mutex);
                    break;
                    // zvisok po deleny
                case 5:
                    printf("Zvisok po deleni polynomov:\n");
                    pthread_mutex_lock(kd->mutex);
                    *kd->polyD[2] = zvisokDelPolynomi(kd->polyD[0], kd->polyD[1]);
                    kd->progKod = 0;
                    pthread_mutex_unlock(kd->mutex);
                    break;
                    // koniec programu
                case 6:
                    pthread_mutex_lock(kd->mutex);
                    *kd->koniec = 0;
                    kd->progKod = 0;
                    pthread_mutex_unlock(kd->mutex);
                    printf("\n__________________koniec___________________\n");
                    break;
                    // zadanie znova hodnot
                case 7:
                    pthread_mutex_lock(kd->mutex);
                    *kd->koniec = 2;
                    kd->progKod = 0;
                    pthread_mutex_unlock(kd->mutex);
                   // printf("\n___________________________________\nZadajte nove hodnoty\n");
                    break;
                default:
                   // printf("Zadanu moznost nieje mozne vykonat.\n");
                    pthread_mutex_lock(kd->mutex);
                    kd->progKod = 0;
                    pthread_mutex_unlock(kd->mutex);
                    break;
            }
        }
        if(*kd->koniec == -1) {break;}
    }
    return NULL;
}


int main(int argc, char *argv[])
{
    POLYNOM poly[3];
    int kodPr, koniec = 1, sockfd_m, newsockfd_m;
    char *vyp;


    pthread_t prostrednik, konzument;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);

    SDT sd = {argc, &sockfd_m, &newsockfd_m, &argv, &mutex};

    vytvor_servra(&sd, true);
    vytvor_spojenia(&sd);

    KDT kd =  {newsockfd_m, kodPr, &koniec, &mutex, &poly[0], &poly[1],&poly[2]};

    pthread_create(&prostrednik, NULL, &komunikacia, &kd);
    pthread_create(&konzument, NULL, &pocitanie, &kd);

    pthread_join(prostrednik, NULL);
    pthread_join(konzument, NULL);

    pthread_mutex_destroy(&mutex);

    vytvor_servra(&sd, false);
    printf("Koniec spojenia. %d\n",newsockfd_m);
    return 0;
}