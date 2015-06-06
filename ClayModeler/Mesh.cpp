#include "StdAfx.h"
#include "Mesh.h"
#include "Vertex.h"
#include "Particle.h"
#include "vec.h"
#include "Face.h"

CMesh::CMesh(void)
{
	m_toolRadius = 0.5f;
	m_bDirtyNormal = false;
	m_bWireFrame = false;
	m_CurTri = 0;
	m_toolNum = 1;
}


CMesh::~CMesh(void)
{
}


void CMesh::Init()
{
}


void CMesh::DrawParticles()
{
	for(int i=0; i<particles.size(); i++)
	{
		particles[i].DrawParticle();
	}
}

void CMesh::Draw()
{	
	glBegin(GL_TRIANGLES);

	// Update Normals
	if(m_bDirtyNormal) UpdateNormals();

	for(int i=0; i< faces.size(); i++)
	{
		int *v = faces[i].v;

		// Clay Color
		glColor3f(0.8,0.5f,0.3f);	
	
		glNormal3f(vertices[v[0]].normal.x, vertices[v[0]].normal.y, vertices[v[0]].normal.z);
		glVertex3f(vertices[v[0]].coord.x, vertices[v[0]].coord.y, vertices[v[0]].coord.z);

		glNormal3f(vertices[v[1]].normal.x, vertices[v[1]].normal.y, vertices[v[1]].normal.z);
		glVertex3f(vertices[v[1]].coord.x, vertices[v[1]].coord.y, vertices[v[1]].coord.z);

		glNormal3f(vertices[v[2]].normal.x, vertices[v[2]].normal.y, vertices[v[2]].normal.z);
		glVertex3f(vertices[v[2]].coord.x, vertices[v[2]].coord.y, vertices[v[2]].coord.z);
	}

	glEnd();

	glDisable(GL_LIGHTING);
	glPushMatrix();

	GLfloat m [16];
	glGetFloatv(GL_MODELVIEW_MATRIX, m);
	glLoadIdentity();
	glTranslatef(0, 0, 0.01);
	
	glMultMatrixf(m);
	
	glLineWidth(1.0f);
	glBegin(GL_LINES);
	glColor3f(0.8,0.8f,0.8f);	
	
	glColor3f(1.0,1.0f,1.0f);	
	
	if(m_bWireFrame)
	{
		//Draw WireFrame
		for(int i=0; i < faces.size(); i++)
		{
			int in = faces[i].v[0];
			int in1 = faces[i].v[1];
			int in2 = faces[i].v[2];

			vec3 a;
			a = vertices[in].coord;
			glVertex3f(a.x, a.y, a.z);
			a = vertices[in1].coord;
			glVertex3f(a.x, a.y, a.z);


			a = vertices[in1].coord;
			glVertex3f(a.x, a.y, a.z);
			a = vertices[in2].coord;
			glVertex3f(a.x, a.y, a.z);

			a = vertices[in2].coord;
			glVertex3f(a.x, a.y, a.z);
			a = vertices[in].coord;
			glVertex3f(a.x, a.y, a.z);
		}
	}

	glLineWidth(1.0f);

	glEnd();

	glPopMatrix();
	glEnable(GL_LIGHTING);

	GLUquadricObj* sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	glColor3f(1.0,0.5,0.5);
	
	//Draw Modify Vertices
	for(int i = 0; i<modify_vertices.size(); i++)
	{
		glPushMatrix();
		glTranslatef(vertices[modify_vertices[i]].coord.x, 
			vertices[modify_vertices[i]].coord.y, vertices[modify_vertices[i]].coord.z);
		gluSphere(sphere, 0.025 ,6,4); 

		glPopMatrix();
	}

	DrawParticles();
	gluDeleteQuadric(sphere);

}


