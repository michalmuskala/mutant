#ifndef MUTANT_DISPLAY_H_
#define MUTANT_DISPLAY_H_

#include <SDL.h>
#include "image.h"

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

typedef struct {
    int width, height;
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Rect *orig_rect, *work_rect;
    SDL_Rect *orig_name_rect, *work_name_rect;
    SDL_Rect *stats_rect;
} Display;

typedef enum {
    RECT_ALL,
    RECT_ORIG,
    RECT_WORK,
    RECT_ORIG_NAME,
    RECT_WORK_NAME,
    RECT_STATS
} DisplayArea;

extern int init_display();
extern void quit_display();

extern Display *create_display(int, int);
extern void free_display(Display *);
extern void refresh_display(Display *);
extern void clear_display(Display *);

extern Image *convert_image(Display *, RawImage *);
extern Image * init_dynamic_image(Display *, const int, const int);
extern int render_image(Display *, const Image *, const DisplayArea);
extern int write_on_rect(Display *, const DisplayArea, const char *);

#endif /* MUTANT_DISPLAY_H_ */
