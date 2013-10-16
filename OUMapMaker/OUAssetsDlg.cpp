// OUAssetsDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "OUMapMaker.h"
#include "OUAssetsDlg.h"


// OUAssetsDlg 对话框

IMPLEMENT_DYNAMIC(OUAssetsDlg, CDialog)

OUAssetsDlg::OUAssetsDlg(CWnd* pParent /*=NULL*/)
	: CDialog(OUAssetsDlg::IDD, pParent)
{

}

OUAssetsDlg::~OUAssetsDlg()
{
}

void OUAssetsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(OUAssetsDlg, CDialog)
END_MESSAGE_MAP()


// OUAssetsDlg 消息处理程序