// SubDivide - Add Vert, Edge, Face
void CMesh::DoSubDivide()
{
	// faces와 edges의 copy생성
	vector<CFace> faces_copy;
	vector<CEdge> edges_copy;

	for(int i=0; i< faces.size(); i++)
	{
		faces_copy.push_back( faces[i] );
	}
	for(int i=0; i< edges.size(); i++)
	{
		edges_copy.push_back( edges[i] );
	}

	// 버텍스 개수 저장
	int VertexSize = vertices.size();
	
	// edge 와 face는 다시만들어야함.
	faces.clear();
	edges.clear();

	
	// 새로운 버텍스 추가
	for(int i=0; i<edges_copy.size(); i++)
	{
		vec3 p1 = vertices[edges_copy[i].v[0]].coord;
		vec3 p2 = vertices[edges_copy[i].v[1]].coord;
		vec3 v = (p1 + p2)/2.0f;
		CVertex nv = CVertex(normalize(v) * 1.0f);
		nv.index = VertexSize + i;
		vertices.push_back( nv );
	}


	// 새로운 페이스 추가
	for(int i=0; i<faces_copy.size(); i++)
	{
		int ind[6];
		ind[0] = faces_copy[i].v[0];
		ind[1] = faces_copy[i].v[1];
		ind[2] = faces_copy[i].v[2];
		ind[3] = VertexSize + faces_copy[i].edgesIndex[0];
		ind[4] = VertexSize + faces_copy[i].edgesIndex[1];
		ind[5] = VertexSize + faces_copy[i].edgesIndex[2];

		faces.push_back( CFace(ind[0], ind[3], ind[5]) );
		faces.push_back( CFace(ind[3], ind[1], ind[4]) );
		faces.push_back( CFace(ind[3], ind[4], ind[5]) );
		faces.push_back( CFace(ind[5], ind[4], ind[2]) );
	}

	BuildEdge();
	m_maxLength = edges[0].getLength(vertices[edges[0].v[0]].coord, vertices[edges[0].v[1]].coord);

	m_bDirtyNormal = true;
}


// Check Intersecting for all Triangles
int CMesh::isIntersectPlane(vec3 p0, vec3 p1, vec3 A, vec3 B, vec3 C, vec3 & I)
{
	//--------Find Ray Intersect Plane
	// X = P + kV
	// ma + nb = r
	// if m + n > 0 and m + n < 1 => TRUE

	//P is NearVec
	vec3 u,v,n;
	vec3 dir, w0, w;
	float r,a,b;

	vec3 v0 = A;
	vec3 v1 = B;
	vec3 v2 = C;

	u = v1-v0;
	v = v2-v0;
	n = cross(u,v);

	if(length(n)==0.0f)
		return -1;

	dir = p1-p0;
	
	w0 = p0-v0;

	a = -dot(n,w0);
	b = dot(n,dir);

	if(fabs(b) < 0.000001f)
	{
		if(a == 0)
			return 2;
		else return 0;
	}

	r = a/b;
	if( r < 0.0)
		return 0;

	I = p0 + r * dir;
	 
	float uu, uv, vv, wu, wv, D;

	uu = dot(u,u);
	uv = dot(u,v);
	vv = dot(v,v);
	w = I- v0;
	wu = dot(w,u);
	wv = dot(w,v);

	D = uv * uv - uu * vv;

	float s,t;
	
	s = (uv * wv - vv * wu) / D;
	t = (uv * wu - uu * wv) / D;

	if(s < 0.0 || s > 1.0)
		return 0;

	if(t < 0.0 || (s+t) > 1.0)
		return 0;

	return 1;
}

void CMesh::findPickedPoint(float* p)
{
	unProject(p);
	vector<vec3> pickedVertices;

	// 내가 찍고 있는 점을 찾는다.
	for(int i = 0; i <faces.size(); i++)
	{
		int or1 = faces[i].v[0];
		int or2 = faces[i].v[1];
		int or3 = faces[i].v[2];

		vec3 A = GetVertexPosition(or1);
		vec3 B = GetVertexPosition(or2);
		vec3 C = GetVertexPosition(or3);

		if(isIntersectPlane(nearV, farV, A, B, C, pickedVertex))
		{
			pickedVertices.push_back(pickedVertex);
		}
	}

	// 가장 가까운 점으로 설정한다.
	GLdouble m_matrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, m_matrix);
	GLdouble p_matrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, p_matrix);
	GLint v_matrix[4];
	glGetIntegerv(GL_VIEWPORT, v_matrix); 


	double min = DBL_MAX;
	for(int i=0; i < pickedVertices.size(); i++)
	{
		double x,y,z;
		gluProject((double)pickedVertices[i].x, (double)pickedVertices[i].y, (double)pickedVertices[i].z, m_matrix, p_matrix, v_matrix, &x, &y, &z);
			if( z < min )
			{
				min = z;
				pickedVertex = pickedVertices[i];
			}
	}
}

