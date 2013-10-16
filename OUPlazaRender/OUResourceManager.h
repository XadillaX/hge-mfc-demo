/***********************************
 *
 * O游棋牌渲染系统
 * - 资源管理类 -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OURESOURCEMANAGER_H__
#define __OURESOURCEMANAGER_H__
#pragma once
#include "hge.h"
#include <map>
#include <string>
#include "x2singleton.h"
#include "CXadillaXPack.h"
#include "OUStringFunc.h"
using namespace std;

#ifndef _DEBUG
#pragma comment(lib, "XadillaXPack.lib")
#else
#pragma comment(lib, "XadillaXPack_d.lib")
#endif

typedef DWORD HRESMGR;
typedef tagXadillaXPackFileData OUPACKFILEDATA;
typedef tagXadillaXPackFileData *POUPACKFILEDATA;

class OURENDER_EXPORT OUResourceManager
{
public:
    OUResourceManager(void);

    bool                        OpenPackage(string& szFilename, string& szKey);
    bool                        IsOpened();
    OUPACKFILEDATA&             GetFile(string& szFilename);

private:
    string                      m_szFilename;
    string                      m_szKey;
    CXadillaXPack               m_Pack;
};

class OURENDER_EXPORT OUResourcePool : public X2Singleton<OUResourcePool>
{
public:
    OUResourcePool();
    virtual ~OUResourcePool();

public:
    HRESMGR                     OpenPackage(string& szFilename, string& szKey);
    OUPACKFILEDATA              GetResource(string& szFilename);
    OUPACKFILEDATA              GetResource(HRESMGR hMgr, string& szFilename);
    OUPACKFILEDATA              GetResourceUnpacked(string& szFilename);

    size_t                      GetFilesizeUnpacked(string& szFilename);

private:
    map<HRESMGR, OUResourceManager*> m_Manager;

    map<string, OUPACKFILEDATA> m_DataCache;
};
#endif
