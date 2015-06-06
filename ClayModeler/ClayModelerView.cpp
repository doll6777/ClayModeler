
// ClayModelerView.cpp : implementation of the CClayModelerView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ClayModeler.h"
#endif

#include "ClayModelerDoc.h"
#include "ClayModelerView.h"

#include <vgl.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CClayModelerView

IMPLEMENT_DYNCREATE(CClayModelerView, CView)

BEGIN_MESSAGE_MAP(CClayModelerView, CView)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEHWHEEL()
	ON_COMMAND(ID_EDIT_PUSH, &CClayModelerView::OnEditPush)
	ON_COMMAND(ID_EDIT_PULL, &CClayModelerView::OnEditPull)
	ON_COMMAND(ID_EDIT_TWIST, &CClayModelerView::OnEditTwist)
	ON_WM_MOUSEWHEEL()
END_MESSAGE_MAP()

// CClayModelerView construction/destruction

CClayModelerView::CClayModelerView()
{
	// TODO: add construction code here
	m_hRC = 0;
	m_pDC = 0;

	m_CX = 0;
	m_CY = 0;
	
	m_Aspect = 1.0f;
	m_bToolMode = false;
}

CClayModelerView::~CClayModelerView()
{
}
void CClayModelerView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	X_Angle = 0.0;
	Y_Angle = 0.0;
	m_Rotate = mat4(1.0f);
	m_Trans = vec3(0.0f,0.0f,0.0f);

	m_Size = 2.0f;

	m_Far = m_Size*100.0f;
	m_Near = -m_Size*100.01f;

	m_bRBut = false;
	m_bMBut = false;
	m_bLBut = false;

	m_bShowLightSource = true;

}
BOOL CClayModelerView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CClayModelerView drawing

void CClayModelerView::OnDraw(CDC* /*pDC*/)
{
	CClayModelerDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

		DrawScene();	

	// TODO: add draw code for native data here
}

// CClayModelerView diagnostics

#ifdef _DEBUG
void CClayModelerView::AssertValid() const
{
	CView::AssertValid();
}

void CClayModelerView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CClayModelerDoc* CClayModelerView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CClayModelerDoc)));
	return (CClayModelerDoc*)m_pDocument;
}
#endif //_DEBUG


// CClayModelerView message handlers


int CClayModelerView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	m_pDC = new CClientDC(this);
	
	if(NULL == m_pDC)
	{
		::AfxMessageBox(_T("Can not get DC. \n"));
		return FALSE;
	}
	if(!SetupPixelFormat())
	{
		::AfxMessageBox(_T("SetupPixelFormat failed.\n"));
		return FALSE;
	}
	if(0==(m_hRC = wglCreateContext(m_pDC->GetSafeHdc())))
	{
		::AfxMessageBox(_T("wglCreateContext failed.\n"));
		return FALSE;
	}
	if(FALSE == wglMakeCurrent(m_pDC->GetSafeHdc(), m_hRC))
	{
		::AfxMessageBox(_T("wglMakeCurrent failed.\n"));
		return FALSE;
	}

	CClayModelerDoc* pDoc = (CClayModelerDoc*) GetDocument();

	pDoc->mesh.DoSubDivide();
	pDoc->mesh.DoSubDivide();
	pDoc->mesh.DoSubDivide();
	
	return 0;
}


void CClayModelerView::OnDestroy()
{
	CView::OnDestroy();
	
	// TODO: Add your message handler code here
	if(FALSE == wglMakeCurrent(0,0))
	{
		::AfxMessageBox(_T("wglMakeCurrent failed.\n"));
	}
	if(m_hRC && (FALSE == wglDeleteContext(m_hRC)))
	{
		::AfxMessageBox(_T("WglDeleteContext failed.\n"));
	}
	if(m_pDC)
		delete m_pDC;	
}


BOOL CClayModelerView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: Add your message handler code here and/or call default

//	return CView::OnEraseBkgnd(pDC);
	return true;
}

BOOL CClayModelerView::SetupPixelFormat( PIXELFORMATDESCRIPTOR* pPFD)
{
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER
		,
		PFD_TYPE_RGBA,
		24,
		0,0,0,0,0,0,
		8,
		0,
		0,
		0,0,0,0,
		16,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};
	int pixelformat;

	PIXELFORMATDESCRIPTOR* pPFDtoUse;

	pPFDtoUse = (0 == pPFD)? &pfd : pPFD;

	if(0==(pixelformat = 
		::ChoosePixelFormat(m_pDC->GetSafeHdc(),pPFDtoUse)))
	{
		::AfxMessageBox(_T("ChoosePixelFormat failed."));
			return FALSE;
	}

	if(FALSE == ::SetPixelFormat( m_pDC->GetSafeHdc(),
		pixelformat,pPFDtoUse) )
	{
		::AfxMessageBox(_T("SetPixelFormat failed."));
		return FALSE;
	}
	return TRUE;
}

