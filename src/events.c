#include "events.h"

#include <SDL.h>

int
check_events(void)
{
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            return 0;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_q:
            case SDLK_ESCAPE:
                return 0;
            default:
                break;
            }
        default:
            break;
        }
    }

    return 1;
}
