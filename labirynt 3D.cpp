   #include <iostream>
   #include <conio.h>
   #include <cstdlib>
   #include <time.h>
   #include <fstream>


using namespace std;

    int miara_z = 10, miara_y = 10, miara_x = 10;  // musza byæ te same wartosci
    int labirynt[10][10][10];                     // musza byæ te same wartosci

 //   int miara_z = 22, miara_y = 22, miara_x = 22;  // musza byæ te same wartosci
 //   int labirynt[22][22][22];                     // musza byæ te same wartosci

    int droga_wyjscia[3000][3];
    float poz_x = 0, poz_y = 0, poz_z = 0;
    fstream plik;

    void tworzenie_labiryntu();
    int szukanie_drogi();

    void rysowanie_labiryntu();
    void rysowanie_drogi(int dlugosc_drogi);


int main()
{
    srand(time(NULL)) ;

    int *pole = &labirynt[0][0][0]; // czyszczenie pol labiryntu
    for (int i = 0 ; i < miara_y * miara_x * miara_z ; i++) *pole++ = 0;

    tworzenie_labiryntu();

    rysowanie_drogi(szukanie_drogi());

    /************ TEST - poprawnosci wyszukiwania drogi: rysuj koordynaty w consoli **************
    for(int i = 0 ; i < dlugosc_drogi ; i++)
    { cout << "z: " << droga_wyjscia[i][0] << "   y: " << droga_wyjscia[i][1] << "   x: " << droga_wyjscia[i][2] << "     \n"; }*/

    labirynt[0][0][0] += 1; labirynt[miara_z - 1][miara_y - 1][miara_x - 1] += 4; //dorysowanie wejcia i wyjscia

    /************ TEST - poprawnosci budowy labiryntu: rysuj plaszczyzny labiruntu w consoli **************
    string ekran = "";
    for(int z = 0 ; z < miara_z ; z++)
    {
        for(int y = 0 ; y < miara_y ; y++)
        {
            for(int x = 0 ; x < miara_x ; x++)
            {
                int komnata = labirynt[z][y][x] - (labirynt[z][y][x] & (16 + 32));
                if (komnata == 0) {ekran += ' '; ekran += ' ';};
                if (komnata == 1) {ekran += char(192); ekran += ' ';};
                if (komnata == 2) {ekran += ' ' ; ekran +=  char(196);};
                if (komnata == 3) {ekran += char(192) ; ekran +=  char(196);};
                if (komnata == 4) {ekran += char(218) ; ekran +=  ' ';};
                if (komnata == 5) {ekran += char(179) ; ekran +=  ' ';};
                if (komnata == 6) {ekran += char(218) ; ekran +=  char(196);};
                if (komnata == 7) {ekran += char(195) ; ekran +=  char(196);};
                if (komnata == 8) {ekran += char(196) ; ekran +=  ' ';};
                if (komnata == 9) {ekran += char(217) ; ekran +=  ' ';};
                if (komnata == 10) {ekran += char(196) ; ekran +=  char(196);};
                if (komnata == 11) {ekran += char(193) ; ekran +=  char(196);};
                if (komnata == 12) {ekran += char(191) ; ekran +=  ' ';};
                if (komnata == 13) {ekran += char(180) ; ekran +=  ' ';};
                if (komnata == 14) {ekran += char(194) ; ekran +=  char(196);};
                if (komnata == 15) {ekran += char(197) ; ekran +=  char(196);};

                komnata = labirynt[z][y][x] & 48;
                if (komnata == 0) ekran += ' ' ;
                if (komnata == 16) ekran += char(203) ;
                if (komnata == 32) ekran += char(202) ;
                if (komnata == 48) ekran += char(186) ;
             }
             ekran +=  "\n";
        }
        ekran +=  "\n";
    }
    ekran +=  "\n\n";
    cout << ekran;*/

    rysowanie_labiryntu();
}

/***************** FUNKCJE LOGICZNE ***************/

