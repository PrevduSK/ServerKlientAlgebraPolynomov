#include "prvok.h"

#define NASOB_PRVKU 12
// porovnave hodnotu nasobnost a vrati rozdiel hodnot ak je ak pr1 > pr2 : rovn > 0, bude kladne ak pr1 > pr2 : rovn > 0, bude kladne
int porovnajNasobPrvkov(PRVOK * pr1, PRVOK * pr2) {
    int rovn = 0;//, rovn0
    if(pr1->nasobnost != pr2->nasobnost){
        // ak pr1 > pr2 : rovn > 0, bude kladne
        // ak pr1 < pr2 : rovn < 0, bude zaporne
       // rovn0 = pr1->nasobnost - pr2->nasobnost;
       rovn = pr1->nasobnost - pr2->nasobnost;
    } 
  /*  if(rovn0 < NASOB_PRVKU && rovn0 > -NASOB_PRVKU) {
        rovn = rovn0;
    } */
    return rovn;
}

PRVOK scitajPrvky(PRVOK * pr1, PRVOK * pr2) {
    PRVOK pr3;
    if(porovnajNasobPrvkov(pr1,pr2) == 0) {
        pr3.cislo = scitajZlomky(&pr1->cislo,&pr2->cislo);
        pr3.nasobnost = pr1->nasobnost;
    }else if(porovnajNasobPrvkov(pr1,pr2) > 0 && porovnajNasobPrvkov(pr1,pr2) < NASOB_PRVKU) {pr3= *pr1;}
    else if(porovnajNasobPrvkov(pr1,pr2) < 0 && porovnajNasobPrvkov(pr1,pr2) > -NASOB_PRVKU) {pr3= *pr2;} //
    return pr3;
}

PRVOK odcitajPrvky(PRVOK * pr1, PRVOK * pr2) {
    PRVOK pr3;
    if(porovnajNasobPrvkov(pr1,pr2) == 0) {
        pr3.cislo = odcitajZlomky(&pr1->cislo,&pr2->cislo);
        pr3.nasobnost = pr1->nasobnost;
    }else if(porovnajNasobPrvkov(pr1,pr2) > 0 && porovnajNasobPrvkov(pr1,pr2) < NASOB_PRVKU) {pr3= *pr1;}
    else if(porovnajNasobPrvkov(pr1,pr2) < 0 && porovnajNasobPrvkov(pr1,pr2) > -NASOB_PRVKU) {pr3= *pr2;} //
    return pr3;
}
// nasobz prvky polynomu
PRVOK nasobPrvky(PRVOK * pr1, PRVOK * pr2) {
    PRVOK pr3;
    pr3.cislo = nasobZlomky(&pr1->cislo,&pr2->cislo);
    pr3.cislo = jednotkovyZlomok(&pr3.cislo);
    pr3.nasobnost = pr1->nasobnost + pr2->nasobnost;
    return pr3;
}
// deli prvky polynomu
PRVOK delPrvky(PRVOK * pr1, PRVOK * pr2) {
    PRVOK pr3;
    pr3.cislo = delZlomky(&pr1->cislo,&pr2->cislo);
    pr3.cislo = jednotkovyZlomok(&pr3.cislo);
    pr3.nasobnost = pr1->nasobnost - pr2->nasobnost;
    return pr3;
}
// vymaze prvok z polynomu
void vymazPrvok(PRVOK * pr1) {
    vymazZlomok(&pr1->cislo);
    pr1->nasobnost = 0;
}
// vipisuje zlomok a nasobnost ako prvok polynomu
void vypisPrvok(PRVOK * pr1, char * vpisovac1) {
    vypisZlomok(&pr1->cislo, vpisovac1);
    if(pr1->nasobnost > 1) {
        //printf("x^%d",pr1->nasobnost);
        //*vpisovac1 = ("x^"+pr1->nasobnost);
        sprintf(vpisovac1,"%sx^%d",vpisovac1,pr1->nasobnost);
    }else if(pr1->nasobnost == 1) {
            //printf("x");
            //*vpisovac1 = "x";
        sprintf(vpisovac1,"%sx",vpisovac1);
        }
    //printf("Vlozene v prvku %s \n",vpisovac1);
}
// nacita z konzoly zlomok a nasobnost ktore ulozi ako prvok polynommu
void nacitajZPrvok(PRVOK * pr1, char ** citac1) {
    char * pomCit1;
    nacitajZZlomok(&pr1->cislo,&(*citac1));
    if(NULL != strchr(*citac1,'^')){
            pomCit1= strchr(*citac1,'^');
            pomCit1++;
            pr1->nasobnost= atoi(pomCit1);
            pomCit1++;
            *citac1 = pomCit1;
        } else if(NULL != strchr(*citac1,'x')) {
                    pomCit1= strchr(*citac1,'x');
                    pr1->nasobnost = 1;
                    pomCit1++;
                    *citac1 = pomCit1;
                } else if(NULL != strchr(*citac1,'\0')) {
                        pr1->nasobnost = 0;
                        **citac1 = '\0';
                    }
}

#undef NASOB_PRVKU