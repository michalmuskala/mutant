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

    state->image = init_dynamic_image(original->w, original->h);
    state->current.triangles = init_triangles(original->w, original->h);
    state->best.triangles = init_triangles(original->w, original->h);

    if ((state->image == NULL) ||
        (state->current.triangles == NULL) ||
        (state->best.triangles == NULL)) {
        free_annealing(state);
        return NULL;
    }

    state->best.rate = 0;
    state->current.rate = 0;
    state->original = original;
    state->generation = 0;
    state->temperature = 1;

    return state;
}

static int
is_acceptable(const double best, const double current, const double temp)
{
    double rate = 0;

    /*
     * Optimize by exiting early if new is better then old.
     * Calculation would do just that, but why bother with exp and rand
     */
    if (current < best) {
        return 1;
    }

    rate = exp((current - best) / temp);

    printf("Switch chance: %f\n", rate);

    return rate > (((double) rand()) / RAND_MAX);
}

void
step_annealing(Annealing *state)
{
    mutate_triangles(state->current.triangles);
    rasterize_triangles(state->current.triangles, state->image);
    state->current.rate = rate_image(state->original, state->image);

    state->generation++;
    if (state->temperature > 0) {
        state->temperature -= options->temp_step;
    }

    printf("Generation #%03ld (T: %f) %g \n",
           state->generation, state->temperature, state->current.rate);

    if (is_acceptable(state->best.rate, state->current.rate,
                      state->temperature)) {
        /* Make current triangles our best */
        /* printf("New! %g replaces %g\n", state->current.) */
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
