/ int**Vass Bence - Konyhakert v1.3 * /
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
/**
A kertnagyság definiálása és a maximális növényszám definiálása.
A teszteléshez elegek ezek az értékek de itt átírva az értékeket egyből személyreszabottan felhasználókész a program
*/
#define sor 2
#define oszlop 2
#define maxnoveny 50

/**
A növenymatrix deklarálása ez a két dimenziós tömb határozza meg,
hogy melyik növény után mit lehet ültetni az optimáls termésátlag elérése érdekében.
*/
int novmatrix[maxnoveny][maxnoveny];

/**
Az adatszerkezet alapja ez a kertstruktúra,ami egyi kétszeresen láncolt listaelemet deklarál,
ebben tárolja a progran kert fizikai felépítését,
valamint hogy melyik évben volt az adott eltárolt ültetési szisztéma.
*/
typedef struct kert
{
    unsigned int ev;     //kertstruktúra
    int kerttomb[sor][oszlop];
    struct kert* kov;
    struct kert* elozo;
} kert;

/**
Globális pointerek a kertstruktúrákból álló láncoltlistához.
*/
kert* elso = NULL, *utolso = NULL;

/**
Struktúraszerkezet a növények neveit és kódjai tároló láncolt listához, a strázsákat helyettesítik.
*/
typedef struct noveny
{
    unsigned int novenykod;
    char novenynev[50];
    struct noveny* kov;
    struct noveny* elozo;
} noveny;

/**
Globális pointerek a novénystruktúrákból álló láncolt listához, a strázsákat helyettesítik.
*/
noveny* noelso = NULL, *noutolso = NULL;

/**
Keresőfüggvény a növénylistában, a keresett listaelem címével tér vissza.
*/
noveny* novenykeres(noveny* ak, int kod)
{

    while (ak->kov != NULL && ak->novenykod < kod)
    {
        ak = ak->kov;
    }

    return ak;

}

/**
Dinamikus memóriafoglaló függvény ami az uj növénylista elemnek foglal helyet.
*/
noveny* ujnovenyfoglal()
{
    noveny *p = (noveny*)malloc(sizeof(noveny));
    return p;
}

/**
Az új vagy módosított növénylistaelemmet befűzi a növényeket tartalmazó láncolt listába.
*/
void  novenylistafuz(noveny* p)
{
    noveny* akt;

    if (noelso == NULL && noutolso == NULL)
    {
        noelso = p;
        noutolso = p;
        p->kov = NULL;
        p->elozo = NULL;
    }
    else
    {
        akt = noelso;
        while (akt != NULL)
        {
            if (akt->novenykod < p->novenykod && akt->kov != NULL)
            {
                akt = akt->kov;
            }
            else if (akt->novenykod > p->novenykod && akt->elozo == NULL)
            {
                p->kov = akt;
                p->elozo = NULL;
                noelso = p;
                akt->elozo = p;
                akt = NULL;
            }
            else if (akt->novenykod < p->novenykod && akt->kov == NULL)
            {
                p->kov = akt->kov;
                p->elozo = akt;
                akt->kov = p;
                noutolso = p;
                akt = NULL;
            }
            else if (akt->novenykod == p->novenykod) // ListaElem UpDate
            {
                p->elozo = akt->elozo;
                p->kov = akt->kov;

                if (akt->kov != NULL)
                    akt->kov->elozo = p;
                else
                    noutolso = p;

                if (akt->elozo != NULL)
                    akt->elozo->kov = p;
                else
                    noelso = p;

                free(akt);
                akt = NULL;
            }
            else
            {
                p->kov = akt;
                p->elozo = akt->elozo;
                akt->elozo->kov = p;
                akt->elozo = p;
                akt = NULL;
            }

        }
    }
}

/**
Növénystruktúra kiírása a képernyőre.
*/
void novscr()
{
    noveny* novscr = noelso;

    do
    {
        printf("\n Novenykod:%d a noveny neve:%s", novscr->novenykod, novscr->novenynev);
        novscr = novscr->kov;
    }
    while (novscr != NULL);
}

