#include "target.h"

void Target::tick(float x, float y, float dt)
{
	m_X = x;
	m_Y = y;
}

void Target::draw(SDL_Renderer* r)
{
	SDL_Rect visual = { m_X - 0.5f * m_Size, m_Y - 0.5f * m_Size, m_Size, m_Size };
	SDL_SetRenderDrawColor(r, 100, 100, 150, 0xff);
	SDL_RenderFillRect(r, &visual);
}
