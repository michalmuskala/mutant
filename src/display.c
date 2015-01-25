#include "display.h"

#include <stdio.h>
#include <stdlib.h>
#include "options.h"

static Display *display = NULL;

static void
clear_display(void)
{
    if (display == NULL) {
        return;
    }

    SDL_SetRenderDrawColor(display->renderer,
                           BACKGROUND_R, BACKGROUND_G,
                           BACKGROUND_B, BACKGROUND_A);
    SDL_RenderClear(display->renderer);
}

static void
free_display(void)
{
    if (display == NULL) {
        return;
    }

    if (display->window != NULL) {
        SDL_DestroyWindow(display->window);
    }

    if (display->renderer != NULL) {
        SDL_DestroyRenderer(display->renderer);
    }

    if (display->orig_rect != NULL) {
        free(display->orig_rect);
    }

    if (display->work_rect != NULL) {
        free(display->work_rect);
    }

    if (display->orig_name_rect != NULL) {
        free(display->orig_name_rect);
    }

    if (display->work_name_rect != NULL) {
        free(display->work_name_rect);
    }

    if (display->stats_rect != NULL) {
        free(display->stats_rect);
    }

    free(display);

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

static Display *
create_display(int w, int h)
{
    Display *display = NULL;

    /* We want null pointers */
    display = calloc(1, sizeof(*display));

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
        free_display();
        return NULL;
    }

    display->width = 2 * w + 2 * BORDER_WIDTH + STATS_WIDTH;
    display->height = h + IMAGE_NAME_HEIGHT + BORDER_WIDTH;

    set_rect(display->orig_rect,
             0, IMAGE_NAME_HEIGHT + 2 * BORDER_WIDTH,
             w, h);
    set_rect(display->work_rect,
             w + BORDER_WIDTH, IMAGE_NAME_HEIGHT + 2 * BORDER_WIDTH,
             w, h);
    set_rect(display->orig_name_rect,
             BORDER_WIDTH, BORDER_WIDTH,
             w - 2 * BORDER_WIDTH, IMAGE_NAME_HEIGHT);
    set_rect(display->work_name_rect,
             w + 3 * BORDER_WIDTH, BORDER_WIDTH,
             w - 2 * BORDER_WIDTH, IMAGE_NAME_HEIGHT);
    set_rect(display->stats_rect,
             w * 2 + BORDER_WIDTH * 2, BORDER_WIDTH,
             STATS_WIDTH, display->height - 2 * BORDER_WIDTH);

    if (SDL_CreateWindowAndRenderer(display->width, display->height,
                                    SDL_WINDOW_SHOWN,
                                    &display->window, &display->renderer)) {
        fprintf(stderr, "Initializing display: %s\n", SDL_GetError());
        free_display();
        return NULL;
    }

    return display;
}

int
init_display(int w, int h)
{
    if (SDL_Init(SDL_INIT_VIDEO) == -1) {
        fprintf(stderr, "Initializing display: %s\n", SDL_GetError());
        return -1;
    }

    display = create_display(w, h);

    if (display == NULL) {
        return -1;
    }

    clear_display();

    atexit(free_display);

    return 0;
}

static SDL_Rect *
rect_from_area(const Display *display, const DisplayArea area)
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

Image *
init_static_image(char *name)
{
    SDL_Surface *surface = NULL, *raw = NULL;
    Image *i = NULL;
    int row = 0;
    unsigned int *dst = NULL, *src = NULL;

    raw = IMG_Load(name);

    if (raw == NULL) {
        fprintf(stderr, "Reading image: %s\n", IMG_GetError());
        return NULL;
    }

    surface = SDL_ConvertSurfaceFormat(raw, MUTANT_SDL_FORMAT, 0);

    if (surface == NULL) {
        fprintf(stderr, "Reading image: %s\n", SDL_GetError());
        SDL_FreeSurface(raw);
        return NULL;
    }

    SDL_FreeSurface(raw);

    i = malloc(sizeof(*i));

    if (i == NULL) {
        perror("Reading image");
        SDL_FreeSurface(surface);
        return NULL;
    }

    i->texture = SDL_CreateTextureFromSurface(display->renderer, surface);

    if (i->texture == NULL) {
        fprintf(stderr, "Reading image: %s\n", SDL_GetError());
        free_image(i);
        SDL_FreeSurface(surface);
        return NULL;
    }

    if (SDL_QueryTexture(i->texture, &i->iformat, &i->access, &i->w, &i->h)) {
        fprintf(stderr, "Reading image: %s\n", SDL_GetError());
        free_image(i);
        SDL_FreeSurface(surface);
        return NULL;
    }

    i->format = SDL_AllocFormat(i->iformat);

    if (i->format == NULL) {
        fprintf(stderr, "Initializing image: %s\n", SDL_GetError());
        free_image(i);
        SDL_FreeSurface(surface);
        return NULL;
    }

    i->buffer = malloc(sizeof(*i->buffer) * i->w * i->h);

    if (i->buffer == NULL) {
        perror("Initializing image");
        free_image(i);
        SDL_FreeSurface(surface);
        return NULL;
    }

    if (SDL_MUSTLOCK(surface)) {
        SDL_LockSurface(surface);
    }

    src = raw->pixels;
    dst = i->buffer;

    for (row = 0; row < raw->h; row++) {
        memcpy(dst, src, raw->w * sizeof(*src));
        dst += raw->w;
        src += raw->pitch / sizeof(*src);
    }

    SDL_UnlockSurface(surface);
    SDL_FreeSurface(surface);

    return i;
}

Image *
init_dynamic_image(const int w, const int h)
{
    Image *i = NULL;
    int buffer_len = 0;

    /* Set to zero to have NULL pointers */
    i = calloc(1, sizeof(*i));

    if (i == NULL) {
        perror("Reading image");
        return NULL;
    }

    i->w = w;
    i->h = h;
    i->iformat = MUTANT_SDL_FORMAT;
    i->access = SDL_TEXTUREACCESS_STREAMING;
    i->texture = SDL_CreateTexture(display->renderer, i->iformat,
                                   i->access, i->w, i->h);

    if (i->texture == NULL) {
        fprintf(stderr, "Initializing image: %s\n", IMG_GetError());
        free_image(i);
        return NULL;
    }

    i->format = SDL_AllocFormat(i->iformat);

    if (i->format == NULL) {
        fprintf(stderr, "Initializing image: %s\n", IMG_GetError());
        free_image(i);
        return NULL;
    }

    buffer_len = sizeof(*i->buffer) * w * h;
    i->buffer = malloc(buffer_len);

    if (i->buffer == NULL) {
        perror("Initializing image");
        free_image(i);
        return NULL;
    }

    clear_image(i);

    return i;
}

int
render_image(const Image *image, const DisplayArea area)
{
    SDL_Rect *rect = NULL;

    if (display == NULL || image == NULL) {
        return -1;
    }

    rect = rect_from_area(display, area);

    if (SDL_RenderCopy(display->renderer, image->texture, NULL, rect)) {
        fprintf(stderr, "Rendering image: %s\n", SDL_GetError());
        return -1;
    }

    SDL_RenderPresent(display->renderer);

    return 0;
}
