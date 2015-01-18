#ifndef MUTANT_TRIANGLES_H_
#define MUTANT_TRIANGLES_H_

#include "triangle.h"

typedef struct TRAINGLES {
    TRIANGLE *triangles;
    int max_count;
    int count;
} TRIANGLES;

extern TRIANGLES *init_triangles(int);
extern void free_triangles(TRIANGLES *);

extern TRIANGLES *random_triangles(int, int, int);
extern void rasterize_triangles(TRIANGLES *, IMAGE *);

#endif /* MUTANT_TRIANGLES_H_ */
