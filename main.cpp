#include <iostream>
#include <SDL/SDL.h>
#include <target.h>
#include <gs.h>
#include <glm/glm.hpp>
#include <coords.h>

extern const size_t FrameWidth = 1200;
extern const size_t FrameHeight = 640;

void tickSim(GuidanceSystem& gs, Target& target, float dt);

int main(int argc, char* argv[]) 
{
    const float tickPeriod = 5.0f;  // Time delay between simulation updates (ms).
    const float dt = tickPeriod / 1000.0f;  // Simulation timestep (s)

    // Setup SDL window and rendering context.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Missile Guidance System", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        FrameWidth, FrameHeight, SDL_WINDOW_SHOWN);
    if (!window) {
        SDL_Quit();
        return -1;
    }

    SDL_Renderer* r = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!r) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // Setup Guidance-system parameters.
    const glm::vec2 gsPosition(FrameWidth / 2.0f, FrameHeight); // SDL coordinate space.
    const float gsRange = 700.0f;
    const float gsFOV = 60.0f;
    GuidanceSystem gs(gsPosition, gsRange, gsFOV);

    // Setup target parameters.
    const glm::vec2 targetPos(250, 200); // SDL coordinate space.
    const float targetSize = 15.0f; // pixels.
    Target target(targetPos, targetSize);

    // Application Loop ...
    SDL_Event e;
    bool quit = false;
    bool allowTick = true;
    auto t0 = SDL_GetTicks();
    
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    
        // Update simulation.
        if (allowTick) { 
            tickSim(gs, target, dt);
            allowTick = false;
        }

        // Render the frame.
        SDL_SetRenderDrawColor(r, 255, 255, 255, 0xff);
        SDL_RenderClear(r);
        target.draw(r);
        gs.draw(r);
        SDL_RenderPresent(r);

        // Manage simulation update rate.
        const auto elapsedTime = SDL_GetTicks() - t0;
        if (elapsedTime >= tickPeriod) {
            t0 = SDL_GetTicks();
            allowTick = true;
        }
    }

    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
}

void tickSim(GuidanceSystem& gs, Target& target, float dt)
{
    target.tick(dt);
    gs.tick(target, dt);
}