void CMesh::unProject(float *p)
{
	// ScreenCoord to WorldCoord
	GLdouble o1[3], o2[3];

	// Get MVP Matrices
	GLdouble m_matrix[16];
	glGetDoublev(GL_MODELVIEW_MATRIX, m_matrix);
	GLdouble p_matrix[16];
	glGetDoublev(GL_PROJECTION_MATRIX, p_matrix);
	GLint v_matrix[4];
	glGetIntegerv(GL_VIEWPORT, v_matrix); 
	
	gluUnProject((double)p[0], (double)(v_matrix[3] - p[1]), (double)0, m_matrix, p_matrix, v_matrix, &o1[0], &o1[1], &o1[2]);
	gluUnProject((double)p[0], (double)(v_matrix[3] - p[1]), (double)1, m_matrix, p_matrix, v_matrix, &o2[0], &o2[1], &o2[2]);
	
	nearV = vec3(o1[0], o1[1], o1[2]);
	farV = vec3(o2[0], o2[1], o2[2]);
}

void CMesh::UpdateNormals()
{
	for(int i=0; i< vertices.size(); i++)
	{
		vertices[i].normal = 0;
	}
	for (int i = 0; i< faces.size(); i++)
	{
		faces[i].normal = 0;
	}

	for(int i=0; i< faces.size(); i++)
	{
		int *v = faces[i].v;

		vec3 a = vertices[v[0]].coord - vertices[v[1]].coord;
		vec3 b = vertices[v[2]].coord - vertices[v[1]].coord;

		// 페이스의 엣지 순서가 뒤바뀐듯.
		vec3 n = cross(b, a);
		n = normalize(n);

		faces[i].normal = n;
		
		for(int j=0; j<3; j++)
			vertices[v[j]].normal  += n;	
	}

	for(int i=0; i< vertices.size(); i++)
	{
		vertices[i].normal = normalize(vertices[i].normal);
	}

	m_bDirtyNormal = false;

}

void CMesh::FindModifyVertices()
{
	// Particle과 r안에 있는 모든 점들을 큐에 담는다.
	modify_vertices.clear();

	for(int i = 0; i <vertices.size(); i++)
	{
		for(int j=0; j < particles.size(); j++)
		{
			// vertices[i]가 r 안에 있는지 검사한다.
			
			if(vertices[i].isInRadius(particles[j], particles[j].radius))
			{
				modify_vertices.push_back(i);
				break;
			}
		}
	}
}

void CMesh::mergeSameEdges()
{
	vector<CEdge> TempEdges;
	TempEdges.resize( edges.size() );
	
	for(int i=0; i< edges.size(); i++)
		TempEdges[i] = edges[i];

	for(int i=0; i < TempEdges.size(); i++)
	{
		if( TempEdges[i].check == false) continue;

		for(int k=0; k<2; k++)
		{
			int fsize = vertices[TempEdges[i].v[k]].neighborFaces.size();
		
			for(int j=0; j< fsize; j++)
			{
				int nf = vertices[TempEdges[i].v[k]].neighborFaces[j];
				
				for(int v=0; v< 3; v++)
				{
					int ei = faces[nf].edgesIndex[v];

					// 같은 엣지 찾음
					if( TempEdges[i].IsSame( TempEdges[ei] ) && i != ei)
					{	
						TempEdges[ei].check = false;
						TempEdges[ei].index = i;
						TempEdges[i].neighborFaces[1] = TempEdges[ei].neighborFaces[0];
					}				
				}
			}
		
		}
	}

	edges.clear();

	for(int i=0; i< TempEdges.size(); i++)
	{
		if(TempEdges[i].check == true)
		{
			edges.push_back( TempEdges[i] );
			TempEdges[i].index = edges.size()-1;
		}
		else
		{
			TempEdges[i].index = TempEdges[TempEdges[i].index].index;
			continue;
		}
	}

	// 페이스 정보 추가
	for(int i=0; i<faces.size(); i++)
	{
		for(int k=0; k<3; k++)
			faces[i].edgesIndex[k] = TempEdges[faces[i].edgesIndex[k]].index;
	}


}


