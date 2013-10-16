// OUWorld.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "OUWorld.h"
#include "OUWorldDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// COUWorldApp

BEGIN_MESSAGE_MAP(COUWorldApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// COUWorldApp 构造

COUWorldApp::COUWorldApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 COUWorldApp 对象

COUWorldApp theApp;


// COUWorldApp 初始化

BOOL COUWorldApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();


	SetRegistryKey(_T("O游棋牌"));

	COUWorldDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	return FALSE;
}
