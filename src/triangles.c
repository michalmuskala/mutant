#include "triangles.h"

#include "options.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

TRIANGLES *
init_triangles(int count, int max_w, int max_h)
{
    TRIANGLES *triangles = NULL;

    if (count <= 0) {
        fprintf(stderr, "Triangles count must be positive\n");
        return NULL;
    }

    triangles = malloc(sizeof(TRIANGLES));

    if (triangles == NULL) {
        perror("Initializing triangles");
        return NULL;
    }

    triangles->triangles = calloc(sizeof(TRIANGLE), count);

    if (triangles->triangles == NULL) {
        free_triangles(triangles);
        return NULL;
    }

    triangles->max_count = count;
    triangles->count = 0;
    triangles->max_w = max_w;
    triangles->max_h = max_h;

    /* Add initial triangle */
    randomize_triangle(&triangles->triangles[triangles->count],
                       triangles->max_w, triangles->max_h);
    triangles->count++;

    return triangles;
}

void
free_triangles(TRIANGLES *triangles)
{
    if (triangles == NULL) {
        return;
    }

    if (triangles->triangles != NULL) {
        free(triangles->triangles);
    }

    free(triangles);
}

TRIANGLES *
random_triangles(int count, int max_w, int max_h)
{
    TRIANGLES *triangles = NULL;

    triangles = init_triangles(count, max_w, max_h);

    if (triangles == NULL) {
        return NULL;
    }

    for (triangles->count = 0;
         triangles->count < triangles->max_count;
         triangles->count++) {
        randomize_triangle(&triangles->triangles[triangles->count],
                           triangles->max_w, triangles->max_h);
    }

    return triangles;
}

void
rasterize_triangles(TRIANGLES *triangles, IMAGE *image)
{
    int i = 0;

    clear_image(image);

    for (i = 0; i < triangles->count; i++) {
        normalize_triangle(&triangles->triangles[i]);
        rasterize_triangle(&triangles->triangles[i], image);
    }
}

static void
add_triangle_to_triangles(TRIANGLES *t)
{
    randomize_triangle(&t->triangles[t->count], t->max_w, t->max_h);
    t->count++;
}

static void
delete_triangle_from_triangles(TRIANGLES *t)
{
    int deli = 0;

    deli = rand() % t->count;

    if (deli == t->count - 1) {
        /* It's last triangle - decrease count */
        t->count--;
    } else {
        /* move following triangles back by 1 */
        memmove(&t->triangles[deli], &t->triangles[deli+1],
                sizeof(t->triangles[deli]) * (t->count - deli));
        t->count--;
    }
}

#define CHECK_CHANCE(chance) (rand() % (chance) == 0)

void
mutate_triangles(TRIANGLES *t)
{
    int i = 0;

    if (t->count < t->max_count && CHECK_CHANCE(options->addition_chance)) {
        fprintf(stderr, "Adding triangle\n");
        add_triangle_to_triangles(t);
        return;
    }

    if (t->count > 0 && CHECK_CHANCE(options->deletion_chance)) {
        fprintf(stderr, "Deleting triangle\n");
        delete_triangle_from_triangles(t);
        return;
    }

    for (i = 0; i < t->count; i++) {
        if (CHECK_CHANCE(options->mutation_chance)) {
            fprintf(stderr, "Mutating triangle#%d\n", i);
            mutate_triangle(&t->triangles[i], t->max_w, t->max_h);
        }
    }
}
