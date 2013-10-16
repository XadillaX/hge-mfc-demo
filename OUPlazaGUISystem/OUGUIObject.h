/***********************************
 *
 * O������GUIϵͳ
 * - GUI���� -
 * ����: XadillaX (admin#xcoder.in)
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

/** �ӿؼ�Map: key -> �ӿؼ���� */
typedef map<string, POUGUI_CHILD_BODY> GUIMap;
typedef list<POUGUI_CHILD_BODY> GUIList;

/** GUI���� */
class OUGUI_API OUGUIObject
{
public:
    OUGUIObject();
    virtual ~OUGUIObject();

public:
    virtual void                OnDrawBoard(DWORD dwColor);

    /** �¼���Ӧ�麯�������̳� */
public:
    /** ��ʱ�¼� */
    virtual void                OnIdle()                                {}
    /** ����Ⱦʱ */
    virtual void                OnRender()                              {}
    /** ���߼�����ʱ */
    virtual void                OnUpdate(float fDelta, float fMX, float fMY){}

    /** ������λ��ʱ */
    virtual void                OnSetPos(float fX, float fY)            {}
    /** �����ô�Сʱ */
    virtual void                OnSetSize(float fW, float fH)           {}
    /** ������״̬ʱ** */
    virtual void                OnSetStatus(bool bStatus)               {}
    /** �������Ƿ���ʾʱ */
    virtual void                OnSetShowMode(bool bShowMode)           {}
    /** �������Ƿ�����ʱ */
    virtual void                OnSetEnableMode(bool bEnableMode)       {}
    /** ���ý���ʱ */
    virtual void                OnSetFocusControl(bool bFocusMode)      {}

    /** ������ʱ */
    virtual void                OnMouseEnter()                          {}
    /** ����뿪ʱ */
    virtual void                OnMouseLeave()                          {}
    /** ����ƶ�ʱ: fMX, fMYΪ�����ڴ˿ؼ������λ�� */
    virtual void                OnMouseMove(float fMX, float fMY)       {}
    /** �������ʱ */
    virtual void                OnMouseLButtonDown()                    {}
    /** ���̧��ʱ */
    virtual void                OnMouseLButtonUp()                      {}
    /** �Ҽ����� */
    virtual void                OnMouseRButtonDown()                    {}
    /** �Ҽ�̧�� */
    virtual void                OnMouseRButtonUp()                      {}
    /** �м����� */
    virtual void                OnMouseMButtonDown()                    {}
    /** �м�̧�� */
    virtual void                OnMouseMButtonUp()                      {}
    /** ������ */
    virtual void                OnMouseWheel(int nMouseWheel)           {}
    /** ���̰��� */
    virtual void                OnKeyDown(int nKey)                     {}
    /** ����̧�� */
    virtual void                OnKeyUp(int nKey)                       {}
    /** �����û� */
    virtual void                OnKeyPress(int nKey)                    {}
    
    /** �����¼� */
    virtual LRESULT             OnWndProc(HWND hWnd, DWORD dwMsg, WPARAM& wParam, LPARAM& lParam){ return (LRESULT)0; }
    /** ��ײ���� */
    virtual bool                OnCheckCollision(float fMX, float fMY)  { return (fMX >= m_ptPos.m_fX) && (fMX <= m_ptPos.m_fX + m_ptSize.m_fX) && (fMY >= m_ptPos.m_fY) && (fMY <= m_ptPos.m_fY + m_ptSize.m_fY); }
    /** ����ؼ� */
    virtual bool                OnLoadFromXml(TiXmlElement* pElement, OUGUIObject* pWillParent);

