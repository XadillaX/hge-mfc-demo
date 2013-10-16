// OUAssetsPane.cpp : 实现文件
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



// OUAssetsPane 消息处理程序



int OUAssetsPane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CDockablePane::OnCreate(lpCreateStruct) == -1)
        return -1;

    // TODO:  在此添加您专用的创建代码
    // 创建选项卡窗口:
    if (!m_AssetsDlg.Create(IDD_OUASSETSDLG,this))
    {
        TRACE0("未能创建输出选项卡窗口/n");
        return -1;      // 未能创建
    }
    m_AssetsDlg.ShowWindow(SW_SHOW);

    return 0;
}

void OUAssetsPane::OnSize(UINT nType, int cx, int cy)
{
    CDockablePane::OnSize(nType, cx, cy);

    // TODO: 在此处添加消息处理程序代码
    m_AssetsDlg.SetWindowPos(this, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
    m_AssetsDlg.ShowWindow(SW_SHOW);
}
