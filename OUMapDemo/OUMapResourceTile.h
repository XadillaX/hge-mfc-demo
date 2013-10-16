#ifndef __OUMAPRESOURCETILE_H__
#define __OUMAPRESOURCETILE_H__
#pragma once
#include "oumapresourceobject.h"
#include <string>
using namespace std;

#define OUMAP_RES_PATH_TILE     "tile\\road\\tile"

#define OUMAP_RES_MAX_EXT       5
const char g_szMapResExt[5][5] = {
    ".oga", ".png", ".jpg", ".tga", ".bmp"
};

class OUMapResourceTile : public OUMapResourceObject
{
public:
    OUMapResourceTile(void);
    virtual ~OUMapResourceTile(void);

    virtual bool                Load(DWORD dwID);
    virtual void                Update(float fDeltaTime);
    virtual void                Render(float fX, float fY);
    virtual void                RenderStretch(float x1, float y1, float w, float h);
    
    virtual void                SetColor(DWORD dwColor);
    virtual DWORD               GetColor();

    virtual void                SetZ(float z);
    virtual float               GetZ();

    void                        Release();

protected:
    virtual const char*         MakeFilePath(DWORD dwID, const char* szExt);

protected:
    POUGUI_IMG                  m_pImg;
    HTEXTURE                    m_hTex;
};

#endif
