#pragma once
#include "vec.h"
using namespace std;

class CEdge
{
public:
	int v[2];
	int index;

	double originLength;
	double currentLength;

	int neighborFaces[2];
	CEdge(int a, int b, int fn);
	CEdge(int a, int b, int fn0, int fn1);
	CEdge(void);
	~CEdge(void);

	double getLength(vec3 a, vec3 b)
	{
		double dist;
		dist = (a.x-b.x)*(a.x-b.x) + (a.y-b.y)*(a.y-b.y) + (a.z-b.z)*(a.z-b.z);
		dist = sqrt(dist);
		return dist;
	}

	bool isLonger()
	{
		if(originLength*1.5 < currentLength)
			return true;
		return false;
	}

	bool check;
	bool IsSame(const CEdge& a)
	{
		if(v[0] == a.v[0] && v[1] == a.v[1]) return true;
		if(v[1] == a.v[0] && v[0] == a.v[1]) return true;
		return false;
	}
};

