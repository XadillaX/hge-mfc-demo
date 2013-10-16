/***********************************
 *
 * O游棋牌GUI系统
 * - GUI基类 -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#include "StdAfx.h"
#include "OUGUIObject.h"

OUGUISINGLETON_IMPL(GUIFACTORY);

///////////////////////////////////////////////////////////
// Windows消息响应Attached

LRESULT CALLBACK __GUIWndMsgProc(HWND hWnd, UINT dwMessage, WPARAM wParam, LPARAM lParam)
{
    OUGUIObject* ctrl = OUGUIObject::GetInputControl();
    if(ctrl != NULL)
    {
        return ctrl->OnWndProc(hWnd, dwMessage, wParam, lParam);
    }

    return FALSE;
}

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// Wrapper

OUGUI_CHILD_BODY_tag::OUGUI_CHILD_BODY_tag()
{
    m_pObject = NULL;
    m_bAutoDelete = true;
    m_szControlKey = "";
    m_dwControlID = 0;
}

OUGUI_CHILD_BODY_tag::~OUGUI_CHILD_BODY_tag()
{
    if(m_bAutoDelete)
    {
        delete m_pObject;
        m_pObject = NULL;
    }
    m_szControlKey = "";
    m_dwControlID = 0;
}

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// 静态成员

OUGUIObject* OUGUIObject::m_pActiveControl = NULL;
OUGUIObject* OUGUIObject::m_pFocusControl = NULL;
OUGUIObject* OUGUIObject::m_pInputControl = NULL;
OUGUIObject* OUGUIObject::m_pModalControl = NULL;
OUGUIObject* OUGUIObject::m_pMouseCaptureControl = NULL;
HGE* OUGUIObject::m_pHGE = NULL;
::CRITICAL_SECTION OUGUIObject::m_csChildCtrl;

///////////////////////////////////////////////////////////
// 静态函数

void OUGUIObject::InitializeGUIEnviroment()
{
    ::InitializeCriticalSection(&m_csChildCtrl);
    m_pHGE = hgeCreate(HGE_VERSION);

    Assert(m_pHGE != NULL, "Can't create HGE pointer");
    m_pHGE->AttachWindowsProc(__GUIWndMsgProc);
}

void OUGUIObject::ReleaseGUIEnviroment()
{
    ::DeleteCriticalSection(&m_csChildCtrl);
    m_pHGE->Release();

    m_pHGE->AttachWindowsProc(NULL);
}

///////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////
// 工厂对象

REGISTERGUI("object", OUGUIObject, __ou_gui_object_creator);

///////////////////////////////////////////////////////////

OUGUIObject::OUGUIObject() :
    m_bShowMode(true),
    m_bEnableMode(true),
    m_bMoveMode(false),

    m_dwControlID(0),
    m_szControlKey(""),

    m_bMouseEnter(false),
    m_bMouseLButton(false),
    m_bMouseRButton(false),
    m_bMouseMButton(false),
    m_nMouseWheel(0),

    m_pParent(NULL),
    m_bInited(false),
    m_pWrapper(NULL),
    m_bRoot(false),

    m_bStayBottom(false)
{
    memset(&m_ptPos, 0, sizeof(m_ptPos));
    memset(&m_ptSize, 0, sizeof(m_ptSize));
    memset(&m_ptMousePos, 0, sizeof(m_ptMousePos));
    memset(&m_rtMoveRange, 0, sizeof(m_rtMoveRange));
}

OUGUIObject::~OUGUIObject()
{
    //GUI_CHILD_CTRL_LOCK;

    //for(GUIMap::iterator it = m_pControlList.begin(); it != m_pControlList.end(); it++)
    //{
    //    SAFEDEL(it->second);
    //}
    //m_pControlList.clear();
    for(GUIList::iterator it = m_pControlList.begin(); it != m_pControlList.end(); it++)
    {
        SAFEDEL(*it);
    }
    m_pControlList.clear();
}

void OUGUIObject::UpdateGUI(float fDeltaTime, float fX, float fY)
{
    OUGUIObject* pControl;
    float fDX;
    float fDY;
    OUPOINT pt;

    //for(GUIMap::iterator it = m_pControlList.begin(); it != m_pControlList.end(); it++)
    //{
    //    pControl = it->second->m_pObject;
    //    
    //    pt = pControl->GetRelativePos();
    //    fDX = fX - pt.m_fX;
    //    fDY = fY - pt.m_fY;
    //    pControl->UpdateGUI(fDeltaTime, fDX, fDY);
    //}

    pt = GetRelativePos();
    for(GUIList::reverse_iterator it = m_pControlList.rbegin(); it != m_pControlList.rend(); it++)
    {
        pControl = (*it)->m_pObject;

        if(!pControl->GetShowMode()) continue;

        fDX = fX - pt.m_fX;
        fDY = fY - pt.m_fY;
        pControl->UpdateGUI(fDeltaTime, fDX, fDY);
    }

    /** 更新自身 */
    UpdateSelf(fDeltaTime, fX, fY);

    /** 查看焦点控件 */
    OUGUIObject* pFocus = OUGUIObject::GetFocusControl();
    //if(pFocus)
    //{
    //    /** 若自身或者有子控件是焦点，则将其移至父控件的链表尾 */
    //    if(this == pFocus || this->IsChild(pFocus))
    //    {
    //        if(m_pParent) m_pParent->MoveToTail(this);
    //    }
    //}
    for(GUIList::iterator it = m_pControlList.begin(); it != m_pControlList.end(); it++)
    {
        pControl = (*it)->m_pObject;
        if(pFocus == pControl || pControl->IsChild(pFocus))
        {
            MoveToTail(pControl);
            break;
        }
    }
}

