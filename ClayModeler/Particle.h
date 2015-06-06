#pragma once
#include "vec.h"

class CParticle
{
public:
	vec3 m_dir;
	float m_angle;
	float radius;
	vec3 position;

	void DrawParticle();

	CParticle(void);
	~CParticle(void);
};

