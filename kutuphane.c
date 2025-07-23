#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dirent.h"
#include <time.h>

int girisBasarili = 0;
char aktifKullanici[100];
#define WIDTH 70
void printCenter(const char *text);
void temizleBuffer() {
    int ch;
    while ((ch = getchar()) != '\n' && ch != EOF);
}
void kitapEkle() {
    char kitapAdi[100];
    char kitapYili[100];
    temizleBuffer();
    printf("Kitap adını giriniz : ");
    fgets(kitapAdi, sizeof(kitapAdi), stdin);
    kitapAdi[strcspn(kitapAdi, "\n")] = '\0';
    printf("Kitap yılını giriniz : ");
    fgets(kitapYili, sizeof(kitapYili), stdin);
    kitapYili[strcspn(kitapYili, "\n")] = '\0'; 

    
    FILE *dosya = fopen("kitap.txt", "a");

    if (dosya == NULL) {
        printf("Dosya açılamadı.\n");
        return;
    }
    fprintf(dosya, "%s:%s\n", kitapAdi, kitapYili);  
    fclose(dosya);

    ekranTemizle();

    printf("✅  Başarılı şekilde '%s' adlı kitabı eklediniz!\n", kitapAdi);
}
void kitapListele() {
    FILE *listele = fopen("kitap.txt", "r");
    char satir[200];
    int k = 1; 

    if (listele == NULL) {
        printf("❌ Dosya açılamadı veya kitap bulunamadı.\n");
        return;
    }

    printf("\n--- 📚 Kitap Listesi ---\n");

    while (fgets(satir, sizeof(satir), listele) != NULL) {
        satir[strcspn(satir, "\n")] = '\0';  

        char *listeKitapAdi = strtok(satir, ":");
        char *listeKitapYili = strtok(NULL, ":");

        if (listeKitapAdi != NULL && listeKitapYili != NULL) {
            printf("%d. %s (%s)\n", k, listeKitapAdi, listeKitapYili);
        } else {
            printf("%d. [Hatalı kayıt]\n", k);
        }

        k++;
    }
    printf("------------------------\n");

    fclose(listele);
}
int kitapSay(int satirSayisi){
    char satir[200];
    FILE *say = fopen("kitap.txt","r");
    while(fgets(satir, sizeof(satir), say) != NULL){
        satirSayisi++;
    }
    fclose(say);
    return satirSayisi;
}
void kitapSil() {
    int secim;
    kitapListele();

    printf("Hangisini silmek istiyorsunuz ? :");
    scanf("%d", &secim);
    getchar(); 
    
    char satir[200];
    int satirSayisi = 0;
    satirSayisi = kitapSay(satirSayisi);

    if(secim>satirSayisi || secim == 0){
        ekranTemizle();
        printf("❌  Geçersiz seçim!\n");
    }else{

        FILE *oku = fopen("kitap.txt", "r");
        FILE *ekle = fopen("kitap-gecici.txt", "w");
        int i = 1;

        if (oku == NULL || ekle == NULL) {
        ekranTemizle();
        printf("❌ Dosyalar açılamadı.\n");
        return;
        }
    
        while (fgets(satir, sizeof(satir), oku) != NULL) {
        satir[strcspn(satir, "\n")] = '\0';

        if (secim != i) {
            char *tempKitapAdi = strtok(satir, ":");
            char *tempKitapYili = strtok(NULL, ":");

            if (tempKitapAdi != NULL && tempKitapYili != NULL) {
                fprintf(ekle, "%s:%s\n", tempKitapAdi, tempKitapYili);
            }
        }
        i++;
        }

        fclose(oku);
        fclose(ekle);
        remove("kitap.txt");
        rename("kitap-gecici.txt", "kitap.txt");
        ekranTemizle();
        printf("✅ Kitap başarıyla silindi.\n");
    }



}
void oduncAl() {
    kitapListele();

    int secim;
    printf("Hangi kitabı almak istiyorsun ? : ");
    scanf("%d", &secim);
    int satirSayisi = 0;
    satirSayisi = kitapSay(satirSayisi);

    if (secim > satirSayisi || secim < 1) {
        ekranTemizle();
        printf("❌ Geçersiz seçim!\n");
        return;
    }

    
    FILE *oku = fopen("kitap.txt", "r");
    if (oku == NULL) {
        printf("Dosya açılamadı.\n");
        return;
    }

    char satir[200];
    int i = 1;
    char secilenSatir[200] = "";

    while (fgets(satir, sizeof(satir), oku) != NULL) {
        if (i == secim) {
            strcpy(secilenSatir, satir);  
            break;
        }
        i++;
    }

    fclose(oku);

    if (strlen(secilenSatir) == 0) {
        ekranTemizle();
        printf("❌ Seçilen kitap bulunamadı.\n");
        return;
    }

    
    secilenSatir[strcspn(secilenSatir, "\n")] = '\0';  
    char *oduncKitapAdi = strtok(secilenSatir, ":");
    char *oduncKitapYili = strtok(NULL, ":");


    char dosyaAdi[150];
    sprintf(dosyaAdi, "bin/odunc_%s.txt", aktifKullanici);
    FILE *oduncEkle = fopen(dosyaAdi, "a");
    fprintf(oduncEkle, "%s:%s\n", oduncKitapAdi, oduncKitapYili);
    fclose(oduncEkle);
    ekranTemizle();


    printf("✅ %s adlı kitap ödünç alındı!\n", oduncKitapAdi);

    FILE *orijinal = fopen("kitap.txt", "r");
    FILE *gecici = fopen("kitap-gecici.txt", "w");
    int j = 1;
    while (fgets(satir, sizeof(satir), orijinal) != NULL) {
        if (j != secim) {
            fputs(satir, gecici);  
        }
        j++;
    }

    fclose(orijinal);
    fclose(gecici);
    remove("kitap.txt");
    rename("kitap-gecici.txt", "kitap.txt");
}
void kitapIadeEt(){
    char geciciDosyaAdi[150];
    sprintf(geciciDosyaAdi, "bin\\odunc_%s_temp.txt", aktifKullanici); 
    char dosyaAdi[150];
    sprintf(dosyaAdi, "bin\\odunc_%s.txt", aktifKullanici);
    FILE *oku = fopen(dosyaAdi,"r");
    if(oku == NULL){
        printf("❌ Geçersiz seçim!\n");
        return;
    }
    int secim;
    char satir[200];
    char satir2[200];
    int k=1;
    int satirSayisi=0;
    time_t t;         
    struct tm *tm_info;
    printf("\n--- 📚 Ödünç Kitap Listesi ---\n");
    while(fgets(satir,sizeof(satir),oku) != NULL)
    {
        satir[strcspn(satir, "\n")] = '\0'; 
        char* oduncKitapAdi = strtok(satir,":");
        char* oduncKitapYili = strtok(NULL,":");
        printf("%d. %s (%s)\n", k, oduncKitapAdi, oduncKitapYili);
        k++;
        satirSayisi++;
    }
    fclose(oku);
    printf("------------------------------\n");
    printf("Hangi kitabı iade edeceksin ? : ");
    scanf("%d",&secim);

    if(secim>satirSayisi || secim == 0)
    {
        ekranTemizle();
        printf("❌ Geçersiz seçim!\n");
        return;
    }
    oku = fopen(dosyaAdi, "r"); 
    FILE *gecici = fopen(geciciDosyaAdi,"w");
    int x=1;

    while(fgets(satir2,sizeof(satir2),oku) != NULL)
    {
        if(x != secim){
             satir2[strcspn(satir2, "\n")] = '\0'; 
             char* iadeKitapAdi = strtok(satir2,":");
             char* iadeKitapYili = strtok(NULL,":");
             fprintf(gecici, "%s:%s\n", iadeKitapAdi, iadeKitapYili);
        }
        if(x == secim)
        {
            satir2[strcspn(satir2, "\n")] = '\0'; 
            FILE *kitapEkle = fopen("kitap.txt","a");
            FILE *logla = fopen("iadelog.txt","a");
            char* iadeEkleAdi = strtok(satir2,":");
            char* iadeEkleYili = strtok(NULL,":");
            ekranTemizle();
            time(&t);
            tm_info = localtime(&t);
            fprintf(logla, "%s:%s:%s:%02d:%02d:%d:%02d:%02d:%02d\n\n", iadeEkleAdi, iadeEkleYili,aktifKullanici,  tm_info->tm_mday,   tm_info->tm_mon + 1,tm_info->tm_year + 1900,  tm_info->tm_hour,
           tm_info->tm_min, 
           tm_info->tm_sec);  
            fclose(logla);
            printf("✅ %s adlı kitap iade edildi!\n", iadeEkleAdi);
            fprintf(kitapEkle, "%s:%s\n", iadeEkleAdi, iadeEkleYili);
            fclose(kitapEkle);
        }
        x++;
    }
    fclose(oku);
    fclose(gecici);
    remove(dosyaAdi);
    rename(geciciDosyaAdi,dosyaAdi);


}
void alinanKitapListesi(){

    char dosyaAdi[150];
    sprintf(dosyaAdi, "bin\\odunc_%s.txt", aktifKullanici); //
    FILE *oku = fopen(dosyaAdi,"r");
    if(oku == NULL){
        printf("❌ Geçersiz seçim!\n");
        return;
    }
    char satir[200];
    int k=1;
    printf("\n--- 📚 Ödünç Kitap Listesi ---\n");
    while(fgets(satir,sizeof(satir),oku) != NULL)
    {
        satir[strcspn(satir, "\n")] = '\0'; 
        char* oduncKitapAdi = strtok(satir,":");
        char* oduncKitapYili = strtok(NULL,":");
        printf("%d. %s (%s)\n", k, oduncKitapAdi, oduncKitapYili);
        k++;
    }
    printf("------------------------------\n");
    fclose(oku);


}
void personelGirisYap()
{
    ekranTemizle();
    char girilenKullanici[100];
    char girilenSifre[100];
    char satir[200];
    

    printf("\nKullanıcı adınızı girin: ");
    scanf("%s", girilenKullanici);

    printf("Şifrenizi girin: ");
    scanf("%s", girilenSifre);

    FILE *dosya = fopen("personelkullanici.txt", "r");
    if (dosya == NULL) {
        printf("Kayıtlı kullanıcı bulunamadı.\n");
        return;
    }



    while (fgets(satir, sizeof(satir), dosya) != NULL) {
        satir[strcspn(satir, "\n")] = '\0';  

        char *kayitliKullanici = strtok(satir, ":");
        char *kayitliSifre = strtok(NULL, ":");

        if (kayitliKullanici && kayitliSifre &&
            strcmp(girilenKullanici, kayitliKullanici) == 0 &&
            strcmp(girilenSifre, kayitliSifre) == 0) {
            girisBasarili = 1;
            break;
        }
    }

    fclose(dosya);
    ekranTemizle();
    if (girisBasarili) {
        printf("\n✔️ Giriş başarılı. Hoş geldin, %s!\n", girilenKullanici);
    } else {
        printf("\n❌ Hatalı kullanıcı adı veya şifre.\n");
    }

}
void girisYap() {
    ekranTemizle();
    char girilenKullanici[100];
    char girilenSifre[100];
    char satir[200];

    printf("\nKullanıcı adınızı girin: ");
    scanf("%s", girilenKullanici);

    printf("Şifrenizi girin: ");
    scanf("%s", girilenSifre);

    FILE *dosya = fopen("kullanici.txt", "r");
    if (dosya == NULL) {
        printf("\n❌  Kayıtlı kullanıcı bulunamadı.\n");
        return;
    }



    while (fgets(satir, sizeof(satir), dosya) != NULL) {
        satir[strcspn(satir, "\n")] = '\0';  

        char *kayitliKullanici = strtok(satir, ":");
        char *kayitliSifre = strtok(NULL, ":");

        if (kayitliKullanici && kayitliSifre &&
            strcmp(girilenKullanici, kayitliKullanici) == 0 &&
            strcmp(girilenSifre, kayitliSifre) == 0) {
            girisBasarili = 1;
            strcpy(aktifKullanici,kayitliKullanici);
            break;
        }
    }

    fclose(dosya);
    ekranTemizle();
    if (girisBasarili) {
        printf("\n✔️  Giriş başarılı. Hoş geldin, %s!\n", girilenKullanici);
    } else {
        printf("\n❌  Hatalı kullanıcı adı veya şifre.\n");
    }
}
void kayitOl() {
    ekranTemizle();
    char yeniKullanici[100];
    char yeniSifre[100];

    printf("Yeni kullanıcı adınızı girin: ");
    scanf("%s", yeniKullanici);

    printf("Yeni şifrenizi girin: ");
    scanf("%s", yeniSifre);


    FILE *kontrol = fopen("kullanici.txt", "r");
    char satir[200];
    while (fgets(satir, sizeof(satir), kontrol) != NULL) {
        satir[strcspn(satir, "\n")] = '\0';
        char *mevcutKullanici = strtok(satir, ":");
        if (mevcutKullanici != NULL && strcmp(yeniKullanici, mevcutKullanici) == 0) {
            printf("⚠️  Bu kullanıcı adı zaten alınmış. Lütfen başka bir ad deneyin.\n");
            fclose(kontrol);
            return;
        }
    }
    fclose(kontrol);

    FILE *dosya = fopen("kullanici.txt", "a");
    if (dosya == NULL) {
        printf("Dosya açılamadı.\n");
        return;
    }

    fprintf(dosya, "%s:%s\n", yeniKullanici, yeniSifre);
    fclose(dosya);
    ekranTemizle();
    printf("✅  Başarılı şekilde kayıt oldunuz, %s!\n", yeniKullanici);
}
void ekranTemizle() {
    printf("\033[2J\033[H");
}
void tumAlinanKitapListesi(){
        ekranTemizle();
        char dosyaAdi[150];
        DIR *klasor = opendir("bin");
        struct dirent *d;     
        char kontrol[] = {"odunc_"};
        char satir[200];
        int k=1;
        char kullaniciAdi[100];
 
        printf("\n------------------------- 📚 Ödünç Kitap Listesi -------------------------\n");
        while ((d = readdir(klasor)) != NULL) {
            if(strncmp(d->d_name,kontrol,strlen(kontrol)) == 0)
            {
                sprintf(dosyaAdi,"bin/%s",d->d_name);
                strcpy(kullaniciAdi, d->d_name + 6); 
                kullaniciAdi[strlen(kullaniciAdi) - 4] = '\0';
                FILE *oku = fopen(dosyaAdi,"r");
                while(fgets(satir,sizeof(satir),oku) != NULL)
                {
                satir[strcspn(satir, "\n")] = '\0'; 
                char* oduncKitapAdi = strtok(satir,":");
                char* oduncKitapYili = strtok(NULL,":");
                printf("%2d. %-40s (%s) — 👤 %s\n", k, oduncKitapAdi, oduncKitapYili, kullaniciAdi);
                k++;
                fclose(oku);
        }
     

            }
        }
        closedir(klasor);                        
        printf("--------------------------------------------------------------------------\n");    

}
void iadeLogla(){
    ekranTemizle();
    FILE *oku = fopen("iadelog.txt","r");
    if(oku == NULL){
        printf("❌ Geçersiz seçim!\n");
        return;
    }
    char satir[100];
    int k=1;
    printf("======================================================================\n");
    printCenter("📚 İade Kitap Geçmişi");
    printf("======================================================================\n");
    while (fgets(satir, sizeof(satir), oku) != NULL) {
    satir[strcspn(satir, "\n")] = '\0';  // \n temizle
    if (strlen(satir) == 0) continue;    // boş satır varsa atla

    char* iadeKitapAdi       = strtok(satir, ":");
    char* iadeKitapYili      = strtok(NULL, ":");
    char* iadeKitapKullanici = strtok(NULL, ":");
    char* iadeGun            = strtok(NULL, ":");
    char* iadeAy             = strtok(NULL, ":");
    char* iadeYil            = strtok(NULL, ":");
    char* iadeSaat           = strtok(NULL, ":");
    char* iadeDakika         = strtok(NULL, ":");
    char* iadeSaniye         = strtok(NULL, ":");

    if (!iadeKitapAdi || !iadeKitapYili || !iadeKitapKullanici ||
        !iadeGun || !iadeAy || !iadeYil || !iadeSaat || !iadeDakika || !iadeSaniye) {
        printf("⚠️ Hatalı satır: %s\n", satir);
        continue;
    }

    printf("%2d. %-40.40s (%s) — 👤 %-15.15s | %s-%s-%s %s:%s:%s\n",
           k++, iadeKitapAdi, iadeKitapYili,
           iadeKitapKullanici,
           iadeGun, iadeAy, iadeYil,
           iadeSaat, iadeDakika, iadeSaniye);
}

    fclose(oku);
    printf("======================================================================\n");

}