void OUGUIObject::RenderGUI()
{
    if(!GetShowMode()) return;

    /** 渲染自身 */
    OnRender();

    /** 渲染子控件 */
    for(GUIList::iterator it = m_pControlList.begin(); it != m_pControlList.end(); it++)
    {
        (*it)->m_pObject->RenderGUI();
    }
}

void OUGUIObject::UpdateSelf(float fDeltaTime, float fMX, float fMY)
{
    /** 如果鼠标不在HGE窗口内 */
    //if(!m_pHGE->Input_IsMouseOver()) return;

    bool bLButton = m_pHGE->Input_GetKeyState(HGEK_LBUTTON);
    bool bRButton = m_pHGE->Input_GetKeyState(HGEK_RBUTTON);
    bool bMButton = m_pHGE->Input_GetKeyState(HGEK_MBUTTON);
    int nWheel = m_pHGE->Input_GetMouseWheel();

    OnUpdate(fDeltaTime, fMX, fMY);

    /** 若控件不可用 */
    if(!GetEnableMode()) return;
    if(!GetShowMode()) return;

    /** 若有鼠标捕获的控件 */
    if(OUGUIObject::m_pMouseCaptureControl &&
        OUGUIObject::m_pMouseCaptureControl != this &&
        !IsParent(OUGUIObject::m_pMouseCaptureControl))
    {
        return;
    }

    /** 若有模式窗口 */
    if(OUGUIObject::m_pModalControl &&
        OUGUIObject::m_pModalControl != this &&
        !IsParent(OUGUIObject::m_pModalControl))
    {
        return;
    }

    /** 若有控件在激活状态下 */
    if(OUGUIObject::m_pActiveControl &&
        OUGUIObject::m_pActiveControl != this)
    {
        return;
    }
    
    /** 如果鼠标没有被按下，则检测是不是OnEnter */
    if(!bLButton && OnCheckCollision(fMX, fMY))
    {
        //SetMouseEnter(OnCheckCollision(fMX, fMY));
        SetMouseEnter(true);
    }
    /** 如果鼠标抬起，则检测是不是OnLeave */
    else
    if(!bLButton && !OnCheckCollision(fMX, fMY) && GetMouseEnter())
    {
        SetMouseEnter(false);

        this->m_bMouseLButton = false;
        this->m_bMouseMButton = false;
        this->m_bMouseRButton = false;
        this->m_nMouseWheel = 0;
       
        if(OUGUIObject::GetActiveControl() == this)
        {
            OUGUIObject::SetActiveControl(NULL);
        }

        return;
    }

    if(GetMouseEnter() || OUGUIObject::GetMouseCaptureControl() == this)
    {
        SetMousePos(fMX, fMY);
        SetMouseLButton(bLButton);
        SetMouseRButton(bRButton);
        SetMouseMButton(bMButton);
        SetMouseWheel(nWheel);

        /** 键盘响应 */
        for(int i = 0; i < 256; i++)
        {
            if(i == HGEK_LBUTTON) continue;
            if(i == HGEK_RBUTTON) continue;
            if(i == HGEK_MBUTTON) continue;

            if(m_pHGE->Input_KeyDown(i)) OnKeyDown(i);
            else
            if(m_pHGE->Input_KeyUp(i)) OnKeyUp(i);
        }

        OnIdle();
    }
    else
    {
        /** 清空状态 */
        SetMouseLButton(false);
        SetMouseRButton(false);
        SetMouseMButton(false);
        SetMouseWheel(nWheel);
    }
}

