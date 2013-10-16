//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
// @file        : X2ObjectFactory.h
// @project     : XSquareENGINE
// @date        : 2012/09/09
// @author      : XadillaX
// @copyright   : 2012-2013 X2Studio
//
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
#ifndef __OUGUIOBJECTFACTORY_H__
#define __OUGUIOBJECTFACTORY_H__
#pragma once

#include <map>
#include <string>
#include "OUGUISingleton.h"
#include "../OUPlazaRender/OUStringFunc.h"
using namespace std;

template <class T>
class OUGUI_API OUGUIObjectFactory : public OUGUISingleton<OUGUIObjectFactory<T>>
{
public:
    typedef T* (*tCreator)();
    typedef map<string, tCreator> tCreatorMap;

    bool reg(const char* type, tCreator procedure)
    {
        return reg(string(type), procedure);
    }

    bool reg(const string& type, tCreator procedure)
    {
        OUStringFunc str(type.c_str());
        string newstr = str.ToUppercase();

        if(_map.find(newstr) != _map.end())
        {
            return false;
        }

        _map[newstr] = procedure;
        return true;
    }

    T* create(const char* type)
    {
        return create(string(type));
    }

    T* create(const string& type)
    {
        OUStringFunc str(type.c_str());
        string newstr = str.ToUppercase();

        tCreatorMap::iterator it = _map.find(newstr);
        if(_map.end() == it)
        {
            return NULL;
        }

        tCreator t = it->second;
        return t();
    }

private:
    tCreatorMap _map;
};

#endif
