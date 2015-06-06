#pragma once
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "Particle.h"
#include "vec.h"
#include <list>
using namespace std;

class CMesh
{
public:
	struct longEdge{
		int ind;
		float len;
		longEdge()
		{
			ind = -1;
			len = 0;
		}
		longEdge(int i, float l)
		{
			ind = i;
			len = l;
		}

	};

	struct longEdge_less
	{
		bool operator()(const longEdge& lhs, const longEdge& rhs) const
		{
			return lhs.len > rhs.len;
		}
	};

	enum{TOOL_MODE_PULL=1, TOOL_MODE_PUSH, TOOL_MODE_TWIST};

	vector<CVertex> vertices;
	vector<CEdge> edges;
	vector<CFace> faces;
	list<longEdge> long_edges;
	vector<int> vertices_order;
	vector<int> origin_vertices_order;
	vector<int> modify_vertices;

	vec3 origin_vertices[4];
	vec3 sphere_central;
	vec3 movingDir;

	vec3 nearV, farV;
	vec3 pickedVertex;

	int divLevel;
	CParticle particle;
	vector <CParticle> particles;

	CMesh(void);
	~CMesh(void);

	int isIntersectPlane(vec3 nv, vec3 fv, vec3 A, vec3 B, vec3 C, vec3 & I);

	bool m_bDirtyNormal;
	bool m_bWireFrame;

	float m_toolRadius;
	int m_toolNum;
	int m_CurTri;

	void CopyOtherMesh(CMesh&);
	void UpdateNormals();
	void FindModifyVertices();
	void CreateParticle();
	void DoDeformation();
	void DoSubDivide();
	void ClearVectors();
	void AddTwistParticle();
	void AddPullParticles();
	void findPickedPoint(float*);
	void DrawParticles();

	void DoPartialSubDivide();
	void Init();
	void subDivide(CVertex& a, CVertex& b, CVertex& c, int level);
	void unProject(float *p);
	void mergeSameEdges();
	void Draw();
	float m_maxLength;

	vec3 CMesh::GetVertexPosition(int ind)
	{
		vec3 out(0.0f);
		if(ind<0 || ind>vertices.size()-1) return out;
		out = vertices[ind].coord;

		return out;
	}

	void BuildEdge(void);
	void ParticlesMove(void);
	vec3 GetMovingDir();
};