void tworzenie_labiryntu()
{
    int pozycja_x, pozycja_y, pozycja_z;
    int kierunek = 0, wolne[6] = {0,0,0,0,0,0};
    int licznik;

    pozycja_x = rand() % miara_x;
    pozycja_y = rand() % miara_y;
    pozycja_z = rand() % miara_z;

    licznik = (miara_x * miara_y * miara_z) - 1 ;

    do {
        do {
            kierunek = 0;

            if (pozycja_x              && labirynt[pozycja_z][pozycja_y][pozycja_x - 1] == 0) wolne[kierunek++] = 8;   //lewo
            if (pozycja_x != miara_x-1 && labirynt[pozycja_z][pozycja_y][pozycja_x + 1] == 0) wolne[kierunek++] = 2;   //prawo
            if (pozycja_y              && labirynt[pozycja_z][pozycja_y - 1][pozycja_x] == 0) wolne[kierunek++] = 1;   //tyl
            if (pozycja_y != miara_y-1 && labirynt[pozycja_z][pozycja_y + 1][pozycja_x] == 0) wolne[kierunek++] = 4;   //przod
            if (pozycja_z              && labirynt[pozycja_z - 1][pozycja_y][pozycja_x] == 0) wolne[kierunek++] = 32;   //dol
            if (pozycja_z != miara_z-1 && labirynt[pozycja_z + 1][pozycja_y][pozycja_x] == 0) wolne[kierunek++] = 16;   //gora

            if (kierunek)
            {
                int wybor_kierunku = rand() % (kierunek);
                int przesuniecie = wolne[wybor_kierunku];
                labirynt[pozycja_z][pozycja_y][pozycja_x] += przesuniecie;

                if (przesuniecie == 8) {pozycja_x--; labirynt[pozycja_z][pozycja_y][pozycja_x] = 2; };
                if (przesuniecie == 2) {pozycja_x++; labirynt[pozycja_z][pozycja_y][pozycja_x] = 8; };
                if (przesuniecie == 1) {pozycja_y--; labirynt[pozycja_z][pozycja_y][pozycja_x] = 4; };
                if (przesuniecie == 4) {pozycja_y++; labirynt[pozycja_z][pozycja_y][pozycja_x] = 1; };
                if (przesuniecie == 32) {pozycja_z--; labirynt[pozycja_z][pozycja_y][pozycja_x] = 16; };
                if (przesuniecie == 16) {pozycja_z++; labirynt[pozycja_z][pozycja_y][pozycja_x] = 32; };

                licznik--;
            }
        } while (kierunek);

        do {
            pozycja_x++; if (pozycja_x == miara_x)
            {
                pozycja_x = 0; pozycja_y++; if (pozycja_y == miara_y)
                {
                    pozycja_y = 0; pozycja_z++; if (pozycja_z == miara_z)
                    pozycja_z = 0;
                }
            }
        } while (!labirynt[pozycja_z][pozycja_y][pozycja_x]);
    } while (licznik > 0);
}

int szukanie_drogi()
{
    int pacze_z = 0, pacze_y = 0, pacze_x = 0, licznik_komnat = 0;
    int mapa[miara_z][miara_y][miara_x];
    int *adres = &droga_wyjscia[0][0], *adresss = &mapa[0][0][0], *adressso = &labirynt[0][0][0];
    int suma_wyjsc, komnata;

    for (int i = 0 ; i < 3000 * 3 ; i++) *adres++ = 0;          //czyszczenie tablicy
    for (int i = 0 ; i < miara_z * miara_y * miara_x ; i++) *adresss++ = *adressso++;          //przepisanie labiryntu

    do
    {
        komnata = mapa[pacze_z][pacze_y][pacze_x];
        suma_wyjsc = (komnata & 1) + (komnata & 2) / 2 + (komnata & 4) / 4 + (komnata & 8) / 8 + (komnata & 16) / 16 + (komnata & 32) / 32;

        if (suma_wyjsc == 0)             // slepa uliczka, cofaj do poprzedniego skrzyzowania
        {do { licznik_komnat--;
              pacze_z = droga_wyjscia[licznik_komnat][0];
              pacze_y = droga_wyjscia[licznik_komnat][1];
              pacze_x = droga_wyjscia[licznik_komnat][2];
        } while ( mapa[pacze_z][pacze_y][pacze_x] == 0); continue;}

        droga_wyjscia[licznik_komnat][0] =  pacze_z;               // zapisz klejne lokalizacje
        droga_wyjscia[licznik_komnat][1] =  pacze_y;
        droga_wyjscia[licznik_komnat][2] =  pacze_x;

        if (komnata &  1) {mapa[pacze_z][pacze_y][pacze_x] --   ; pacze_y--; mapa[pacze_z][pacze_y][pacze_x] -=  4; licznik_komnat++; continue;}
        if (komnata &  2) {mapa[pacze_z][pacze_y][pacze_x] -=  2; pacze_x++; mapa[pacze_z][pacze_y][pacze_x] -=  8; licznik_komnat++; continue;}
        if (komnata &  4) {mapa[pacze_z][pacze_y][pacze_x] -=  4; pacze_y++; mapa[pacze_z][pacze_y][pacze_x] -=  1; licznik_komnat++; continue;}
        if (komnata &  8) {mapa[pacze_z][pacze_y][pacze_x] -=  8; pacze_x--; mapa[pacze_z][pacze_y][pacze_x] -=  2; licznik_komnat++; continue;}
        if (komnata & 16) {mapa[pacze_z][pacze_y][pacze_x] -= 16; pacze_z++; mapa[pacze_z][pacze_y][pacze_x] -= 32; licznik_komnat++; continue;}
        if (komnata & 32) {mapa[pacze_z][pacze_y][pacze_x] -= 32; pacze_z--; mapa[pacze_z][pacze_y][pacze_x] -= 16; licznik_komnat++; continue;}

    } while (!(pacze_y == miara_y - 1 && pacze_x == miara_x - 1 && pacze_z == miara_z - 1));

    droga_wyjscia[licznik_komnat][0] =  pacze_z;               // wyjscie
    droga_wyjscia[licznik_komnat][1] =  pacze_y;
    droga_wyjscia[licznik_komnat][2] =  pacze_x;

//    cout << "dlugosc grogi przez labirynt: " << licznik_komnat + 1;
    return ++licznik_komnat;
}

