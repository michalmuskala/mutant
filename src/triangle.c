#include "triangle.h"

#include <stdlib.h>
#include <stdio.h>

static void
randomize_vertices(Triangle *triangle, const int max_w, const int max_h)
{
    triangle->v1.x = rand() % max_w;
    triangle->v2.x = rand() % max_w;
    triangle->v3.x = rand() % max_w;

    triangle->v1.y = rand() % max_h;
    triangle->v2.y = rand() % max_h;
    triangle->v3.y = rand() % max_h;
}

static void
randomize_color(Triangle *triangle)
{
    triangle->color.r = rand() % COLOR_MAX;
    triangle->color.g = rand() % COLOR_MAX;
    triangle->color.b = rand() % COLOR_MAX;
}

static void
randomize_alpha(Triangle *triangle)
{
    triangle->color.a = rand() % COLOR_MAX;
}

void
randomize_triangle(Triangle *triangle, const int max_w, const int max_h)
{
    randomize_vertices(triangle, max_w, max_h);
    randomize_color(triangle);
    randomize_alpha(triangle);
}

void
log_triangle(const Triangle *triangle)
{
    fprintf(stderr,
            "Triangle: A = (%4d,%4d) B = (%4d,%4d) C = (%4d,%4d)"
            " r=%3d g=%3d b=%3d a=%3d\n",
            triangle->v1.x, triangle->v1.y,
            triangle->v2.x, triangle->v2.y,
            triangle->v3.x, triangle->v3.y,
            triangle->color.r, triangle->color.g,
            triangle->color.b, triangle->color.a);
}

static void
swap_vertices(Vertice *a, Vertice *b)
{
    Vertice tmp = *a;
    *a = *b;
    *b = tmp;
}

void
normalize_triangle(Triangle *t)
{
    if (t->v1.y > t->v3.y) {
        swap_vertices(&t->v1, &t->v3);
    }

    if (t->v1.y > t->v2.y) {
        swap_vertices(&t->v1, &t->v2);
    }

    if (t->v2.y > t->v3.y) {
        swap_vertices(&t->v2, &t->v3);
    }
}

static void
rasterize_top_triangle(const Vertice *v1, const Vertice *v2, const Vertice *v3,
                       const Color *color, Image *image)
{
    /* invslope = 1 / slope = dx / dy */
    double invslope1 = 0, invslope2 = 0;
    double currx1 = 0, currx2 = 0;
    int curry = 0;

    invslope1 = (double) (v3->x - v1->x) / (v3->y - v1->y);
    invslope2 = (double) (v3->x - v2->x) / (v3->y - v2->y);

    for (curry = v3->y, currx1 = v3->x, currx2 = v3->x;
         curry > v1->y;
         curry--, currx1 -= invslope1, currx2 -= invslope2) {
        draw_hline(image, curry, (int) currx1, (int) currx2, color);
    }
}

static void
rasterize_bottom_triangle(const Vertice *v1, const Vertice *v2,
                          const Vertice *v3, const Color *color, Image *image)
{
    /* invslope = 1 / slope = dx / dy */
    double invslope1 = 0, invslope2 = 0;
    double currx1 = 0, currx2 = 0;
    int curry = 0;

    invslope1 = (double) (v2->x - v1->x) / (v2->y - v1->y);
    invslope2 = (double) (v3->x - v1->x) / (v3->y - v1->y);

    for (curry = v1->y, currx1 = v1->x, currx2 = v1->x;
         curry <= v2->y;
         curry++, currx1 += invslope1, currx2 += invslope2) {
        draw_hline(image, curry, (int) currx1, (int) currx2, color);
    }
}

void
rasterize_triangle(const Triangle *t, Image *image)
{
    Vertice middle = {0, 0};

    if (t->v2.y == t->v3.y) {
        rasterize_bottom_triangle(&t->v1, &t->v2, &t->v3, &t->color, image);
    } else if (t->v1.y == t->v2.y) {
        rasterize_top_triangle(&t->v1, &t->v2, &t->v3, &t->color, image);
    } else {
        middle.x = (int)
            (t->v1.x
             + (((double) (t->v2.y - t->v1.y) / (double) (t->v3.y - t->v1.y))
                * (t->v3.x - t->v1.x)));
        middle.y = t->v2.y;

        rasterize_bottom_triangle(&t->v1, &t->v2, &middle, &t->color, image);
        rasterize_top_triangle(&t->v2, &middle, &t->v3, &t->color, image);
    }
}

enum choice {
    RANDOMIZE_POSITION = 0,
    MUTATE_POSITION,
    RANDOMIZE_COLOR,
    MUTATE_COLOR,
    RANDOMIZE_ALPHA,
    MUTATE_ALPHA,
    CHOICES_COUNT
};

void
mutate_triangle(Triangle *triangle, const int max_w, const int max_h)
{
    switch(rand() % CHOICES_COUNT) {
    case RANDOMIZE_POSITION:
        randomize_vertices(triangle, max_w, max_h);
        break;
    case MUTATE_POSITION:
        break;
    case RANDOMIZE_COLOR:
        randomize_color(triangle);
        break;
    case MUTATE_COLOR:
        break;
    case RANDOMIZE_ALPHA:
        randomize_alpha(triangle);
        break;
    case MUTATE_ALPHA:
        break;
    default:
        abort();
    }
}
