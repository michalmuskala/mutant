#ifndef MUTANT_IMAGE_H_
#define MUTANT_IMAGE_H_

#include <SDL_image.h>
#include "color.h"

/* Our default format, try to convert everything to it */
#define MUTANT_SDL_FORMAT SDL_PIXELFORMAT_RGBA8888

extern int init_image();
extern void quit_image();

typedef struct IMAGE {
    int w, h, access;
    unsigned int format;
    SDL_Texture *texture;
} IMAGE;

typedef SDL_Surface RawImage;

extern RawImage *read_raw_image(char *);
extern IMAGE *init_dynamic_image(int, int);
extern void free_image(IMAGE *);

extern void draw_hline(IMAGE *, const int, const int, const int, const COLOR *);

#endif /* MUTANT_IMAGE_H_ */
