// OUWorld.cpp : ����Ӧ�ó��������Ϊ��
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


// COUWorldApp ����

COUWorldApp::COUWorldApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� COUWorldApp ����

COUWorldApp theApp;


// COUWorldApp ��ʼ��

BOOL COUWorldApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()�����򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();


	SetRegistryKey(_T("O������"));

	COUWorldDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	return FALSE;
}