bool OUGUIObject::AddControl(OUGUIObject *pControl, bool bAutoDelete, bool back)
{
    /** 增加控件在同一时间只能一起增加一个——包括所有控件在内，为了让ID和Key等不冲突 */
    GUI_CHILD_CTRL_LOCK;

    if(pControl == NULL) return false;

    /** 查看子控件是否初始化 */
    if(!pControl->IsInitialized()) return false;

    /** 查看是否已有此Key */
    //if(m_pControlList.find(pControl->GetKey()) != m_pControlList.end())
    //{
    //    return false;
    //}

    /** 查看是否已有此ID和此Key */
    for(GUIList::iterator it = m_pControlList.begin(); it != m_pControlList.end(); it++)
    {
        POUGUI_CHILD_BODY pWrapper = *it;
        if(pWrapper->m_dwControlID == pControl->GetID() || pWrapper->m_szControlKey == pControl->GetKey())
        {
            return false;
        }
        //if(it->second->m_dwControlID == pControl->GetID())
        //{
        //    return false;
        //}
    }

    /** 设置父对象 */
    pControl->SetParent(this);

    /** 新控件体 */
    OUGUI_CHILD_BODY* ctrl = new OUGUI_CHILD_BODY();
    ctrl->m_pObject = pControl;
    ctrl->m_bAutoDelete = bAutoDelete;
    ctrl->m_dwControlID = pControl->GetID();
    ctrl->m_szControlKey = pControl->GetKey();

    pControl->SetWrapper(ctrl);

    //m_pControlList[ctrl->m_szControlKey] = ctrl;
    if(back) m_pControlList.push_back(ctrl);
    else m_pControlList.push_front(ctrl);

    return true;
}

OUGUIObject* OUGUIObject::GetControl(string szKey)
{
    GUI_CHILD_CTRL_LOCK;

    /** 不存在子对象 */
    //if(m_pControlList.find(szKey) == m_pControlList.end()) return NULL;
    for(GUIList::iterator it = m_pControlList.begin(); it != m_pControlList.end(); it++)
    {
        if((*it)->m_szControlKey == szKey) return (*it)->m_pObject;
    }

    return NULL;
}

