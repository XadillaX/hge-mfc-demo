/***********************************
 *
 * O�����ƴ�������
 * - CALLBACK��Ա����ί���� -
 * ����: XadillaX (admin#xcoder.in)
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
 * @brief �ص������¼�����
 * �ص�ʱ������¼�����
 */
class OUGUI_API OUGUIEvent
{
public:
    OUGUIEvent(){}
    virtual ~OUGUIEvent(){}
};

/**
 * @brief ί�к�������
 * C++���Ա����ί�еĶ���
 */
typedef void (OUGUICallBackObject::*CALLBACK_FUNC)(OUGUIObject*, OUGUIEvent*);

/**
 * @brief ί�к������
 * ί�к������������ί�к���ָ���Լ��丸�����ָ�롣
 *
 * ����ʱ��(pPar->*pFunc)
 */
typedef struct tagOUGUI_CBWRAPPER
{
    OUGUICallBackObject* pPar;
    CALLBACK_FUNC pFunc;
} OUGUI_CBWRAPPER, *POUGUI_CBWRAPPER;

/**
 * @brief ί�л���
 * ����Ҫ�ó�Ա����ί�е���̳�����༴�ɵ���
 *  1. AddCallBack
 *  2. GetCallBack
 *  3. RunCallBack
 * ��������������ί���ˡ�
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