void CClayModelerView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	m_CX = cx;
	m_CY = cy;

	m_Aspect=(float)cx/(float)cy;
	glViewport(0,0,cx,cy);
}

float bg_Color[] = { 0.2f, 0.2f, 0.2f};


void CClayModelerView::DrawScene()
{
	glEnable(GL_DEPTH_TEST);
	//glClearColor(bg_Color[0], bg_Color[1], bg_Color[2], 1.0f);

	glClear(GL_DEPTH_BUFFER_BIT);

	//------------------------- Draw BackGround Gradient
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-1, 1, -1, 1, m_Near, m_Far);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glDisable(GL_LIGHTING);
	glBegin(GL_QUADS);
	glColor3f(0.8f, 0.8f, 0.8f);
	glVertex2f(-1, 1);
	glVertex2f(1, 1);
	glColor3f(0.3f, 0.3f, 0.3f);
	glVertex2f(1, -1);
	glVertex2f(-1, -1);
	glEnd();
	//---------------------------------------------------


	//----------------------------Draw Model-------------
	glClear(GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(m_Size<0.0f) m_Size = 0.0f;
	glOrtho(-m_Size,m_Size,-m_Size/m_Aspect,m_Size/m_Aspect,m_Near, m_Far);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
		
	glPolygonMode(GL_FRONT,GL_FILL);

	glTranslatef(0.0f,0.0f,-20.0f);

	DrawSettingLight();

	GLfloat rot[16];
	for(int i=0; i<4; i++)
		for(int j=0; j<4; j++)
			rot[i*4+j] = m_Rotate[i][j];

	glMultMatrixf(rot);

	DrawAxis(1, 1.0f);
	
	glTranslatef(m_Trans[0], m_Trans[1], m_Trans[2]);

	DrawObjects();
	//-----------------------------------------------------
	glFlush();





	SwapBuffers(m_pDC->m_hDC);
}



void CClayModelerView::DrawAxis(int mode, float scale)
{
	GLUquadricObj*	q = gluNewQuadric();
	gluQuadricDrawStyle(q, GLU_FILL);
	gluQuadricNormals(q, GLU_SMOOTH);

	glPushMatrix();

	if(mode == 0)
	{
		glTranslatef(-(10.0f-1.0f), -(10.0f)/m_Aspect+1.0f, 0.0f);
		
		GLfloat rot[16];
		for(int i=0; i<4; i++)
			for(int j=0; j<4; j++)
				rot[i*4+j] = m_Rotate[i][j];

		glMultMatrixf(rot);

	}
		glDisable(GL_LIGHTING);
		glBegin(GL_LINES);
			glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(1.0f*scale,0.0f,0.0f);
			glColor3f(0.0f,1.0f,0.0f);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(0.0f,1.0f*scale,0.0f);
			glColor3f(0.0f,0.0f,1.0f);
			glVertex3f(0.0f,0.0f,0.0f);
			glVertex3f(0.0f,0.0f,1.0f*scale);
		glEnd();
		glEnable(GL_LIGHTING);

		glPushMatrix();
			glTranslatef(0.0f,0.0f,1.0f*scale);
			gluCylinder(q, 0.1*scale,0.0,0.2*scale, 8,1);	
		glPopMatrix();

		glPushMatrix();
			glTranslatef(1.0f*scale,0.0f,0.0f);
			glColor3f(1.0f,0.0f,0.0f);
			glRotatef(90.0f,0.0f,1.0f,0.0f);
			gluCylinder(q, 0.1*scale,0.0,0.2*scale, 8,1);	
		glPopMatrix();

		glPushMatrix();
			glTranslatef(0.0f,1.0f*scale,0.0f);
			glColor3f(0.0f,1.0f,0.0f);
			glRotatef(-90.0f,1.0f,0.0f,0.0f);
			gluCylinder(q, 0.1*scale,0.0,0.2*scale, 8,1);	
		glPopMatrix();
	
	glPopMatrix();

	gluDeleteQuadric(q);
}


