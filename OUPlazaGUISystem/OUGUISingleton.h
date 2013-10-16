//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// @file        : X2Singleton.h
// @project     : XSquareENGINE
// @date        : 2012/09/09
// @author      : XadillaX
// @copyright   : 2012-2013 X2Studio
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifndef __OUGUISINGLETON_H__
#define __OUGUISINGLETON_H__
#pragma once
#include "global.h"

template <class T>
class OUGUI_API OUGUISingleton
{
public:
    static T& instance();

protected:
    OUGUISingleton(){}
    virtual ~OUGUISingleton(){}
    OUGUISingleton(OUGUISingleton const &){}
    OUGUISingleton & operator = (OUGUISingleton const &){return *this;}
};

#define OUGUISINGLETON_IMPL(T) template<> \
    T& OUGUISingleton<T>::instance() \
    { \
        static T instance; \
        return instance; \
    }

#endif
