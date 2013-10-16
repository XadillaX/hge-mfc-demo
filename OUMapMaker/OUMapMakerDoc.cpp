// OUMapMakerDoc.cpp : OUMapMakerDoc 类的实现
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


// OUMapMakerDoc 构造/析构

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




// OUMapMakerDoc 序列化

void OUMapMakerDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// OUMapMakerDoc 诊断

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


// OUMapMakerDoc 命令