void CClayModelerView::DrawSettingLight(void)
{
	GLfloat global_ambient[] = {0.4f,0.4f,0.4f,1.0f};
	
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
	glShadeModel(GL_SMOOTH);

	GLfloat ambientLight0[] = {0.0f,0.0f,0.0f,1.0f};
	GLfloat diffuseLight0[] = {1.0f,1.0f,1.0f,1.0f};
	GLfloat specularLight0[] = {1.0f,1.0f,1.0f,1.0f};
	GLfloat position0[] = {300.0f,300.0f,300.0f,0.0f};				// 45 degree to the vertical

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight0);
	glLightfv(GL_LIGHT0, GL_POSITION, position0);


	GLfloat ambientLight1[] = {0.0f,0.0f,0.0f,1.0f};
	GLfloat diffuseLight1[] = {0.3f,0.3f,0.4f,1.0f};
	GLfloat specularLight1[] = {0.3f,0.3f,0.4f,1.0f};
	GLfloat position1[] = {-300.0f,-300.0f,0.0f,0.0f};				// 45 degree to the vertical

	glLightfv(GL_LIGHT1, GL_AMBIENT, ambientLight1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseLight1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specularLight1);
	glLightfv(GL_LIGHT1, GL_POSITION, position1);


	glEnable(GL_FLAT);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);

	glEnable(GL_LIGHT0);
	glColorMaterial ( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
	glEnable(GL_COLOR_MATERIAL);

	GLfloat mspecular[] = {0.2f,0.2f,0.3f,1.0f};
	GLfloat memission[] = {0.0f,0.0f,0.0f,1.0f};

	glMaterialfv ( GL_FRONT, GL_SPECULAR, mspecular) ;
	glMaterialfv ( GL_FRONT, GL_EMISSION, memission) ;


	glMateriali (GL_FRONT, GL_SHININESS, 50);


	if(m_bShowLightSource)
	{
		GLUquadricObj* sphere;
		sphere = gluNewQuadric();

		glDisable(GL_LIGHTING);

		glPushMatrix();
		glTranslatef(position0[0], position0[1], position0[2]);
		glColor4fv(diffuseLight0);
		gluSphere(sphere, 2.00f,16,16);
		glPopMatrix();

		glPushMatrix();
		glTranslatef(position1[0], position1[1], position1[2]);
		glColor4fv(diffuseLight1);
		gluSphere(sphere, 2.00f,16,16);
		glPopMatrix();

		gluDeleteQuadric(sphere);	
		glEnable(GL_LIGHTING);

	}
}

void CClayModelerView::DrawObjects(void)
{
	CClayModelerDoc* pDoc = GetDocument();
	pDoc->mesh.Draw();
}


void CClayModelerView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	m_bLBut = true;
	SetCapture();	
		
	if((nFlags & MK_CONTROL) != MK_CONTROL)
	{
		CClayModelerDoc* pDoc = GetDocument();
		
		float new_pt[2];
		new_pt[0] = point.x;
		new_pt[1] = point.y;
		
		pDoc->mesh.findPickedPoint(new_pt);
		pDoc->mesh.CreateParticle();
		pDoc->mesh.FindModifyVertices();
		m_bToolMode = true;
	}
	
		MouseDownPoint=point;
		Invalidate(true);
	
	CView::OnLButtonDown(nFlags, point);
}


void CClayModelerView::OnLButtonUp(UINT nFlags, CPoint point)
{
	CClayModelerDoc* pDoc = GetDocument();
	pDoc->mesh.ClearVectors();

	MouseDownPoint=CPoint(0,0);
	m_bLBut = false;
	m_bToolMode = false;
	
	// release mouse capture
	ReleaseCapture();
	CView::OnLButtonUp(nFlags, point);
}


void CClayModelerView::OnMButtonDown(UINT nFlags, CPoint point)
{
	MouseDownPoint=point;
	{
		m_bMBut = true;
		SetCapture();	
	}
	CView::OnMButtonDown(nFlags, point);
}


void CClayModelerView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	MouseDownPoint=CPoint(0,0);
	m_bMBut = false;

	// release mouse capture
	ReleaseCapture();
	CView::OnMButtonUp(nFlags, point);
	Invalidate();
}


void CClayModelerView::OnRButtonDown(UINT nFlags, CPoint point)
{
	{	
		MouseDownPoint=point;	
		m_bRBut = true;							// for Zooming in and out
		SetCapture();	
	}
	CView::OnRButtonDown(nFlags, point);
}

void CClayModelerView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	MouseDownPoint=CPoint(0,0);
	m_bRBut = false;
// release mouse capture
	ReleaseCapture();
}

void CClayModelerView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}

