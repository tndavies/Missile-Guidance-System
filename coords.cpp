#include <coords.h>

extern const size_t FrameWidth;
extern const size_t FrameHeight;

// Utility function to convert from SDL window coordinates (used in drawing),
// to simulation coordinates (used in physics calculations).
glm::vec2 toSimSpace(glm::vec2 sdl) {
    const auto GSx = sdl.x - FrameWidth * 0.5f;
    const auto GSy = FrameHeight - sdl.y;
    return glm::vec2(GSx, GSy);
}

// Utility function to go the other way around.
glm::vec2 toSDLSpace(glm::vec2 gs) {
    const auto SDLx = gs.x + FrameWidth * 0.5f;
    const auto SDLy = FrameHeight - gs.y;
    return glm::vec2(SDLx, SDLy);
}