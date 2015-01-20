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

RawImage *
read_raw_image(char *file)
{
    RawImage *read = NULL;

    read = IMG_Load(file);

    if (read == NULL) {
        fprintf(stderr, "Reading image: %s\n", IMG_GetError());
        return NULL;
    }

    return read;
}

int
update_texture_image(IMAGE *i)
{
    unsigned int *dst = NULL, *src = NULL;
    int pitch = 0;
    int row = 0;

    if (SDL_LockTexture(i->texture, NULL, (void **) &dst, &pitch)) {
        fprintf(stderr, "Updating texture: %s\n", SDL_GetError());
        return -1;
    }

    src = i->buffer;

    for (row = 0; row < i->h; row++) {
        memcpy(dst, src, i->w * sizeof(*dst));
        dst += pitch / sizeof(*dst);
        src += i->w;
    }

    SDL_UnlockTexture(i->texture);

    return 0;
}

void
free_image(IMAGE *image)
{
    if (image == NULL) {
        return;
    }

    if (image->texture != NULL) {
        SDL_DestroyTexture(image->texture);
    }

    if (image->buffer != NULL) {
        free(image->buffer);
    }

    if (image->format != NULL) {
        SDL_FreeFormat(image->format);
    }

    free(image);
}


double
rate_image(const IMAGE *original, const IMAGE *modified)
{
    unsigned int *org_pixel = NULL, *org_pixel_end = NULL;
    unsigned int *mod_pixel = NULL;
    COLOR orgc = {0}, modc = {0};
    double rate = 0;

    org_pixel_end = original->buffer + original->w * original->h;

    for (org_pixel = original->buffer, mod_pixel = modified->buffer;
         org_pixel <= org_pixel_end;
         mod_pixel++, org_pixel++) {
        SDL_GetRGB(*org_pixel, original->format, &orgc.r, &orgc.g, &orgc.b);
        SDL_GetRGB(*mod_pixel, modified->format, &modc.r, &modc.g, &modc.b);

        rate += color_distance(orgc, modc);
    }

    return rate;
}

void
draw_hline(IMAGE *image, const int y, const int x1, const int x2,
           const COLOR *color)
{
    byte r = 0, g = 0, b = 0, a = 0;
    byte ar = 0, ag = 0, ab = 0;
    double alpha = 0, invalpha = 0, alpha_src = 0;
    unsigned int *pixel = NULL, *pixel_end = NULL;

    alpha = (double) color->a / COLOR_MAX;
    invalpha = 1 - alpha;

    ar = color->r * alpha;
    ag = color->g * alpha;
    ab = color->b * alpha;

    pixel = image->buffer + image->w * y + x1;
    pixel_end = pixel + (x2 - x1);

    for (; pixel <= pixel_end; pixel++) {
        SDL_GetRGBA(*pixel, image->format, &r, &g, &b, &a);

        alpha_src = (double) a / COLOR_MAX;

        r = (ar + r * alpha_src * invalpha) / (alpha_src);
        g = (ag + g * alpha_src * invalpha) / (alpha_src);
        b = (ab + b * alpha_src * invalpha) / (alpha_src);

        a = (alpha + alpha_src * invalpha) * COLOR_MAX;

        *pixel = SDL_MapRGBA(image->format, r, g, b, a);
    }
}
