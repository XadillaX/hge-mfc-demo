
// OUMapDemo.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// COUMapDemoApp:
// �йش����ʵ�֣������ OUMapDemo.cpp
//

class COUMapDemoApp : public CWinAppEx
{
public:
	COUMapDemoApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern COUMapDemoApp theApp;