
// OUMapMakerEXDoc.h : OUMapMakerEXDoc ��Ľӿ�
//


#pragma once


class OUMapMakerEXDoc : public CDocument
{
protected: // �������л�����
	OUMapMakerEXDoc();
	DECLARE_DYNCREATE(OUMapMakerEXDoc)

    bool                        firstnew;

// ����
public:

// ����
public:
    SIZE                        GetDocumentSize()               { return m_DocSize; }
    virtual void                SetModifiedFlag(BOOL bModified); 

// ��д
public:
	virtual BOOL OnNewDocument();
//	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~OUMapMakerEXDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
    /** �ĵ� */
    SIZE                        m_DocSize;

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
public:
    virtual BOOL OnSaveDocument(LPCTSTR lpszPathName);
    virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
//    virtual void ReportSaveLoadException(LPCTSTR lpszPathName, CException* e, BOOL bSaving, UINT nIDPDefault);
};


