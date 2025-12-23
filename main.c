#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* struck birkaç veriable türünü tek bir yerde toplar.
 * Dizilerde tek veri tipi kullanılırken struck da bırden fazla verı tıpını kendi olusturdugumuz verı tıpıne koyabılırz
 */
struct Personel {
    char sicil[50];
    char zamanStr[10];
    char islem;
    int toplamDakika;
};
int saatDakikayaCevir(char *zaman) {
    int saat, dakika;
    sscanf(zaman,"%d:%d",&saat,&dakika);
    return(saat*60+dakika);
}
int main (void){
    FILE *fgun, *fgec, *ferken;
    struct Personel p;

    int mesaiBaslangic=9*60; //9.00=540 dakika
    int mesaiBitis=17*60; //

    char enGecGelenSicil[50]="";
    int enGecGecikmeSuresi=0;

    char enErkenCikanSicil[50]="";
    int enErkenErkenCikmaSuresi=0;
    int enErkenCikanDakika=mesaiBitis;

    fgun=fopen("gunluk.dat","r");

    if (fgun==NULL) {
        printf("Errore apertur un gunluk.dat\n");
        return 1;
    }

    while (fscanf(fgun,"%s %s %c", p.sicil,p.zamanStr,&p.islem)!=EOF) {
        int suankiDakika=saatDakikayaCevir(p.zamanStr);

        if (p.islem== '<') {
            if (suankiDakika>mesaiBaslangic) {
                int gecikme=suankiDakika-mesaiBaslangic;
                if (gecikme>enGecGecikmeSuresi) {
                    enGecGecikmeSuresi=gecikme;
                    strcpy(enGecGelenSicil,p.sicil);
                }
            }
        }
        else if (p.islem=='>') {
            if (suankiDakika<mesaiBitis) {
                int erkenCikma=mesaiBitis-suankiDakika;

                if (suankiDakika<enErkenCikanDakika) {
                    enErkenCikanDakika=suankiDakika;
                    enErkenErkenCikmaSuresi=erkenCikma;
                    strcpy(enErkenCikanSicil,p.sicil);

                }
            }
        }
    }
    fclose(fgun);

    fgec=fopen("gec.date","w"   );
    if (fgec!=NULL) {
        if (enGecGecikmeSuresi>0) {
            fprintf(fgec,"%s %d\n", enGecGelenSicil, enGecGecikmeSuresi);
        }
        else {
            fprintf(fgec,"Gec kalan personel yok\n");
        }
        fclose(fgec);
    }
    ferken = fopen("erken.dat", "w");
    if (ferken != NULL) {
        if (enErkenErkenCikmaSuresi > 0) {
            fprintf(ferken, "%s %d\n", enErkenCikanSicil, enErkenErkenCikmaSuresi);
        } else {
            fprintf(ferken, "Erken cikan personel yok.\n");
        }
        fclose(ferken);
    }

    return 0;
}