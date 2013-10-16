//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// @file        : X2Singleton.h
// @project     : XSquareENGINE
// @date        : 2012/09/09
// @author      : XadillaX
// @copyright   : 2012-2013 X2Studio
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifndef __X2SINGLETON_H__
#define __X2SINGLETON_H__
#pragma once
#include "hge.h"

template <class T>
class __declspec(dllexport) X2Singleton
{
public:
    static T& instance()
    {
        static T instance;
        return instance;
    }

protected:
    X2Singleton(){}
    virtual ~X2Singleton(){}
    X2Singleton(X2Singleton const &){}
    X2Singleton & operator = (X2Singleton const &){return *this;}
};

#endif
