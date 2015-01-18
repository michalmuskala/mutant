#ifndef MUTANT_IMAGE_H_
#define MUTANT_IMAGE_H_

#include <SDL_image.h>
#include "color.h"

/* Our default format, try to convert everything to it */
#define MUTANT_SDL_FORMAT SDL_PIXELFORMAT_RGBA8888

typedef struct IMAGE {
    int w, h, access;
    unsigned int iformat;
    unsigned int *buffer;
    SDL_PixelFormat *format;
    SDL_Texture *texture;
} IMAGE;

typedef SDL_Surface RawImage;

extern int init_image();
extern void quit_image();

extern RawImage *read_raw_image(char *);
extern int update_texture_image(IMAGE *);
extern void free_image(IMAGE *);

extern void draw_hline(IMAGE *, const int, const int, const int, const COLOR *);

#endif /* MUTANT_IMAGE_H_ */