OUGUIObject* OUGUIObject::GetControl(DWORD dwID)
{
    GUI_CHILD_CTRL_LOCK;

    //for(GUIMap::iterator it = m_pControlList.begin(); it != m_pControlList.end(); it++)
    //{
    //    if(it->second->m_dwControlID == dwID)
    //    {
    //        return it->second->m_pObject;
    //    }
    //}

    for(GUIList::iterator it = m_pControlList.begin(); it != m_pControlList.end(); it++)
    {
        if((*it)->m_dwControlID == dwID) return (*it)->m_pObject;
    }

    return NULL;
}

bool OUGUIObject::DelControl(string szKey)
{
    GUI_CHILD_CTRL_LOCK;

    //GUIMap::iterator it = m_pControlList.find(szKey);
    //if(it == m_pControlList.end()) return false;

    //delete it->second;
    //m_pControlList.erase(it);

    for(GUIList::iterator it = m_pControlList.begin(); it != m_pControlList.end(); it++)
    {
        if((*it)->m_szControlKey == szKey)
        {
            delete (*it);
            m_pControlList.erase(it);

            return true;
        }
    }

    return false;
}

bool OUGUIObject::DelControl(DWORD dwID)
{
    GUI_CHILD_CTRL_LOCK;

    //for(GUIMap::iterator it = m_pControlList.begin(); it != m_pControlList.end(); it++)
    //{
    //    if(it->second->m_pObject->GetID() == dwID)
    //    {
    //        delete it->second;
    //        m_pControlList.erase(it);
    //        return true;
    //    }
    //}

    for(GUIList::iterator it = m_pControlList.begin(); it != m_pControlList.end(); it++)
    {
        if((*it)->m_dwControlID == dwID)
        {
            delete (*it);
            m_pControlList.erase(it);

            return true;
        }
    }

    return false;
}

bool OUGUIObject::SetKey(string szKey)
{
    if(szKey == m_szControlKey) return true;

    if(m_pParent == NULL)
    {
        m_szControlKey = szKey;
        if(m_pWrapper != NULL) m_pWrapper->m_szControlKey = szKey;

        return true;
    }

    /** 寻找同级控件 */
    if(m_pParent->GetControl(szKey) != NULL) return false;

    m_szControlKey = szKey;
    if(m_pWrapper != NULL) m_pWrapper->m_szControlKey = szKey;
    return true;
}

bool OUGUIObject::SetID(DWORD dwID)
{
    if(dwID == m_dwControlID) return true;

    if(m_pParent == NULL)
    {
        m_dwControlID = dwID;
        if(m_pWrapper != NULL) m_pWrapper->m_dwControlID = dwID;

        return true;
    }

    /** 寻找同级控件 */
    if(m_pParent->GetControl(dwID) != NULL) return false;

    m_dwControlID = dwID;
    if(m_pWrapper != NULL) m_pWrapper->m_dwControlID = dwID;
    return true;
}

OUPOINT OUGUIObject::GetAbsolutePos()
{
    if(m_pParent == NULL) return m_ptPos;
    else return m_ptPos + m_pParent->GetAbsolutePos();
}

void OUGUIObject::SetAbsolutePos(OUPOINT pos)
{
    if(m_pParent == NULL) m_ptPos = pos;
    else m_ptPos = pos - m_pParent->GetAbsolutePos();

    OnSetPos(m_ptPos.m_fX, m_ptPos.m_fY);

    AdjustPosRange(m_ptPos.m_fX, m_ptPos.m_fY);
}

void OUGUIObject::SetMoveRange(float fX, float fY, float fW, float fH)
{
    m_rtMoveRange.m_fX1 = fX;
    m_rtMoveRange.m_fY1 = fY;
    m_rtMoveRange.SetWidth(fW);
    m_rtMoveRange.SetHeight(fH);

    AdjustPosRange(m_ptPos.m_fX, m_ptPos.m_fY);
}

