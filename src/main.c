#include "main.h"

#include "image.h"
#include "display.h"
#include "triangles.h"
#include "options.h"
#include <stdlib.h>
#include <time.h>

static IMAGE *orig_img = NULL;
static IMAGE *img = NULL;
static DISPLAY *main_display = NULL;
static TRIANGLES *main_triangles = NULL;

static void
free_orig_img()
{
    free_image(orig_img);
}

static void
free_img()
{
    free_image(img);
}

static void
free_main_display()
{
    free_display(main_display);
}

static void
free_main_triangles()
{
    free_triangles(main_triangles);
}

int
main(int argc, char **argv)
{
    RawImage *raw_image = NULL;
    int i = 0;

    if (parse_options(argc, argv) == NULL) {
        return -1;
    }
    atexit(free_options);

    if (init_display() || init_image()) {
        return -1;
    }
    atexit(quit_image);
    atexit(quit_display);

    raw_image = read_raw_image(options->image);
    if (raw_image == NULL) {
        return -1;
    }

    main_display = create_display(raw_image->w, raw_image->h);
    if (main_display == NULL) {
        return -1;
    }
    atexit(free_main_display);

    orig_img = convert_image(main_display, raw_image);
    if (orig_img == NULL) {
        return -1;
    }
    atexit(free_orig_img);

    clear_display(main_display);

    if (render_image(main_display, orig_img, RECT_ORIG)) {
        return -1;
    }
    refresh_display(main_display);

    /* Keep things consistient across runs for the time being */
    /* srand(time(NULL)); */
    srand(1);

    main_triangles = init_triangles(options->max_triangles,
                                    orig_img->w, orig_img->h);
    if (main_triangles == NULL) {
        return -1;
    }
    atexit(free_main_triangles);

    img = init_dynamic_image(main_display, orig_img->w, orig_img->h);
    if (img == NULL) {
        return -1;
    }
    atexit(free_img);

    if (update_texture_image(img)) {
        return -1;
    }

    if (render_image(main_display, img, RECT_WORK)) {
        return -1;
    }

    refresh_display(main_display);

    rasterize_triangles(main_triangles, img);

    if (update_texture_image(img)) {
        return -1;
    }

    if (render_image(main_display, img, RECT_WORK)) {
        return -1;
    }

    refresh_display(main_display);

    fprintf(stderr, "Difference between images: %g\n",
            rate_image(orig_img, img));

    delay(options->delay);

    for (i = 0; i < 1; i++) {
        mutate_triangles(main_triangles);

        rasterize_triangles(main_triangles, img);

        if (update_texture_image(img)) {
            return -1;
        }

        if (render_image(main_display, img, RECT_WORK)) {
            return -1;
        }

        refresh_display(main_display);

        fprintf(stderr, "Difference between images: %g\n",
                rate_image(orig_img, img));
    }

    return 0;
}
