
// OUMapMakerEXDoc.cpp : OUMapMakerEXDoc ���ʵ��
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


// OUMapMakerEXDoc ����/����

OUMapMakerEXDoc::OUMapMakerEXDoc() :
    firstnew(true)
{
	// TODO: �ڴ����һ���Թ������

}

OUMapMakerEXDoc::~OUMapMakerEXDoc()
{
}

#include "OUNewMapDlg.h"
BOOL OUMapMakerEXDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

    /** ��һ�δ����ĵ� */
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
    //ע�⣺"�޸Ĺ�"��ʶǰ�Ŀո�  
    //����ֹ"Save As"�Ի����е��ĵ�����������  
    CString strIndicator=" *";  
    if(!IsModified() && bModified)  
    {  
        //1���ĵ���ǰ��"û���޸Ĺ���"��������"�ѸĶ�����"  
        SetTitle(strTitle + strIndicator);  
    }  
    else if( IsModified() && !bModified)  
    {  
        //2���ĵ���ǰ��"�޸Ĺ���"��������"δ�޸ĵ�"  
        SetTitle( strTitle.Left(strTitle.GetLength()-strIndicator.GetLength() ) );  
    }  
    //ǿ�Ƹ��¿�ܴ��ڵı���(�⽫ʹ��ܴ�����ʾ�µı���)  
    UpdateFrameCounts();  
    //��󣬵��ñ�׼����  
    CDocument::SetModifiedFlag(bModified);  
}  


// OUMapMakerEXDoc ���л�

//void OUMapMakerEXDoc::Serialize(CArchive& ar)
//{
//	if (ar.IsStoring())
//	{
//		// TODO: �ڴ���Ӵ洢����
//	}
//	else
//	{
//		// TODO: �ڴ���Ӽ��ش���
//	}
//}


// OUMapMakerEXDoc ���

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


// OUMapMakerEXDoc ����

BOOL OUMapMakerEXDoc::OnSaveDocument(LPCTSTR lpszPathName)
{
    // TODO: �ڴ����ר�ô����/����û���
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

    // TODO:  �ڴ������ר�õĴ�������
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
//    // TODO: �ڴ����ר�ô����/����û���
//    MessageBox(NULL, "a", "b", 0);
//
//    CDocument::ReportSaveLoadException(lpszPathName, e, bSaving, nIDPDefault);
//}
