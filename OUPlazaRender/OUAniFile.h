/***********************************
 *
 * O游棋牌渲染系统
 * - 动画文件包 -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OUANIFILE_H__
#define __OUANIFILE_H__
#pragma once
#include "hge.h"
#include <map>
#include <vector>
#include <hgeanim.h>
#include "OUResourceManager.h"
#include "OUStringFunc.h"
using namespace std;

#define OUAF_MAGIC_NUM 0x41474F58

struct ouaheader
{
    unsigned long magic_num;
    unsigned int fps;
    unsigned long pic_num;
    unsigned int width;
    unsigned int height;
    int spotx;
    int spoty;

    unsigned int clr_num;
    unsigned int clr_byte;
};

struct pixinfo
{
    unsigned int idx;
    unsigned short repeat;
};

struct ouaniinfo
{
    ouaheader header;
    HTEXTURE tex;
};

//////////////////////////////////////////////////////////////////////////////////////////////////
//
// 格式： Header (头, FPS，图片数量，宽高，热点xy，字典数量, 像素字节长) -> 字典 -> 各图片各像素 [idx, repeat]
//
//////////////////////////////////////////////////////////////////////////////////////////////////

class OURENDER_EXPORT OUAniFile
{
public:
    OUAniFile(void);
    virtual ~OUAniFile(void);

    /** 从 *.OGA 文件载入 */
    bool                        LoadFromFile(const char* filename)
    {
        SAFEDEL(m_pAnim);

        /** 在缓存中找 */
        OUStringFunc fn(filename);
        fn.ToUppercase();
        if(__OUANITEXTURE.find(fn.GetString()) != __OUANITEXTURE.end())
        {
            ouaniinfo info = __OUANITEXTURE[fn.GetString()];
            return LoadFromTexture(info.tex, info.header.width, info.header.height, info.header.pic_num, info.header.fps, info.header.spotx, info.header.spoty);
        }

        OUPACKFILEDATA data = OUResourcePool::instance().GetResource(string(filename));
        if(data.m_dwFileSize == 0) return false;
        
        if(LoadFromMemory((char*)data.m_pBuf, data.m_dwFileSize))
        {
            ouaniinfo info;
            info.header = m_Header;
            info.tex = m_hTex;
            __OUANITEXTURE[fn.GetString()] = info;
            return true;
        }
        else return false;
    }

    /** 从内存载入 */
    bool                        LoadFromMemory(char* mem, int size);
    /** 从纹理载入 */
    bool                        LoadFromTexture(HTEXTURE hTex, int nWidth, int nHeight, int count, int fps = 20, int hotspotx = 0, int hotspoty = 0);
    /** 获取动画元素 */
    hgeAnimation*               GetAnimation() { return m_pAnim; }

    static bool                 InitAll()
    {
        m_pHGE = hgeCreate(HGE_VERSION);
        if(!m_pHGE) return false;
        else return true;
    }
    static void                 ReleaseAll()
    {
        for(map<string, ouaniinfo>::iterator it = __OUANITEXTURE.begin(); it != __OUANITEXTURE.end(); it++)
        {
            if(0 != it->second.tex)
            {
                m_pHGE->Texture_Free(it->second.tex);
            }
        }
        __OUANITEXTURE.clear();
        m_pHGE->Release();
    }

private:
    ouaheader                   m_Header;

    HTEXTURE                    m_hTex;
    hgeAnimation*               m_pAnim;

    /** 字典 */
    vector<DWORD>               m_dwDict;

    static map<string, ouaniinfo> __OUANITEXTURE;
    static HGE*                 m_pHGE;
};

#endif
