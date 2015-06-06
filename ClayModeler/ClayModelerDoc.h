
// ClayModelerDoc.h : interface of the CClayModelerDoc class
//


#pragma once
#include "Mesh.h"
#include "Particle.h"

class CClayModelerDoc : public CDocument
{
protected: // create from serialization only
	CClayModelerDoc();
	DECLARE_DYNCREATE(CClayModelerDoc)

// Attributes
public:
	CMesh mesh;
	CMesh mesh_origin;


// Operations
public:
	void CreateOriginMesh();
// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
#ifdef SHARED_HANDLERS
	virtual void InitializeSearchContent();
	virtual void OnDrawThumbnail(CDC& dc, LPRECT lprcBounds);
#endif // SHARED_HANDLERS

// Implementation
public:
	virtual ~CClayModelerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()

#ifdef SHARED_HANDLERS
	// Helper function that sets search content for a Search Handler
	void SetSearchContent(const CString& value);
#endif // SHARED_HANDLERS
public:
	afx_msg void OnViewWireframemode();
};
