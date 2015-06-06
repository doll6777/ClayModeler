#include "StdAfx.h"
#include "Edge.h"


CEdge::CEdge(void)
{
	check = true;
	neighborFaces[0] = neighborFaces[1] = -1;
}

CEdge::CEdge(int a, int b, int fn)
{
	check = true;

	neighborFaces[0] = fn;
	neighborFaces[1] = -1;

	v[0] = a;
	v[1] = b;
}

CEdge::CEdge(int a, int b, int fn0, int fn1)
{
	check = true;

	neighborFaces[0] = fn0;
	neighborFaces[1] = fn1;

	v[0] = a;
	v[1] = b;
}

CEdge::~CEdge(void)
{
}