    /** ��̬���� */
public:
    /** ��ʼ��GUI���� */
    static void                 InitializeGUIEnviroment();
    /** �ͷ�GUI���� */
    static void                 ReleaseGUIEnviroment();
    /** ��ȡ�ٽ��� */
    static CRITICAL_SECTION&    GetChildCtrlCriticalSection()           { return m_csChildCtrl; }
    /** ��ȡ��겶��ؼ� */
    static OUGUIObject*         GetMouseCaptureControl()                { return OUGUIObject::m_pMouseCaptureControl; }
    /** ������겶��ؼ� */
    static void                 SetMouseCaptureControl(OUGUIObject* pObj){ OUGUIObject::m_pMouseCaptureControl = pObj; }
    /** ���ý���ؼ� */
    static void                 SetFocusControl(OUGUIObject* pObj)      { if(OUGUIObject::m_pFocusControl == pObj) return; if(OUGUIObject::m_pFocusControl) OUGUIObject::m_pFocusControl->OnSetFocusControl(false); OUGUIObject::m_pFocusControl = pObj; if(pObj) pObj->OnSetFocusControl(true); }
    /** ��ȡ����ؼ� */
    static OUGUIObject*         GetFocusControl()                       { return OUGUIObject::m_pFocusControl; }
    /** ���ü���ؼ� */
    static void                 SetActiveControl(OUGUIObject* pObj)     { OUGUIObject::m_pActiveControl = pObj; }
    /** ��ȡ����ؼ� */
    static OUGUIObject*         GetActiveControl()                      { return m_pActiveControl; }
    /** ����Modal���� */
    static void                 SetModalControl(OUGUIObject* pObj)      { OUGUIObject::m_pModalControl = pObj; }
    /** ��ȡModal���� */
    static OUGUIObject*         GetModalControl()      { return OUGUIObject::m_pModalControl; }

public:
    /** fX��fY����Ҫ��������� */
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

    /** ���ÿ��ƶ���Χ */
    void                        SetMoveRange(float fX, float fY, float fW, float fH);
    /** ��Ӧ���ƶ���Χ */
    BYTE                        AdjustPosRange(float fX, float fY);

    /** ��겶�� */
    void                        CaptureMouse(bool bCapture);

    /** �Ƿ��ӿؼ����������ӿؼ�������ؼ�...�� */
    bool                        IsChild(OUGUIObject* pControl);
    /** �Ƿ񸸿ռ䣨�������ȿռ䣩 */
    bool                        IsParent(OUGUIObject* pControl);
    /** ��ĳ���ӿؼ���������β������Խ����Ĳ�����Խǰ�棩 */
    bool                        MoveToTail(OUGUIObject* pControl);

    /** ���������� */
    void                        SetMouseEnter(bool bEnter);
    /** �鿴����Ƿ��ڿؼ��� */
    bool                        GetMouseEnter()                         { return m_bMouseEnter; }
    /** ���������� */
    void                        SetMouseLButton(bool bDown);
    /** ��������Ҽ� */
    void                        SetMouseRButton(bool bDown);
    /** ��������м� */
    void                        SetMouseMButton(bool bDown);
    /** ��ȡ */
    bool                        GetMouseLButton()                       { return m_bMouseLButton; }
    bool                        GetMouseRButton()                       { return m_bMouseRButton; }
    bool                        GetMouseMButton()                       { return m_bMouseMButton; }
    /** ���ù��� */
    void                        SetMouseWheel(int nWheel);
    int                         GetMouseWheel()                         { return m_nMouseWheel; }

    /** ����ؼ� */
    bool                        LoadFromXml(TiXmlElement* pElement, OUGUIObject* pWillParent);

    /** �Ƿ���Զ���ڵײ� */
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
    /** ������λ�� */
    OUPOINT                     m_ptMousePos;

    bool                        m_bMouseEnter;
    bool                        m_bMouseLButton;
    bool                        m_bMouseRButton;
    bool                        m_bMouseMButton;
    int                         m_nMouseWheel;

    OU_RECT                     m_rtMoveRange;

    OUGUIObject*                m_pParent;
    //������ôд
    //GUIMap                      m_pControlList;
    GUIList                     m_pControlList;

    POUGUI_CHILD_BODY           m_pWrapper;

    static OUGUIObject*         m_pActiveControl;
    static OUGUIObject*         m_pFocusControl;
    static OUGUIObject*         m_pMouseCaptureControl;
    static OUGUIObject*         m_pModalControl;
    static OUGUIObject*         m_pInputControl;

    static CRITICAL_SECTION     m_csChildCtrl;

    /** ��Զ���ڵײ� */
    bool                        m_bStayBottom;
};

typedef OUGUIObjectFactory<OUGUIObject> GUIFACTORY;

#endif
