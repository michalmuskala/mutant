#ifndef MUTANT_IMAGE_H_
#define MUTANT_IMAGE_H_

#include <SDL_image.h>
#include "color.h"

extern int init_image();
extern void quit_image();

typedef struct IMAGE {
    SDL_Surface *surface;
    SDL_Texture *texture;
} IMAGE;

extern IMAGE *read_static_image(char *);
extern void free_image(IMAGE *);
extern SDL_Surface *surface_from_image(const IMAGE *);
extern void lock_image(IMAGE *);
extern void unlock_image(IMAGE *);

extern int width_image(IMAGE *);
extern int height_image(IMAGE *);

extern void draw_hline(IMAGE *, const int, const int, const int, const COLOR);

#endif /* MUTANT_IMAGE_H_ */
