/***********************************
 *
 * O游棋牌GUI系统
 * - Global -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __GLOBAL_H__
#define __GLOBAL_H__
#pragma once

#pragma warning( disable : 4100 )	// 'identifier' : unreferenced formal parameter
#pragma warning( disable : 4127 )	// conditional expression is constant
#pragma warning( disable : 4311 )	// 'variable' : pointer truncation from 'type' to 'type'
#pragma warning( disable : 4312 )	// 'variable' : conversion from 'type' to 'type' of greater size
#pragma warning( disable : 4512 )	// 'class' : assignment operator could not be generated
#pragma warning( disable : 4702 )	// unreachable code

#ifdef OUPLAZAGUISYSTEM_EXPORTS
#define OUGUI_API __declspec(dllexport)
#else
#define OUGUI_API __declspec(dllimport)
#endif

#include <atltrace.h>
#include "../OUPlazaRender/HGE.h"
#include "../OUPlazaRender/hgeSprite.h"
#include "../OUPlazaRender/hgeAnim.h"
#include "../OUPlazaRender/hgeGUI.h"
#include "../OUPlazaRender/tinyxml/tinyxml.h"
#include "../OUPlazaRender/OUResourceManager.h"
#include "OUGUIObjectFactory.h"
#include "../OUPlazaRender/OULockScope.h"
#include "../OUPlazaRender/OUAniFile.h"

#ifndef SAFEDEL
#define SAFEDEL(p) { if(p != NULL) { delete p; p = NULL; } }
#endif

#ifndef SAFEDELARRAY
#define SAFEDELARRAY(p) { if(p != NULL) { delete []p; p = NULL; } }
#endif

//Assert宏，若断言失败则输出出错信息
#ifndef _DEBUG
#define Assert(_Expression, _Msg) 0
#else
#define Assert(_Expression, _Msg) { if(!_Expression) printf("%s: %s\n",#_Expression, _Msg); }
#endif

class OUGUIObject;

/** 子控件外包 */
typedef class OUGUI_API OUGUI_CHILD_BODY_tag
{
public:
    OUGUIObject*                m_pObject;
    bool                        m_bAutoDelete;

    string                      m_szControlKey;
    DWORD                       m_dwControlID;

    OUGUI_CHILD_BODY_tag();
    ~OUGUI_CHILD_BODY_tag();
} OUGUI_CHILD_BODY, *POUGUI_CHILD_BODY;

/////////////////////////////////////////////
//工厂对象注册简化宏
#define REGISTERGUI(RegisterName, ClassName, FunctionName) \
    namespace FunctionName { \
    OUGUIObject* creator() { ClassName* c = new ClassName(); return (OUGUIObject*)c; } \
    bool flag = GUIFACTORY::instance().reg(RegisterName, creator); \
    }
/////////////////////////////////////////////

///////////////////////////
// float精度
const float eps = 1e-6;

// 判断float相减是小于0等于0还是大于0
inline int sgn(float a)
{
    return a < -eps ? -1 : a < eps ? 0 : 1;
}
///////////////////////////

typedef struct OU_POINT_tag
{
    float               m_fX;
    float               m_fY;

    inline OU_POINT_tag operator + (OU_POINT_tag pt)
    {
        OU_POINT_tag n;
        n.m_fX = m_fX + pt.m_fX;
        n.m_fY = m_fY + pt.m_fY;

        return n;
    }

    inline OU_POINT_tag operator - (OU_POINT_tag pt)
    {
        OU_POINT_tag n;
        n.m_fX = m_fX - pt.m_fX;
        n.m_fY = m_fY - pt.m_fY;

        return n;
    }
} OUPOINT, *POUPOINT;

typedef struct OU_RECT_tag
{
    float               m_fX1;
    float               m_fX2;
    float               m_fY1;
    float               m_fY2;

    OU_RECT_tag()
    {
        m_fX1 = m_fX2 = m_fY1 = m_fY2 = 0.0f;
    }

    OU_RECT_tag(float x1, float y1, float x2, float y2)
    {
        m_fX1 = x1;
        m_fY1 = y1;
        m_fX2 = x2;
        m_fY2 = y2;
    }

    void SetWidth(float fW)
    {
        m_fX2 = m_fX1 + fW;
    }

    void SetHeight(float fH)
    {
        m_fY2 = m_fY1 + fH;
    }

    float getWidth()
    {
        return sgn(m_fX2 - m_fX1) == 0 ? 0 : (m_fX2 - m_fX1);
    }

    float getHeight()
    {
        return sgn(m_fY2 - m_fY1) == 0 ? 0 : (m_fY2 - m_fY1);
    }
} OU_RECT, *POU_RECT;

