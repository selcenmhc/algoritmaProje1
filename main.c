#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// personel bilgilerini tutacak struct tanımı
struct Personel {
    char sicil[50];
    char zamanStr[10];
    char islem;
    int toplamDakika;

};
//fonksiyon tanımlanması
int saatDakikayaCevir(char *zaman) {
    int saat, dakika;
    sscanf(zaman, "%d:%d", &saat, &dakika);
    return (saat * 60) + dakika;
}

int main(void) {
    FILE *fgun, *fgec, *ferken; //dosyalar tanımlandı
    struct Personel p; // Bu veriler artık p yapısının içinde tutuluyor

    int mesaiBaslangic = 9 * 60;
    int mesaiBitis = 17 * 60;

    // Başlangıçta 'enGecGelen' için 0 (min), 'enErkenCikan' için mesai bitişi (max) veriyoruz.
    char enGecGelenSicil[50] = "";
    int enGecGecikmeSuresi = 0;

    char enErkenCikanSicil[50] = "";
    int enErkenErkenCikmaSuresi = 0;
    int enErkenCikanDakika = mesaiBitis; // Başlangıçta 17:00 kabul ediyoruz

    // INT_MAX INT_MIN
    // #include <limits.h>

    fgun = fopen("gunluk.dat", "r"); //gunluk.dat dosyasını okuma modunda açar

    if (fgun == NULL) {
        printf("Error opening files\n");
        return 1; //dosya açılmazsa programı sonlandır.
    }

    /* else {
        printf("Files opened\n");
    }*/

    // Dosya sonuna kadar oku
    // gelen format 5341998 08:40 <  bu şekildedir
    while (fscanf(fgun, "%s %s %c", p.sicil, p.zamanStr, &p.islem) != EOF) {
        int suankiDakika = saatDakikayaCevir(p.zamanStr); //yukarıdakı fonksıyondan hesaplayıp suankıdakika ya eşitliyor

        // --- GİRİŞ KONTROLÜ (<) ---
        if (p.islem == '<') {
            if (suankiDakika > mesaiBaslangic) {
                int gecikme = suankiDakika - mesaiBaslangic;
                if (gecikme > enGecGecikmeSuresi) {
                    enGecGecikmeSuresi = gecikme; //gecikme süresi arttıkça gecikme güncellenir.
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

    // En geç geleni 'gec.dat' dosyasına yaz
    fgec = fopen("gec.dat", "w"); //gec.dat dosyasını yazdır modunda açar
    if (fgec != NULL) {
        if (enGecGecikmeSuresi > 0) {
            fprintf(fgec, "%s %d\n", enGecGelenSicil, enGecGecikmeSuresi); //bilgileri dosyaya yazdırır
            printf("Gec kalan kaydedildi: %s (%d dk)\n", enGecGelenSicil, enGecGecikmeSuresi); //gec kalanın bilgisini ekrana yazdırır
        } else {
            // Hiç geç kalan yoksa
            fprintf(fgec, "Gec kalan personel yok.\n");
            printf("Gec kalan personel yok.\n");
        }
        fclose(fgec); //dosyayı kapatır.
    }

    // En erken çıkanı 'erken.dat' dosyasına yaz
    ferken = fopen("erken.dat", "w"); //erken.dat dosyasını yazdır modunda açar
    if (ferken != NULL) {
        if (enErkenErkenCikmaSuresi > 0) {
            fprintf(ferken, "%s %d\n", enErkenCikanSicil, enErkenErkenCikmaSuresi); //bilgileri dosyaya yazdırır
            printf("Erken cikan kaydedildi: %s (%d dk)\n", enErkenCikanSicil, enErkenErkenCikmaSuresi);//erken çıkanın bilgisini ekrana yazdırır
        } else {
            // Hiç erken çıkan yoksa
            fprintf(ferken, "Erken cikan personel yok.\n");
            printf("Erken cikan personel yok.\n");
        }
        fclose(ferken); //dosyayı kapatır
    }

    return 0;
}