#include <glm/glm.hpp>
#include <missile.h>
#include <coords.h>
#include <cmath>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/vector_angle.hpp>

Missile::Missile(glm::vec2 target, float speed)
{
	m_Pos = { 0.0f, 0.0f }; // Start missile at guidance-system (sim coords).
	m_Acc = { 0.0f, 0.0f };

	m_InitialLOS = glm::normalize( calcLOS(target) );
	m_Vel = m_InitialLOS * speed;
	m_prevLOSAngle = 0.0f;
}

void Missile::tick(const Target& target, float dt)
{
	// -- Proportional Navigation Algorithm -- //
	
	// 1) Calc angle between reference line & target (LOS angle).
	m_LOS = glm::normalize( calcLOS(target.getPos()) );
	const float currLOSA = glm::angle(m_InitialLOS, m_LOS);

	// 2) Calc temporal ROC of LOS angle.
	const float rocLOSA = (currLOSA - m_prevLOSAngle) / dt;
	m_prevLOSAngle = currLOSA;
	
	// 3) Find missile velocity upon target.
	const auto velDelta = getVelocity() - target.getVelocity();
	const auto closingVel = glm::dot(m_LOS, velDelta);

	// 4) Use PN formula to find correction acceleration needed.
	const auto Aggresiveness = 0.01f; // [Tunable Parameter]
	const auto corrAccel = Aggresiveness * closingVel * rocLOSA;
	
	const glm::vec3 zAxis = { 0,0,1 };
	const glm::vec3 velCastUp = { m_Vel.x, m_Vel.y, 0.0f }; // Cast 2D velocity -> 3D.

	// Obtain the appropriate acceleration vector that is perpendicular to the velocity.
	const auto sign = std::sinf(glm::angle(m_Vel, m_LOS));
	const auto perpVec = (sign <= 0.0f) ? glm::cross(velCastUp, zAxis) : -glm::cross(velCastUp, zAxis);
	m_Acc = glm::vec2(perpVec.x, perpVec.y) * corrAccel;

	// 5) Adjust missile velocity vector.
	m_Vel += m_Acc * dt;
	
	// 6) Move missile according to velocity.
	m_Pos += m_Vel * dt;
}

void Missile::draw(SDL_Renderer* r)
{
	const auto visualSize = 8.0f; // Length of square (in pixels).

	// Draw the missile.
	const auto mPos = toSDLSpace(m_Pos);
	SDL_FRect visual{ mPos.x - 0.5f * visualSize, mPos.y - 0.5f * visualSize, visualSize, visualSize };
	SDL_SetRenderDrawColor(r, 0, 0, 0, 0xff);
	SDL_RenderFillRectF(r, &visual);

	// Visualisation of LOS vector.
	const auto visualLOS = m_LOS * 30.0f;
	SDL_SetRenderDrawColor(r, 0, 100, 255, 0xff);
	SDL_RenderDrawLineF(r, mPos.x, mPos.y, mPos.x + visualLOS.x, mPos.y - visualLOS.y);

	// Visualisation of velocity vector.
	const auto velPos = glm::normalize(m_Vel) * 45.0f;
	SDL_SetRenderDrawColor(r, 0, 200, 0, 0xff);
	SDL_RenderDrawLineF(r, mPos.x, mPos.y, mPos.x + velPos.x, mPos.y - velPos.y);

	// Visualisation of acceleration vector.
	const auto accPos = m_Acc;
	SDL_SetRenderDrawColor(r, 255, 70, 0, 0xff);
	SDL_RenderDrawLineF(r, mPos.x, mPos.y, mPos.x + accPos.x, mPos.y - accPos.y);
}

// Calculates the vector from the missile position, to
// the target's position (in simulation space).
glm::vec2 Missile::calcLOS(glm::vec2 target) 
{ 
	return target - m_Pos; 
}