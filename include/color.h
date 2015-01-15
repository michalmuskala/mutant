#ifndef MUTANT_COLOR_H_
#define MUTANT_COLOR_H_

typedef unsigned char byte;

typedef struct COLOR {
    byte r, g, b, a;
} COLOR;

#define COLOR_MAX 256

extern COLOR make_color(const byte, const byte, const byte, const byte);

#endif /* MUTANT_COLOR_H_ */