/**
Az új növénylista elem feltöltése és befűzése, valamint az ültetési feltételek meghatározása.
*/
void ujnoveny()
{
    unsigned int sznkod = 0, i, kotkes;
    noveny* p = ujnovenyfoglal();


    printf("\nAdja meg az uj noveny nevet!\n");
    scanf("%s", &p->novenynev);

    while (novmatrix[sznkod][0] != -1)
    {
        sznkod++;
    }

    p->novenykod = sznkod;

    novenylistafuz(p);

    printf("\n Novenykod:%d a noveny neve:%s", p->novenykod, p->novenynev);

    for (i = 0; i <= sznkod; i++)
    {
        printf("\n %s nevu noveny utan hany evvel johet %s nevu noveny ", p->novenynev, novenykeres(noelso, i)->novenynev);
        scanf("%d", &kotkes);
        novmatrix[sznkod][i] = kotkes;
    }

    for (i = 0; i < sznkod; i++)
    {
        printf("\n %s nevu noveny utan hany evvel johet %s nevu noveny ", novenykeres(noelso, i)->novenynev, p->novenynev);
        scanf("%d", &kotkes);
        novmatrix[i][sznkod] = kotkes;
    }

    printf("\n Novenykod:%d a noveny neve:%s", p->novenykod, p->novenynev);

}

/**
Kertkereső függvény ami a keresett kert címével tér vissza.
*/
kert* kertkeres(kert* ak, int ev)
{

    while (ak->kov != NULL && ak->ev > ev)
    {
        ak = ak->kov;
    }

    return ak;

}

/**
Dinamikus memóriafoglaló függvény ami az uj kertlista elemnek foglal helyet.
*/
kert* ujkertfoglal()
{
    kert *p = (kert*)malloc(sizeof(kert));
    return p;
}

/**
Az új vagy módosított kertlistaelemmet befűzi a kerteket tartalmazó láncolt listába.
*/
void kertlistafuz(kert* p)
{
    kert* akt;

    if (elso == NULL)
    {
        elso = p;
        utolso = p;
        p->kov = NULL;
        p->elozo = NULL;
    }
    else
    {
        akt = elso;

        while (akt != NULL)
        {
            if (akt->ev < p->ev && akt->kov != NULL)
            {
                akt = akt->kov;
            }
            else if (akt->ev > p->ev && akt->elozo == NULL)
            {
                p->kov = akt;
                p->elozo = NULL;
                elso = p;
                akt->elozo = p;
                akt = NULL;
            }
            else if (akt->ev < p->ev && akt->kov == NULL)
            {
                p->kov = akt->kov;
                p->elozo = akt;
                akt->kov = p;
                utolso = p;
                akt = NULL;
            }
            else if (akt->ev == p->ev) // ListaElem UpDate
            {
                p->elozo = akt->elozo;
                p->kov = akt->kov;

                if (akt->kov != NULL)
                    akt->kov->elozo = p;
                else
                    utolso = p;

                if (akt->elozo != NULL)
                    akt->elozo->kov = p;
                else
                    elso = p;

                free(akt);
                akt = NULL;
            }
            else
            {
                p->kov = akt;
                p->elozo = akt->elozo;
                akt->elozo->kov = p;
                akt->elozo = p;
                akt = NULL;
            }

        }
    }
}

/**
A kertstruktúra kiírása a képernyőre.
*/
void kertki_scr(kert* kscr)
{
    unsigned int x, y, i, j;

    char novenyscr[50];


    printf("\n\n%d evi kert\n\n     ", kscr->ev);
    for (y = 1; y <= oszlop; y++)
    {
        printf("%d. oszlop           ", y);
    }
    printf("\n");
    for (x = 0; x < sor; x++)
    {
        printf("%d. sor ", x);
        for (y = 0; y < oszlop; y++)
        {
            for (i = 0; i < 50; i++)
            {
                novenyscr[i] = '#';
            }

            strcpy(novenyscr, novenykeres(noelso, kscr->kerttomb[x][y])->novenynev);

            i = 0;

            while (novenyscr[i] != '#')
            {
                printf("%c", novenyscr[i]);
                i++;
            }
            for (j = 20; j > i + 1; j--)
            {
                printf(" ");
            }

        }
        printf("\n");
    }
}

/**
A kertstruktúra 2 dimenziós tömbjének feltöltése.
*/
void ujkerttombfeltolt()
{
    unsigned int x, y, novenykod;
    kert *p = ujkertfoglal();
    printf("\n  Adja meg az evet\n");
    scanf("%d", &p->ev);
    printf("\nAdja meg a novenyek kodjait helynek megfeleloen:\n");
    for (x = 0; x < sor; x++)
    {
        for (y = 0; y < oszlop; y++)
        {

            printf("\n  Sor:%d Oszlop:%d helyen levo noveny kodja? ", x + 1, y + 1);
            scanf("%d", &novenykod);
            p->kerttomb[x][y] = novenykod;
        }
    }
    kertlistafuz(p);
    //printf("\nAz adatbázisba felvitt kert:\n\n");
    //kertki_scr(p);

}

