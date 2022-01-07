//
// Created by HP on 24. 11. 2021.
//


/* klinetovy sa vypise aby zadal polynomi, po zadani polynomu si vyberie moznost co robit
 * klient zapisuje polinomi ktore sa ukladaju do pamete aj zo soketom
 * ked klietn zapisuje polynom do pameti tak k nemu nema pristup server
 * nakoniec nacita z pameti vysleok a zobraziho pouzivatelovy
 *
 * nebolo by odveci aby malo pristup viacero kientov
 */

// socket structura

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    int sockfd, n, koniec, cis;
        // inicializacia adresy ku ktorej sa chce pripojit
    struct sockaddr_in serv_addr;
    struct hostent* server;

    char buffer[64], zber, * ukaz;

    if (argc < 3)
    {
        fprintf(stderr,"usage %s hostname port\n", argv[0]);
        return 1;
    }
        // funkcia vrati info o hostitelskom pc na zaklade jeho mena
    server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "Error, no such host\n");
        return 2;
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy(
            (char*)server->h_addr,
            (char*)&serv_addr.sin_addr.s_addr,
            server->h_length
    );  // htons() funkcia skonvertuje 16-bit cele cislo z reprezentacie pc, na sietovu
    serv_addr.sin_port = htons(atoi(argv[2]));
        // vytvorenie socketu
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("Error creating socket");
        return 3;
    }
        // poziadane o spojenie so serverom
    if(connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("Error connecting to socket");
        return 4;
    }


        // komunikacia so serverom
    printf("Please enter a message: ");
    // bzero(buffer,256);
   // fgets(buffer, 255, stdin);


    koniec= 1;
    printf("-- Zadajte polynom so zlomkom (v tvare: npr.: 3/4x^2+1/2x...) a stlacte enter\n-- poznamka: Vsetky cisla su zapisuju v tvare zlomku aj cele cisle npr.: 4 zapiste ako 4/1\n");
    do {
        int i = 0;
        bzero(buffer, 64);
        do { // klient posiela polynomi
            printf("\t----Zadajte %d. polynom----\n", (i + 1));
            //fgets(buffer, 63, stdin);
            scanf("%s", buffer);
            n = (int) write(sockfd, buffer, strlen(buffer));
            if (n < 0) {
                perror("Error writing to socket");
                return 5;
            } else if (n > 0) { ++i; }
            bzero(buffer, 64);
        } while (i < 2);

        printf("\n---------------------------------------\n");

        i = 0;
        do { // klient nacita polynomi
            bzero(buffer, 64);
            n = (int) read(sockfd, buffer, 63);
            if (n < 0) {
                perror("Error reading from socket");
                return 6;
            } else if (n > 0) {
                printf("%d. %s \n", i + 1, buffer);
                ++i;
            }
        } while (i < 2);

        do{ // klient posle prikaz
            printf("\t----Zadajte moznost pre vykonanie operacie s polynomami----\n -- 1:Scitanie polynomov\n -- 2:Odcitanie polynomov\n -- 3:Nasobenie polynomov\n -- 4:Delenie polynomov\n"
                   " -- 5:Zvisok po deleni polynomov\n -- 6:Ukoncenie programu\n -- 7:Zadanie novych polynomov\n"); //Zadaj polynomi

            bzero(buffer, 64);
            scanf("%s", buffer);
            n = (int) write(sockfd, buffer, strlen(buffer));
            if (n < 0) {
                perror("Error writing to socket");
                return 5;
            }
            ukaz = strchr(buffer, '\0');
            --ukaz;
            cis = (int) strtol(buffer, &ukaz, 0);
            if (cis > 0) {
                if (cis == 6) { koniec = 0; }
                if (cis == 7) { koniec = 2; }
                printf("Zadani prikaz %d \n", (int) strtol(buffer, &ukaz, 0));

                bzero(buffer, 64); // klient nacita vysledok
                if(koniec == 1) {
                    n = (int) read(sockfd, buffer, 63);
                    if (n < 0) {
                        perror("Error reading from socket");
                        return 6;
                    } else if (n > 0) { printf("Vysledok %s \n", buffer); }
                }
            }
            bzero(buffer,64);

        } while (koniec == 1);
    } while (koniec > 1);


    sleep(10);
    printf("Koniec spojenia. %s\n",buffer);
    close(sockfd);

    return 0;
}