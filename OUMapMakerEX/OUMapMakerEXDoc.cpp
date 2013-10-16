
// OUMapMakerEXDoc.cpp : OUMapMakerEXDoc 类的实现
//

#include "stdafx.h"
#include "OUMapMakerEX.h"

#include "OUMapMakerEXDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// OUMapMakerEXDoc

IMPLEMENT_DYNCREATE(OUMapMakerEXDoc, CDocument)

BEGIN_MESSAGE_MAP(OUMapMakerEXDoc, CDocument)
END_MESSAGE_MAP()


// OUMapMakerEXDoc 构造/析构

OUMapMakerEXDoc::OUMapMakerEXDoc() :
    firstnew(true)
{
	// TODO: 在此添加一次性构造代码

}

OUMapMakerEXDoc::~OUMapMakerEXDoc()
{
}

#include "OUNewMapDlg.h"
BOOL OUMapMakerEXDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

    /** 第一次创建文档 */
    if(firstnew)
    {
        firstnew = false;
        m_DocSize = OUMapView::GetDocSize(DEFAULT_MAP_WIDTH, DEFAULT_MAP_HEIGHT, DEFAULT_TILE_WIDTH, DEFAULT_TILE_HEIGHT);
        
        return TRUE;
    }
    else
    {
        OUNewMapDlg newdlg;
        if(newdlg.DoModal() == IDOK)
        {
            OM_NEWMAP_PARAM* param2 = new OM_NEWMAP_PARAM;
            strcpy(param2->m_szName, newdlg.m_szMapName);
            param2->m_nWidth = newdlg.m_dwWidth;
            param2->m_nHeight = newdlg.m_dwHeight;
            param2->m_nTileWidth = newdlg.m_dwTileWidth;
            param2->m_nTileHeight = newdlg.m_dwTileHeight;
            g_pView->SendMessage(OM_NEWMAP, param2);

            m_DocSize = OUMapView::GetDocSize(newdlg.m_dwWidth, newdlg.m_dwHeight, newdlg.m_dwTileWidth, newdlg.m_dwTileHeight);
            
            SetTitle(param2->m_szName);
            this->SetModifiedFlag(false);
            return TRUE;
        }
        else return FALSE;
    }

	return TRUE;
}

void OUMapMakerEXDoc::SetModifiedFlag(BOOL bModified)  
{  
    //CDocument::SetModifiedFlag(bModified);
    //return;

    CString strTitle=GetTitle();  
    //注意："修改过"标识前的空格  
    //它防止"Save As"对话框中的文档名出现问题  
    CString strIndicator=" *";  
    if(!IsModified() && bModified)  
    {  
        //1、文档以前是"没有修改过的"，现在是"已改动过的"  
        SetTitle(strTitle + strIndicator);  
    }  
    else if( IsModified() && !bModified)  
    {  
        //2、文档以前是"修改过的"，现在是"未修改的"  
        SetTitle( strTitle.Left(strTitle.GetLength()-strIndicator.GetLength() ) );  
    }  
    //强制更新框架窗口的标题(这将使框架窗口显示新的标题)  
    UpdateFrameCounts();  
    //最后，调用标准函数  
    CDocument::SetModifiedFlag(bModified);  
}  


// OUMapMakerEXDoc 序列化

//void OUMapMakerEXDoc::Serialize(CArchive& ar)
//{
//	if (ar.IsStoring())
//	{
//		// TODO: 在此添加存储代码
//	}
//	else
//	{
//		// TODO: 在此添加加载代码
//	}
//}


// OUMapMakerEXDoc 诊断

#ifdef _DEBUG
void OUMapMakerEXDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void OUMapMakerEXDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// OUMapMakerEXDoc 命令

BOOL OUMapMakerEXDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    // TODO: 在此添加专用代码和/或调用基类
    if(g_pView)
    {
        if(g_pView->SaveMap(lpszPathName))
        {
            SetModifiedFlag(false);
            return TRUE;
        }
        else return FALSE;
    }

    return CDocument::OnSaveDocument(lpszPathName);
}

#include "global.h"
BOOL OUMapMakerEXDoc::OnOpenDocument(LPCTSTR lpszPathName)
{
    if (!CDocument::OnOpenDocument(lpszPathName))
        return FALSE;

    // TODO:  在此添加您专用的创建代码
    if(firstnew)
    {
        //firstnew = false;
        ::g_bFirstWillOpen = true;
        ::g_szFirstWillOpenName = lpszPathName;
        OnNewDocument();

        //MessageBox(NULL, "a", "b", 0);

        return true;
    }

    if(g_pView)
    {
        if(g_pView->OpenMap(lpszPathName))
        {
            this->SetModifiedFlag(false);
            m_DocSize = OUMapView::GetDocSize(g_pView->GetMapWidth(), g_pView->GetMapHeight(), g_pView->GetTileWidth(), g_pView->GetTileHeight());
           
            //g_pView->MoveArea(0, 0, false);

            return true;
        }
        else return false;
    }
    else return FALSE;

    return TRUE;
}

//void OUMapMakerEXDoc::ReportSaveLoadException(LPCTSTR lpszPathName, CException* e, BOOL bSaving, UINT nIDPDefault)
//{
//    // TODO: 在此添加专用代码和/或调用基类
//    MessageBox(NULL, "a", "b", 0);
//
//    CDocument::ReportSaveLoadException(lpszPathName, e, bSaving, nIDPDefault);
//}