/********* FUNKCJE RYSUJACE *****************/

    void pole_na_plaszczyznie_X(float x, float z1, float y1, float z2, float y2, float z3, float y3, float z4, float y4);
    void pole_na_plaszczyznie_Y(float y, float x1, float z1, float x2, float z2, float x3, float z3, float x4, float z4);
    void pole_na_plaszczyznie_Z(float z, float x1, float y1, float x2, float y2, float x3, float y3, float x4, float y4);

    void gora_otwarte();
    void gora_zamkniete();
    void prawo_otwarte();
    void prawo_zamkniete();
    void przod_otwarte();
    void przod_zamkniete();
    void dol_otwarte();
    void dol_zamkniete();
    void lewo_otwarte();
    void lewo_zamkniete();
    void tyl_otwarte();
    void tyl_zamkniete();
    void obudowa();

    void belka_plus_x();
    void belka_minus_x();
    void belka_plus_y();
    void belka_minus_y();
    void belka_plus_z();
    void belka_minus_z();

void rysowanie_labiryntu()
{
    string nglowek = "# Alias OBJ Model File\n# made by Michal Aniol xd\n\n"
    "mtllib Untitled.mtl\n\ng Mesh1 Model\n\nusemtl FrontColor\n";

    plik.open("labirynt 3D.obj", ios::out); // zapis mapy labiryntu w formie graficznej
    plik << nglowek;

    poz_x = 0; poz_y = 0; poz_z = 0;
    for(int z = 0 ; z < miara_z ; z++)
    {
        for(int y = 0 ; y < miara_y ; y++)
        {
            for(int x = 0 ; x < miara_x ; x++)
            {
                int ktore_sciany_sa_otwarte = labirynt[z][y][x];
                if (ktore_sciany_sa_otwarte &  1) lewo_otwarte();  else lewo_zamkniete();
                if (ktore_sciany_sa_otwarte &  2) przod_otwarte(); else przod_zamkniete();
                if (ktore_sciany_sa_otwarte &  4) prawo_otwarte(); else prawo_zamkniete();
                if (ktore_sciany_sa_otwarte &  8) tyl_otwarte();   else tyl_zamkniete();
                if (ktore_sciany_sa_otwarte & 16) gora_otwarte();  else gora_zamkniete();
                if (ktore_sciany_sa_otwarte & 32) dol_otwarte();   else dol_zamkniete();

                poz_x += 100;
            }
            poz_x = 0; poz_y += 100;
        }
        poz_y = 0; poz_z += 100;
    }

    obudowa();

    plik.close();
}