BYTE OUGUIObject::AdjustPosRange(float fX, float fY)
{
    /**
     * 0x00: 无移动
     * 0x01: X轴调整
     * 0x02: Y轴调整
     * 0x03: XY轴都调整
     */
    BYTE res = 0x00;

    if(sgn(m_rtMoveRange.getWidth()) && sgn(m_rtMoveRange.getHeight()))
    {
        /** X太左边了 */
        if(fX < m_rtMoveRange.m_fX1)
        {
            fX = m_rtMoveRange.m_fX1;
            res |= 0x01;
        }

        /** Y太上面了 */
        if(fY < m_rtMoveRange.m_fY1)
        {
            fY = m_rtMoveRange.m_fY1;
            res |= 0x02;
        }

        /** X太右边了 */
        if(fX + m_ptSize.m_fX > m_rtMoveRange.m_fX2)
        {
            fX = m_rtMoveRange.m_fX2 - m_ptSize.m_fX;
            res |= 0x01;
        }

        /** Y太下面了 */
        if(fY + m_ptSize.m_fY > m_rtMoveRange.m_fY2)
        {
            fY = m_rtMoveRange.m_fY2 - m_ptSize.m_fY;
            res |= 0x02;
        }
    }

    if(res)
    {
        SetRelativePos(fX, fY);
    }

    return res;
}

void OUGUIObject::CaptureMouse(bool bCapture)
{
    if(!bCapture)
    {
        if(this == OUGUIObject::GetMouseCaptureControl())
        {
            OUGUIObject::SetMouseCaptureControl(NULL);
        }
    }
    else
    {
        OUGUIObject::SetMouseCaptureControl(this);
    }
}

bool OUGUIObject::IsChild(OUGUIObject* pControl)
{
    //for(GUIMap::iterator it = m_pControlList.begin(); it != m_pControlList.end(); it++)
    //{
    //    if(pControl == it->second->m_pObject) return true;
    //    if(it->second->m_pObject->IsChild(pControl)) return true;
    //}

    for(GUIList::iterator it = m_pControlList.begin(); it != m_pControlList.end(); it++)
    {
        if(pControl == (*it)->m_pObject) return true;
        if((*it)->m_pObject->IsChild(pControl)) return true;
    }

    return false;
}

bool OUGUIObject::IsParent(OUGUIObject* pControl)
{
    if(pControl == NULL) return false;
    if(m_pParent == NULL) return false;

    if(m_pParent == pControl) return true;

    return m_pParent->IsParent(pControl);
}

bool OUGUIObject::MoveToTail(OUGUIObject *pControl)
{
    /** 若是底层控件 */
    if(pControl->IsStayBottom()) return false;

    for(GUIList::iterator it = m_pControlList.begin(); it != m_pControlList.end(); it++)
    {
        if(pControl == (*it)->m_pObject)
        {
            POUGUI_CHILD_BODY pWrapper = (*it);
            m_pControlList.erase(it);

            m_pControlList.push_back(pWrapper);
            return true;
        }
    }

    return false;
}

void OUGUIObject::SetMouseEnter(bool bEnter)
{
    if(bEnter == m_bMouseEnter) return;

    m_bMouseEnter = bEnter;

    if(m_bMouseEnter) OnMouseEnter();
    else OnMouseLeave();
}

void OUGUIObject::SetMouseLButton(bool bDown)
{
    if(bDown == m_bMouseLButton) return;

    m_bMouseLButton = bDown;

    if(m_bMouseLButton)
    {
        /** 设置控件 */
        if(!IsChild(OUGUIObject::GetFocusControl()))
        {
            OUGUIObject::SetFocusControl(this);
        }

        /** 激活控件 (如拖曳) */
        if(!IsChild(OUGUIObject::GetActiveControl()))
        {
            OUGUIObject::SetActiveControl(this);
        }

        OnMouseLButtonDown();
    }
    else
    {
        if(OUGUIObject::GetActiveControl() == this)
        {
            OUGUIObject::SetActiveControl(NULL);
        }

        OnMouseLButtonUp();
    }
}

