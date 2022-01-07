#include "polynom.h"

#define DLZKA_CITACA 34
    //ak to je polynom vrati maximalin pocet neznamich ktore moze obsaovat
int jeToPolynom(POLYNOM * p1) {
    int stpnP = p1->polePrvk[0].nasobnost,pocNez = p1->pocetHodnP, pom =0;
    if(stpnP >= pocNez){
        pom = stpnP +1;
    } return pom;
}
// porovnava velkost na zaklade nasobnosti neznamych
int porovnajStupenPolynomu(POLYNOM * p1, POLYNOM * p2, int i, int j) {
    // p1 == p2
    int pomVStupP=0;
    //if(i <= p1->pocetHodnP && j <= p2->pocetHodnP) {
       /* if(p1->polePrvk[i].nasobnost != p2->polePrvk[j].nasobnost) {
            // p1 > p2 vrati rozdiel ako kladne cislo
            // p2 > p1 vrati rozdiel ako zaporne cislo
            // 1>0  1-0 == 1;  0<1 0-1 == -1 
            pomVStupP = p1->polePrvk[i].nasobnost - p2->polePrvk[j].nasobnost;
        } */ 
    if(0 != porovnajNasobPrvkov(&p1->polePrvk[i],&p2->polePrvk[j])) {
        pomVStupP = porovnajNasobPrvkov(&p1->polePrvk[i],&p2->polePrvk[j]);
    } return pomVStupP; 
}
// porovnava velokst polynomu na zaklade poctu neznamych
int porovnajPocetHodnotPolynomu(POLYNOM * p1, POLYNOM * p2) {
    if(p1->pocetHodnP > p2->pocetHodnP) {
        return p1->pocetHodnP;
    } else {return p2->pocetHodnP;}
}
// vrati vysledok scitani 
POLYNOM scitajPolynomi(POLYNOM * p1, POLYNOM * p2) { //
    POLYNOM p3;
    p3.pocetHodnP = 0;
    int i =0,j =0,pomPocHodI = p1->pocetHodnP,pomPocHodJ = p2->pocetHodnP;
      // i<= pomPocHod prtoze aj prvok: nasobnost = 0 ma cislo,
    while(i<= pomPocHodI || j<= pomPocHodJ) {
        //!!! pozor pravidla operacie s mocninami: Treba upravit vfunkie vypoctov alebo
        // za definovat zlomky ako strukturu!!!
        // ak je zadany polynom ktori ma vynechane niektore neznama tak ich nevie doplnit
        if(porovnajStupenPolynomu(p1,p2,i,j) == 0) {
            p3.polePrvk[p3.pocetHodnP] = scitajPrvky(&p1->polePrvk[i],&p2->polePrvk[j]);
            p3.pocetHodnP++;
            if(i < pomPocHodI)      {i++;}
            if(j < pomPocHodJ)      {j++;}
            // pocet hodnot zladit zo stupnom polynomu per.: ak polynom je 4 stupna tak pocet neznamich(prvkov) ktore moze obsahovat sa rovna 4+1 a menaj 
            // takze pocetHodnP <= nasobnost[0] + 1 ; nasdobn = 4, tak polynom moze obsahovat najviac 5 prvkov //najnenej 1
            if(jeToPolynom(p1) >= jeToPolynom(p2)){
                if(jeToPolynom(p1) == p3.pocetHodnP) {
                    p3.pocetHodnP--;
                    break;
                }
            } else if(jeToPolynom(p2) == p3.pocetHodnP) {
                        p3.pocetHodnP--;
                        break;
                    }
            continue;
        }  

       if(porovnajStupenPolynomu(p1,p2,i,j) >0) {
            if(i < pomPocHodI){
                p3.polePrvk[p3.pocetHodnP] = p1->polePrvk[i];
                p3.pocetHodnP++;
                if(i < pomPocHodI)      {i++;}
                continue;
            } else if(j == pomPocHodJ) {
                p3.polePrvk[p3.pocetHodnP] = p2->polePrvk[j];
                break;
            }
        }
        if(porovnajStupenPolynomu(p1,p2,i,j) <0) {
            if(j < pomPocHodJ){
                p3.polePrvk[p3.pocetHodnP] = p2->polePrvk[j];
                p3.pocetHodnP++;
                if(j < pomPocHodJ)      {j++;}
                continue;
            } else if(i == pomPocHodI) {
                p3.polePrvk[p3.pocetHodnP] = p1->polePrvk[i];
                break;
            }
        } 
   }
    return p3;
}
// vrati vysledok odcitania
POLYNOM odcitajPolynomi(POLYNOM * p1, POLYNOM * p2) {
    POLYNOM p3;
    p3.pocetHodnP = 0;
    int i =0,j =0,pomPocHodI = p1->pocetHodnP,pomPocHodJ= p2->pocetHodnP;
    while(i<= pomPocHodI || j<= pomPocHodJ) {
        //!!! pozor pravidla operacie s mocninami: Treba upravit vfunkie vypoctov alebo
        // za definovat zlomky ako strukturu!!!
        if(porovnajStupenPolynomu(p1,p2,i,j) == 0) {
            p3.polePrvk[p3.pocetHodnP] = odcitajPrvky(&p1->polePrvk[i],&p2->polePrvk[j]);
            p3.pocetHodnP++;
            if(i < pomPocHodI)      {i++;}
            if(j < pomPocHodJ)      {j++;}
            // pocet hodnot zladit zo stupnom polynomu per.: ak polynom je 4 stupna tak pocet neznamich(prvkov) ktore moze obsahovat sa rovna 4+1 a menaj 
            // takze pocetHodnP <= nasobnost[0] + 1 ; nasdobn = 4, tak polynom moze obsahovat najviac 5 prvkov //najnenej 1
            if(jeToPolynom(p1) >= jeToPolynom(p2)){  //p3.pocetHodnP == (pomPocHodI +1) + (pomPocHodJ +1) 
                if(jeToPolynom(p1) == p3.pocetHodnP) {
                    p3.pocetHodnP--;
                    break;
                }
            } else if(jeToPolynom(p2) == p3.pocetHodnP) {
                        p3.pocetHodnP--;
                        break;
                    }
            continue;
        }
        if(porovnajStupenPolynomu(p1,p2,i,j) >0) {
            if(i < pomPocHodI){
                p3.polePrvk[p3.pocetHodnP] = p1->polePrvk[i];
                //p3.polePrvk[p3.pocetHodnP].nasobnost = p1->polePrvk[i].nasobnost;
                p3.pocetHodnP++;
                if(i < pomPocHodI)      {i++;}
                continue;
            } else if(j == pomPocHodJ) {
                        p3.polePrvk[p3.pocetHodnP].cislo.citatel = -p2->polePrvk[j].cislo.citatel;
                        p3.polePrvk[p3.pocetHodnP].cislo.menovatel = p2->polePrvk[j].cislo.menovatel;
                        p3.polePrvk[p3.pocetHodnP].nasobnost = p2->polePrvk[j].nasobnost;
                        break;
                    }
        }
        if(porovnajStupenPolynomu(p1,p2,i,j) <0) {
            if(j < pomPocHodJ){
                p3.polePrvk[p3.pocetHodnP].cislo.citatel = -p2->polePrvk[j].cislo.citatel;
                p3.polePrvk[p3.pocetHodnP].cislo.menovatel = p2->polePrvk[j].cislo.menovatel;
                p3.polePrvk[p3.pocetHodnP].nasobnost = p2->polePrvk[j].nasobnost;
                p3.pocetHodnP++;
                if(j < pomPocHodJ)      {j++;}
                continue;
            } else if(i == pomPocHodI) {
                p3.polePrvk[p3.pocetHodnP] = p1->polePrvk[i];
                break;
            }
        }
    }
    return p3;
}
// vrati vysledok nasobenia
POLYNOM nasobPolynomi(POLYNOM * p1, POLYNOM * p2) {
    POLYNOM p3,p4,p5;
    p3.pocetHodnP = 0;
    p4.pocetHodnP=0;
    p5.pocetHodnP=0;
    int i =0,j =0,pomPocHodI = p1->pocetHodnP,pomPocHodJ= p2->pocetHodnP,pomSigSuc= 0;
      // i<= pomPocHod prtoze aj prvok: nasobnost = 0 ma cislo,
    while(j<= pomPocHodJ) {
        //!!! ak je p1 < p2 tak vysledok nieje spravni
        while(i<= pomPocHodI) {
            //0,2,4,6...
            if(j%2 == 0) {
                p3.polePrvk[p3.pocetHodnP] = nasobPrvky(&p1->polePrvk[i],&p2->polePrvk[j]);
                p3.pocetHodnP++;
                i++;
            //1,3,5,7...
            } else {
                p4.polePrvk[p4.pocetHodnP] = nasobPrvky(&p1->polePrvk[i],&p2->polePrvk[j]);
                p4.pocetHodnP++;
                i++;
            }
        } 
        if(j%2 == 0) {p3.pocetHodnP--;}
        else {p4.pocetHodnP--;} 
        // j=1,3,5,7... j%2!=0
       if(j!=0 && j%2!=0 && pomSigSuc==0) {
            p5= scitajPolynomi(&p3,&p4);
            pomSigSuc++;
            //j=2,4,6... pomSigSuc = 1 pomSigSuc%2!=0
        }else if(j!=0 && j%2==0 && pomSigSuc%2!=0) {
            p5= scitajPolynomi(&p3,&p5);
            pomSigSuc++;
        }else if(j!=0 && j%2!=0 && pomSigSuc%2==0) {
            p5= scitajPolynomi(&p4,&p5);
            pomSigSuc--;   
        } 
        if(j%2 !=0) {
            p3.pocetHodnP=0;
            p4.pocetHodnP=0;
        }
        i=0;
        j++;
    }
    return p5;
}
// vrati vysledok delania
POLYNOM delPolynomi(POLYNOM * p1, POLYNOM * p2) {
    POLYNOM p3,p4,p5,p6;
    p3.pocetHodnP = 0;
    p4.pocetHodnP=0;
    p5.pocetHodnP=0;
    p6.pocetHodnP=0;
    int i =0,j =0,pomPocHodI = p1->pocetHodnP,pomPocHodJ= p2->pocetHodnP;//, 
   
    if(porovnajStupenPolynomu(p1,p2,i,j) > 0){
        do{
            do{
                if(j== 0){
                    p3.polePrvk[p3.pocetHodnP] = delPrvky(&p1->polePrvk[i],&p2->polePrvk[j]);
                }
                p4.polePrvk[j] = nasobPrvky(&p3.polePrvk[p3.pocetHodnP],&p2->polePrvk[j]);
                if(porovnajStupenPolynomu(p1,&p4,i,j) == 0) {
                    p5.polePrvk[j] = odcitajPrvky(&p1->polePrvk[i],&p4.polePrvk[j]);
                    i++,j++;
                }else {i++;};
                    
            }while(j<= pomPocHodJ);
            j=i;
            p5.polePrvk[j] = p1->polePrvk[i];
            p5.pocetHodnP++; //=1
            p3.pocetHodnP++; //=1
            j=0;
            if(p5.polePrvk[p5.pocetHodnP].nasobnost - p2->polePrvk[j].nasobnost >= 0) {
                
                p3.polePrvk[p3.pocetHodnP] = delPrvky(&p5.polePrvk[p5.pocetHodnP],&p2->polePrvk[j]);
                
                while(j<= pomPocHodJ) {
                    p4.polePrvk[j] = nasobPrvky(&p3.polePrvk[p3.pocetHodnP],&p2->polePrvk[j]);
                    j++;
                }
                i=1,j=0;
                while(j<= pomPocHodJ) {
                    if(porovnajStupenPolynomu(&p5,&p4,i,j) == 0) {
                        p6.polePrvk[j] = odcitajPrvky(&p5.polePrvk[i],&p4.polePrvk[j]);
                        i++,j++;
                    }else {i++;};
                } 
                if(p5.polePrvk[p5.pocetHodnP].nasobnost - p2->polePrvk[0].nasobnost == 0) {break;}
            }
            
            p6.pocetHodnP=j; // 2, i == 3
            p6.polePrvk[p6.pocetHodnP] = p1->polePrvk[i];
            p6.pocetHodnP--; //=1
            p3.pocetHodnP++; //=2
            j=0;
            if(p6.polePrvk[p6.pocetHodnP].nasobnost - p2->polePrvk[j].nasobnost >= 0) {
                
                p3.polePrvk[p3.pocetHodnP] = delPrvky(&p6.polePrvk[p6.pocetHodnP],&p2->polePrvk[j]);
                
                while(j<= pomPocHodJ){
                    p4.polePrvk[j] = nasobPrvky(&p3.polePrvk[p3.pocetHodnP],&p2->polePrvk[j]);
                    j++;
                }
                //zvisok
                j=0; //i=1,
                while(j<= pomPocHodJ) {
                    if(porovnajStupenPolynomu(&p6,&p4,i,j) == 0) {
                        p5.polePrvk[j] = odcitajPrvky(&p6.polePrvk[p6.pocetHodnP],&p4.polePrvk[j]);
                        p6.pocetHodnP++,j++;
                    }else {i++;};
                }
                if(p6.polePrvk[p6.pocetHodnP].nasobnost - p2->polePrvk[0].nasobnost == 0) {break;}
            }
            
        }while(i<= pomPocHodI);
    } else {
            p3.polePrvk[0].cislo.citatel = 0;
            p3.polePrvk[0].cislo.menovatel = 1;
            p3.polePrvk[0].nasobnost = 0;
            printf("Nieje mozne delit polynom x^%d polynomom x^%d\n", p1->polePrvk[0].nasobnost,p2->polePrvk[0].nasobnost);
          //  char *buf;
       // vypisPolynom(&p3,buf);
      //  printf("%s\n", buf);
        vymazPolynom(&p3);
    }
    return p3;
}
// funkcia vrati zvisko po deleni
POLYNOM zvisokDelPolynomi(POLYNOM * p1, POLYNOM * p2) {
    POLYNOM p3,p4,p5,p6;
    p3.pocetHodnP = 0;
    p4.pocetHodnP=0;
    p5.pocetHodnP=0;
    p6.pocetHodnP=0;
    int i =0,j =0,pomPocHodI = p1->pocetHodnP,pomPocHodJ= p2->pocetHodnP,pomPocP5 = p5.pocetHodnP, pomPocP6 = p6.pocetHodnP;//,

    
    if(porovnajStupenPolynomu(p1,p2,i,j) > 0){
        do{
            do{
                if(j== 0){
                   p3.polePrvk[p3.pocetHodnP] = delPrvky(&p1->polePrvk[i],&p2->polePrvk[j]);
                }
                p4.polePrvk[j] = nasobPrvky(&p3.polePrvk[p3.pocetHodnP],&p2->polePrvk[j]);

                if(porovnajStupenPolynomu(p1,&p4,i,j) == 0) {
                    p5.polePrvk[j] = odcitajPrvky(&p1->polePrvk[i],&p4.polePrvk[j]);
                    i++,j++;
                }else {i++;};
                
            }while(j<= pomPocHodJ);
            j=i;
            p5.polePrvk[j] = p1->polePrvk[i];
            pomPocP5++; //=1
            p3.pocetHodnP++; //=1
            j=0;
            if(p5.polePrvk[pomPocP5].nasobnost - p2->polePrvk[j].nasobnost >= 0) {
                
                p3.polePrvk[p3.pocetHodnP] = delPrvky(&p5.polePrvk[pomPocP5],&p2->polePrvk[j]);
                
                while(j<= pomPocHodJ) {
                    p4.polePrvk[j] = nasobPrvky(&p3.polePrvk[p3.pocetHodnP],&p2->polePrvk[j]);
                    j++;
                }
                i=1,j=0;
                while(j<= pomPocHodJ) {
                    if(porovnajStupenPolynomu(&p5,&p4,i,j) == 0) {
                        p6.polePrvk[j] = odcitajPrvky(&p5.polePrvk[i],&p4.polePrvk[j]);
                        i++,j++;
                        if(p6.pocetHodnP < pomPocHodJ){
                            p6.pocetHodnP++;
                        }
                    }else {i++;};
                } 
                if(p5.polePrvk[pomPocP5].nasobnost - p2->polePrvk[0].nasobnost == 0) {return p6;}
            }
            
            pomPocP6=j; // 2, i == 3
            p6.polePrvk[pomPocP6] = p1->polePrvk[i];
            pomPocP6--; //=1
            p3.pocetHodnP++; //=2
            j=0;
            if(p6.polePrvk[pomPocP6].nasobnost - p2->polePrvk[j].nasobnost >= 0) {
                
                p3.polePrvk[p3.pocetHodnP] = delPrvky(&p6.polePrvk[pomPocP6],&p2->polePrvk[j]);
                
                while(j<= pomPocHodJ){
                    p4.polePrvk[j] = nasobPrvky(&p3.polePrvk[p3.pocetHodnP],&p2->polePrvk[j]);
                    j++;
                }
                //zvisok
                i=1,j=0; //i=1,
                while(j<= pomPocHodJ) {
                    if(porovnajStupenPolynomu(&p6,&p4,i,j) == 0) {
                        p5.polePrvk[j] = odcitajPrvky(&p6.polePrvk[i],&p4.polePrvk[j]);
                        i++,j++;
                        if(p5.pocetHodnP < pomPocHodJ){
                            p5.pocetHodnP++;
                        }
                    } else {i++;};
                }
                if(p6.polePrvk[pomPocP6].nasobnost - p2->polePrvk[0].nasobnost == 0) {return p5;}
            }
                
        }while(i<= pomPocHodI);
    } else {
        p5.polePrvk[0].cislo.citatel = 0;
        p5.polePrvk[0].cislo.menovatel = 0;
        p5.polePrvk[0].nasobnost = 0;
        printf("Nieje mozne vypocitat zvisok po deleni\n");
     /*   char *buf;
        vypisPolynom(&p5,buf);
        printf("%s\n", buf); */
        vymazPolynom(&p5);
        }
    return p5;
}
// nastavi hodnotu pocetHodnP na 0
void initP(POLYNOM * p1){
    p1->pocetHodnP = 0;
}
// nacita z konzoly cely polynom a
    // na miesto polynom * p1 dat baffer *char
