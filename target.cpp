#include "target.h"

void Target::tick(float dt)
{
	m_Pos += velocity() * dt;
}

void Target::draw(SDL_Renderer* r)
{
	SDL_Rect visual = { m_Pos.x - 0.5f * m_Size, m_Pos.y - 0.5f * m_Size, m_Size, m_Size };
	SDL_SetRenderDrawColor(r, 100, 100, 150, 0xff);
	SDL_RenderFillRect(r, &visual);
}
