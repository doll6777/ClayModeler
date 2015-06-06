#include "StdAfx.h"
#include "Vertex.h"
#include "Particle.h"
#include "vec.h"
#include <math.h>

CVertex::CVertex(void)
{
}

CVertex::CVertex(vec3 v)
{
	coord = v;
}

CVertex::~CVertex(void)
{
}

bool CVertex::isInRadius(CParticle particle, float r)
{
	vec3 p = particle.position;

	//TRACE("%f ", sqrt( (p.x-coord.x) * (p.x-coord.x) + (p.x-coord.y) * (p.x-coord.y) + (p.x-coord.z) * (p.x-coord.z)));
	if( sqrt( (p.x-coord.x) * (p.x-coord.x) + (p.y-coord.y) * (p.y-coord.y) + (p.z-coord.z) * (p.z-coord.z)) <= r)
		return true;

	else
		return false;
}