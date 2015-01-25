#include "color.h"

#include <math.h>

Color
make_color(const byte r, const byte g, const byte b, const byte a)
{
    Color color = {0};

    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;

    return color;
}

/* TODO: some better metric, maybe deltaE in the CIELab colorspace? */
double
color_distance(const Color x, const Color y)
{
    int dr = 0, dg = 0, db = 0;

    dr = x.r - y.r;
    dg = x.g - y.g;
    db = x.b - y.b;

    return sqrt(dr * dr + dg * dg + db * db) / COLOR_MAX;
}
