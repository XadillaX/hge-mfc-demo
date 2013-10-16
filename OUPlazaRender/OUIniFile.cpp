#include "OUIniFile.h"
using namespace std;

OUIniFile::OUIniFile(void) :
    m_szFilename("")
{
}

OUIniFile::~OUIniFile(void)
{
}

void OUIniFile::SetFile(const char *filename)
{
    m_szFilename = filename;
    m_Map.clear();
}

int OUIniFile::GetInt(const char *sec, const char *key, int def)
{
    if(m_Map[sec].find(key) != m_Map[sec].end())
    {
        return atoi(m_Map[sec][key].c_str());
    }

    int num = GetPrivateProfileInt(sec, key, def, m_szFilename.c_str());
    m_Map[sec][key] = num;

    return num;
}

string OUIniFile::GetString(const char *sec, const char *key, const char *def)
{
    if(m_Map[sec].find(key) != m_Map[sec].end())
    {
        return m_Map[sec][key];
    }

    static char val[512];
    ::GetPrivateProfileString(sec, key, def, val, 512, m_szFilename.c_str());

    m_Map[sec][key] = val;

    return m_Map[sec][key];
}

bool OUIniFile::SetInt(const char* sec, const char* key, int val)
{
    static char num[15];
    sprintf(num, "%d", val);

    return SetString(sec, key, num);
}

bool OUIniFile::SetString(const char* sec, const char* key, const char* val)
{
    bool res = ::WritePrivateProfileString(sec, key, val, m_szFilename.c_str());
    if(!res) return false;

    m_Map[sec][key] = val;
    return true;
}
