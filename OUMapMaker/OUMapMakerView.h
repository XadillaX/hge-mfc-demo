// OUMapMakerView.h : OUMapMakerView ��Ľӿ�
//


#pragma once


class OUMapMakerView : public CView
{
protected: // �������л�����
	OUMapMakerView();
	DECLARE_DYNCREATE(OUMapMakerView)

// ����
public:
	OUMapMakerDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~OUMapMakerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    HGE*                        m_pHGE;
    HANDLE                      m_hThreadHandle;

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnClose();
    afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // OUMapMakerView.cpp �еĵ��԰汾
inline OUMapMakerDoc* OUMapMakerView::GetDocument() const
   { return reinterpret_cast<OUMapMakerDoc*>(m_pDocument); }
#endif

