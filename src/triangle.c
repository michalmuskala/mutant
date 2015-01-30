#include "triangle.h"

#include <stdlib.h>
#include <stdio.h>

static int
randbetween(const int min, const int max)
{
    return min + (rand() % (max - min + 1));
}

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

#define VERTICES_DELTA 50

static void
mutate_vertices(Triangle *t, const int max_w, const int max_h)
{
    t->v1.x += randbetween(-VERTICES_DELTA, VERTICES_DELTA);
    t->v2.x += randbetween(-VERTICES_DELTA, VERTICES_DELTA);
    t->v3.x += randbetween(-VERTICES_DELTA, VERTICES_DELTA);

    t->v1.y += randbetween(-VERTICES_DELTA, VERTICES_DELTA);
    t->v2.y += randbetween(-VERTICES_DELTA, VERTICES_DELTA);
    t->v3.y += randbetween(-VERTICES_DELTA, VERTICES_DELTA);

    if (t->v1.x < 0) {
        t->v1.x = 0;
    } else if (t->v1.x > max_w) {
        t->v1.x = max_w;
    }

    if (t->v2.x < 0) {
        t->v2.x = 0;
    } else if (t->v2.x > max_w) {
        t->v2.x = max_w;
    }

    if (t->v3.x < 0) {
        t->v3.x = 0;
    } else if (t->v3.x > max_w) {
        t->v3.x = max_w;
    }

    if (t->v1.y < 0) {
        t->v1.y = 0;
    } else if (t->v1.y > max_h) {
        t->v1.y = max_h;
    }

    if (t->v2.y < 0) {
        t->v2.y = 0;
    } else if (t->v2.y > max_h) {
        t->v2.y = max_h;
    }

    if (t->v3.y < 0) {
        t->v3.y = 0;
    } else if (t->v3.y > max_h) {
        t->v3.y = max_h;
    }
}

static void
randomize_color(Triangle *triangle)
{
    triangle->color.r = rand() % COLOR_MAX;
    triangle->color.g = rand() % COLOR_MAX;
    triangle->color.b = rand() % COLOR_MAX;
}

#define COLOR_DELTA 10

static void
mutate_color(Triangle *t)
{
    int r = 0, g = 0, b = 0;

    r = t->color.r + randbetween(-COLOR_DELTA, COLOR_DELTA);
    g = t->color.g + randbetween(-COLOR_DELTA, COLOR_DELTA);
    b = t->color.b + randbetween(-COLOR_DELTA, COLOR_DELTA);

    if (r < 0) {
        t->color.r = 0;
    } else if (r > COLOR_MAX) {
        t->color.r = COLOR_MAX;
    } else {
        t->color.r = r;
    }

    if (g < 0) {
        t->color.g = 0;
    } else if (g > COLOR_MAX) {
        t->color.g = COLOR_MAX;
    } else {
        t->color.g = g;
    }

    if (b < 0) {
        t->color.b = 0;
    } else if (b > COLOR_MAX) {
        t->color.b = COLOR_MAX;
    } else {
        t->color.b = b;
    }
}

#define ALPHA_MIN 25
#define ALPHA_MAX 230

static void
randomize_alpha(Triangle *triangle)
{
    triangle->color.a = randbetween(ALPHA_MIN, ALPHA_MAX);
}

static void
mutate_alpha(Triangle *t)
{
    int a = 0;

    a = t->color.a + randbetween(-COLOR_DELTA, COLOR_DELTA);

    if (a < ALPHA_MIN) {
        t->color.a = ALPHA_MIN;
    } else if (a > ALPHA_MAX) {
        t->color.a = ALPHA_MAX;
    } else {
        t->color.a = a;
    }
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
    MUTATE_POSITION = 0,
    MUTATE_POSITION2,
    RANDOMIZE_POSITION,
    MUTATE_COLOR,
    MUTATE_COLOR2,
    RANDOMIZE_COLOR,
    MUTATE_ALPHA,
    MUTATE_ALPHA2,
    RANDOMIZE_ALPHA,
    CHOICES_COUNT
};

void
mutate_triangle(Triangle *triangle, const int max_w, const int max_h)
{
    switch(rand() % CHOICES_COUNT) {
    case MUTATE_POSITION:
    case MUTATE_POSITION2:
        mutate_vertices(triangle, max_w, max_h);
        break;
    case RANDOMIZE_POSITION:
        randomize_vertices(triangle, max_w, max_h);
        break;
    case MUTATE_COLOR:
    case MUTATE_COLOR2:
        mutate_color(triangle);
        break;
    case RANDOMIZE_COLOR:
        randomize_color(triangle);
        break;
    case MUTATE_ALPHA:
    case MUTATE_ALPHA2:
        mutate_alpha(triangle);
        break;
    case RANDOMIZE_ALPHA:
        randomize_alpha(triangle);
        break;
    default:
        abort();
    }
}
