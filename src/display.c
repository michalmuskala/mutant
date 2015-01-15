#include "display.h"

#include <stdio.h>
#include <stdlib.h>

int
init_display()
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        fprintf(stderr, "Initializing display: %s\n", SDL_GetError());
        return -1;
    }

    return 0;
}

void
quit_display()
{
    SDL_Quit();
}

static void
set_rect(SDL_Rect *rect, int x, int y, int w, int h)
{
    rect->x = x;
    rect->y = y;
    rect->w = w;
    rect->h = h;
}

DISPLAY *
create_display(int image_width, int image_height)
{
    DISPLAY *display = NULL;

    display = malloc(sizeof(DISPLAY));
    memset(display, 0, sizeof(DISPLAY));

    if (display == NULL) {
        perror("Initializing display");
        return NULL;
    }

    display->orig_rect = malloc(sizeof(SDL_Rect));
    display->work_rect = malloc(sizeof(SDL_Rect));
    display->orig_name_rect = malloc(sizeof(SDL_Rect));
    display->work_name_rect = malloc(sizeof(SDL_Rect));
    display->stats_rect = malloc(sizeof(SDL_Rect));

    if ((display->orig_rect == NULL) ||
        (display->work_rect == NULL) ||
        (display->orig_name_rect == NULL) ||
        (display->work_name_rect == NULL) ||
        (display->stats_rect == NULL)) {
        perror("Initializing display");
        free_display(display);
        return NULL;
    }

    display->width = 2 * image_width + 2 * BORDER_WIDTH + STATS_WIDTH;
    display->height = image_height + IMAGE_NAME_HEIGHT + BORDER_WIDTH;

    set_rect(display->orig_rect,
             0, IMAGE_NAME_HEIGHT + 2 * BORDER_WIDTH,
             image_width, image_height);
    set_rect(display->work_rect,
             image_width + BORDER_WIDTH, IMAGE_NAME_HEIGHT + 2 * BORDER_WIDTH,
             image_width, image_height);
    set_rect(display->orig_name_rect,
             BORDER_WIDTH, BORDER_WIDTH,
             image_width - 2 * BORDER_WIDTH, IMAGE_NAME_HEIGHT);
    set_rect(display->work_name_rect,
             image_width + 3 * BORDER_WIDTH, BORDER_WIDTH,
             image_width - 2 * BORDER_WIDTH, IMAGE_NAME_HEIGHT);
    set_rect(display->stats_rect,
             image_width * 2 + BORDER_WIDTH * 2, BORDER_WIDTH,
             STATS_WIDTH, display->height - 2 * BORDER_WIDTH);

    if (SDL_CreateWindowAndRenderer(display->width, display->height,
                                    SDL_WINDOW_SHOWN,
                                    &display->window, &display->renderer)) {
        fprintf(stderr, "Initializing display: %s\n", SDL_GetError());
        free_display(display);
        return NULL;
    }

    if (display->window == NULL || display->renderer == NULL) {
        fprintf(stderr, "Initializing display: %s\n", SDL_GetError());
        free_display(display);
        return NULL;
    }

    return display;
}

void
free_display(DISPLAY *display)
{
    if (display == NULL) {
        SDL_Quit();
        return;
    }

    if (display->window != NULL) {
        SDL_DestroyWindow(display->window);
    }

    if (display->renderer != NULL) {
        SDL_DestroyRenderer(display->renderer);
    }

    free(display->orig_rect);
    free(display->work_rect);
    free(display->orig_name_rect);
    free(display->work_name_rect);
    free(display->stats_rect);
    free(display);
}

void
refresh_display(DISPLAY *display)
{
    SDL_RenderPresent(display->renderer);
}

static SDL_Rect *
rect_from_area(const DISPLAY *display, const DISPLAY_AREA area)
{
    switch (area) {
    case RECT_ALL:       return NULL;
    case RECT_ORIG:      return display->orig_rect;
    case RECT_WORK:      return display->work_rect;
    case RECT_ORIG_NAME: return display->orig_name_rect;
    case RECT_WORK_NAME: return display->work_name_rect;
    case RECT_STATS:     return display->stats_rect;
    default:             return NULL;
    }
}

int
render_image(DISPLAY *display, const IMAGE *image, const DISPLAY_AREA area)
{
    SDL_Texture *texture = NULL;
    SDL_Rect *rect = NULL;
    SDL_Surface *surface = NULL;

    if (display == NULL || image == NULL) {
        return -1;
    }

    surface = surface_from_image(image);

    texture = SDL_CreateTextureFromSurface(display->renderer, surface);

    if (texture == NULL) {
        fprintf(stderr, "Rendering image: %s\n", SDL_GetError());
        return -1;
    }

    rect = rect_from_area(display, area);

    if (SDL_RenderCopy(display->renderer, texture, NULL, rect)) {
        fprintf(stderr, "Rendering image: %s\n", SDL_GetError());
        SDL_DestroyTexture(texture);
        return -1;
    }

    SDL_DestroyTexture(texture);
    return 0;
}

void
clear_display(DISPLAY *display)
{
    if (display == NULL) {
        return;
    }

    SDL_SetRenderDrawColor(display->renderer,
                           BACKGROUND_R, BACKGROUND_G,
                           BACKGROUND_B, BACKGROUND_A);
    SDL_RenderClear(display->renderer);
}

/* int */
/* write_on_rect(DISPLAY *display, const DISPLAY_AREA area, const char *str) */
/* { */
/*     /\* SDL_Rect *rect = NULL; *\/ */
/*     /\* int res; *\/ */

/*     /\* rect = rect_from_area(display, area); *\/ */

/*     /\* if (rect == NULL) { *\/ */
/*     /\*     return -1; *\/ */
/*     /\* } *\/ */

/*     /\* res = stringRGBA(display->renderer, *\/ */
/*     /\*                   rect->x, rect->y, str, TEXT_R, TEXT_G, TEXT_B, TEXT_A); *\/ */

/*     /\* if (res) { *\/ */
/*     /\*     fprintf(stderr, "Writing: %s\n", SDL_GetError()); *\/ */
/*     /\* } *\/ */

/*     /\* return res; *\/ */
/*     return 0; */
/* } */
