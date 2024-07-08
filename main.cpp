#include <iostream>
#include <SDL/SDL.h>
#include <target.h>
#include <gs.h>
#include <vec2.h>

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

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    const float GS_Size = 25.0f;
    GuidanceSystem gs(0.5f * (FramebufferWidth - GS_Size), FramebufferHeight - GS_Size - 2, 
        GS_Size, 600.0f, 60);
    
    Target target(5, 5, 25, 0.025f);

    SDL_Event e;
    bool quit = false;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            }
        }
    
        // simulate ...
        int x, y;
        SDL_GetMouseState(&x, &y);

        target.tick(x, y);
        gs.tick(target);

        // Rendering ...

        // Clear framebuffer
        SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
        SDL_RenderClear(renderer);

        // Line from GS to target
        {
            const auto target_size = target.getSize();
            const auto gs_size = gs.getSize();

            SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0xff);
            //SDL_RenderDrawLine(renderer, gs.getX() + 0.5f * gs_size, gs.getY(),
                //target.getX() + 0.5f * target_size, target.getY() + target_size);
        }

        // Target object
        {
            const auto Size = target.getSize();
            SDL_Rect visual = { target.getX() - 0.5f * Size, target.getY() - 0.5f * Size, Size, Size };

            SDL_SetRenderDrawColor(renderer, 0, 0, 0xff, 0xff);
            SDL_RenderFillRect(renderer, &visual);
        }

        // Guidance System Object
        {
            const auto Size = gs.getSize();
            SDL_Rect visual = { gs.getX() - 0.5f*Size, gs.getY() - 0.5f * Size, Size, Size};

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
            SDL_RenderFillRect(renderer, &visual);

            SDL_SetRenderDrawColor(renderer, 50, 50, 50, 0xff);

            const auto Gx = gs.getX();
            const auto Gy = gs.getY();
            const auto A = gs.getA();
            const auto B = gs.getB();

            SDL_RenderDrawLineF(renderer, Gx, Gy, A.x, A.y);
            SDL_RenderDrawLineF(renderer, Gx, Gy, B.x, B.y);
            SDL_RenderDrawLineF(renderer, A.x, A.y, B.x, B.y);

            const Missile* missile = gs.getMissile();
            if (missile) {
                const auto mpos = missile->getPos();
                SDL_FRect visual{ mpos.x, mpos.y, 5, 5 };
                SDL_SetRenderDrawColor(renderer, 0xff, 0, 0, 0xff);
                SDL_RenderFillRectF(renderer, &visual);
            }
        }

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

	return 0;
}