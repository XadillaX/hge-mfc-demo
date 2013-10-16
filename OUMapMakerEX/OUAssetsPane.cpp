// OUAssetsPane.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "OUMapMakerEX.h"
#include "OUAssetsPane.h"


// OUAssetsPane

IMPLEMENT_DYNAMIC(OUAssetsPane, CDockablePane)

OUAssetsPane::OUAssetsPane()
{

}

OUAssetsPane::~OUAssetsPane()
{
}


BEGIN_MESSAGE_MAP(OUAssetsPane, CDockablePane)
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()



// OUAssetsPane ��Ϣ�������



int OUAssetsPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  �ڴ������ר�õĴ�������
    // ����ѡ�����:
    if (!m_AssetsDlg.Create(IDD_OUASSETSDLG,this))
    {
        TRACE0("δ�ܴ������ѡ�����/n");
        return -1;      // δ�ܴ���
    }
    m_AssetsDlg.ShowWindow(SW_SHOW);

    return 0;
}

void OUAssetsPane::OnSize(UINT nType, int cx, int cy)
{
    CDockablePane::OnSize(nType, cx, cy);

    // TODO: �ڴ˴������Ϣ����������
    m_AssetsDlg.SetWindowPos(this, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
    m_AssetsDlg.ShowWindow(SW_SHOW);
}
