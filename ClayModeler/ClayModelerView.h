
// ClayModelerView.h : interface of the CClayModelerView class
//

#pragma once

#include <vgl.h>

#include "vec.h"
#include <mat.h>



class CClayModelerView : public CView
{
protected: // create from serialization only
	CClayModelerView();
	DECLARE_DYNCREATE(CClayModelerView)

// Attributes
public:
	CClayModelerDoc* GetDocument() const;

	HGLRC m_hRC;
	CDC* m_pDC;
	virtual BOOL SetupPixelFormat(PIXELFORMATDESCRIPTOR* pPFD =0);

	void bitmapInfo( int wi, int he, BITMAPINFO& DIBInfo, int& dx, int& dy );

	CPoint MouseDownPoint;
	double X_Angle;
	double Y_Angle;
	mat4 m_Rotate;
	vec3 m_Trans;
	
	int m_CX;
	int m_CY;

	float m_Aspect;
	float m_Size;
	float m_Far;
	float m_Near;

	bool m_bRBut;
	bool m_bMBut;
	bool m_bLBut;
	bool m_bToolMode;

	void DrawAxis(int mode=0, float scale=1.0f);
	void DrawScene();
	void DrawObjects(void);

	void DrawSettingLight(void);

	bool m_bShowLightSource;

	

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnInitialUpdate();
protected:

// Implementation
public:
	virtual ~CClayModelerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnEditPush();
	afx_msg void OnEditPull();
	afx_msg void OnEditTwist();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};

#ifndef _DEBUG  // debug version in ClayModelerView.cpp
inline CClayModelerDoc* CClayModelerView::GetDocument() const
   { return reinterpret_cast<CClayModelerDoc*>(m_pDocument); }
#endif