void nacitajZPolynom(POLYNOM * p1, char *citac) {
        // nacita polynom
    //char citac[DLZKA_CITACA];
    char * pomCit;
    //scanf("%s",citac);
    pomCit = citac;

    while(pomCit != strchr(pomCit,'\0')) {
        nacitajZPrvok(&p1->polePrvk[p1->pocetHodnP],&pomCit);
        if(pomCit == strchr(pomCit,'\0')) {
            //pomC = 0; 
            continue;}
        else {p1->pocetHodnP++;}
    }
}
// vypise nakonyolu polynom
void vypisPolynom(POLYNOM * p1, char *vpisovac) {
    // vypise polynom
    sprintf(vpisovac,'\0');

    //printf("vlozene v polynom %s \n",vpisovac);
    int i = 0;
    while(i <= p1->pocetHodnP) {
        vypisPrvok(&p1->polePrvk[i], vpisovac);
        i++;
    }
    //printf("\n");
    sprintf(vpisovac,"%s\n",vpisovac);
    //printf("Kompletne %s \n",vpisovac);
}
// vymaze polynom tak ze znuluje vsetky jeho hodnoty
void vymazPolynom(POLYNOM * p1) {
    int i = p1->pocetHodnP;
    do{
        vymazPrvok(&p1->polePrvk[i]);
        p1->pocetHodnP--;
        i--;
    } while(-1 < i);
}
// prida polynom do programu tak ze zavol funkciu initP() a nacitajZPolynom()
void pridajPolynom(POLYNOM * p1, char *citac) {
    initP(p1);
    nacitajZPolynom(p1, citac);
    //vypisPolynom(p1);
}

#undef DLZKA_CITACA