#pragma once


#include "vec.h"
#include <vector>
#include "Particle.h"
#include "Face.h"

using namespace std;

class CVertex
{
public:
	int index;
	vec3 coord;
	vec3 normal;

	vector<int> neighborFaces;
	vector<int> neighborEdges;

	//1복사생성자
	//만들어놓기
	//2연산자오버로딩 =

	CVertex(void);
	CVertex(vec3 v);

	bool isInRadius(CParticle v, float r);
	bool isSame(const CVertex &v)
	{
		vec3 vert = v.coord;
		if (vert.x == coord.x &&
			vert.y == coord.y &&
			vert.z == coord.z)
			return true;
		else
			return false;

	}
	CVertex(const CVertex &a)
	{
		coord = a.coord;
		index = a.index;
		for(int i=0; i<neighborFaces.size(); i++)
		{
			neighborFaces[i] = a.neighborFaces[i];
		}
	}
	~CVertex(void);
};