void OUGUIObject::SetMouseRButton(bool bDown)
{
    if(bDown == m_bMouseRButton) return;
    m_bMouseRButton = bDown;

    if(m_bMouseRButton) OnMouseRButtonDown();
    else OnMouseRButtonUp();
}

void OUGUIObject::SetMouseMButton(bool bDown)
{
    if(bDown == m_bMouseMButton) return;
    m_bMouseMButton = bDown;

    if(m_bMouseMButton) OnMouseMButtonDown();
    else OnMouseMButtonUp();
}

void OUGUIObject::SetMouseWheel(int nWheel)
{
    m_nMouseWheel = nWheel;

    if(0 != nWheel) OnMouseWheel(nWheel);
}

bool OUGUIObject::OnLoadFromXml(TiXmlElement* pElement, OUGUIObject* pWillParent)
{
    if(pWillParent)
    {
        OUPOINT ParentSize = pWillParent->GetSize();
        SetMoveRange(0, 0, ParentSize.m_fX, ParentSize.m_fY);
    }

    int id;
    pElement->QueryIntAttribute("id", &id);
    string key;
    key = pElement->Attribute("name");

    SetID(id);
    SetKey(key);

    //TiXmlElement* pNode;
    //for(pNode = pElement->FirstChildElement(); pNode != NULL; pNode = pNode->NextSiblingElement())
    //{
    //    Assert(pNode->Value(), "Empty node value.");
    //    OUStringFunc str = pNode->Value();
    //    str.ToUppercase();

    //    if(str.GetString() == "ID")
    //    {
    //        SetID(atoi(pNode->GetText()));
    //    }
    //    else
    //    if(str.GetString() == "NAME")
    //    {
    //        SetKey(pNode->GetText());
    //    }
    //}

    return true;
}

bool OUGUIObject::LoadFromXml(TiXmlElement* pElement, OUGUIObject* pWillParent)
{
    /** 载入自身 */
    if(!OnLoadFromXml(pElement, pWillParent)) return false;

    /** 搜索子控件节点 */
    TiXmlElement* pChild;
    for(pChild = pElement->FirstChildElement(); pChild != NULL; pChild = pChild->NextSiblingElement())
    {
        Assert(pChild->Value(), "Node value is null.");
        OUStringFunc str(pChild->Value());
        if(str.ToUppercase() == "CHILDREN") break;
    }

    /** 有子控件 */
    if(pChild)
    {
        TiXmlElement* pNode;
        for(pNode = pChild->FirstChildElement(); pNode != NULL; pNode = pNode->NextSiblingElement())
        {
            Assert(pChild->Value(), "Node value is null.");
            OUGUIObject* pControl = GUIFACTORY::instance().create(pNode->Value());
            if(pControl->LoadFromXml(pNode, this))
            {
                AddControl(pControl, !pControl->IsStayBottom());
            }
            else delete pControl;
        }
    }

    return true;
}

void OUGUIObject::OnDrawBoard(DWORD dwColor)
{
    OUPOINT ptPos1 = GetAbsolutePos();
    OUPOINT ptPos2 = ptPos1 + m_ptSize;
    
    m_pHGE->Gfx_RenderLine(ptPos1.m_fX, ptPos1.m_fY, ptPos1.m_fX, ptPos2.m_fY, dwColor);
    m_pHGE->Gfx_RenderLine(ptPos1.m_fX, ptPos2.m_fY, ptPos2.m_fX, ptPos2.m_fY, dwColor);
    m_pHGE->Gfx_RenderLine(ptPos2.m_fX, ptPos2.m_fY, ptPos2.m_fX, ptPos1.m_fY, dwColor);
    m_pHGE->Gfx_RenderLine(ptPos2.m_fX, ptPos1.m_fY, ptPos1.m_fX, ptPos1.m_fY, dwColor);
}

