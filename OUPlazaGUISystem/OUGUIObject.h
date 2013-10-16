/***********************************
 *
 * O游棋牌GUI系统
 * - GUI基类 -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OUGUIOBJECT_H__
#define __OUGUIOBJECT_H__
#pragma once
#include "Global.h"
#include <string>
#include "OUFont.h"
#include <map>
#include <list>
#include "OUGUICallBackObject.h"
#include <vector>
using namespace std;

#define GUI_CHILD_CTRL_LOCK OULockScope oLock(GetChildCtrlCriticalSection())

/** 子控件Map: key -> 子控件外包 */
typedef map<string, POUGUI_CHILD_BODY> GUIMap;
typedef list<POUGUI_CHILD_BODY> GUIList;

/** GUI基类 */
class OUGUI_API OUGUIObject
{
public:
    OUGUIObject();
    virtual ~OUGUIObject();

public:
    virtual void                OnDrawBoard(DWORD dwColor);

    /** 事件响应虚函数：供继承 */
public:
    /** 闲时事件 */
    virtual void                OnIdle()                                {}
    /** 在渲染时 */
    virtual void                OnRender()                              {}
    /** 在逻辑函数时 */
    virtual void                OnUpdate(float fDelta, float fMX, float fMY){}

    /** 在设置位置时 */
    virtual void                OnSetPos(float fX, float fY)            {}
    /** 在设置大小时 */
    virtual void                OnSetSize(float fW, float fH)           {}
    /** 在设置状态时** */
    virtual void                OnSetStatus(bool bStatus)               {}
    /** 在设置是否显示时 */
    virtual void                OnSetShowMode(bool bShowMode)           {}
    /** 在设置是否有用时 */
    virtual void                OnSetEnableMode(bool bEnableMode)       {}
    /** 设置焦点时 */
    virtual void                OnSetFocusControl(bool bFocusMode)      {}

    /** 鼠标进入时 */
    virtual void                OnMouseEnter()                          {}
    /** 鼠标离开时 */
    virtual void                OnMouseLeave()                          {}
    /** 鼠标移动时: fMX, fMY为鼠标对于此控件的相对位置 */
    virtual void                OnMouseMove(float fMX, float fMY)       {}
    /** 左键按下时 */
    virtual void                OnMouseLButtonDown()                    {}
    /** 左键抬起时 */
    virtual void                OnMouseLButtonUp()                      {}
    /** 右键按下 */
    virtual void                OnMouseRButtonDown()                    {}
    /** 右键抬起 */
    virtual void                OnMouseRButtonUp()                      {}
    /** 中键按下 */
    virtual void                OnMouseMButtonDown()                    {}
    /** 中键抬起 */
    virtual void                OnMouseMButtonUp()                      {}
    /** 鼠标滚轮 */
    virtual void                OnMouseWheel(int nMouseWheel)           {}
    /** 键盘按下 */
    virtual void                OnKeyDown(int nKey)                     {}
    /** 键盘抬起 */
    virtual void                OnKeyUp(int nKey)                       {}
    /** 键盘敲击 */
    virtual void                OnKeyPress(int nKey)                    {}
    
    /** 窗口事件 */
    virtual LRESULT             OnWndProc(HWND hWnd, DWORD dwMsg, WPARAM& wParam, LPARAM& lParam){ return (LRESULT)0; }
    /** 碰撞测试 */
    virtual bool                OnCheckCollision(float fMX, float fMY)  { return (fMX >= m_ptPos.m_fX) && (fMX <= m_ptPos.m_fX + m_ptSize.m_fX) && (fMY >= m_ptPos.m_fY) && (fMY <= m_ptPos.m_fY + m_ptSize.m_fY); }
    /** 载入控件 */
    virtual bool                OnLoadFromXml(TiXmlElement* pElement, OUGUIObject* pWillParent);