void CMesh::ClearVectors()
{
	particles.clear();
	modify_vertices.clear();
}


void CMesh::DoDeformation()
{
	int n = 20;	
	
	for(int k=0; k< n; k++)
	{
		for(int i=0; i< modify_vertices.size(); i++)
		{
			vec3 qp, q, r1, p;
			
			q = vertices[modify_vertices[i]].coord;
			qp = vec3(0, 0, 0);

			for(int j=0; j< particles.size(); j++)
			{
				p = particles[j].position;
				r1 = q - p;

				float dist = sqrt(dot(r1, r1));
				r1 = r1 / dist;

				float ww = dist / particles[j].radius;
				float w = 0.0f;

				if(particles[j].radius > dist)
					w = 1 + ww*ww*ww * (ww*(15 - 6*ww) - 10);

				qp += (cross(particles[j].m_dir, r1) * particles[j].m_angle /n * w);
				
			}
			q += qp;
			vertices[modify_vertices[i]].coord = q;
		}
	}

	DoPartialSubDivide();
	m_bDirtyNormal = true;

}

////////////////////// Twist
void CMesh::AddTwistParticle()
{
	CParticle p;
	p.m_angle = 0.2f * 3.141592f / 180.0f;
	p.m_dir = normalize(farV - nearV);
	p.position = pickedVertex;
	p.radius = 1.0f;
	particles.push_back(p);
}


///////////////////// Pull / Push
void CMesh::AddPullParticles()
{
	float radius = m_toolRadius;

	vec3 p0 = pickedVertex;
	vec3 a = vec3(1,0,0);
	movingDir = GetMovingDir();
	vec3 n = movingDir;
	n = normalize(n);

	vec3 u = a - dot(a, n)*n;
	u = normalize(u);

	if(dot(a,n) > 0.99)
	{
		a = vec3(0,1,0);
		u = a-dot(a, n)*n;
		u = normalize(u);
	}

	vec3 v = cross(u, n);
	v = normalize(v);

	vec3 dir;
	vec3 pos;

	int NumParticle = 8;

	for(int i=0; i< NumParticle; i++)
	{
		CParticle p;
		p.radius = 2*radius;
		p.m_angle = 0.2f * 3.141592f / 180.0f;
		if(m_toolNum == 2) p.m_angle *= (-1);
		float radTheta = (2 * 3.141592f / NumParticle) * i;
		
		vec3 d = vec3(sin(radTheta), -cos(radTheta), 0);
		dir.x = u.x*d.x + v.x*d.y + n.x*d.z;
		dir.y = u.y*d.x + v.y*d.y + n.y*d.z;
		dir.z = u.z*d.x + v.z*d.y + n.z*d.z;

		p.m_dir = dir;
		
		vec3 q = vec3(radius* cos(radTheta), radius* sin(radTheta), 0);
		pos.x = u.x*q.x + v.x*q.y + n.x*q.z + p0.x;
		pos.y = u.y*q.x + v.y*q.y + n.y*q.z + p0.y;
		pos.z = u.z*q.x + v.z*q.y + n.z*q.z + p0.z;
		p.position = pos;
		
		particles.push_back(p);
	}
}

void CMesh::CreateParticle()
{
	particles.clear();

	switch (m_toolNum)
	{
	case 1:
	case 2:
		AddPullParticles();
		break;
	case 3:
		AddTwistParticle();
		break;
	default:
		break;
	}
}

