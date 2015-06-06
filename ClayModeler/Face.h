#pragma once
#include <vector>
#include "vec.h"
#include "Edge.h"

using namespace std;
class CFace
{
public:

	int v[3];
	vec3 normal;
	int edgesIndex[3];
	CFace(void);
	CFace(int a, int b, int c);
	~CFace(void);
};

