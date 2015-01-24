#ifndef MUTANT_TRIANGLES_H_
#define MUTANT_TRIANGLES_H_

#include "triangle.h"
#include "image.h"

#define ADDITION_CHANCE 10
#define MUTATION_CHANCE 20
#define DELETION_CHANCE 30

#define CHECK_CHANCE(chance) (rand() % (chance) == 0)

typedef struct TRAINGLES {
    int max_count;
    int count;
    int max_w, max_h;
    TRIANGLE *triangles;
} TRIANGLES;

extern TRIANGLES *init_triangles(int, int, int);
extern void free_triangles(TRIANGLES *);

extern TRIANGLES *random_triangles(int, int, int);
extern void rasterize_triangles(TRIANGLES *, IMAGE *);
extern void mutate_triangles(TRIANGLES *);

#endif /* MUTANT_TRIANGLES_H_ */
