#ifndef MUTANT_DISPLAY_H_
#define MUTANT_DISPLAY_H_

#include <SDL.h>
#include "image.h"

#define MUTANT_SDL_FORMAT SDL_PIXELFORMAT_RGBA8888
#define MUTANT_FULL_DEPTH 32

#define WINDOW_NAME "Mutant - image evolution"
#define IMAGE_NAME_HEIGHT 16
#define BORDER_WIDTH 2
#define BACKGROUND_R 255
#define BACKGROUND_G 255
#define BACKGROUND_B 255
#define BACKGROUND_A 255
#define TEXT_R 0
#define TEXT_G 0
#define TEXT_B 0
#define TEXT_A 255
#define STATS_WIDTH 200

#define delay SDL_Delay

typedef struct DISPLAY {
    int width, height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect *orig_rect, *work_rect;
    SDL_Rect *orig_name_rect, *work_name_rect;
    SDL_Rect *stats_rect;
} DISPLAY;

typedef enum {
    RECT_ALL,
    RECT_ORIG,
    RECT_WORK,
    RECT_ORIG_NAME,
    RECT_WORK_NAME,
    RECT_STATS
} DISPLAY_AREA;

extern int init_display();
extern void quit_display();

extern DISPLAY *create_display(int, int);
extern void free_display(DISPLAY *);
extern void refresh_display(DISPLAY *);
extern void clear_display(DISPLAY *);

extern int render_image(DISPLAY *, const IMAGE *, const DISPLAY_AREA);
extern int write_on_rect(DISPLAY *, const DISPLAY_AREA, const char *);

#endif /* MUTANT_DISPLAY_H_ */
