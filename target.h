#pragma once

class Target {
public:
	Target(float x, float y, float size, float speed_x) :
		m_X(x), m_Y(y), m_Size(size), m_SpeedX(speed_x) {}
	
	~Target() {}

	void tick(float x, float y);

	auto getSize() const { return m_Size; }
	auto getX() const { return m_X; }
	auto getY() const { return m_Y; }

private:
	float m_X, m_Y;
	float m_SpeedX;
	float m_Size;
};