#ifndef MUTANT_COLOR_H_
#define MUTANT_COLOR_H_

typedef unsigned char byte;

typedef struct {
    byte r, g, b, a;
} Color;

#define COLOR_MAX 255

extern Color make_color(const byte, const byte, const byte, const byte);
extern double color_distance(const Color, const Color);

#endif /* MUTANT_COLOR_H_ */
