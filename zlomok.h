#ifndef ZLOMOK_H
#define ZLOMOK_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct zlomok{
    int citatel;
    unsigned int menovatel;
}ZLOMOK;

//int porovnajMenZlomku(ZLOMOK * z1, ZLOMOK * z2);
ZLOMOK scitajZlomky(ZLOMOK * z1, ZLOMOK * z2);
ZLOMOK odcitajZlomky(ZLOMOK * z1, ZLOMOK * z2);
ZLOMOK nasobZlomky(ZLOMOK * z1, ZLOMOK * z2);
ZLOMOK delZlomky(ZLOMOK * z1, ZLOMOK * z2);
ZLOMOK jednotkovyZlomok(ZLOMOK * z1);
void vymazZlomok(ZLOMOK * z1);
void vypisZlomok(ZLOMOK * z1, char * vpisovac0);
void nacitajZZlomok(ZLOMOK * z1, char ** citac0);

#endif