#include <SDL2/SDL.h>

#include <stdio.h>

struct Event
{
    SDL_KeyboardEvent *key;
    SDL_MouseMotionEvent *motion;
    SDL_MouseButtonEvent *button;
};

struct Event *SDL_WaitEventEx(struct Event *events)
{
    SDL_Event event;

    SDL_WaitEvent(&event);

    memcpy(events->key, &event.key, sizeof(SDL_KeyboardEvent));
    memcpy(events->motion, &event.motion, sizeof(SDL_MouseMotionEvent));
    memcpy(events->button, &event.button, sizeof(SDL_MouseButtonEvent));

    return events;
}

void printnum(int num)
{
    printf("%d\n", num);
}