typedef class tagOUGUI_IMG
{
public:
    union OUGUI_ANI_SPR
    {
        OUAniFile*                  m_pAnim;
        hgeSprite*                  m_pSprite;
    } m_Img;

    /** 为True时为hgeSprite*，否则为OUAniFile*。 */
    bool                            m_bStatic;

    tagOUGUI_IMG()
    {
        m_Img.m_pSprite = NULL;
        m_bStatic = true;
    }

    ~tagOUGUI_IMG()
    {
        Release();
    }

    void Release()
    {
        if(m_bStatic)
        {
            SAFEDEL(m_Img.m_pSprite);
        }
        else
        {
            SAFEDEL(m_Img.m_pAnim);
        }

        m_bStatic = true;
    }

    virtual OUPOINT GetSize()
    {
        OUPOINT pt = { 0, 0 };
        if(m_bStatic && m_Img.m_pSprite)
        {
            pt.m_fX = m_Img.m_pSprite->GetWidth();
            pt.m_fY = m_Img.m_pSprite->GetHeight();
        }
        else
        if(m_Img.m_pAnim)
        {
            pt.m_fX = m_Img.m_pAnim->GetAnimation()->GetWidth();
            pt.m_fY = m_Img.m_pAnim->GetAnimation()->GetHeight();
        }

        return pt;
    }

    virtual void Render(float x, float y)
    {
        if(m_bStatic && m_Img.m_pSprite) m_Img.m_pSprite->Render(x, y);
        else
        if(m_Img.m_pAnim) m_Img.m_pAnim->GetAnimation()->Render(x, y);
    }

    virtual void SetColor(DWORD color)
    {
        if(m_bStatic && m_Img.m_pSprite) m_Img.m_pSprite->SetColor(color);
        else
        if(m_Img.m_pAnim) m_Img.m_pAnim->GetAnimation()->SetColor(color);
    }

    virtual DWORD GetColor()
    {
        if(m_bStatic && m_Img.m_pSprite) return m_Img.m_pSprite->GetColor();
        else
        if(m_Img.m_pAnim) return m_Img.m_pAnim->GetAnimation()->GetColor();

        return 0;
    }

    virtual void RenderStretch(float x, float y, float w, float h)
    {
        //if(m_bStatic && m_Img.m_pSprite) m_Img.m_pSprite->Render(x, y);
        //else
        //if(m_Img.m_pAnim) m_Img.m_pAnim->GetAnimation()->Render(x, y);
        //return;

        if(m_bStatic && m_Img.m_pSprite)
        {
            m_Img.m_pSprite->RenderStretch(x, y, x + w, y + h);
        }
        else
        if(m_Img.m_pAnim)
        {
            m_Img.m_pAnim->GetAnimation()->RenderStretch(x, y, x + w, y + h);
        }
    }

    virtual void Update(float fDelta)
    {
        if(m_bStatic && m_Img.m_pSprite) return;
        else
        if(m_Img.m_pAnim) m_Img.m_pAnim->GetAnimation()->Update(fDelta);
    }

    virtual void Play()
    {
        if(m_bStatic && m_Img.m_pSprite) return;
        else
        if(m_Img.m_pAnim) m_Img.m_pAnim->GetAnimation()->Play();
    }

    virtual void Stop()
    {
        if(m_bStatic && m_Img.m_pSprite) return;
        else
        if(m_Img.m_pAnim) m_Img.m_pAnim->GetAnimation()->Stop();
    }

    virtual void Resume()
    {
        if(m_bStatic && m_Img.m_pSprite) return;
        else
        if(m_Img.m_pAnim) m_Img.m_pAnim->GetAnimation()->Resume();
    }

    virtual bool IsPlaying()
    {
        if(m_bStatic) return false;
        else
        if(m_Img.m_pAnim) m_Img.m_pAnim->GetAnimation()->IsPlaying();

        return false;
    }

    virtual hgeAnimation* GetAnimation()
    {
        if(!m_bStatic) return m_Img.m_pAnim->GetAnimation();
        return NULL;
    }

    virtual hgeSprite* GetSprite()
    {
        if(m_bStatic) return m_Img.m_pSprite;
        return NULL;
    }

    virtual void SetZ(float z)
    {
        if(m_bStatic)
        {
            if(m_Img.m_pSprite) m_Img.m_pSprite->SetZ(z);
        }
        else
        {
            if(m_Img.m_pAnim) m_Img.m_pAnim->GetAnimation()->SetZ(z);
        }
    }

    virtual float GetZ()
    {
        if(m_bStatic)
        {
            if(m_Img.m_pSprite) return m_Img.m_pSprite->GetZ();
        }
        else
        {
            if(m_Img.m_pAnim) return m_Img.m_pAnim->GetAnimation()->GetZ();
        }

        return 1.0f;
    }

    virtual void SetSpot(float x, float y)
    {
        if(m_bStatic && m_Img.m_pSprite) m_Img.m_pSprite->SetHotSpot(x, y);
        else
        if(m_Img.m_pAnim) m_Img.m_pAnim->GetAnimation()->SetHotSpot(x, y);
    }

    virtual bool GetSpot(float* x, float* y)
    {
        if(m_bStatic)
        {
            if(m_Img.m_pSprite)
            {
                m_Img.m_pSprite->GetHotSpot(x, y);
                return true;
            }
            return false;
        }
        else
        {
            if(m_Img.m_pAnim && m_Img.m_pAnim->GetAnimation())
            {
                m_Img.m_pAnim->GetAnimation()->GetHotSpot(x, y);
                return true;
            }
            return false;
        }
    }
} OUGUI_IMG, *POUGUI_IMG;

#endif

