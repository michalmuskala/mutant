#include "main.h"

#include "image.h"
#include "display.h"
#include "triangles.h"
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

#define MAX_TRIANGLES 60

int
main(int argc, char **argv)
{
    RawImage *raw_image = NULL;

    if (argc < 3) {
        return -1;
    }

    if (init_display() || init_image()) {
        return -1;
    }
    atexit(quit_image);
    atexit(quit_display);

    raw_image = read_raw_image(argv[1]);
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
    srand(time(NULL)/* time(NULL) */);

    main_triangles = random_triangles(MAX_TRIANGLES,
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

    delay(atoi(argv[2]));


    /* while (!quit()) { */
    /*     mutate_triangles(triangles); */
    /*     rasterize_triangles(triangles); */
    /*     render_image(triangles); */
    /*     refresh_display(main_display); */
    /* } */

    return 0;
}
