/***********************************
 *
 * O游棋牌渲染系统
 * - 字符串函数类 -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OUSTRINGFUNC_H__
#define __OUSTRINGFUNC_H__
#pragma once
#include "hge.h"
#include <string>
using namespace std;

class OURENDER_EXPORT OUStringFunc
{
public:
    OUStringFunc(void);
    OUStringFunc(const char* szString);
    OUStringFunc(string& szString);
    virtual ~OUStringFunc(void);

    void                        SetString(string& szString);
    string&                     GetString();

public:
    string&                     ToUppercase();
    string&                     ToLowercase();
    string&                     Trim();
    string&                     LeftTrim();
    string&                     RightTrim();
    string&                     GetExt();

    enum OUPathType
    {
        OUPT_UPPERCASE,
        OUPT_LOWERCASE,
        OUPT_NOCHANGE,
        
        OUPT_MAX
    };
    string&                     FormatPath(OUPathType oupt);

    static bool                 CaseInsensitiveEqual(string a, string b);

private:
    string                      m_szString;
};

#endif
