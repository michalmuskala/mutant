#ifndef MUTANT_IMAGE_H_
#define MUTANT_IMAGE_H_

#include <SDL_image.h>
#include "color.h"

/* Our default format, try to convert everything to it */
#define MUTANT_SDL_FORMAT SDL_PIXELFORMAT_RGBA8888

typedef struct {
    int w, h, access;
    unsigned int iformat;
    unsigned int *buffer;
    SDL_PixelFormat *format;
    SDL_Texture *texture;
} Image;

extern int init_image();
extern void quit_image();

extern int get_image_dimensions(char *, int *, int *);
extern int update_texture_image(Image *);
extern void free_image(Image *);
extern double rate_image(const Image *, const Image *);
extern void clear_image(Image *);

extern void draw_hline(Image *, const int, const int, const int, const Color *);

#endif /* MUTANT_IMAGE_H_ */
