#include "StdAfx.h"
#include "Particle.h"


CParticle::CParticle(void)
{
}


CParticle::~CParticle(void)
{
}

void CParticle::DrawParticle()
{
	GLUquadricObj* sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	glColor3f(0,1,0);

	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	gluSphere(sphere, 0.1 ,6,4); 
	glPopMatrix();

	gluDeleteQuadric(sphere);

	//glLineWidth(2.0f);
	//glColor3f(0,0,1);
	//glBegin(GL_LINES);
	//glVertex3f(position.x, position.y, position.z);
	//glVertex3f(m_dir.x+position.x, m_dir.y+position.y, m_dir.z+position.z);
	//glEnd();





}