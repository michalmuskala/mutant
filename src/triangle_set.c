#include "triangle_set.h"

#include "options.h"
#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

TriangleSet *
init_triangles(int max_w, int max_h)
{
    TriangleSet *triangles = NULL;

    triangles = malloc(sizeof(*triangles));

    if (triangles == NULL) {
        perror("Initializing triangles");
        return NULL;
    }

    triangles->max_count = options->max_triangles;
    triangles->triangles = calloc(triangles->max_count,
                                  sizeof(*triangles->triangles));

    if (triangles->triangles == NULL) {
        perror("Initializing triangles");
        free_triangles(triangles);
        return NULL;
    }

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
copy_triangles(TriangleSet *new, const TriangleSet *old)
{
    new->count = old->count;
    new->max_w = old->max_w;
    new->max_h = old->max_h;

    memcpy(new->triangles, old->triangles,
           new->count * sizeof(*new->triangles));
}


void
free_triangles(TriangleSet *triangles)
{
    if (triangles == NULL) {
        return;
    }

    if (triangles->triangles != NULL) {
        free(triangles->triangles);
    }

    free(triangles);
}

void
rasterize_triangles(TriangleSet *triangles, Image *image)
{
    int i = 0;

    clear_image(image);

    for (i = 0; i < triangles->count; i++) {
        normalize_triangle(&triangles->triangles[i]);
        rasterize_triangle(&triangles->triangles[i], image);
    }
}

static void
add_triangle_to_triangles(TriangleSet *t)
{
    randomize_triangle(&t->triangles[t->count], t->max_w, t->max_h);
    t->count++;
}

static void
delete_triangle_from_triangles(TriangleSet *t)
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
mutate_triangles(TriangleSet *t)
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
