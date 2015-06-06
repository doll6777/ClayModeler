
// ClayModelerDoc.cpp : implementation of the CClayModelerDoc class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "ClayModeler.h"
#endif

#include "ClayModelerDoc.h"

#include <propkey.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CClayModelerDoc

IMPLEMENT_DYNCREATE(CClayModelerDoc, CDocument)

BEGIN_MESSAGE_MAP(CClayModelerDoc, CDocument)
	ON_COMMAND(ID_VIEW_WIREFRAMEMODE, &CClayModelerDoc::OnViewWireframemode)
END_MESSAGE_MAP()


// CClayModelerDoc construction/destruction

CClayModelerDoc::CClayModelerDoc()
{
	// TODO: add one-time construction code here
	// mesh = CMesh();
	mesh.Init();
	CreateOriginMesh();
	mesh.CopyOtherMesh(mesh_origin);
}

CClayModelerDoc::~CClayModelerDoc()
{
}

BOOL CClayModelerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}

void CClayModelerDoc::CreateOriginMesh()
{
	mesh_origin.sphere_central = vec3(0,0,0);

	vec3 vc[6] = 
	{
		vec3(0,1,0), vec3(0,0,1), vec3(1,0,0), vec3(0,0,-1), vec3(-1,0,0), vec3(0,-1,0)
	};

	int id[24] =
	{
		0,1,2,  0,2,3,  0,3,4,  0,4,1, 
		5,2,1,  5,3,2,  5,4,3,  5,1,4
	};

	// vertex 추가
	for(int i=0; i<6; i++)
	{
		mesh_origin.vertices.push_back( CVertex( vc[i] ) );
		mesh_origin.vertices[ mesh_origin.vertices.size()-1 ].index = i;
	}

	// face와 엣지 추가
	for(int i=0; i<24; i+=3)
	{		
		CFace face = CFace(id[i], id[i+1], id[i+2]);
		mesh_origin.faces.push_back( face );
	}

	mesh_origin.BuildEdge();
	mesh_origin.sphere_central = vec3(0,0,0);
	mesh_origin.divLevel = 0;
}


// CClayModelerDoc serialization

void CClayModelerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

#ifdef SHARED_HANDLERS

// Support for thumbnails
void CClayModelerDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Modify this code to draw the document's data
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Support for Search Handlers
void CClayModelerDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Set search contents from document's data. 
	// The content parts should be separated by ";"

	// For example:  strSearchContent = _T("point;rectangle;circle;ole object;");
	SetSearchContent(strSearchContent);
}

void CClayModelerDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CClayModelerDoc diagnostics

#ifdef _DEBUG
void CClayModelerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CClayModelerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CClayModelerDoc commands


void CClayModelerDoc::OnViewWireframemode()
{
	mesh.m_bWireFrame = !mesh.m_bWireFrame;
	UpdateAllViews(NULL);
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}
