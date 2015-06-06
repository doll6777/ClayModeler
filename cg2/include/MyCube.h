#pragma once

#include <vgl.h>
#include <vec.h>
#include <mat.h>

struct MyCubeVertex
{
	vec4 position;
	vec4 color;
	vec2 texCoord;
};

class MyCube
{
public:
	MyCube(void);
	~MyCube(void);

	static const int NumVertices = 36;

	MyCubeVertex Vertices[NumVertices];
	GLuint Init(int num, float ratio, vec2*);
	void SetPositionAndColorAttribute(GLuint program);

	vec3 *tvertices;
	int num;
	void ColorCube(vec4 * vin, vec4 * cin);
	void Quad (int a, int b, int c, int d, vec4 * vin, vec4 * cin);
	vec2 *Rvertices;
	int NumCurVertices;
	void getModelProjMatrix(mat4, mat4);
	GLuint vao;
	GLuint buffer;
	bool bInitialized;
	GLdouble modelMat[16], projMat[16];
	GLint vportMat[16];
	
	void getZvalues(vec2 tvertices[12],int,int);
	void Draw(GLuint program);
};



MyCube::MyCube(void)
{
	bInitialized = false;
	NumCurVertices = 0;
}

MyCube::~MyCube(void)
{
}

void MyCube::Quad (int a, int b, int c, int d, vec4 * vin, vec4 * cin)
{
	//printf("%d\n", num);
	Vertices[NumCurVertices].position = vin[a]; Vertices[NumCurVertices].color = cin[a]; 
	//Vertices[NumCurVertices].texCoord =vec2(0,1);
	Vertices[NumCurVertices].texCoord = Rvertices[0];
	
	NumCurVertices++;
	Vertices[NumCurVertices].position = vin[b]; Vertices[NumCurVertices].color = cin[b];
	//Vertices[NumCurVertices].texCoord = vec2(0,0);
	Vertices[NumCurVertices].texCoord = Rvertices[1];
	NumCurVertices++;
	Vertices[NumCurVertices].position = vin[c]; Vertices[NumCurVertices].color = cin[c]; 
	//Vertices[NumCurVertices].texCoord = vec2(1,0);
	Vertices[NumCurVertices].texCoord =Rvertices[2];
	NumCurVertices++;
	
	Vertices[NumCurVertices].position = vin[a]; Vertices[NumCurVertices].color = cin[a];
	//Vertices[NumCurVertices].texCoord = vec2(0,1);
	Vertices[NumCurVertices].texCoord =Rvertices[0];
	NumCurVertices++;
	Vertices[NumCurVertices].position = vin[c]; Vertices[NumCurVertices].color = cin[c];
	//Vertices[NumCurVertices].texCoord = vec2(1,0);
	Vertices[NumCurVertices].texCoord =Rvertices[2];
	NumCurVertices++;
	Vertices[NumCurVertices].position = vin[d]; Vertices[NumCurVertices].color = cin[d]; 
	//Vertices[NumCurVertices].texCoord = vec2(1,1);
	Vertices[NumCurVertices].texCoord =Rvertices[3];
	NumCurVertices++;



}

void MyCube::ColorCube(vec4 * vin, vec4 * cin)
{
	Quad(1,0,3,2, vin, cin);
	Quad(2,3,7,6, vin, cin);
	Quad(3,0,4,7, vin, cin);
	Quad(6,5,1,2, vin, cin);
	Quad(4,5,6,7, vin, cin);
	Quad(5,4,0,1, vin, cin);
}


//배열변수에 P M 행렬 저장
void MyCube::getModelProjMatrix(mat4 m_mat, mat4 p_mat)
{



	int n1 = 0;
	int n2 = 0;
	//16배열에 저장
	for(int i=0; i<4; i++)
		for(int k=0; k<4; k++)
		{
			modelMat[n1++] = m_mat[k][i];
			projMat[n2++] = p_mat[k][i];
		}

	//Get Viewport Matrix
	glGetIntegerv(GL_VIEWPORT, vportMat);
	


}




