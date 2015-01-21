#ifndef MUTANT_TRIANGLE_H_
#define MUTANT_TRIANGLE_H_

#include "image.h"
#include "color.h"

typedef struct VERTICE {
    int x, y;
} VERTICE;

typedef struct TRIANGLE {
    VERTICE v1, v2, v3;
    COLOR color;
} TRIANGLE;

extern void randomize_triangle(TRIANGLE *, const int, const int);
extern void log_triangle(const TRIANGLE *);
extern void normalize_triangle(TRIANGLE *);
extern void rasterize_triangle(const TRIANGLE *, IMAGE *);
extern void mutate_triangle(TRIANGLE *);

#endif /* MUTANT_TRIANGLE_H_ */
