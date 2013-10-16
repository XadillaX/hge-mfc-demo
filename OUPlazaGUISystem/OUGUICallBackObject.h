/***********************************
 *
 * O游棋牌大厅世界
 * - CALLBACK成员函数委托类 -
 * 作者: XadillaX (admin#xcoder.in)
 *
 ***********************************/
#ifndef __OUGUICALLBACKOBJECT_H__
#define __OUGUICALLBACKOBJECT_H__
#pragma once
#include "Global.h"
#include <string>
#include <map>
using namespace std;

class OUGUIObject;
class OUGUICallBackObject;

/**
 * @brief 回调参数事件基类
 * 回调时所需的事件对象
 */
class OUGUI_API OUGUIEvent
{
public:
    OUGUIEvent(){}
    virtual ~OUGUIEvent(){}
};

/**
 * @brief 委托函数定义
 * C++类成员函数委托的定义
 */
typedef void (OUGUICallBackObject::*CALLBACK_FUNC)(OUGUIObject*, OUGUIEvent*);

/**
 * @brief 委托函数外包
 * 委托函数外包，包括委托函数指针以及其父对象的指针。
 *
 * 调用时即(pPar->*pFunc)
 */
typedef struct tagOUGUI_CBWRAPPER
{
    OUGUICallBackObject* pPar;
    CALLBACK_FUNC pFunc;
} OUGUI_CBWRAPPER, *POUGUI_CBWRAPPER;

/**
 * @brief 委托基类
 * 将需要用成员函数委托的类继承这个类即可调用
 *  1. AddCallBack
 *  2. GetCallBack
 *  3. RunCallBack
 * 三个函数来进行委托了。
 */
class OUGUI_API OUGUICallBackObject
{
public:
    OUGUICallBackObject(void);
    virtual ~OUGUICallBackObject(void);

public:
    static bool                 AddCallBack(string name, CALLBACK_FUNC func, OUGUICallBackObject* parent, bool overwrite = false);
    static OUGUI_CBWRAPPER*     GetCallBack(string name);

    static void                 RunCallBack(string name, OUGUIObject* pSender, OUGUIEvent* pEvent);

    static void                 Clear();

protected:
    static map<string, OUGUI_CBWRAPPER*> m_pFuncMap;
};
#endif
