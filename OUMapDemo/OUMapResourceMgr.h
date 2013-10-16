#ifndef __OUMAPRESOURCEMGR_H__
#define __OUMAPRESOURCEMGR_H__
#pragma once
#include "Global.h"
#include <map>
#include "OUMapResourceObject.h"
using namespace std;

typedef map<int, OUMapResourceObject*> MP_I_OURO;
typedef map<OUMAP_RES_TYPE, MP_I_OURO> OUMapResMap;

#include "OUMapResourceTile.h"
#include "OUMapResourceCarpet.h"

class OUMapResourceMgr
{
public:
    OUMapResourceMgr(void);
    ~OUMapResourceMgr(void);

    OUMapResourceObject*        GetImage(OUMAP_RES_TYPE type, int id);

    OUMapResourceTile*          GetTileImage(int id);
    OUMapResourceCarpet*        GetCarpetImage(int id);

private:
    bool                        LoadRes(OUMAP_RES_TYPE type, int id);

private:
    OUMapResMap                 m_ResMap;
};

#endif
