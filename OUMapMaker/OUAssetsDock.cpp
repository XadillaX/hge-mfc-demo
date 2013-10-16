// OUAssetsDock.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OUMapMaker.h"
#include "OUAssetsDock.h"


// OUAssetsDock

IMPLEMENT_DYNAMIC(OUAssetsDock, CDockablePane)

OUAssetsDock::OUAssetsDock()
{

}

OUAssetsDock::~OUAssetsDock()
{
}


BEGIN_MESSAGE_MAP(OUAssetsDock, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()



// OUAssetsDock ��Ϣ�������



int OUAssetsDock::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  �ڴ������ר�õĴ�������
    if(!m_AssetsDlg.Create(IDD_OUASSETSDLG, this))
    {
        TRACE0("δ�ܴ������ѡ����ڡ�\n");
        return -1;
    }

    m_AssetsDlg.ShowWindow(SW_SHOW);
    
    return 0;
}

void OUAssetsDock::OnSize(UINT nType, int cx, int cy)
{
    CDockablePane::OnSize(nType, cx, cy);

    // TODO: �ڴ˴������Ϣ����������
    m_AssetsDlg.SetWindowPos(this, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
    m_AssetsDlg.ShowWindow(SW_SHOW);
}
