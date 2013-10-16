/***********************************
 *
 * O游棋牌渲染系统
 * - 配置文件类 -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OUINIFILE_H__
#define __OUINIFILE_H__
#pragma once
#include "hge.h"
#include <windows.h>
#include <string>
#include <map>
using namespace std;

class OURENDER_EXPORT OUIniFile
{
public:
    OUIniFile(void);
    OUIniFile(const char* filename) { SetFile(filename); }
    virtual ~OUIniFile(void);

    void                        SetFile(const char* filename);

    int                         GetInt(const char* sec, const char* key, int def = 0);
    string                      GetString(const char* sec, const char* key, const char* def = "");

    bool                        SetInt(const char* sec, const char* key, int val);
    bool                        SetString(const char* sec, const char* key, const char* val);

protected:
    std::string                 m_szFilename;

    map<string, map<string, string>> m_Map;
};

#endif