int main() {
    int secim;
    int secim2;
    int secim3;
    int dogruMu;
    ekranTemizle();
    do {
        printf("\n--- GİRİŞ PANELİ ---\n");
        printf("1.Personel Girişi Yap\n");
        printf("2.Üye girişi yap\n");
        printf("3.Kayıt Ol\n");
        printf("4.Çıkış\n");
        printf("--------------------\n");
        printf("Seçiminiz: ");
        scanf("%d", &secim);
        ekranTemizle();
        



        switch (secim) {
            case 1:
                girisBasarili = 0;
                personelGirisYap();
                if(girisBasarili == 1){
                    do{ 
                            printf("\n--- PERSONEL PANELİ ---\n");
                            printf("1.Kitap Ekle\n");
                            printf("2.Kitap Sil\n");
                            printf("3.Kitapları Listele\n");
                            printf("4.Ödünç Kitapları Listele\n");
                            printf("5.İade Logları Listele\n");
                            printf("6.Çıkış\n");
                            printf("-----------------------\n");
                            printf("Seçiminiz: ");
                            scanf("%d", &secim2);
                            ekranTemizle();
                            
                            
                            switch (secim2)
                            {
                            case 1:
                                kitapEkle();
                                break;
                            case 2:
                                kitapSil();
                                break;
                            case 3:
                                kitapListele();
                                break;
                            case 4:
                                tumAlinanKitapListesi();
                                break;

                            case 5:
                                iadeLogla();
                                break;
                            case 6:
                                printf("Personel panelind4en çıkılıyor...\n");
                                break;
                            default:
                            printf("Geçersiz seçim, tekrar deneyin.\n");
                            }

                }while(secim2 != 6);
                }
                
                break;
            case 2:

                girisBasarili = 0;
                girisYap();
                
                if(girisBasarili==1){
                    do{ 
                            printf("\n--- ÜYE PANELİ ---\n");
                            printf("1.Kitapları Listele\n");
                            printf("2.Ödünç Kitap Al\n");
                            printf("3.Kitap İade Et\n");
                            printf("4.Alınan Kitapları Listele\n");
                            printf("5.Çıkış\n");
                            printf("------------------\n");
                            printf("Seçiminiz: ");
                            scanf("%d", &secim3);
                            ekranTemizle();
                            

                            switch (secim3)
                            {
                            case 1:
                                kitapListele();
                                break;
                            case 2:
                                oduncAl();
                                break;
                            case 3:
                                kitapIadeEt();
                                break;
                            case 4:
                                alinanKitapListesi();
                                break;
                            case 5:
                                printf("Üye panelinden çıkılıyor...\n");
                                break;
                            default:
                            printf("Geçersiz seçim, tekrar deneyin.\n");
                            }

                }while(secim3 != 5);
                }
                
                break;
            case 3:
                kayitOl();
                break;
            case 4:
                printf("Programdan çıkılıyor...\n");
                break;
            default:
                printf("Geçersiz seçim, tekrar deneyin.\n");
        }
    } while (secim != 4);

    return 0;
}


void printCenter(const char *text) {
    int len = strlen(text);
    int padding = (WIDTH - len) / 2;
    for (int i = 0; i < padding; i++) printf(" ");
    printf("%s\n", text);
}
