// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// OUMapMaker.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"


OUIniFile g_IniFile(::OUGetCurrentPath() + "\\config.ini");
string g_szCurSkinName("");

char* FormatSkinFilename(const char* name)
{
    static char filename[512];
    sprintf(filename, "skin\\%s", name);

    return filename;
}