void CMesh::CopyOtherMesh(CMesh& mesh)
{
	for(int i=0; i< mesh.vertices.size(); i++)
	{
		vertices.push_back ( mesh.vertices[i] );
	}

	for(int i=0; i< mesh.faces.size(); i++)
	{
		faces.push_back( mesh.faces[i] );
	}

	for(int i=0; i< mesh.edges.size(); i++)
	{
		edges.push_back( mesh.edges[i] );
	}

	sphere_central = mesh.sphere_central;

	divLevel = mesh.divLevel;
}


void CMesh::BuildEdge(void)
{
	// 엣지 만들기
	for(int i=0; i<faces.size(); i++)
	{
		for(int k=0; k<3; k++)
		{
			CEdge edge = CEdge(faces[i].v[k%3], faces[i].v[(k+1)%3], i);
			// 초기 엣지 길이저장
			edge.originLength = edge.getLength(vertices[faces[i].v[k%3]].coord, vertices[faces[i].v[(k+1)%3]].coord);
			edges.push_back(edge);
			faces[i].edgesIndex[k] = edges.size()-1;
			vertices[faces[i].v[k]].neighborFaces.push_back(i);
		}
	}
	// 중복 엣지 제거 및 페이스의 엣지 업데이트
	mergeSameEdges();
}


void CMesh::ParticlesMove(void)
{
	for(int i=0; i<particles.size(); i++)
	{
		particles[i].position += movingDir*particles[i].m_angle*4.0f;
	}
}   



