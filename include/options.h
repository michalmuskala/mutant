#ifndef MUTANT_OPTIONS_H_
#define MUTANT_OPTIONS_H_

typedef struct {
    char *image;
    int max_triangles;
    int addition_chance;
    int deletion_chance;
    int mutation_chance;
    int swap_chance;
    int noseed;
    double temp_step;
    char bgr, bgg, bgb;
} Options;

/* Default options storage */
extern Options *options;

extern Options *parse_options(int, char **);
extern void free_options();

#endif /* MUTANT_OPTIONS_H_ */
