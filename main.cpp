#include <iostream>
#include <SDL/SDL.h>
#include <target.h>
#include <gs.h>
#include <glm/glm.hpp>
#include <coords.h>

glm::vec2 SDLtoGS(glm::vec2 sdl) {
    const auto GSx = sdl.x - FrameWidth * 0.5f;
    const auto GSy = FrameHeight - sdl.y;
    return glm::vec2(GSx, GSy);
}

glm::vec2 GStoSDL(glm::vec2 gs) {
    const auto SDLx = gs.x + FrameWidth * 0.5f;
    const auto SDLy = FrameHeight - gs.y;
    return glm::vec2(SDLx, SDLy);
}

int main(int argc, char* argv[]) 
{
    const uint16_t FramebufferWidth = 1200;
    const uint16_t FramebufferHeight= 640;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Missile Guidance System", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        FramebufferWidth, FramebufferHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* r = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!r) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    GuidanceSystem gs(0.5f * FramebufferWidth, FramebufferHeight, 800.0f, 90);

    Target target(glm::vec2(250, 200), 25);

    SDL_Event e;
    bool quit = false;
    bool tick_simulation = true;
    auto t0 = SDL_GetTicks();
    
    const auto msDeltaTime = 5.0f;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    
        if (tick_simulation) {
            const auto dt = msDeltaTime / 1000.0f; // time-step in seconds.
            
            target.tick(dt);
            gs.tick(target, dt);

            tick_simulation = false;
        }

        // Rendering ...
        SDL_SetRenderDrawColor(r, 255, 255, 255, 0xff);
        SDL_RenderClear(r);
        target.draw(r);
        gs.draw(r);

        SDL_RenderPresent(r);

        const auto dt = SDL_GetTicks() - t0;
        if (dt >= msDeltaTime) {
            t0 = SDL_GetTicks();
            tick_simulation = true;
        }
    }

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
}