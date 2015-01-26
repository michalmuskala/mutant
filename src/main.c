#include "main.h"

#include "image.h"
#include "display.h"
#include "triangle_set.h"
#include "options.h"
#include "events.h"
#include "annealing.h"
#include <stdlib.h>
#include <time.h>

static Image *original = NULL;

static void
free_original(void)
{
    free_image(original);
}

static Annealing *state = NULL;

static void
free_state(void)
{
    free_annealing(state);
}

int
main(int argc, char **argv)
{
    int w = 0, h = 0;

    if (parse_options(argc, argv) == NULL) {
        return -1;
    }

    if (init_image()) {
        return -1;
    }

    if (get_image_dimensions(options->image, &w, &h)) {
        return -1;
    }

    if (init_display(w, h)) {
        return -1;
    }

    original = init_static_image(options->image);
    if (original == NULL) {
        return -1;
    }
    atexit(free_original);

    if (render_image(original, RECT_ORIG)) {
        return -1;
    }

    state = init_annealing(original);
    if (state == NULL) {
        return -1;
    }
    atexit(free_state);

    if (options->noseed) {
        srand(0);
    }
    else {
        srand(time(NULL));
    }

    while (check_events()) {
        /* Render only evenry 10th generation */
        if (state->generation % 10 == 0) {
            if ((update_texture_image(state->image)) ||
                (render_image(state->image, RECT_WORK))) {
                break;
            }
        }

        step_annealing(state);
    }

    return 0;
}
