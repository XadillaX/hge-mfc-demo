#ifndef __OUMAPMAKEREXVIEW_H__
#define __OUMAPMAKEREXVIEW_H__

// OUMapMakerEXView.h : OUMapMakerEXView ��Ľӿ�
//
#pragma once
#include "OUMapMakerEXDoc.h"

struct OUMAPMAKER_CHANGE_DOC_SIZE
{
    SIZE                size;
    bool                used;
};


class OUMapMakerEXView : public CScrollView
{
protected: // �������л�����
	OUMapMakerEXView();
	DECLARE_DYNCREATE(OUMapMakerEXView)

// ����
public:
	OUMapMakerEXDoc*            GetDocument() const;

// ����
public:
    void                        SetScrollSize(int nWidth, int nHeight);

// ��д
public:
	virtual void                OnDraw(CDC* pDC);
	virtual BOOL                PreCreateWindow(CREATESTRUCT& cs);

protected:
	virtual BOOL                OnPreparePrinting(CPrintInfo* pInfo);
	virtual void                OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void                OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~OUMapMakerEXView();
#ifdef _DEBUG
	virtual void                AssertValid() const;
	virtual void                Dump(CDumpContext& dc) const;
#endif

protected:
    HGE*                        m_pHGE;
    HANDLE                      m_hThreadHandle;
    bool                        m_bHGEDlgCreated;
    OUMAPMAKER_CHANGE_DOC_SIZE  m_Size;

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void                OnFilePrintPreview();
	afx_msg void                OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void                OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()

public:
    afx_msg void OnClose();
    afx_msg void OnSize(UINT nType, int cx, int cy);
//    afx_msg BOOL OnEraseBkgnd(CDC* pDC);
    virtual void OnInitialUpdate();
protected:
    virtual void OnUpdate(CView* /*pSender*/, LPARAM /*lHint*/, CObject* /*pHint*/);
    virtual BOOL OnScrollBy(CSize sizeScroll, BOOL bDoScroll = TRUE);
public:
    virtual BOOL OnScroll(UINT nScrollCode, UINT nPos, BOOL bDoScroll = TRUE);
    afx_msg void OnPaint();
    afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
    afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // OUMapMakerEXView.cpp �еĵ��԰汾
inline OUMapMakerEXDoc* OUMapMakerEXView::GetDocument() const
   { return reinterpret_cast<OUMapMakerEXDoc*>(m_pDocument); }
#endif

#endif
