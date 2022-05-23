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

void SDL_PlayAudio(const char *source)
{
    SDL_AudioSpec wav_spec;
    Uint32 wav_length;
    Uint8 *wav_buffer;

    if (SDL_LoadWAV(source, &wav_spec, &wav_buffer, &wav_length) == NULL)
    {
        fprintf(stderr, "Could not open test.wav: %s\n", SDL_GetError());
    }
    else
    {
        SDL_AudioDeviceID dev = SDL_OpenAudioDevice(NULL, 0, &wav_spec, NULL, 0);
        if (SDL_QueueAudio(dev, wav_buffer, wav_length) < 0)
        {
            fprintf(stderr, "Could not open test.wav: %s\n", SDL_GetError());
        }
        SDL_PauseAudioDevice(dev, 0);
        SDL_FreeWAV(wav_buffer);
    }
}

SDL_Rect *SDL_RenderGetViewportEx(SDL_Renderer *renderer, SDL_Rect *rect)
{
    SDL_RenderGetViewport(renderer, rect);

    return rect;
}
