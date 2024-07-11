#include <missile.h>
#include <coords.h>
#include <cmath>
#include <iostream>
#include <glm/glm.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

glm::vec2 calc_trajectory(glm::vec2 from, glm::vec2 to)
{
	return glm::normalize(to - from);
}

Missile::Missile(glm::vec2 target, float speed) :
	m_Pos(0, 0), m_Speed(speed)
{
	m_InitialLOS = calc_trajectory(m_Pos, target);
	m_CurrLOS = m_InitialLOS;
	m_prevLOSA = 0.0f;
	
	m_Vel = m_InitialLOS * 80.0f;
	m_Acc = { 0.0f, 0.0f };
}

void Missile::tick(const Target& target, float dt)
{
	// Guidance algorithm.
	// 1) Calc angle between reference line & target (LOS angle).
	const auto tpos = SDLtoGS(target.getPos());
	m_CurrLOS = calc_trajectory(m_Pos, tpos);
	float currLOSA = glm::angle(m_InitialLOS, m_CurrLOS);

	// 2) Calc temporal ROC of LOS angle.
	float rocLOSA = (currLOSA - m_prevLOSA) / dt;
	m_prevLOSA = currLOSA;
	
	// 3) Find missile velocity upon target
	auto velDelta = velocity() - target.velocity();
	auto closingVel = glm::dot(m_CurrLOS, velDelta);

	// 4) Use PN formula to find correction acceleration needed.
	const auto Aggresiveness = 1.0f; // todo: >1 is unstable, try Runge-Kutta integration.
	auto corrAccel = Aggresiveness * closingVel * rocLOSA;
	
	auto accUnitVec = glm::cross(glm::vec3(m_Vel.x, m_Vel.y, 0.0f), { 0,0,1 });
	auto sign = std::sinf(glm::angle(m_Vel, m_CurrLOS));
	std::cout << glm::degrees(sign) << std::endl;
	if (sign > 0.0f) accUnitVec = -accUnitVec;
	
	m_Acc = glm::vec2(accUnitVec.x, accUnitVec.y) * corrAccel;

	// 5) Adjust missile velocity vector.
	m_Vel += m_Acc * dt;
	
	// 6) Move missile according to velocity.
	m_Pos += m_Vel * dt;
}

void Missile::draw(SDL_Renderer* r)
{
	const auto Size = 8.0f;
	const auto mpos = GStoSDL(m_Pos);
	SDL_FRect visual{ mpos.x - 0.5f * Size, mpos.y - 0.5f * Size, Size, Size };
	SDL_SetRenderDrawColor(r, 0, 0, 0, 0xff);
	SDL_RenderFillRectF(r, &visual);


	const auto LOS = m_CurrLOS * 30.0f;
	SDL_SetRenderDrawColor(r, 0, 100, 255, 0xff);
	SDL_RenderDrawLineF(r, mpos.x, mpos.y, mpos.x + LOS.x, mpos.y - LOS.y);

	const auto vpos = glm::normalize(m_Vel) * 45.0f;
	SDL_SetRenderDrawColor(r, 0, 200, 0, 0xff);
	SDL_RenderDrawLineF(r, mpos.x, mpos.y, mpos.x + vpos.x, mpos.y - vpos.y);

	const auto apos = m_Acc;
	SDL_SetRenderDrawColor(r, 255, 0, 0, 0xff);
	SDL_RenderDrawLineF(r, mpos.x, mpos.y, mpos.x + apos.x, mpos.y - apos.y);
}