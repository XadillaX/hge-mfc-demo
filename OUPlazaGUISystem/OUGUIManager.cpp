#include "StdAfx.h"
#include "OUGUIManager.h"

///////////////////////////////////////////////////////////
// 工厂对象

REGISTERGUI("manager", OUGUIManager, __ou_gui_manager_creator);

///////////////////////////////////////////////////////////

OUGUIManager::OUGUIManager(void)
{
    m_szControlKey = "root";
    m_dwControlID = 0xffffffff;

    m_bRoot = true;

    SetAbsolutePos(0, 0);
}

OUGUIManager::~OUGUIManager(void)
{
}

bool OUGUIManager::LoadXml(string& szFilename)
{
    OUPACKFILEDATA pData = OUResourcePool::instance().GetResource(szFilename);
    if(0 == pData.m_dwFileSize)
    {
        return false;
    }

    TiXmlDocument* pDoc = new TiXmlDocument();
    pDoc->Parse((const char*)pData.m_pBuf, 0, TiXmlEncoding::TIXML_ENCODING_LEGACY);

    /** 开始解析 */
    return LoadFromXml(pDoc->RootElement(), NULL);
}

bool OUGUIManager::OnLoadFromXml(TiXmlElement* pElement, OUGUIObject* pWillParent)
{
    OUStringFunc str(pElement->Value());
    if(str.ToUppercase() != "MANAGER") return false;
    else
    {
        m_bInited = true;
        return true;
    }
}
