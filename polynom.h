#ifndef POLYNOM_H
#define POLYNOM_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "prvok.h"

#define STUPEN_POLYN 8

typedef struct polynom{
    PRVOK polePrvk[STUPEN_POLYN];
    int pocetHodnP;
}POLYNOM;

int jeToPolynom(POLYNOM * p1);
int porovnajStupenPolynomu(POLYNOM * p1, POLYNOM * p2, int i, int j);
int porovnajPocetHodnotPolynomu(POLYNOM * p1, POLYNOM * p2);
POLYNOM scitajPolynomi(POLYNOM * p1, POLYNOM * p2); //POLYNOM
POLYNOM odcitajPolynomi(POLYNOM * p1, POLYNOM * p2);
POLYNOM nasobPolynomi(POLYNOM * p1, POLYNOM * p2);
POLYNOM delPolynomi(POLYNOM * p1, POLYNOM * p2);
POLYNOM zvisokDelPolynomi(POLYNOM * p1, POLYNOM * p2);
void initP(POLYNOM * p1);
void nacitajZPolynom(POLYNOM * p1, char *citac);
void vypisPolynom(POLYNOM * p1, char *vpisovac);
void vymazPolynom(POLYNOM * p1);
void pridajPolynom(POLYNOM * p1, char *citac);

#undef STUPEN_POLYN
#endif