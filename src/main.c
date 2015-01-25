#include "main.h"

#include "image.h"
#include "display.h"
#include "triangle_set.h"
#include "options.h"
#include "events.h"
#include <stdlib.h>
#include <time.h>

static Image *orig_img = NULL;
static Image *img = NULL;
static TriangleSet *main_triangles = NULL;

static void
free_orig_img(void)
{
    free_image(orig_img);
}

static void
free_img(void)
{
    free_image(img);
}

static void
free_main_triangles(void)
{
    free_triangles(main_triangles);
}

int
main(int argc, char **argv)
{
    int w, h;

    if (parse_options(argc, argv) == NULL) {
        return -1;
    }

    if (init_image()) {
        return -1;
    }

    /* init_display relies on dimensions being available */
    if (get_image_dimensions(options->image, &w, &h)) {
        return -1;
    }

    if (init_display(w, h)) {
        return -1;
    }

    orig_img = init_static_image(options->image);
    if (orig_img == NULL) {
        return -1;
    }
    atexit(free_orig_img);

    if (render_image(orig_img, RECT_ORIG)) {
        return -1;
    }

    /* Keep things consistient across runs for the time being */
    /* srand(time(NULL)); */
    srand(1);

    main_triangles = init_triangles(options->max_triangles,
                                    orig_img->w, orig_img->h);
    if (main_triangles == NULL) {
        return -1;
    }
    atexit(free_main_triangles);

    img = init_dynamic_image(orig_img->w, orig_img->h);
    if (img == NULL) {
        return -1;
    }
    atexit(free_img);

    rasterize_triangles(main_triangles, img);

    if (update_texture_image(img)) {
        return -1;
    }

    if (render_image(img, RECT_WORK)) {
        return -1;
    }

    fprintf(stderr, "Difference between images: %g\n",
            rate_image(orig_img, img));

    delay(options->delay);

    while (check_events()) {
        mutate_triangles(main_triangles);

        rasterize_triangles(main_triangles, img);

        if (update_texture_image(img)) {
            return -1;
        }

        if (render_image(img, RECT_WORK)) {
            return -1;
        }

        fprintf(stderr, "Difference between images: %g\n",
                rate_image(orig_img, img));
    }

    return 0;
}