void CMesh::DoPartialSubDivide(void)
{
	long_edges.clear();

	// Check If Longer than Origin
	for (int i = 0; i<edges.size(); i++)
	{
		CEdge e = edges[i];

		edges[i].originLength = m_maxLength;
		edges[i].currentLength = e.getLength(vertices[e.v[0]].coord, vertices[e.v[1]].coord);

		struct longEdge le(i, e.currentLength);

		if (e.isLonger())
		{
			// Insert in list
			long_edges.push_back(le);
		}
	}
	
	// 긴 엣지대로 소팅
	long_edges.sort(longEdge_less());


	list<longEdge>::iterator itor;
	for (itor = long_edges.begin(); itor != long_edges.end(); itor++)
	{
		int edgeInd = itor->ind;
		int a, b, c, d, m;
		int e0, e1, e2, e3, e4, e5, e6, e7;
		int f0, f1, f2, f3;

		e0 = e1 = e2 = e3 = e4 = e5 = e6 = e7 = -1;
		a = b = c = d = m = -1;

		CEdge edge = edges[edgeInd];
		int *neighborFace = edge.neighborFaces;
		f0 = neighborFace[0];
		f1 = neighborFace[1];

		// 긴 엣지 리스트에 담음
		c = edge.v[0];
		a = edge.v[1];
		bool ck = false;

		for (int i = 0; i < 3; i++)
		{
			if (faces[f0].v[i] == edge.v[0] && faces[f0].v[(i + 1) % 3] == edge.v[1])
				ck = true;
		}
		if (ck == false)
			swap(a, c);

		vec3 m_ver = (vertices[a].coord + vertices[c].coord) / 2.0f;
		vertices.push_back(m_ver);
		m = vertices.size() - 1;
		modify_vertices.push_back(m);

		// Find b , d
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 2; j++)
			{

				if (faces[neighborFace[j]].v[i] != a &&
					faces[neighborFace[j]].v[i] != c)

					if (j == 0)
						b = faces[neighborFace[j]].v[i];
					else
						d = faces[neighborFace[j]].v[i];
			}
		}


		// Face 추가
		faces[f0].v[0] = a;
		faces[f0].v[1] = b;
		faces[f0].v[2] = m;

		faces[f1].v[0] = a;
		faces[f1].v[1] = m;
		faces[f1].v[2] = d;

		faces.push_back(CFace(b, c, m));
		f2 = faces.size() - 1;

		faces.push_back(CFace(m, c, d));
		f3 = faces.size() - 1;

		// 엣지 찾기, 엣지 추가
		for (int i = 0; i < 3; i++)
		{
			int pInd = faces[f0].edgesIndex[i];

			if (pInd != edgeInd
				&& (edges[pInd].v[0] == a || edges[pInd].v[1] == a))
				e0 = pInd;

			else if (pInd != edgeInd
				&& (edges[pInd].v[0] == c || edges[pInd].v[1] == c))
				e1 = pInd;

			pInd = faces[f1].edgesIndex[i];

			if (pInd != edgeInd
				&& (edges[pInd].v[0] == a || edges[pInd].v[1] == a))
				e2 = pInd;

			else if (pInd != edgeInd
				&& (edges[pInd].v[0] == c || edges[pInd].v[1] == c))
				e3 = pInd;
		}


		e4 = edgeInd;
		edges[e4].v[0] = a; edges[e4].v[1] = m;
		edges[e4].originLength = m_maxLength;
		edges[e4].currentLength = edges[e4].getLength(vertices[edges[e4].v[0]].coord, vertices[edges[e4].v[1]].coord);
		if (edges[e4].isLonger()) long_edges.push_back(longEdge(e4, edges[e4].currentLength));

		edges.push_back(CEdge(b, m, f0, f2));
		e5 = edges.size() - 1;
		edges[e5].originLength = m_maxLength;
		edges[e5].currentLength = edges[e5].getLength(vertices[edges[e5].v[0]].coord, vertices[edges[e5].v[1]].coord);
		if (edges[e5].isLonger()) long_edges.push_back(longEdge(e5, edges[e5].currentLength));

		edges.push_back(CEdge(m, c, f2, f3));
		e6 = edges.size() - 1;
		edges[e6].originLength = m_maxLength;
		edges[e6].currentLength = edges[e6].getLength(vertices[edges[e6].v[0]].coord, vertices[edges[e6].v[1]].coord);
		if (edges[e6].isLonger()) long_edges.push_back(longEdge(e6, edges[e6].currentLength));

		edges.push_back(CEdge(m, d, f1, f3));
		e7 = edges.size() - 1;
		edges[e7].originLength = m_maxLength;
		edges[e7].currentLength = edges[e7].getLength(vertices[edges[e7].v[0]].coord, vertices[edges[e7].v[1]].coord);
		if (edges[e7].isLonger()) long_edges.push_back(longEdge(e7, edges[e7].currentLength));


		// Edge NeighborFace 수정
		for (int i = 0; i < 2; i++)
		{
			if (edges[e1].neighborFaces[i] == f0)
				edges[e1].neighborFaces[i] = f2;

			if (edges[e3].neighborFaces[i] == f1)
				edges[e3].neighborFaces[i] = f3;
		}

		// Face 정보 수정
		faces[f0].edgesIndex[0] = e0;
		faces[f0].edgesIndex[1] = e5;
		faces[f0].edgesIndex[2] = e4;

		faces[f1].edgesIndex[0] = e4;
		faces[f1].edgesIndex[1] = e7;
		faces[f1].edgesIndex[2] = e2;

		faces[f2].edgesIndex[0] = e1;
		faces[f2].edgesIndex[1] = e6;
		faces[f2].edgesIndex[2] = e5;

		faces[f3].edgesIndex[0] = e6;
		faces[f3].edgesIndex[1] = e3;
		faces[f3].edgesIndex[2] = e7;

		vertices[m].neighborFaces.push_back(f0);
		vertices[m].neighborFaces.push_back(f1);
		vertices[m].neighborFaces.push_back(f2);
		vertices[m].neighborFaces.push_back(f3);

		vertices[b].neighborFaces.push_back(f2);
		vertices[d].neighborFaces.push_back(f3);

		for (int i = 0; i < vertices[c].neighborFaces.size(); i++)
		{
			if (vertices[c].neighborFaces[i] == f0)
				vertices[c].neighborFaces[i] = f2;
			else if (vertices[c].neighborFaces[i] == f1)
				vertices[c].neighborFaces[i] = f3;
		}

		// Normal ?
		UpdateNormals();
	}
}

vec3 CMesh::GetMovingDir()
{
	movingDir = nearV - farV;
	vec3 n = movingDir;
	n = normalize(n);
	return n;
}
