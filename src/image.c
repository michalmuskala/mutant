#include "image.h"

int
init_image()
{
    const int flags = IMG_INIT_PNG;
    int result = 0;

    result = IMG_Init(flags);
    if ((result & flags) != flags) {
        fprintf(stderr, "Failed to init image: %s", IMG_GetError());
        return -1;
    }

    return 0;
}

void
quit_image()
{
    IMG_Quit();
}

IMAGE *
read_static_image(char *file)
{
    IMAGE *image = NULL;
    SDL_Surface *read = NULL;

    /* Set to zero to have NULL pointers */
    image = calloc(1, sizeof(IMAGE));

    if (image == NULL) {
        perror("Reading image");
        return NULL;
    }

    read = IMG_Load(file);

    if (read == NULL) {
        fprintf(stderr, "Reading image: %s\n", IMG_GetError());
        free_image(image);
        return NULL;
    }

    image->surface = SDL_ConvertSurfaceFormat(read, MUTANT_SDL_FORMAT, 0);

    if (image->surface == NULL) {
        fprintf(stderr, "Reading image: %s", IMG_GetError());
        free_image(image);
        SDL_FreeSurface(read);
        return NULL;
    }

    SDL_FreeSurface(read);

    return image;
}

void
free_image(IMAGE *image)
{
    if (image == NULL) {
        return;
    }

    if (image->surface != NULL) {
        SDL_FreeSurface(image->surface);
    }

    free(image);
}

SDL_Surface *
surface_from_image(const IMAGE *image)
{
    return image->surface;
}

int
width_image(IMAGE *image)
{
    return image->surface->w;
}

int
height_image(IMAGE *image)
{
    return image->surface->h;
}

void
lock_image(IMAGE *image)
{
    if (SDL_MUSTLOCK(image->surface) != SDL_TRUE) {
        return;
    }

    if (SDL_LockSurface(image->surface)) {
        fprintf(stderr, "Locking image: %s", SDL_GetError());
    }
}

void
unlock_image(IMAGE *image)
{
    SDL_UnlockSurface(image->surface);
}

void
draw_hline(IMAGE *image, const int y, const int x1, const int x2,
           const COLOR color)
{
    byte r = 0, g = 0, b = 0, a = 0;
    byte ar = 0, ag = 0, ab = 0;
    double alpha = 0, invalpha = 0, alpha_src = 0;
    unsigned int *pixel = NULL, *pixel_end = NULL;
    SDL_Surface *s = NULL;

    alpha = (double) color.a / COLOR_MAX;
    invalpha = 1 - alpha;

    ar = color.r * alpha;
    ag = color.g * alpha;
    ab = color.b * alpha;

    s = image->surface;
    pixel = ((unsigned int *) s->pixels) + s->w * y + x1;
    pixel_end = pixel + (x2 - x1);

    for (; pixel <= pixel_end; pixel++) {
        SDL_GetRGBA(*pixel, s->format, &r, &g, &b, &a);

        alpha_src = (double) a / COLOR_MAX;

        r = (ar + r * alpha_src * invalpha) / (alpha_src);
        g = (ag + g * alpha_src * invalpha) / (alpha_src);
        b = (ab + b * alpha_src * invalpha) / (alpha_src);

        a = (alpha + alpha_src * invalpha) * COLOR_MAX;

        *pixel = SDL_MapRGBA(s->format, r, g, b, a);
    }
}
