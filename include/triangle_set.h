#ifndef MUTANT_TRIANGLE_SET_H_
#define MUTANT_TRIANGLE_SET_H_

#include "triangle.h"
#include "image.h"

typedef struct {
    int max_count;
    int count;
    int max_w, max_h;
    Triangle *triangles;
} TriangleSet;

extern TriangleSet *init_triangles(int, int, int);
extern void free_triangles(TriangleSet *);

extern TriangleSet *random_triangles(int, int, int);
extern void rasterize_triangles(TriangleSet *, Image *);
extern void mutate_triangles(TriangleSet *);

#endif /* MUTANT_TRIANGLE_SET_H_ */
