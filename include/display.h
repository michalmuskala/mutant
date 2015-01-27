#ifndef MUTANT_DISPLAY_H_
#define MUTANT_DISPLAY_H_

#include <SDL.h>
#include "image.h"

#define BORDER_WIDTH 2
#define BACKGROUND_R 255
#define BACKGROUND_G 255
#define BACKGROUND_B 255
#define BACKGROUND_A 255

typedef struct {
    int width, height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect *orig_rect, *work_rect;
} Display;

typedef enum {
    RECT_ORIG,
    RECT_WORK
} DisplayArea;

extern int init_display(const int, const int);

extern Image *init_static_image(const char *);
extern Image *init_dynamic_image(const int, const int);
extern int render_image(const Image *, const DisplayArea);

#endif /* MUTANT_DISPLAY_H_ */