void rysowanie_drogi(int dlugosc_drogi)
{
    string nglowek = "# Alias OBJ Model File\n# made by Michal Aniol xd\n\n"
    "mtllib Untitled.mtl\n\ng Mesh1 Model\n\nusemtl FrontColor\n";

    plik.open("droga 3D.obj", ios::out); // zapis drogi przez labirynt w formie graficznej
    plik << nglowek;

    poz_z = 50.0f; // pierwsza belka na wejsciu
    poz_y = 60.0f;
    poz_x = 50.0f;
    belka_plus_y();

    for (int i = 0 ; i < dlugosc_drogi - 1 ; i++)
    {
        poz_z = (droga_wyjscia[i][0] * 100) + 50.0f;  // poczatek drogi na poczatek labiryntu
        poz_y = (droga_wyjscia[i][1] * 100) + 50.0f;
        poz_x = (droga_wyjscia[i][2] * 100) + 50.0f;

        int z = droga_wyjscia[i][0] - droga_wyjscia[i + 1][0];  // sprawdzenie w ktora
        int y = droga_wyjscia[i][1] - droga_wyjscia[i + 1][1];
        int x = droga_wyjscia[i][2] - droga_wyjscia[i + 1][2];

        if(x == 1) belka_plus_x();
        if(x == -1) belka_minus_x();
        if(y == 1) belka_plus_y();
        if(y == -1) belka_minus_y();
        if(z == 1) belka_plus_z();
        if(z == -1) belka_minus_z();
    }

    poz_z = (droga_wyjscia[dlugosc_drogi-1][0] * 100) + 50.0f; // ostatnia belka na wyjsciu
    poz_y = (droga_wyjscia[dlugosc_drogi-1][1] * 100) + 50.0f;
    poz_x = (droga_wyjscia[dlugosc_drogi-1][2] * 100) + 50.0f;
    belka_minus_y();

    plik.close();
}

/************* POMNIEJSZE FUNKCJE RYSUJACE *****************/

void obudowa() // sciany wokol labiruntu
{
    poz_x = 0; poz_y = -100; poz_z = 0;

    prawo_otwarte();    // przednia sciana
    pole_na_plaszczyznie_Y (90, 0, 90, 90, 90, 0, 90+((miara_z - 1) * 100) + 10, 90, 90 + ((miara_z - 1) * 100) + 10);
    pole_na_plaszczyznie_Y (90, 90, 0, miara_x * 100, 0, 90, miara_z * 100, miara_x * 100, miara_z * 100);
    pole_na_plaszczyznie_Y (90, -10, 0, 0, 0, -10, miara_z * 100, 0, miara_z * 100);
    poz_x = (miara_x - 1) * 100; poz_y = miara_y * 100; poz_z = (miara_z - 1) * 100;

    lewo_otwarte();     //tylnia sciana
    pole_na_plaszczyznie_Y (0, 90, 0, 0, 0, 90, -poz_z, 0, -poz_z);
    pole_na_plaszczyznie_Y (0, 100, 100, 90, 100, 100, -poz_z, 90, -poz_z);
    pole_na_plaszczyznie_Y (0, 90, 100, 0, 100, 90, 90, 0, 90);
    pole_na_plaszczyznie_Y (0, 0, 100, -poz_x - 10, 100, 0, -poz_z, -poz_x - 10, -poz_z);

    pole_na_plaszczyznie_X (100, 100, 0, -poz_z, 0, 100, -poz_y - 10, -poz_z, -poz_y - 10); // sciany boczne
    poz_x=0;
    pole_na_plaszczyznie_X (-10, 100, 0, -poz_z, 0, 100, -poz_y - 10, -poz_z, -poz_y - 10);

    poz_y=0;    // zadaszenie
    pole_na_plaszczyznie_Z (100, -10, miara_y * 100, miara_x * 100, miara_y * 100, -10, -10, miara_x * 100, -10);
}

/*************************************************/

void belka_minus_x() {
    pole_na_plaszczyznie_Z (-5, 105, 5, 5, 5, 105, -5, 5, -5);
    pole_na_plaszczyznie_Z (5, 5, 5, 105, 5, 5, -5, 105, -5);
    pole_na_plaszczyznie_Y (5, 105, 5, 5, 5, 105, -5, 5, -5);
    pole_na_plaszczyznie_Y (-5, 5, 5, 105, 5, 5, -5, 105, -5);
    pole_na_plaszczyznie_X (105, 5, 5, -5, 5, 5, -5, -5, -5);
    pole_na_plaszczyznie_X (5, -5, 5, 5, 5, -5, -5, 5, -5); }