/**
A kertki_scr() előválasztófüggvénye bekéri hogy melyik kertet vagy kerteket írja ki a képernyőre.
*/
void kertki()
{
    unsigned int evker;
    kert *pki;

    do
    {
        printf("\n Adja meg a kivant evet %d - %d kozott vagy irjon 1-est ha az osszeset keri \n", elso->ev, utolso->ev);

        scanf("%d", &evker);

        if (evker != 1 && (evker < elso->ev || evker > utolso->ev))
        {
            printf("\n Hibás érték! \n");
            evker = 0;
        }
    }
    while (evker == 0);

    if (evker == 1)
    {
        pki = elso;

        do
        {

            kertki_scr(pki);
            pki = pki->kov;

        }
        while (pki != NULL);


    }
    else
        kertki_scr(kertkeres(elso, evker));

}

/**
A novmátrix irása fáljba.
*/
void kiir_novmatrix()
{
    unsigned int x, y;
    char filename[] = "novmatrix.txt";

    FILE *out = fopen(filename, "wb");

    if (!out)
    {
        printf("Error opening file: %s", filename);
    }

    fwrite(novmatrix, sizeof(novmatrix), 1, out);

    if (fclose(out) != 0)
    {
        printf("Error closing file: %s", filename);
    }
}

/**
A novmátrix olvasása fáljból.
*/
void beolvas_novenymatrix()
{
    unsigned int x, y;
    char filename[] = "novmatrix.txt";

    FILE *inp = fopen(filename, "rb");

    if (!inp)
    {
        printf("\n\nError opening file: %s", filename);
    }

    fread(novmatrix, sizeof(novmatrix), 1, inp);

    if (fclose(inp) != 0)
    {
        printf("\n\nError closing file: %s", filename);
    }
}

/**
A kertlista írása fáljba.
*/
void kiir_kert()
{
    unsigned x, y;
    kert* akt = elso;
    char filename[] = "kert.txt";
    FILE* out = fopen(filename, "wt");

    if (!out)
    {
        printf("Error opening file: %s", filename);
    }

    while (akt != NULL)
    {
        fprintf(out, "\n\n %d", akt->ev);
        for (y = 0; y < sor; y++)
        {
            fprintf(out, "\n");

            for (x = 0; x < oszlop; x++)
            {
                fprintf(out, " %d ", akt->kerttomb[x][y]);
            }
        }
        akt = akt->kov;
    }

    if (fclose(out) != 0)
    {
        printf("Error closing file: %s", filename);
    }
}

/**
A kertlista olvasása fáljból.
*/
void beolvas_kert()
{
    unsigned x, y, flag = 1, ev;
    kert* akt = NULL;
    char filename[] = "kert.txt";
    FILE* inp = fopen(filename, "rt");

    if (!inp)
    {
        printf("\n\nError opening file: %s", filename);
    }

    do
    {
        flag = fscanf(inp, "\n\n %d", &ev);
        if (flag == 1)
        {
            akt = ujkertfoglal();
            akt->ev = ev;

            for (y = 0; y < sor; y++)
            {
                fscanf(inp, "\n");

                for (x = 0; x < oszlop; x++)
                {
                    fscanf(inp, " %d ", &akt->kerttomb[x][y]);
                }
            }
            kertlistafuz(akt);
        }
    } while (flag == 1);

    if (fclose(inp) != 0)
    {
        printf("\n\nError closing file: %s", filename);
    }
}

/**
A növénylista írása fáljba.
*/
void kiir_novenylist()
{
    noveny* akt = elso;
    char filename[] = "novenykod.txt";
    FILE* out = fopen(filename, "wt");

    if (!out)
    {
        printf("Error opening file: %s", filename);
    }
    akt = noelso;
    while (akt != NULL)
    {
        fprintf(out, " %d %s\n", akt->novenykod, akt->novenynev);
        akt = akt->kov;
    }

    if (fclose(out) != 0)
    {
        printf("Error closing file: %s", filename);
    }
}

