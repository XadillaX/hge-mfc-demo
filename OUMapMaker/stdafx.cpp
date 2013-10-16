// stdafx.cpp : 只包括标准包含文件的源文件
// OUMapMaker.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"


OUIniFile g_IniFile(::OUGetCurrentPath() + "\\config.ini");
string g_szCurSkinName("");

char* FormatSkinFilename(const char* name)
{
    static char filename[512];
    sprintf(filename, "skin\\%s", name);

    return filename;
}
