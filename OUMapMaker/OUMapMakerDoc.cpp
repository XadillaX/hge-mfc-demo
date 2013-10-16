// OUMapMakerDoc.cpp : OUMapMakerDoc ���ʵ��
//

#include "stdafx.h"
#include "OUMapMaker.h"

#include "OUMapMakerDoc.h"
#include "OUNewMapDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// OUMapMakerDoc

IMPLEMENT_DYNCREATE(OUMapMakerDoc, CDocument)

BEGIN_MESSAGE_MAP(OUMapMakerDoc, CDocument)
END_MESSAGE_MAP()


// OUMapMakerDoc ����/����

OUMapMakerDoc::OUMapMakerDoc() :
    m_bFirst(true)
{

}

OUMapMakerDoc::~OUMapMakerDoc()
{
}

BOOL OUMapMakerDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

    if(m_bFirst)
    {
        m_bFirst = false;
    }
    else
    {
        OUNewMapDlg dlg;
        if(dlg.DoModal() == IDOK)
        {
        }
        else
        {
            return FALSE;
        }
    }

	return TRUE;
}




// OUMapMakerDoc ���л�

void OUMapMakerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}


// OUMapMakerDoc ���

#ifdef _DEBUG
void OUMapMakerDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void OUMapMakerDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// OUMapMakerDoc ����
