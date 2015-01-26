#ifndef MUTANT_ANNEALING_H_
#define MUTANT_ANNEALING_H_

#include "image.h"
#include "display.h"
#include "triangle_set.h"

typedef struct {
    Image *original;
    Image *image;
    long generation;
    double temperature;
    struct {
        TriangleSet *triangles;
        double rate;
    } current, best;
} Annealing;

extern Annealing *init_annealing(Image *);
extern void step_annealing(Annealing *);
extern void free_annealing(Annealing *);

#endif /* MUTANT_ANNEALING_H_ */
