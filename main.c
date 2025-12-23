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
    sscanf(zaman, "%d:%d", &saat, &dakika);
    return (saat * 60) + dakika;
}

int main(void) {
    FILE *fgun, *fgec, *ferken;
    struct Personel p;

    // (Not: Bu veriler artık p yapısının içinde tutuluyor)

    int mesaiBaslangic = 9 * 60;
    int mesaiBitis = 17 * 60;

    // En geç gelen ve en erken çıkanı tutacak değişkenler
    // Başlangıçta 'enGecGelen' için 0 (min), 'enErkenCikan' için mesai bitişi (max) veriyoruz.
    char enGecGelenSicil[50] = "";
    int enGecGecikmeSuresi = 0; // Kaç dakika geç kaldı?

    char enErkenCikanSicil[50] = "";
    int enErkenErkenCikmaSuresi = 0; // Kaç dakika erken çıktı?
    int enErkenCikanDakika = mesaiBitis; // Başlangıçta 17:00 kabul ediyoruz

    // INT_MAX INT_MIN
    // #include <limits.h>

    fgun = fopen("gunluk.dat", "r");

    if (fgun == NULL) {
        printf("Error opening files\n");
        return 1;
    }

    /* else {
        printf("Files opened\n");
    }*/

    // Dosya sonuna kadar oku
    // Format: SicilNo Saat Durum (Örn: 5341998 08:40 <)
    while (fscanf(fgun, "%s %s %c", p.sicil, p.zamanStr, &p.islem) != EOF) {
        int suankiDakika = saatDakikayaCevir(p.zamanStr); //yukarıdakı fonksıyondan hesaplayıp suankıd ye eşitliyor

        // --- GİRİŞ KONTROLÜ (<) ---
        if (p.islem == '<') {
            if (suankiDakika > mesaiBaslangic) {
                int gecikme = suankiDakika - mesaiBaslangic;
                if (gecikme > enGecGecikmeSuresi) {
                    enGecGecikmeSuresi = gecikme;
                    strcpy(enGecGelenSicil, p.sicil);
                }
            }
        }

        // --- ÇIKIŞ KONTROLÜ (>) ---
        else if (p.islem == '>') {
            // Eğer 17:00'dan önce çıktıysa
            if (suankiDakika < mesaiBitis) {
                int erkenCikma = mesaiBitis - suankiDakika;

                if (suankiDakika < enErkenCikanDakika) {
                    enErkenCikanDakika = suankiDakika;
                    enErkenErkenCikmaSuresi = erkenCikma;
                    strcpy(enErkenCikanSicil, p.sicil);
                }
            }
        }
    }

    fclose(fgun);

    // --- SONUÇLARI YAZDIRMA ---

    // 1. En geç geleni 'gec.dat' dosyasına yaz
    fgec = fopen("gec.dat", "w");
    if (fgec != NULL) {
        if (enGecGecikmeSuresi > 0) {
            fprintf(fgec, "%s %d\n", enGecGelenSicil, enGecGecikmeSuresi);
            printf("Gec kalan kaydedildi: %s (%d dk)\n", enGecGelenSicil, enGecGecikmeSuresi);
        } else {
            // Hiç geç kalan yoksa
            fprintf(fgec, "Gec kalan personel yok.\n");
            printf("Gec kalan personel yok.\n");
        }
        fclose(fgec);
    }

    // 2. En erken çıkanı 'erken.dat' dosyasına yaz
    ferken = fopen("erken.dat", "w");
    if (ferken != NULL) {
        if (enErkenErkenCikmaSuresi > 0) {
            fprintf(ferken, "%s %d\n", enErkenCikanSicil, enErkenErkenCikmaSuresi);
            printf("Erken cikan kaydedildi: %s (%d dk)\n", enErkenCikanSicil, enErkenErkenCikmaSuresi);
        } else {
            // Hiç erken çıkan yoksa
            fprintf(ferken, "Erken cikan personel yok.\n");
            printf("Erken cikan personel yok.\n");
        }
        fclose(ferken);
    }

    return 0;
}