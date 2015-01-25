#ifndef MUTANT_TRIANGLE_H_
#define MUTANT_TRIANGLE_H_

#include "image.h"
#include "color.h"

typedef struct {
    int x, y;
} Vertice;

typedef struct {
    Vertice v1, v2, v3;
    Color color;
} Triangle;

extern void randomize_triangle(Triangle *, const int, const int);
extern void log_triangle(const Triangle *);
extern void normalize_triangle(Triangle *);
extern void rasterize_triangle(const Triangle *, Image *);
extern void mutate_triangle(Triangle *, const int, const int);

#endif /* MUTANT_TRIANGLE_H_ */
