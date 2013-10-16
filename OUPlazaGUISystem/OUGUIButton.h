#ifndef __OUGUIBUTTON_H__
#define __OUGUIBUTTON_H__
#pragma once
#include "ouguiobject.h"

enum OUBUTTON_STATE
{
    OUBS_UP,
    OUBS_HOVER,
    OUBS_DOWN,
    OUBS_DISABLE,

    OUBS_MAX
};

class OUGUI_API OUGUIButton : public OUGUIObject
{
public:
    OUGUIButton(void);
    virtual ~OUGUIButton(void);

    virtual bool                OnLoadFromXml(TiXmlElement* pElement, OUGUIObject* pWillParent);
    virtual void                OnRender();
    virtual void                OnUpdate(float fDelta, float fMX, float fMY);
    
    bool                        SetSpriteImage(string filename);
    bool                        SetAnimationImage(string filename, OUBUTTON_STATE state);

    virtual void                OnMouseEnter();
    virtual void                OnMouseLeave();
    virtual void                OnMouseLButtonDown();
    virtual void                OnMouseLButtonUp();

    virtual void                OnSetEnableMode(bool bEnableMode)
    {
        if(!m_bEnableMode)
        {
            m_OldState = m_State;
            m_State = OUBS_DISABLE;
        }
        else
        {
            m_State = m_OldState;
        }
    }

protected:
    HTEXTURE                    m_hTex;
    OUGUI_IMG                   m_Sprite[4];
    OUBUTTON_STATE              m_State;
    OUBUTTON_STATE              m_OldState;

    string                      m_szOnLButtonDown;
    string                      m_szOnLButtonUp;
};

#endif