void belka_plus_x() {
    pole_na_plaszczyznie_Z (-5, -5, 5, -105, 5, -5, -5, -105, -5);
    pole_na_plaszczyznie_Z (5, -105, 5, -5, 5, -105, -5, -5, -5);
    pole_na_plaszczyznie_Y (5, -5, 5, -105, 5, -5, -5, -105, -5);
    pole_na_plaszczyznie_Y (-5, -105, 5, -5, 5, -105, -5, -5, -5);
    pole_na_plaszczyznie_X (-5, 5, 5, -5, 5, 5, -5, -5, -5);
    pole_na_plaszczyznie_X (-105, -5, 5, 5, 5, -5, -5, 5, -5); }

void belka_minus_y() {
    pole_na_plaszczyznie_Z (-5, 5, 105, -5, 105, 5, 5, -5, 5);
    pole_na_plaszczyznie_Z (5, -5, 105, 5, 105, -5, 5, 5, 5);
    pole_na_plaszczyznie_X (5, 5, 105, -5, 105, 5, 5, -5, 5);
    pole_na_plaszczyznie_X (-5, -5, 105, 5, 105, -5, 5, 5, 5);
    pole_na_plaszczyznie_Y (5, -5, 5, 5, 5, -5, -5, 5, -5);
    pole_na_plaszczyznie_Y (105, 5, 5, -5, 5, 5, -5, -5, -5); }

void belka_plus_y() {
    pole_na_plaszczyznie_Z (-5, 5, -5, -5, -5, 5, -105, -5, -105);
    pole_na_plaszczyznie_Z (5, -5, -5, 5, -5, -5, -105, 5, -105);
    pole_na_plaszczyznie_X (5, 5, -5, -5, -5, 5, -105, -5, -105);
    pole_na_plaszczyznie_X (-5, -5, -5, 5, -5, -5, -105, 5, -105);
    pole_na_plaszczyznie_Y (-5, 5, 5, -5, 5, 5, -5, -5, -5);
    pole_na_plaszczyznie_Y (-105, -5, 5, 5, 5, -5, -5, 5, -5); }

void belka_minus_z() {
    pole_na_plaszczyznie_Y (-5, -5, 105, 5, 105, -5, 5, 5, 5);
    pole_na_plaszczyznie_Y (5, 5, 105, -5, 105, 5, 5, -5, 5);
    pole_na_plaszczyznie_X (5, 105, -5, 105, 5, 5, -5, 5, 5);
    pole_na_plaszczyznie_X (-5, 105, 5, 105, -5, 5, 5, 5, -5);
    pole_na_plaszczyznie_Z (105, 5, -5, -5, -5, 5, 5, -5, 5);
    pole_na_plaszczyznie_Z (5, -5, -5, 5, -5, -5, 5, 5, 5); }

void belka_plus_z() {
    pole_na_plaszczyznie_Y (-5, -5, -5, 5, -5, -5, -105, 5, -105);
    pole_na_plaszczyznie_Y (5, 5, -5, -5, -5, 5, -105, -5, -105);
    pole_na_plaszczyznie_X (5, -5, -5, -5, 5, -105, -5, -105, 5);
    pole_na_plaszczyznie_X (-5, -5, 5, -5, -5, -105, 5, -105, -5);
    pole_na_plaszczyznie_Z (-5, 5, -5, -5, -5, 5, 5, -5, 5);
    pole_na_plaszczyznie_Z (-105, -5, -5, 5, -5, -5, 5, 5, 5); }

/************************************/

void tyl_zamkniete() {
    pole_na_plaszczyznie_X(0, 0, 0, 90, 0, 0, 90, 90, 90); }

void tyl_otwarte() {
    pole_na_plaszczyznie_X (0, 0, 0, 15, 0, 0, 90, 15, 90);
    pole_na_plaszczyznie_X (0, 15, 0, 75, 0, 15, 15, 75, 15);
    pole_na_plaszczyznie_X (0, 15, 75, 75, 75, 15, 90, 75, 90);
    pole_na_plaszczyznie_X (0, 75, 0, 90, 0, 75, 90, 90, 90); }

