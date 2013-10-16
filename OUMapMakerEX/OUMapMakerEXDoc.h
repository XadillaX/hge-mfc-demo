
// OUMapMakerEXDoc.h : OUMapMakerEXDoc 类的接口
//


#pragma once


class OUMapMakerEXDoc : public CDocument
{
protected: // 仅从序列化创建
	OUMapMakerEXDoc();
	DECLARE_DYNCREATE(OUMapMakerEXDoc)

    bool                        firstnew;

// 属性
public:

// 操作
public:
    SIZE                        GetDocumentSize()               { return m_DocSize; }
    virtual void                SetModifiedFlag(BOOL bModified); 

// 重写
public:
	virtual BOOL OnNewDocument();
//	virtual void Serialize(CArchive& ar);

// 实现
public:
	virtual ~OUMapMakerEXDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    /** 文档 */
    SIZE                        m_DocSize;

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
//    virtual void ReportSaveLoadException(LPCTSTR lpszPathName, CException* e, BOOL bSaving, UINT nIDPDefault);
};