/**
A növénylista olvasása fáljból.
*/
void beolvas_novenylist()
{

    unsigned x, y;
    noveny* akt = NULL, *temp = ujnovenyfoglal();
    char filename[] = "novenykod.txt";

    FILE* inp = fopen(filename, "rt");

    if (!inp)
    {
        printf("\n\nError opening file: %s", filename);
    }

    while ((fscanf(inp, " %d %s\n", &temp->novenykod, &temp->novenynev)) == 2)
    {
        akt = ujnovenyfoglal();
        akt->novenykod = temp->novenykod;
        strcpy(akt->novenynev, temp->novenynev);
        novenylistafuz(akt);
    }
    free(temp);

    if (fclose(inp) != 0)
    {
        printf("\n\nError closing file: %s", filename);
    }
}

/**
A kertlista felszabadítása.
*/
void free_kertlist()
{
    kert* akt = elso;

    do
    {
        if (akt != NULL)
        {
            elso = akt->kov;
            free(akt);
            akt = elso;
        }

    } while (elso != NULL);

    utolso = NULL;
}

/**
A növénylista felszabadítása.
*/
void free_novenylist()
{
    noveny* akt = noelso;
    do
    {
        if (akt != NULL)
        {
            noelso = akt->kov;
            free(akt);
            akt = noelso;
        }
    } while (noelso != NULL);

    noutolso = NULL;
}

/**
Mentés,
*/
void save()
{
    kiir_kert();
    kiir_novenylist();
    kiir_novmatrix();
}

/**
Az adott évben optimális ültetési rendet meghatárózós függvény.
*/
void opt_kert()
{

    unsigned int xminta, yminta, novenykod, mintaev, optev, xakt, yakt, felt, xopt, yopt, minpoz_x = 0, minpoz_y = 0, while2flag = 0;;
    kert *opt = ujkertfoglal(), *felt_kert = ujkertfoglal(), *akt, *minta;

    for (xminta = 0; xminta < sor; xminta++)
    {
        for (yminta = 0; yminta < oszlop; yminta++)
        {
            opt->kerttomb[xminta][yminta] = -1;

        }
    }

    printf("\n  Adja meg a mintakert evet\n");
    scanf("%d", &mintaev);

    printf("\n  Adja meg hogy melyik evre\n");
    scanf("%d", &optev);

    minta = kertkeres(elso, mintaev);

    for (xminta = 0; xminta < sor; xminta++)
    {
        for (yminta = 0; yminta < oszlop; yminta++)
        {
            novenykod = minta->kerttomb[xminta][yminta];
            akt = elso;

            for (xopt = 0; xopt < sor; xopt++)
            {
                for (yopt = 0; yopt < oszlop; yopt++)
                {
                    felt_kert->kerttomb[xminta][yminta] = -1;
                }
            }
            /**
                Az optimális ültetési rendet meghatárózó segédtábla (felt_kert) létrehozása.
            */
            do
            {
                for (xakt = 0; xakt < sor; xakt++)
                {
                    for (yakt = 0; yakt < oszlop; yakt++)
                    {
                        if (felt_kert->kerttomb[xakt][yakt] < novmatrix[akt->kerttomb[xakt][yakt]][novenykod])
                        {
                            felt_kert->kerttomb[xakt][yakt] = novmatrix[akt->kerttomb[xakt][yakt]][novenykod];
                        }
                    }
                }
                if (akt != NULL)
                {
                    akt = akt->kov;
                }


            } while (akt != NULL && akt->ev < optev);
            /**
               Az optimális helyek feltöltése a segédtábla (felt_kert) alapján.
            */
            do
            {
                felt = felt_kert->kerttomb[0][0];
                for (xopt = 0; xopt < sor; xopt++)
                {
                    for (yopt = 0; yopt < oszlop; yopt++)
                    {
                        if (felt_kert->kerttomb[xopt][yopt] <= felt)
                        {
                            minpoz_x = xopt;
                            minpoz_y = yopt;
                        }
                    }
                }

                if (opt->kerttomb[minpoz_x][minpoz_y] == -1)
                {
                    opt->kerttomb[minpoz_x][minpoz_y] = novenykod;
                    while2flag = 1;
                }
                else if (felt == 100)
                {
                    for (xopt = 0; xopt < sor; xopt++)
                    {
                        for (yopt = 0; yopt < oszlop; yopt++)
                        {
                            if (opt->kerttomb[xopt][yopt] == -1)
                                opt->kerttomb[xopt][yopt] = novenykod;
                            while2flag = 1;
                        }
                    }
                }
                else
                {
                    felt_kert->kerttomb[minpoz_x][minpoz_y] = 100;
                    while2flag = 0;
                }

            } while (while2flag != 1);
        }
    }
    free(felt_kert);
    opt->ev = optev;
    kertlistafuz(opt);
    kertki_scr(opt);

}

