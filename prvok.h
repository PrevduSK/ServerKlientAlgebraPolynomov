#ifndef PRVOK_H
#define PRVOK_H

#include "zlomok.h"

typedef struct prvokPolynomu{
    ZLOMOK cislo;
    int nasobnost;
}PRVOK;

int porovnajNasobPrvkov(PRVOK * pr1, PRVOK * pr2);
PRVOK scitajPrvky(PRVOK * pr1, PRVOK * pr2);
PRVOK odcitajPrvky(PRVOK * pr1, PRVOK * pr2);
PRVOK nasobPrvky(PRVOK * pr1, PRVOK * pr2);
PRVOK delPrvky(PRVOK * pr1, PRVOK * pr2);
void vymazPrvok(PRVOK * pr1);
void vypisPrvok(PRVOK * pr1, char * vpisovac1);
void nacitajZPrvok(PRVOK * pr1, char ** citac1);

#endif