// OUMapMakerDoc.h : OUMapMakerDoc ��Ľӿ�
//


#pragma once


class OUMapMakerDoc : public CDocument
{
protected:
    bool                        m_bFirst;

protected: // �������л�����
	OUMapMakerDoc();
	DECLARE_DYNCREATE(OUMapMakerDoc)

// ����
public:

// ����
public:

// ��д
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ʵ��
public:
	virtual ~OUMapMakerDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	DECLARE_MESSAGE_MAP()
};


