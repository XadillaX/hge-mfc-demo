#include "StdAfx.h"
#include "OUGUICallBackObject.h"

map<string, OUGUI_CBWRAPPER*> OUGUICallBackObject::m_pFuncMap;

OUGUICallBackObject::OUGUICallBackObject(void)
{
}

OUGUICallBackObject::~OUGUICallBackObject(void)
{
}

bool OUGUICallBackObject::AddCallBack(string name, CALLBACK_FUNC func, OUGUICallBackObject* parent, bool overwrite)
{
    if(m_pFuncMap.find(name) != m_pFuncMap.end() && !overwrite) return false;

    if(NULL == m_pFuncMap[name]) m_pFuncMap[name] = new OUGUI_CBWRAPPER();

    m_pFuncMap[name]->pFunc = func;
    m_pFuncMap[name]->pPar = parent;

    return true;
}

OUGUI_CBWRAPPER* OUGUICallBackObject::GetCallBack(string name)
{
    if(m_pFuncMap.find(name) == m_pFuncMap.end()) return NULL;

    return m_pFuncMap[name];
}

void OUGUICallBackObject::RunCallBack(string name, OUGUIObject* pSender, OUGUIEvent* pEvent)
{
    OUGUI_CBWRAPPER* pWrapper = GetCallBack(name);
    if(NULL == pWrapper) return;

    (pWrapper->pPar->*(pWrapper->pFunc))(pSender, pEvent);
}

void OUGUICallBackObject::Clear()
{
    for(map<string, OUGUI_CBWRAPPER*>::iterator it = m_pFuncMap.begin(); it != m_pFuncMap.end(); it++)
    {
        delete it->second;
    }
    m_pFuncMap.clear();
}