void MyCube::getZvalues(vec2 vertices[12], int width, int height)
{
	GLfloat H = 0.0f;
	tvertices = new vec3[16];


	for(int i=0; i<12; i++)
	{

		//Unprojection

		//printf("%f, %f\n", vertices[i].x, vertices[i].y);
		
		//_1	
		GLdouble objX_1, objY_1, objZ_1;
		GLdouble winX_1, winY_1, winZ_1;
		winX_1 = (vertices[i].x)*width;
		winY_1 = ((vertices[i].y)*(-1)+1)*height;
		winZ_1 = 2;

		GLuint uproj = gluUnProject(winX_1, winY_1, winZ_1, modelMat, projMat, vportMat, &objX_1, &objY_1, &objZ_1);

		//printf("Model Coord[1] = %f %f %f\n", winX_1, winY_1, winZ_1);
		//printf("Model Coord[1] = %f %f %f\n", objX_1, objY_1, objZ_1);

		//_2
		GLdouble objX_2, objY_2, objZ_2;
		GLdouble winX_2, winY_2, winZ_2;
		winX_2 = (vertices[i].x)*width;
		winY_2 = ((vertices[i].y)*(-1)+1)*height;
		winZ_2 = 1;

		uproj = gluUnProject(winX_2, winY_2, winZ_2, modelMat, projMat, vportMat, &objX_2, &objY_2, &objZ_2);
		//printf("Model Coord[2] = %f %f %f\n\n", objX_2, objY_2, objZ_2);


		//if(!uproj) 
		//	printf("Failure UnProject.\n");
		//else
			//printf("Model Coord = %f %f %f\n", winX, winY, winZ);

		/*
		//Projection (확인)
		GLdouble *n_winX, *n_winY, *n_winZ;
		GLdouble o_X, o_Y, o_Z;
		n_winX = new GLdouble;
		n_winY = new GLdouble;
		n_winZ = new GLdouble;

		 uproj = gluProject(*objX_2, *objY_2, *objZ_2, modelMat, projMat, vportMat, n_winX, n_winY, n_winZ);

		//if(!uproj) printf("Failure Projection.\n");
	
		//else
		//{
			printf("전 = %f %f %f\n", winX_2, winY_2, winZ_2);
			printf("확인 = %f %f %f\n\n", *n_winX, *n_winY, *n_winZ);
		//}
		*/

		//Equation
		GLfloat x,y,z;
		x = y = z = 0.0f;
		GLfloat k = 0.0f;

		vec3 p = vec3(objX_1, objY_1, objZ_1);
		vec3 q = vec3(objX_2, objY_2, objZ_2);
		vec3 r = vec3(0,0,0) ;
	

		//printf("P[%d] %f %f %f\n", i, p.x, p.y, p.z);
		//printf("Q[%d] %f %f %f\n", i, q.x, q.y, q.z);
		//printf("%f %f %f\n", r.x, r.y, r.z);

		// using y = 0 (get k)
		if(i < 6)
		{
			r = vec3(x, 0, z);
			k=(-p.y)/(q.y-p.y);
		
			//printf("p=%f\n", (-p.y)/(q.y-p.y));
			//printf("%d] k = %f %f %f\n", i, p.x, p.y, k);
		}
		// using x = 0 (get H)
		else if(i == 6)
		{	
			r = vec3(tvertices[0].x, H, tvertices[0].z);
			k = (r.x-p.x)/(q.x-p.x);
			H = p.y + k * (q.y - p.y);
			printf("H = %f\n", H);
			/////printf("%d] k = %f %f\n", i, p.y, k*(q.y - p.y));
			//printf("%d] k = %f \n",i, tvertices[0]);
		}
		// using H 
		else if(i > 6 && i < 12)
		{
			r = vec3(x, H, y);
			k = (r.y-p.y)/(q.y-p.y);
			//printf("%d] k = %f\n", i, k);
		}

		//Recalculate ; Get Entire CoordinateValues
		r.x = p.x + k*(q.x-p.x);
		r.y = p.y + k*(q.y-p.y);
		r.z = p.z + k*(q.z-p.z);

		//printf("%f *  %f %f\n", k,  (q.x-p.x), k*(q.x-p.x));

		//printf("k = %f\n", k);
		//printf("[%d]p x = %f y = %f z = %f\n",i, p.x, p.y, p.z);
		//printf("[%d]q x = %f y = %f z = %f\n",i, q.x, q.y, q.z);
		//printf("[%d]r x = %f y = %f z = %f\n",i, r.x, r.y, r.z);

		tvertices[i] = r;

		
		////Projection (확인)
		//GLdouble *n_winX, *n_winY, *n_winZ;
		//n_winX = new GLdouble;
		//n_winY = new GLdouble;
		//n_winZ = new GLdouble;

		//uproj = gluProject(tvertices[i].x, tvertices[i].y, tvertices[i].z, modelMat, projMat, vportMat, n_winX, n_winY, n_winZ);

		////if(!uproj) printf("Failure Projection.\n");
		//
		////else
		////{
		//	//printf("전 = %f %f %f\n", winX_1, winY_1, winZ_1);
		//	//printf("확인 = %f %f %f\n\n", *n_winX, *n_winY, *n_winZ);
		////}

	}


	

}



