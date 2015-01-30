# Mutant

Mutant jest programem do reprodukcji obrazów składających się jedynie z trójkątów
metodą symulowanego wyżarzania.

## Użycie programu:
```
Usage: bin/mutant -f <image> [options]

Option   Default      Description
-f       (required)   image to process
-n       50           max number of triangles to use
-m       20           triangle mutation chance
-a       10           triangle addition chance
-d       30           triangle deletion chance
-s       40           triangles swap chance
-t       0.0001       iteration temperature step
-r       0            background red
-g       0            background green
-b       0            background blue
-z       false        skip random generator seeding
-h       -            print this help message
-v       -            print information about program
All chances are expressed as 1 in value provided
Colors are expressed in RGB 0-255
```

Program przyjmuje obrazy w formatach: PNG, JPG, BMP, GIF.

## Kod programu

Kod programu dostępny jest pod adresem
[https://github.com/michalmuskala/mutant](github.com/michalmuskala/mutant)
i jest objęty licencją MIT rozprowadzaną wraz z kodem źródłowym.

## Algorytm symulowanego wyżarzania

Symulowane wyżarzanie to rodzaj algorytmu heurystycznego przeszukującego
przestrzeń alternatywnych rozwiązań problemu w celu wyszukania rozwiązań
najlepszych. Sposób działania symulowanego wyżarzania nieprzypadkowo przypomina
zjawisko wyżarzania w metalurgii.

Kroki algorytmu w klasycznym podejściu podczas minimalizacji funkcji `F(w)`:

1. Losowy wybór punktu startowego `w`. Przyjęcie temperatury `T=MAX`,
2. Wyznaczenie wartości funkcji `F(w)` w punkcie `w`,
3. Wyznaczenie `w' = w + dw`, gdzie `dw` jest realizacją zmiennej losowej
o rozkładzie normalnym z medianą w punkcie `dw` i średnią wariancją równą `T` ,
4. Wyznaczenie wartości funkcji `F(w')` w nowym punkcie,
5. Podstawienie wartości `w'` do  `w` z prawdopodobieństwem danym rozkładem
Boltzmanna `b(E(w')-E(w),T)`,
6. Zmniejszenie temperatury `T=nT`, gdzie `n` jest stałą z przedziału `(0,1)`,
7. Spełnienie kryterium stopu lub powrót do kroku 3.


## Zależności

Program do kompilacji wymaga bibliotek:
* SDL2,
* SDL2_image,
* biblioteka do otwierania zamierzonych obrazów, np. libpng dla PNG.

W systemie operacyjnym Windows program można skompilować używając
środowiska MiniGW.

## Elementy programu

Projekt podzielony jest na następujące moduły (każdy ma plik `.c` w folderze
`src/` oraz plik `.h` w folderze `include/`):
* annealing - implementacja algorytmu symulowanego wyżarzania,
* color - ocena odległości koloru i obsługa struktury Color,
* display - zarządzanie wyświetlaniem okna i obrazów, komunikacja z biblioteką SDL
* events - obsługa wydarzeń okna (zamknięcie aplikacji, itp),
* image - obsługa obrazu, rysowania i oceny podobieństwa obrazów,
* main - zawiera funkcję main,
* options - parsowanie opcji podanych z wiersza poleceń, wystawia globalną zmienną
`options` będącą strukturą opcji domyślnych i podanych przez użytkownika
* triangle - obsługa pojedynczego trójkąta, resteryzacja, mutacja, losowanie,
* triangle_set - obsługa zestawu trójkątów, rasteryzacja, mutacja, kopiowanie.

## Ważne informacje

Funkcje zwracające wskaźnik do struktury zwracają `NULL` w przypadku niepowodzenia.
Wszystkie takie struktury powinny być zwalniane odpowiednią funkcją.

Funkcje zwracające `int` jako sygnał powodzenia zwracają `0` w przypadku powodzenia,
lub `-1` w przypadku błędów.

## Kompilacja

Pod systemami unixowymi, np, ArchLinux:
```
$ git clone https://github.com/michalmuskala/mutant
$ cd mutant
$ yaourt -S SDL2 SDL2_image libpng
$ make
$ bin/mutant
```

Pod windowsem:
```
TODO
```

## Interesujące elementy programu

Najciekawszym, a zarazem najbardziej skomplikowanym elementem programu
jest proces rasteryzacji trójkątów. Użyty algorytm opiera się na
spostrzeżeniu, iż każdy trójkąt da się podzielić na dwa - jeden o płaskim
dole i drugi o płaskiej górze, które to trójkąty rasteryzuje się w sposób prosty.
Na implementację algorytmu składają się 3 funkcje:

* Funkcja wejścia do algorytmu `rasterize_triangle` sprawdza, czy przez przypadek
nie mamy do czynienia z trójkątem już płaskim z którejś strony, jeśli tak po prostu
wywołuje odpowiednią funkcję, w przeciwnym wypadku oblicza punkt pozwalający na
wspomniany podział. Funkcja oczekuje, że trójkąt będzie znormalizowany
(tzn. `y1 <= y2 <= y3`).
```c
void
rasterize_triangle(const Triangle *t, Image *image)
{
    Vertice middle = {0, 0};

    if (t->v2.y == t->v3.y) {
        rasterize_bottom_triangle(&t->v1, &t->v2, &t->v3, &t->color, image);
    } else if (t->v1.y == t->v2.y) {
        rasterize_top_triangle(&t->v1, &t->v2, &t->v3, &t->color, image);
    } else {
        middle.x = (int)
            (t->v1.x
             + (((double) (t->v2.y - t->v1.y)
                 / (double) (t->v3.y - t->v1.y))
                * (t->v3.x - t->v1.x)));
        middle.y = t->v2.y;

        rasterize_bottom_triangle(&t->v1, &t->v2, &middle, &t->color, image);
        rasterize_top_triangle(&t->v2, &middle, &t->v3, &t->color, image);
    }
}
```

* Funkcja `rasterize_top_triangle` która przechodzi po wszystkich y należących do
górnego trójkąta i odpowiednio zwiększając początkową i końcową wartość x rysuje
poziome linie.
```c
static void
rasterize_top_triangle(const Vertice *v1, const Vertice *v2,
                       const Vertice *v3, const Color *color,
                       Image *image)
{
    /* invslope = 1 / slope = dx / dy */
    double invslope1 = 0, invslope2 = 0;
    double currx1 = 0, currx2 = 0;
    int curry = 0;

    invslope1 = (double) (v3->x - v1->x) / (v3->y - v1->y);
    invslope2 = (double) (v3->x - v2->x) / (v3->y - v2->y);

    for (curry = v3->y, currx1 = v3->x, currx2 = v3->x;
         curry > v1->y;
         curry--, currx1 -= invslope1, currx2 -= invslope2) {
        draw_hline(image, curry, (int) currx1, (int) currx2, color);
    }
}
```

* Funkcja `razterize_bottom_triangle` zaimplementowana w sposób analogiczny
do funkcji poprzedniej.
```c
static void
rasterize_bottom_triangle(const Vertice *v1, const Vertice *v2,
                          const Vertice *v3, const Color *color,
                          Image *image)
{
    /* invslope = 1 / slope = dx / dy */
    double invslope1 = 0, invslope2 = 0;
    double currx1 = 0, currx2 = 0;
    int curry = 0;

    invslope1 = (double) (v2->x - v1->x) / (v2->y - v1->y);
    invslope2 = (double) (v3->x - v1->x) / (v3->y - v1->y);

    for (curry = v1->y, currx1 = v1->x, currx2 = v1->x;
         curry <= v2->y;
         curry++, currx1 += invslope1, currx2 += invslope2) {
        draw_hline(image, curry, (int) currx1, (int) currx2, color);
    }
}
```