/**
Kiértékeli hogy két növény mikor volt szomszédos.
*/
void novenyszomszed()
{
    kert* akt = elso;
    unsigned int x, y, novkodx, novkody;

    printf("\nAdja meg a keresett novenynek kodjait szokozzel elvalasztva! (Mi mellet mi?)\n");
    scanf("%d %d", &novkodx, &novkody);
    do
    {

        for (x = 0; x < sor; x++)
        {
            for (y = 0; y < oszlop; y++)
            {
                if (akt->kerttomb[x][y] == novkodx && ((x == 0 ? -1 : akt->kerttomb[x - 1][y]) == novkody) || akt->kerttomb[x + 1][y] == novkody || (y == 0 ? -1 : akt->kerttomb[x][y - 1]) == novkody || akt->kerttomb[x][y + 1] == novkody)
                    printf("\n %d evben %s volt a(z) %s mellett \n", akt->ev, novenykeres(noelso, novkody)->novenynev, novenykeres(noelso, novkodx)->novenynev);
            }
        }
        akt = akt->kov;

    }
    while (akt != NULL);

}

/**
Kiértékeli hogy mikor és hol volt adott növény kronológiai sorrendben.
*/
void noveny_mikmennyhol()
{
    unsigned int novkod, x, y;
    kert* akt = elso;
    printf("\n Adja meg a keresett noveny kodjat!\n");
    scanf("%d", &novkod);

    do
    {

        for (x = 0; x < sor; x++)
        {
            for (y = 0; y < oszlop; y++)
            {
                if (akt->kerttomb[x][y] == novkod)
                    printf("\n %d evben %s volt a(z) %d. sor %d. oszlopaban \n", akt->ev, novenykeres(noelso, novkod)->novenynev, x + 1, y + 1);
            }
        }
        akt = akt->kov;

    }
    while (akt != NULL);

}

/**
A növénymátrix előbeállítása  könnyebb használhatóság érdekében.
*/
void novmatrix_preset()
{
    unsigned int x, y;

    for (y = 0; y < maxnoveny; y++)
    {
        for (x = 0; x < maxnoveny; x++)
        {
            novmatrix[x][y] = -1;
        }

    }
}

int legtobbnov()
{
    unsigned int novkod, x, y;
    kert* akt = elso;
    noveny* ak = noelso;
    int maxkod, hanyszor = 0, szamol = 0;


    while (ak->kov != NULL)
    {
        szamol = 0;
        akt = elso;
        do
        {

            for (x = 0; x < sor; x++)
            {
                for (y = 0; y < oszlop; y++)
                {
                    if (akt->kerttomb[x][y] == ak->novenykod)
                        szamol++;


                }
            }


            akt = akt->kov;

        }
        while (akt != NULL);

        if (szamol > hanyszor)
        {
            hanyszor = szamol;
            maxkod = ak->novenykod;
        }
        ak = ak->kov;
    }
    return maxkod;

}

int main()
{

    unsigned int menuvar;

    novmatrix_preset();
    /**
    Fájlbeolvasás.
    */
    beolvas_novenymatrix();
    beolvas_novenylist();
    beolvas_kert();

    printf("\n\nA %d kodu noveny a %s volt ultetve\n\n", legtobbnov(), novenykeres(noelso, legtobbnov())->novenynev);
    /**
    Egyszerű menű megvalósítása switch-case függvénnyel.
    */
    do
    {
        //menu
        printf("\n\nujkert    :1");
        printf("\nkertkiir  :2");
        printf("\nujnoveny  :3");
        printf("\nnovkodki  :4");
        printf("\nnovszom   :5");
        printf("\noptkert   :6");
        printf("\nnmikmehol :7");
        printf("\nsave      :8");

        printf("\nEXIT      :77 \n");

        scanf("%d", &menuvar);

        switch (menuvar)
        {

        case 1:
            ujkerttombfeltolt();
            break;

        case 2:
            kertki();
            break;

        case 3:
            ujnoveny();
            break;

        case 4:
            novscr();
            break;

        case 5:
            novenyszomszed();
            break;

        case 6:
            opt_kert();
            break;

        case 7:
            noveny_mikmennyhol();
            break;

        case 8:
            save();
            break;

        case 77:
            menuvar = 77;
            break;

        default:
            menuvar = 100;

        }

        /**
        Mentés minden művelet után.
        */
        save();
    }
    while (menuvar != 77);

    /**
    Felhasznált memóriahely felszabadítása;
    */
    free_kertlist();
    free_novenylist();

    return 0;
}
