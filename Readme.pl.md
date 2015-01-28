# Mutant

Mutant jest programem do ewoluowania obrazów metodą symulowanego wyżarzania.

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

## Elementy programu

Projekt podzielony jest na następujące moduły (każy ma plik `.c` w folderze
`src/` oraz plik `.h` w folderze `include/`):
* annealing - implementacja algorytmu symulowanego wyżarzania,
* color - ocena odległości koloru i obsługa struktury Color,
* display - zarządzanie wyświetlaniem okna i obrazów, komunikacja z biblioteką SDL
* events - obsługa wydarzeń okna (zamknięcie aplikacji, itp),
* image - obsługa obrazu, rysowania i oceny podobieństwa obrazów,
* main - zawiera funkcję main,
* options - parsowanie opcji podanych z wiersza poleceń, wystawia globalną zmienną
`options` będącą strukturą opcji domyślnych i podanych przez użytkownika
* triangle - obsługa pojedyczego trójkątu, resteryzacja, mutacja, losowanie,
* triangle_set - obsługa zestawu trójkątów, rasteryzacja, mutacja, kopiowanie.

## Ważne informacje

Funkcje zwracające wskaźnik do struktury zwracają `NULL` w przypadku niepowodzenia.
Wszystkie takie struktury powinny być zwalniane odpowiednią funkcją.

Funkcje zwracające `int` jako sygnał powodzenia zwracają 0 w przypadku powodzenia,
lub -1 w przypadku błędów.

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
