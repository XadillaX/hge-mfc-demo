/***********************************
 *
 * O������GUIϵͳ
 * - ��GUI -
 * ����: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OUGUIMANAGER_H__
#define __OUGUIMANAGER_H__

#pragma once
#include "ouguiobject.h"

class OUGUI_API OUGUIManager : public OUGUIObject
{
public:
    OUGUIManager(void);
    virtual ~OUGUIManager(void);

    virtual bool                OnLoadFromXml(TiXmlElement* pElement, OUGUIObject* pWillParent = NULL);

public:
    bool                        LoadXml(string& szFilename);
};

#endif