void lewo_otwarte() {
    pole_na_plaszczyznie_Y (0, 0, 0, 15, 0, 0, 90, 15, 90);
    pole_na_plaszczyznie_Y (0, 15, 0, 75, 0, 15, 15, 75, 15);
    pole_na_plaszczyznie_Y (0, 15, 75, 75, 75, 15, 90, 75, 90);
    pole_na_plaszczyznie_Y (0, 75, 0, 90, 0, 75, 90, 90, 90); }

void lewo_zamkniete() {
    pole_na_plaszczyznie_Y(0, 0, 0, 90, 0, 0, 90, 90, 90); }

void dol_otwarte() {
    pole_na_plaszczyznie_Z (0, 0, 0, 15, 0, 0, 90, 15, 90);
    pole_na_plaszczyznie_Z (0, 15, 0, 75, 0, 15, 15, 75, 15);
    pole_na_plaszczyznie_Z (0, 15, 75, 75, 75, 15, 90, 75, 90);
    pole_na_plaszczyznie_Z (0, 75, 0, 90, 0, 75, 90, 90, 90); }

void dol_zamkniete() {
    pole_na_plaszczyznie_Z (0, 0, 0, 90, 0, 0, 90, 90, 90); }

void prawo_zamkniete() {
    pole_na_plaszczyznie_Y (90, 0, 0, 90, 0, 0, 90, 90, 90); }

void prawo_otwarte() {
    pole_na_plaszczyznie_Y (90, 0, 0, 15, 0, 0, 90, 15, 90);
    pole_na_plaszczyznie_Y (90, 15, 0, 75, 0, 15, 15, 75, 15);
    pole_na_plaszczyznie_Y (90, 15, 75, 75, 75, 15, 90, 75, 90);
    pole_na_plaszczyznie_Y (90, 75, 0, 90, 0, 75, 90, 90, 90);
    pole_na_plaszczyznie_Z (15, 15, 90, 75, 90, 15, 100, 75, 100);
    pole_na_plaszczyznie_Z (75, 15, 90, 75, 90, 15, 100, 75, 100);
    pole_na_plaszczyznie_X (15, 15, 90, 75, 90, 15, 100, 75, 100);
    pole_na_plaszczyznie_X (75, 15, 90, 75, 90, 15, 100, 75, 100); }

void przod_zamkniete() {
    pole_na_plaszczyznie_X (90, 0, 0, 90, 0, 0, 90, 90, 90); }

void przod_otwarte() {
    pole_na_plaszczyznie_X (90, 0, 0, 15, 0, 0, 90, 15, 90);
    pole_na_plaszczyznie_X (90, 15, 0, 75, 0, 15, 15, 75, 15);
    pole_na_plaszczyznie_X (90, 15, 75, 75, 75, 15, 90, 75, 90);
    pole_na_plaszczyznie_X (90, 75, 0, 90, 0, 75, 90, 90, 90);
    pole_na_plaszczyznie_Y (15, 90, 15, 90, 75, 100, 15, 100, 75);
    pole_na_plaszczyznie_Y (75, 90, 15, 90, 75, 100, 15, 100, 75);
    pole_na_plaszczyznie_Z (15, 90, 15, 90, 75, 100, 15, 100, 75);
    pole_na_plaszczyznie_Z (75, 90, 15, 90, 75, 100, 15, 100, 75); }

void gora_zamkniete() {
    pole_na_plaszczyznie_Z (90, 0, 0, 90, 0, 0, 90, 90, 90); }

void gora_otwarte() {
    pole_na_plaszczyznie_Z (90, 0, 0, 15, 0, 0, 90, 15, 90);
    pole_na_plaszczyznie_Z (90, 15, 0, 75, 0, 15, 15, 75, 15);
    pole_na_plaszczyznie_Z (90, 15, 75, 75, 75, 15, 90, 75, 90);
    pole_na_plaszczyznie_Z (90, 75, 0, 90, 0, 75, 90, 90, 90);
    pole_na_plaszczyznie_X (15, 90, 15, 90, 75, 100, 15, 100, 75);
    pole_na_plaszczyznie_X (75, 90, 15, 90, 75, 100, 15, 100, 75);
    pole_na_plaszczyznie_Y (15, 15, 90, 75, 90, 15, 100, 75, 100);
    pole_na_plaszczyznie_Y (75, 15, 90, 75, 90, 15, 100, 75, 100); }

