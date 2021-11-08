#include <SDL2/SDL.h>

#include <stdio.h>

int SDL_PollKeyEvent(SDL_KeyboardEvent *key)
{
    SDL_Event event;

    if (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_KEYDOWN, SDL_KEYMAPCHANGED))
    {
        memcpy(key, &event.key, sizeof(SDL_KeyboardEvent));

        return 1;
    }

    return 0;
}

void printnum(int num)
{
    printf("%d\n", num);
}
