#include "main.h"

#include "image.h"
#include "display.h"
#include "triangles.h"
#include <stdlib.h>

static IMAGE *orig_img = NULL;
static DISPLAY *main_display = NULL;
static TRIANGLES *main_triangles = NULL;

static void
free_original_image()
{
    free_image(orig_img);
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
    if (argc < 3) {
        return 1;
    }

    if (init_display() || init_image()) {
        return 1;
    }
    atexit(quit_image);
    atexit(quit_display);

    orig_img = read_static_image(argv[1]);
    if (orig_img == NULL) {
        return 1;
    }
    atexit(free_original_image);

    main_display = create_display(width_image(orig_img),
                                  height_image(orig_img));
    if (main_display == NULL) {
        return 1;
    }
    atexit(free_main_display);

    clear_display(main_display);

    if (render_image(main_display, orig_img, RECT_ORIG)) {
        return 1;
    }
    refresh_display(main_display);

    delay(atoi(argv[2]));

    /* Keep things consistient across runs for the time being */
    /* srand(time(NULL)); */
    srand(1);

    main_triangles = random_triangles(MAX_TRIANGLES,
                                      width_image(orig_img),
                                      height_image(orig_img));
    if (main_triangles == NULL) {
        return 1;
    }
    atexit(free_main_triangles);

    /* triangles = init_triangles(); */

    /* while (!quit()) { */
    /*     mutate_triangles(triangles); */
    /*     rasterize_triangles(triangles); */
    /*     render_image(triangles); */
    /*     refresh_display(main_display); */
    /* } */

    return 0;
}
