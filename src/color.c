#include "color.h"

COLOR
make_color(const byte r, const byte g, const byte b, const byte a)
{
    COLOR color = {0};

    color.r = r;
    color.g = g;
    color.b = b;
    color.a = a;

    return color;
}