    /** 静态函数 */
public:
    /** 初始化GUI环境 */
    static void                 InitializeGUIEnviroment();
    /** 释放GUI环境 */
    static void                 ReleaseGUIEnviroment();
    /** 获取临界区 */
    static CRITICAL_SECTION&    GetChildCtrlCriticalSection()           { return m_csChildCtrl; }
    /** 获取鼠标捕获控件 */
    static OUGUIObject*         GetMouseCaptureControl()                { return OUGUIObject::m_pMouseCaptureControl; }
    /** 设置鼠标捕获控件 */
    static void                 SetMouseCaptureControl(OUGUIObject* pObj){ OUGUIObject::m_pMouseCaptureControl = pObj; }
    /** 设置焦点控件 */
    static void                 SetFocusControl(OUGUIObject* pObj)      { if(OUGUIObject::m_pFocusControl == pObj) return; if(OUGUIObject::m_pFocusControl) OUGUIObject::m_pFocusControl->OnSetFocusControl(false); OUGUIObject::m_pFocusControl = pObj; if(pObj) pObj->OnSetFocusControl(true); }
    /** 获取焦点控件 */
    static OUGUIObject*         GetFocusControl()                       { return OUGUIObject::m_pFocusControl; }
    /** 设置激活控件 */
    static void                 SetActiveControl(OUGUIObject* pObj)     { OUGUIObject::m_pActiveControl = pObj; }
    /** 获取激活控件 */
    static OUGUIObject*         GetActiveControl()                      { return m_pActiveControl; }
    /** 设置Modal窗口 */
    static void                 SetModalControl(OUGUIObject* pObj)      { OUGUIObject::m_pModalControl = pObj; }
    /** 获取Modal窗口 */
    static OUGUIObject*         GetModalControl()      { return OUGUIObject::m_pModalControl; }

public:
    /** fX和fY都需要是相对坐标 */
    void                        UpdateGUI(float fDeltaTime, float fX, float fY);
    void                        RenderGUI();

    bool                        AddControl(OUGUIObject* pControl, bool bAutoDelete = true, bool back = true);
    OUGUIObject*                GetControl(string szKey);
    OUGUIObject*                GetControl(DWORD dwID);
    bool                        DelControl(string szKey);
    bool                        DelControl(DWORD dwID);

    static OUGUIObject*         GetInputControl()                       { GUI_CHILD_CTRL_LOCK; return m_pInputControl; }

    void                        SetParent(OUGUIObject* pParent)         { m_pParent = pParent; }
    OUGUIObject*                GetParent()                             { return m_pParent; }

    bool                        SetKey(string szKey);
    bool                        SetID(DWORD dwID);
    string                      GetKey()                                { return m_szControlKey; }
    DWORD                       GetID()                                 { return m_dwControlID; }

    void                        SetWrapper(POUGUI_CHILD_BODY pWrapper)  { m_pWrapper = pWrapper; }
    POUGUI_CHILD_BODY           GetWrapper()                            { return m_pWrapper; }

    bool                        IsInitialized()                         { return m_bInited; }

    bool                        GetShowMode()                           { return m_bShowMode; }
    void                        SetShowMode(bool mode)                  { m_bShowMode = mode; OnSetShowMode(mode); }
    bool                        GetEnableMode()                         { return m_bEnableMode; }
    void                        SetEnableMode(bool mode)                { m_bEnableMode = mode; OnSetEnableMode(mode); }
    bool                        GetMoveMode()                           { return m_bMoveMode; }
    void                        SetMoveMode(bool mode)                  { m_bMoveMode = mode; }

    OUPOINT                     GetRelativePos()                        { return m_ptPos; }
    OUPOINT                     GetAbsolutePos();
    OUPOINT                     GetRelativeMousePos()                   { return m_ptMousePos; }
    OUPOINT                     GetAbsoluteMousePos()                   { return GetAbsolutePos() + m_ptMousePos; }
    OUPOINT                     GetSize()                               { return m_ptSize; }
    void                        SetSize(float fW, float fH)             { m_ptSize.m_fX = fW, m_ptSize.m_fY = fH; OnSetSize(fW, fH); }