/***************************************/

    void pole_na_plaszczyznie_X(float x,
    float z1, float y1, float z2, float y2,
    float z3, float y3, float z4, float y4) {
 //       cout << "v " << (poz_x + x) / 100 << " " << (poz_z + z1) / 100 << " " << (poz_y + y1) / 100 << "\n";
        plik << "v " << (poz_x + x) / 100 << " " << (poz_z + z1) / 100 << " " << (poz_y + y1) / 100 << "\n";
 //       cout << "v " << (poz_x + x) / 100 << " " << (poz_z + z2) / 100 << " " << (poz_y + y2) / 100 << "\n";
        plik << "v " << (poz_x + x) / 100 << " " << (poz_z + z2) / 100 << " " << (poz_y + y2) / 100 << "\n";
  //      cout << "v " << (poz_x + x) / 100 << " " << (poz_z + z4) / 100 << " " << (poz_y + y4) / 100 << "\n";
        plik << "v " << (poz_x + x) / 100 << " " << (poz_z + z4) / 100 << " " << (poz_y + y4) / 100 << "\n";
  //      cout << "v " << (poz_x + x) / 100 << " " << (poz_z + z3) / 100 << " " << (poz_y + y3) / 100 << "\n";
        plik << "v " << (poz_x + x) / 100 << " " << (poz_z + z3) / 100 << " " << (poz_y + y3) / 100 << "\n";
  //      cout << "f -4 -3 -2 -1\n\n";
        plik << "f -4 -3 -2 -1\n\n"; }

    void pole_na_plaszczyznie_Z(float z,
    float x1, float y1, float x2, float y2,
    float x3, float y3, float x4, float y4) {
  //      cout << "v " << (poz_x + x1) / 100 << " " << (poz_z + z) / 100 << " " << (poz_y + y1) / 100 << "\n";
        plik << "v " << (poz_x + x1) / 100 << " " << (poz_z + z) / 100 << " " << (poz_y + y1) / 100 << "\n";
  //      cout << "v " << (poz_x + x2) / 100 << " " << (poz_z + z) / 100 << " " << (poz_y + y2) / 100 << "\n";
        plik << "v " << (poz_x + x2) / 100 << " " << (poz_z + z) / 100 << " " << (poz_y + y2) / 100 << "\n";
   //     cout << "v " << (poz_x + x4) / 100 << " " << (poz_z + z) / 100 << " " << (poz_y + y4) / 100 << "\n";
        plik << "v " << (poz_x + x4) / 100 << " " << (poz_z + z) / 100 << " " << (poz_y + y4) / 100 << "\n";
  //      cout << "v " << (poz_x + x3) / 100 << " " << (poz_z + z) / 100 << " " << (poz_y + y3) / 100 << "\n";
        plik << "v " << (poz_x + x3) / 100 << " " << (poz_z + z) / 100 << " " << (poz_y + y3) / 100 << "\n";
  //      cout << "f -4 -3 -2 -1\n\n";
        plik << "f -4 -3 -2 -1\n\n"; }

    void pole_na_plaszczyznie_Y(float y,
    float x1, float z1, float x2, float z2,
    float x3, float z3, float x4, float z4) {
 //       cout << "v " << (poz_x + x1) / 100 << " " << (poz_z + z1) / 100 << " " << (poz_y + y) / 100 << "\n";
        plik << "v " << (poz_x + x1) / 100 << " " << (poz_z + z1) / 100 << " " << (poz_y + y) / 100 << "\n";
  //      cout << "v " << (poz_x + x2) / 100 << " " << (poz_z + z2) / 100 << " " << (poz_y + y) / 100 << "\n";
        plik << "v " << (poz_x + x2) / 100 << " " << (poz_z + z2) / 100 << " " << (poz_y + y) / 100 << "\n";
  //      cout << "v " << (poz_x + x4) / 100 << " " << (poz_z + z4) / 100 << " " << (poz_y + y) / 100 << "\n";
        plik << "v " << (poz_x + x4) / 100 << " " << (poz_z + z4) / 100 << " " << (poz_y + y) / 100 << "\n";
  //      cout << "v " << (poz_x + x3) / 100 << " " << (poz_z + z3) / 100 << " " << (poz_y + y) / 100 << "\n";
        plik << "v " << (poz_x + x3) / 100 << " " << (poz_z + z3) / 100 << " " << (poz_y + y) / 100 << "\n";
  //      cout << "f -4 -3 -2 -1\n\n";
        plik << "f -4 -3 -2 -1\n\n"; }