GLuint MyCube::Init(int n, float ratio, vec2 *rectVertices)
{
	// The Cube should be initialized only once;
	if(bInitialized == true) return vao;
	vec4 vertex_positions[8];
	num = n;
	Rvertices = rectVertices;
	
	//printf("x = %f y = %f\n", Rvertices[num][0], Rvertices[num][1]);



	switch(num){
		case 0:{
		vertex_positions[0] = vec4( 0, -0.025,  0, 1.0 );
		vertex_positions[1] = vec4( 0,  0,  0, 1.0 );
		vertex_positions[2] = vec4(  ratio,  0,  0, 1.0 );
		vertex_positions[3] = vec4(  ratio, -0.025,  0, 1.0 );
		vertex_positions[4] = vec4( 0, -0.025,  1, 1.0 );
		vertex_positions[5] = vec4( 0,  0,  1, 1.0 );
		vertex_positions[6] = vec4(  ratio,  0,  1, 1.0 );
		vertex_positions[7] = vec4(  ratio, -0.025,  1, 1.0 );
		break;
			  }

		case 1:
			{
		vertex_positions[0] = vec4( 0, 1,  0.025, 1.0 );
		vertex_positions[1] = vec4( 0,  0,  0.025, 1.0 );
		vertex_positions[2] = vec4(  ratio,  0,  0.025, 1.0 );
		vertex_positions[3] = vec4(  ratio, 1,  0.025, 1.0 );
		vertex_positions[4] = vec4( 0, 1,  -0.025, 1.0 );
		vertex_positions[5] = vec4( 0,  0,  -0.025, 1.0 );
		vertex_positions[6] = vec4(  ratio,  0,  -0.025, 1.0 );
		vertex_positions[7] = vec4(  ratio,   1,  -0.025, 1.0 );			
		break;	
			}

		case 2:
			{
		vertex_positions[0] = vec4( -1, -1,  1, 1.0 );
		vertex_positions[1] = vec4( -1,  -0.9,  1, 1.0 );
		vertex_positions[2] = vec4(  1,  -0.9,  1, 1.0 );
		vertex_positions[3] = vec4(  1, -1,  1, 1.0 );
		vertex_positions[4] = vec4( -1, -1,  -5, 1.0 );
		vertex_positions[5] = vec4( -1,  -0.9,  -5, 1.0 );
		vertex_positions[6] = vec4(  1,  -0.9,  -5, 1.0 );
		vertex_positions[7] = vec4(  1, -1,  -5, 1.0 );					
		break;	
			}
			
		case 3:
			{
		vertex_positions[0] = vec4( -1, -1,  1, 1.0 );
		vertex_positions[1] = vec4( -1,  -0.9,  1, 1.0 );
		vertex_positions[2] = vec4(  1,  -0.9,  1, 1.0 );
		vertex_positions[3] = vec4(  1, -1,  1, 1.0 );
		vertex_positions[4] = vec4( -1, -1,  -5, 1.0 );
		vertex_positions[5] = vec4( -1,  -0.9,  -5, 1.0 );
		vertex_positions[6] = vec4(  1,  -0.9,  -5, 1.0 );
		vertex_positions[7] = vec4(  1, -1,  -5, 1.0 );					
		break;	
			}
	}


	vec4 vertex_colors[8] ={
		vec4( 1, 1, 0.6, 1),
		vec4( 1, 1, 0.6, 1),
		vec4( 1, 1, 0.6, 1),
		vec4( 1, 1, 0.6, 1),
		vec4( 1, 1, 0.6, 1),
		vec4( 1, 1, 0.6, 1),
		vec4( 1, 1, 0.6, 1),
		vec4( 1, 1, 0.6, 1)
	};


	ColorCube(vertex_positions, vertex_colors);

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
	
	bInitialized = true;
	return vao;
}

void MyCube::SetPositionAndColorAttribute(GLuint program)
{
	GLuint vPosition = glGetAttribLocation(program, "vPosition");
	glEnableVertexAttribArray(vPosition);
	glVertexAttribPointer(vPosition, 4, GL_FLOAT, GL_FALSE, sizeof(MyCubeVertex), BUFFER_OFFSET(0));

	GLuint vColor = glGetAttribLocation(program, "vColor");
	glEnableVertexAttribArray(vColor);
	glVertexAttribPointer(vColor, 4, GL_FLOAT, GL_FALSE, sizeof(MyCubeVertex), BUFFER_OFFSET(sizeof(vec4)));

	GLuint vTexCoord = glGetAttribLocation(program, "vTexCoord");
	glEnableVertexAttribArray(vTexCoord);
	glVertexAttribPointer(vTexCoord, 2, GL_FLOAT, GL_FALSE, sizeof(MyCubeVertex), BUFFER_OFFSET(sizeof(vec4)+sizeof(vec4)));
}


void MyCube::Draw(GLuint program)
{
	if(!bInitialized) return;			// check whether it is initiazed or not. 
	
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	SetPositionAndColorAttribute(program);
	
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
}