    void                        SetRelativePos(float fX, float fY)      { OUPOINT pt; pt.m_fX = fX; pt.m_fY = fY; SetRelativePos(pt); }
    void                        SetRelativePos(OUPOINT pos)             { m_ptPos = pos; OnSetPos(pos.m_fX, pos.m_fY); AdjustPosRange(m_ptPos.m_fX, m_ptPos.m_fY); }
    void                        SetAbsolutePos(float fX, float fY)      { OUPOINT pt; pt.m_fX = fX; pt.m_fY = fY; SetAbsolutePos(pt); }
    void                        SetAbsolutePos(OUPOINT pos);
    void                        SetMousePos(float fX, float fY)         { OnMouseMove(fX, fY); m_ptMousePos.m_fX = fX, m_ptMousePos.m_fY = fY; }

    /** 设置可移动范围 */
    void                        SetMoveRange(float fX, float fY, float fW, float fH);
    /** 适应可移动范围 */
    BYTE                        AdjustPosRange(float fX, float fY);

    /** 鼠标捕获 */
    void                        CaptureMouse(bool bCapture);

    /** 是否子控件（包括孙子控件，重孙控件...） */
    bool                        IsChild(OUGUIObject* pControl);
    /** 是否父空间（包括祖先空间） */
    bool                        IsParent(OUGUIObject* pControl);
    /** 将某个子控件移至链表尾（链表越后面的层数在越前面） */
    bool                        MoveToTail(OUGUIObject* pControl);

    /** 设置鼠标进入 */
    void                        SetMouseEnter(bool bEnter);
    /** 查看鼠标是否在控件内 */
    bool                        GetMouseEnter()                         { return m_bMouseEnter; }
    /** 设置鼠标左键 */
    void                        SetMouseLButton(bool bDown);
    /** 设置鼠标右键 */
    void                        SetMouseRButton(bool bDown);
    /** 设置鼠标中键 */
    void                        SetMouseMButton(bool bDown);
    /** 获取 */
    bool                        GetMouseLButton()                       { return m_bMouseLButton; }
    bool                        GetMouseRButton()                       { return m_bMouseRButton; }
    bool                        GetMouseMButton()                       { return m_bMouseMButton; }
    /** 设置滚轮 */
    void                        SetMouseWheel(int nWheel);
    int                         GetMouseWheel()                         { return m_nMouseWheel; }

    /** 载入控件 */
    bool                        LoadFromXml(TiXmlElement* pElement, OUGUIObject* pWillParent);

    /** 是否永远呆在底层 */
    bool                        IsStayBottom()                          { return m_bStayBottom; }

protected:
    void                        UpdateSelf(float fDeltaTime, float fMX, float fMY);

protected:
    bool                        m_bRoot;
    static HGE*                 m_pHGE;

    bool                        m_bInited;

    bool                        m_bShowMode;
    bool                        m_bEnableMode;
    bool                        m_bMoveMode;

    DWORD                       m_dwControlID;
    string                      m_szControlKey;

    OUPOINT                     m_ptPos;
    OUPOINT                     m_ptSize;
    /** 相对鼠标位置 */
    OUPOINT                     m_ptMousePos;

    bool                        m_bMouseEnter;
    bool                        m_bMouseLButton;
    bool                        m_bMouseRButton;
    bool                        m_bMouseMButton;
    int                         m_nMouseWheel;

    OU_RECT                     m_rtMoveRange;

    OUGUIObject*                m_pParent;
    //不该这么写
    //GUIMap                      m_pControlList;
    GUIList                     m_pControlList;

    POUGUI_CHILD_BODY           m_pWrapper;

    static OUGUIObject*         m_pActiveControl;
    static OUGUIObject*         m_pFocusControl;
    static OUGUIObject*         m_pMouseCaptureControl;
    static OUGUIObject*         m_pModalControl;
    static OUGUIObject*         m_pInputControl;

    static CRITICAL_SECTION     m_csChildCtrl;

    /** 永远呆在底层 */
    bool                        m_bStayBottom;
};

typedef OUGUIObjectFactory<OUGUIObject> GUIFACTORY;

#endif
