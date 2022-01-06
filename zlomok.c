#include "zlomok.h"

// funkcia scita zlomky a vrati vysledok
ZLOMOK scitajZlomky(ZLOMOK * z1, ZLOMOK * z2) {
    ZLOMOK z3;
    int pomMen=0;
    if(z1->menovatel != z2->menovatel) {
            // z1->menovatel > z2->menovatel
        if(z1->menovatel > z2->menovatel) {
            if(z1->menovatel % z2->menovatel ==0){
                pomMen= (int) z1->menovatel/z2->menovatel;
                z3.menovatel = pomMen * z2->menovatel;
                z3.citatel = pomMen* z2->citatel + z1->citatel;
                return z3;
            }
            else {
                z3.menovatel = z1->menovatel * z2->menovatel;
                z3.citatel = z1->menovatel* z2->citatel + z1->citatel;
                return z3;
            }
        }
        else {    // z1->menovatel < z2->menovatel
            if(z2->menovatel % z1->menovatel ==0){
                pomMen= (int) z2->menovatel/z1->menovatel;
                z3.menovatel = pomMen * z1->menovatel;
                z3.citatel = pomMen* z1->citatel + z2->citatel;
                return z3;
            }
            else {
                z3.menovatel = z1->menovatel * z2->menovatel;
                z3.citatel = z2->menovatel* z1->citatel + z2->citatel;
                return z3;
            }
        }
    } else {     // z1->menovatel == z2->menovatel
        z3.menovatel = z1->menovatel;
        z3.citatel = z1->citatel + z2->citatel;
        return z3;
    }
}
// funkcia odcita zlomky a vrati vysledok
ZLOMOK odcitajZlomky(ZLOMOK * z1, ZLOMOK * z2) {
    ZLOMOK z3;
    int pomMen=0;
    if(z1->menovatel != z2->menovatel) {
            // z1->menovatel > z2->menovatel
        if(z1->menovatel > z2->menovatel) {
            if(z1->menovatel % z2->menovatel ==0){
                pomMen= (int) z1->menovatel/z2->menovatel;
                z3.menovatel = pomMen * z2->menovatel;
                z3.citatel = z1->citatel - pomMen* z2->citatel;
                return z3;
            }
            else {
                z3.menovatel = z1->menovatel * z2->menovatel;
                z3.citatel = z1->citatel - z1->menovatel* z2->citatel;
                return z3;
            }
        }else {    // z1->menovatel < z2->menovatel
            if(z2->menovatel % z1->menovatel ==0){
                pomMen= (int) z2->menovatel/z1->menovatel;
                z3.menovatel = pomMen * z1->menovatel;
                z3.citatel = pomMen* z1->citatel - z2->citatel;
                return z3;
            }
            else {
                z3.menovatel = z1->menovatel * z2->menovatel;
                z3.citatel = z2->menovatel* z1->citatel - z2->citatel;
                return z3;
            }
        }
    } else {     // z1->menovatel == z2->menovatel
            z3.menovatel = z1->menovatel;
            z3.citatel = z1->citatel - z2->citatel;
            return z3;
        }
}
// funkcia nasoby zlomky a vrati vyslednu hodnotu operacie
ZLOMOK nasobZlomky(ZLOMOK * z1, ZLOMOK * z2) {
    ZLOMOK z3;
    z3.menovatel = z1->menovatel * z2->menovatel;
    z3.citatel = z2->citatel * z1->citatel;
    return z3;
}
// funkcia vydeli zlomky a vrati vyslednu hodnotu operacie
ZLOMOK delZlomky(ZLOMOK * z1, ZLOMOK * z2) {
    ZLOMOK z3;
    z3.menovatel = z1->menovatel * z2->citatel;
    z3.citatel = z2->menovatel * z1->citatel;
    return z3;
}
// funkcia rozklada zlomok na zakladny zlomok a vrati jeho hodnotu
ZLOMOK jednotkovyZlomok(ZLOMOK * z1) {
    ZLOMOK z3; 
        for(int i=2; i<10; i++) {
            if((z1->citatel % i == 0) && (z1->menovatel % i == 0)) {
                z3.citatel = z1->citatel / i;    
                z3.menovatel = z1->menovatel / i;
            }else {z3 = *z1;}
        }
        return z3;
}
// funkcia vymze zlomok (vynuluje)
void vymazZlomok(ZLOMOK * z1) {
    z1->citatel = 0;
    z1->menovatel = 0;
}
// funkcia vypise zlomok
void vypisZlomok(ZLOMOK * z1, char * vpisovac0) {
    //printf("%+d/%d",z1->citatel,z1->menovatel);
    //*vpisovac0 = ('%+'+z1->citatel+'/'+z1->menovatel);
    sprintf(vpisovac0,"%s%+d/%d",vpisovac0,z1->citatel,z1->menovatel);
   // printf("Vlozene v zlomok %s \n",vpisovac0);
}
// funkcia nacita zlomok z konzoly
void nacitajZZlomok(ZLOMOK * z1, char ** citac0) {
    char * pomCit0;
   // char znak;
    if(NULL != strchr(*citac0,'\0')) {
        if(atoi(*citac0)== 0) {
            if( strchr(*citac0,'+') == strchr(*citac0,'-') && *citac0 < strchr(*citac0,'x')) { // NULL
                z1->citatel = -1;
                z1->menovatel = 1;
            } else if(NULL != strchr(*citac0,'x')) {
                z1->citatel = 1;
                z1->menovatel = 1;
            }
        } else { // atoi(*citac0)!= 0
            z1->citatel = atoi(*citac0);
           // znak = z1->citatel + '0';
            //pomCit0= strchr(*citac0,znak);
            pomCit0 = *citac0;
            if(pomCit0 == strchr(*citac0,'-') || pomCit0 == strchr(*citac0,'+')){
                pomCit0++;
            }
            pomCit0++;
            if(pomCit0 == strchr(*citac0,'/')){
                pomCit0= strchr(*citac0,'/');
                pomCit0++;
                z1->menovatel = atoi(pomCit0);
                *citac0 = pomCit0;
            } else
            if(pomCit0 == strchr(*citac0,'x')) {
                z1->menovatel = 1;
            } else
            if(NULL != strchr(pomCit0,'\0')) {
                    z1->menovatel = 1;
            }
            else {
                printf("Zadana hondota je neplatna.\n Do dalsirho polynomu zadajte / a slacte enter\n");
            }
        }
    }
}
