#include "polynom.h"

// vyladenia programu, vyutit aj prvok
 // hlavny program nekonecni cyklus ktori sa ukonci na poziadanie
int main(int argc, char **argv) {
    
    POLYNOM poly[2], pomPoly;//poly1, poly2,pomPoly;
    int koniec,cis;
    
    char zber, citac[265];
    printf("-- Zadajte polynom so zlomkom (v tvare: npr.: 3/4x^2+1/2x...) a stlacte enter\n-- poznamka: Vsetky cisla su zapisuju v tvare zlomku aj cele cisle npr.: 4 zapiste ako 4/1\n");
    do{
        koniec= 1;
        int i=0;
       do{
            printf("\t----Zadajte %d. polynom----\n",(i+1));
            bzero(citac,256);
            scanf("%s",citac);
            pridajPolynom(&poly[i], citac);
            ++i;
       } while (i<2);

       /* printf("\t----Zadajte 2. polynom----\n");
        bzero(citac,60);
        scanf("%s",&citac);
        pridajPolynom(&poly2, &citac); */
        printf("\n---------------------------------------\n");
        //bzero(&citac,256);
        vypisPolynom(&poly[0], citac);
        printf("%s\n", citac);
        vypisPolynom(&poly[1], citac);
        printf("%s\n", citac);
        initP(&pomPoly);
        
        printf("\t----Zadajte moznost pre vykonanie operacie s polynomami----\n -- 1:Scitanie polynomov\n -- 2:Odcitanie polynomov\n -- 3:Nasobenie polynomov\n -- 4:Delenie polynomov\n"
        " -- 5:Zvisok po deleni polynomov\n -- 6:Ukoncenie programu\n -- 7:Zadanie novych polynomov\n"); //Zadaj polynomi
        do{
            scanf("%c",&zber);
            cis= atoi(&zber);
            switch (cis) {
                    //. scitaj polynomy
                case 1:
                        printf("Scitanie polynomov\n =");
                        pomPoly= scitajPolynomi(&poly[0],&poly[1]);
                        vypisPolynom(&pomPoly, citac);
                    printf("%s\n",citac);
                        break;
                    // odcitaj polynomy
                case 2:
                        printf("Odcitanie polynomov\n =");
                        pomPoly= odcitajPolynomi(&poly[0],&poly[1]);
                        vypisPolynom(&pomPoly, citac);
                    printf("%s\n",citac);
                    break;
                    // nasob polynomy
                case 3:
                        printf("Nasobenie polynomov\n =");
                        pomPoly= nasobPolynomi(&poly[0],&poly[1]);
                        vypisPolynom(&pomPoly, citac);
                        printf("%s\n",citac);
                    break;
                    // del polynomy
                case 4: 
                        printf("Delenie polynomov\n =");
                        pomPoly= delPolynomi(&poly[0],&poly[1]);
                        vypisPolynom(&pomPoly, citac);
                        break;
                    // zvisok po deleny
                case 5: 
                        printf("Zvisok po deleni polynomov\n =");
                        pomPoly= zvisokDelPolynomi(&poly[0],&poly[1]);
                        vypisPolynom(&pomPoly, citac);
                    printf("%s\n",citac);
                    break;
                    // koniec programu
                case 6: 
                        vymazPolynom(&poly[0]);
                        vymazPolynom(&poly[1]);
                        vymazPolynom(&pomPoly);
                        printf("\n___________________________________koniec______________________________\n");
                        koniec = 0;
                        break;
                    // zadanie znova hodnoat
                case 7: 
                        vymazPolynom(&poly[0]);
                        vymazPolynom(&poly[1]);
                        vymazPolynom(&pomPoly);
                        printf("\n________________________________________________________________________________________________________________\nZadajte nove hodnoty\n");
                        koniec = 2;
                        break;
                default: printf("Zadanu moznost nieje mozne vykonat.\n");
                        break;
                
            } 
        } while(koniec == 1);
    } while(koniec > 1);
	return 0;
}