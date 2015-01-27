#include "annealing.h"

#include <stdlib.h>
#include <math.h>
#include "options.h"

Annealing *
init_annealing(Image *original)
{
    Annealing *state = NULL;

    state = malloc(sizeof(*state));

    if (state == NULL) {
        perror("Initializing annealing");
        return NULL;
    }

    state->original = original;

    state->image = init_dynamic_image(original->w, original->h);
    state->current.triangles = init_triangles(original->w, original->h);
    state->best.triangles = init_triangles(original->w, original->h);

    if ((state->image == NULL) ||
        (state->current.triangles == NULL) ||
        (state->best.triangles == NULL)) {
        free_annealing(state);
        return NULL;
    }

    state->best.rate = rate_image(state->original, state->image);
    if (state->best.rate != state->best.rate) {
        exit(1);
    }
    state->current.rate = 0;
    state->generation = 0;
    state->temperature = 1;

    return state;
}

static int
is_acceptable(const double best, const double current, const double temp)
{
    if (current < best) {
        return 1;
    }
    else if (temp > 0) {
        return exp((current - best) / temp) < ((double) rand() / RAND_MAX);
    }
    else {
        return 0;
    }
}

void
step_annealing(Annealing *state)
{
    mutate_triangles(state->current.triangles);
    rasterize_triangles(state->current.triangles, state->image);
    state->current.rate = rate_image(state->original, state->image);

    state->generation++;
    state->temperature -= options->temp_step;
    if (state->temperature < 0) {
        state->temperature = 0;
    }

    printf("Generation #%03ld (T: %f) %g \n",
           state->generation, state->temperature, state->current.rate);

    if (is_acceptable(state->best.rate, state->current.rate,
                      state->temperature)) {
        /* Make current triangles our best */
        copy_triangles(state->best.triangles, state->current.triangles);
        state->best.rate = state->current.rate;
    }
    else {
        /* Restore best triangles as current for next pass */
        copy_triangles(state->current.triangles, state->best.triangles);
        state->current.rate = state->best.rate;
    }
}

void
free_annealing(Annealing *state)
{
    if (state == NULL) {
        return;
    }

    if (state->image != NULL) {
        free_image(state->image);
    }

    if (state->current.triangles != NULL) {
        free_triangles(state->current.triangles);
    }

    if (state->best.triangles != NULL) {
        free_triangles(state->best.triangles);
    }

    free(state);
}