void CClayModelerView::OnMouseMove(UINT nFlags, CPoint point)
{
	CClayModelerDoc* pDoc = GetDocument();


	Invalidate(TRUE);

	// Ray Picking
	// Show Picking Point

	// 한점을 잡고 끄는 상태일 때가 아니면.
	if(!m_bLBut)
	{
		float new_pt[2];
		new_pt[0] = point.x;
		new_pt[1] = point.y;
		pDoc->mesh.findPickedPoint(new_pt);
		pDoc->mesh.FindModifyVertices();
		
		Invalidate(TRUE);
	}

	// TODO: Add your message handler code here and/or call default
	if (GetCapture()==this)
	{
		if(m_bRBut == true)
		{
			m_Size += double (point.y-MouseDownPoint.y)/10.0f*m_Size*0.01f;
			
			Invalidate();
			MouseDownPoint=point;
		}
		else if(m_bMBut == true)
		{
			float px1, px2, py1, py2, pz1, pz2;
			double wx1, wx2, wy1, wy2, wz1, wz2;
			px1 = point.x;
			px2 = MouseDownPoint.x;
			py1 = point.y;
			py2 = MouseDownPoint.y;
			pz1 = 0.0f;
			pz2 = 0.0f;

			GLint viewport[4];
			GLdouble mvmatrix[16], projmatrix[16];

			glGetIntegerv	(GL_VIEWPORT, viewport);
			glGetDoublev	(GL_MODELVIEW_MATRIX, mvmatrix);
			glGetDoublev	(GL_PROJECTION_MATRIX, projmatrix);

			py1 = viewport[3] - (GLint) py1 - 1;
			py2 = viewport[3] - (GLint) py2 - 1;

			gluUnProject ((GLdouble) px1, (GLdouble) py1, pz1,
					mvmatrix, projmatrix, viewport, &wx1, &wy1, &wz1);
			gluUnProject ((GLdouble) px2, (GLdouble) py2, pz2,
					mvmatrix, projmatrix, viewport, &wx2, &wy2, &wz2);
			

			m_Trans += vec3(wx1-wx2, wy1-wy2, wz1-wz2);
			
			MouseDownPoint=point;
			Invalidate();
		}


		else if(m_bLBut && (nFlags & MK_CONTROL) == MK_CONTROL)				/// L button
		{
			int ySize = 60;
			// increment the object rotation angles
			
			if(MouseDownPoint.y<ySize || m_CY-MouseDownPoint.y<ySize)
			{
				float zangle;
				zangle = float(point.x-MouseDownPoint.x)/3.6f;
				if(m_CY/2-MouseDownPoint.y>0) zangle = -zangle;
				
				mat4 rot = RotateZ(-zangle);
				m_Rotate = m_Rotate*rot;
			}
			else
			{
				vec3 vec_x, vec_y, vec_xy;

				float xangle, yangle;

				xangle = float(point.y-MouseDownPoint.y)/1.0f;
				yangle = float(point.x-MouseDownPoint.x)/1.0f;

				mat4 rot = RotateY(-yangle)*RotateX(-xangle);
				m_Rotate = m_Rotate*rot;
			}

		// redraw the view
			Invalidate(TRUE);
		// remember the mouse point
			MouseDownPoint=point;
		}

		// 한점을 잡고 끄는 상태이면. ( Mesh Update 필요 )
		else if(m_bLBut && (nFlags & MK_CONTROL) != MK_CONTROL)				/// L button
		{
			pDoc->mesh.DoDeformation();
			if(pDoc->mesh.m_toolNum != CMesh::TOOL_MODE_TWIST)
				pDoc->mesh.ParticlesMove();
			Invalidate(TRUE);
			MouseDownPoint=point;
		}
	};	
}


void CClayModelerView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CClayModelerDoc* pDoc = GetDocument();
	if(nChar == VK_SPACE)
	{
		pDoc->mesh.DoSubDivide();
	}
	if (nChar == '1')
	{
		pDoc->mesh.m_toolNum = 1;
	}
	if (nChar == '2')
	{
		pDoc->mesh.m_toolNum = 2;
	}
	if (nChar == '3')
	{
		pDoc->mesh.m_toolNum = 3;
	}
	Invalidate(true);
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


void CClayModelerView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}



void CClayModelerView::OnEditPush()
{
	CClayModelerDoc* pDoc = GetDocument();
	pDoc->mesh.m_toolNum = CMesh::TOOL_MODE_PUSH;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CClayModelerView::OnEditPull()
{
	CClayModelerDoc* pDoc = GetDocument();
	pDoc->mesh.m_toolNum = CMesh::TOOL_MODE_PULL;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


void CClayModelerView::OnEditTwist()
{
	CClayModelerDoc* pDoc = GetDocument();
	pDoc->mesh.m_toolNum = CMesh::TOOL_MODE_TWIST;
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}


BOOL CClayModelerView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	CClayModelerDoc* pDoc = GetDocument();
	if (zDelta > 0)
	{
		pDoc->mesh.m_toolRadius += 0.1f;
	}

	else
	{
		pDoc->mesh.m_toolRadius -= 0.1f;
		if (pDoc->mesh.m_toolRadius < 0.25f) pDoc->mesh.m_toolRadius = 0.25f;
	}
	

	if (m_bToolMode)
	{
		pDoc->mesh.CreateParticle();
		pDoc->mesh.FindModifyVertices();
	}
	Invalidate(true);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}
