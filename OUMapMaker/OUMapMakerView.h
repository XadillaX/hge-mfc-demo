// OUMapMakerView.h : OUMapMakerView 类的接口
//


#pragma once


class OUMapMakerView : public CView
{
protected: // 仅从序列化创建
	OUMapMakerView();
	DECLARE_DYNCREATE(OUMapMakerView)

// 属性
public:
	OUMapMakerDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~OUMapMakerView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    HGE*                        m_pHGE;
    HANDLE                      m_hThreadHandle;

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnClose();
    afx_msg void OnSize(UINT nType, int cx, int cy);
};

#ifndef _DEBUG  // OUMapMakerView.cpp 中的调试版本
inline OUMapMakerDoc* OUMapMakerView::GetDocument() const
   { return reinterpret_cast<OUMapMakerDoc*>(m_pDocument); }
#endif

