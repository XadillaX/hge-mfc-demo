// OUWorld.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// COUWorldApp:
// �йش����ʵ�֣������ OUWorld.cpp
//

class COUWorldApp : public CWinApp
{
public:
	COUWorldApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern COUWorldApp theApp;
