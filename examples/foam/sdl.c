#include <SDL2/SDL.h>

#include <stdio.h>

struct Event
{
    SDL_KeyboardEvent *key;
    SDL_MouseMotionEvent *motion;
};

struct Event *SDL_WaitEventEx(struct Event *events)
{
    SDL_Event event;

    SDL_WaitEvent(&event);

    memcpy(events->key, &event.key, sizeof(SDL_KeyboardEvent));
    memcpy(events->motion, &event.motion, sizeof(SDL_MouseMotionEvent));

    return events;
}

SDL_MouseMotionEvent *SDL_PollMouseMotionEvent(SDL_MouseMotionEvent *motion)
{
    SDL_Event event;

    if (SDL_PeepEvents(&event, 1, SDL_GETEVENT, SDL_MOUSEMOTION, SDL_MOUSEMOTION))
    {
        memcpy(motion, &event.motion, sizeof(SDL_MouseMotionEvent));
    }

    return motion;
}

void printnum(int num)
{
    printf("%d\n", num);
}
