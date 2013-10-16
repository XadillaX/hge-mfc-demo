#include "OUStringFunc.h"

OUStringFunc::OUStringFunc(void) :
    m_szString("")
{
}

OUStringFunc::OUStringFunc(string& szString) :
    m_szString(szString)
{
}

OUStringFunc::OUStringFunc(const char* szString) :
    m_szString(szString)
{
}

OUStringFunc::~OUStringFunc(void)
{
}

void OUStringFunc::SetString(string& szString)
{
    m_szString = szString;
}

string& OUStringFunc::GetString()
{
    return m_szString;
}

string& OUStringFunc::ToUppercase()
{
    for(int i = 0; i < m_szString.length(); i++)
    {
        if((char)m_szString[i] < 0)
        {
            i++;
        }
        else
        if(m_szString[i] >= 'a' && m_szString[i] <= 'z')
        {
            m_szString[i] = m_szString[i] - 'a' + 'A';
        }
    }

    return GetString();
}

string& OUStringFunc::ToLowercase()
{
    for(int i = 0; i < m_szString.length(); i++)
    {
        if((char)m_szString[i] < 0)
        {
            i++;
        }
        else
        if(m_szString[i] >= 'A' && m_szString[i] <= 'Z')
        {
            m_szString[i] = m_szString[i] - 'A' + 'a';
        }
    }

    return GetString();
}

string& OUStringFunc::Trim()
{
    LeftTrim();
    RightTrim();

    return GetString();
}

string& OUStringFunc::LeftTrim()
{
    while(m_szString[0] == ' ' || m_szString[0] == '\t')
    {
        m_szString.erase(0, 1);
    }

    return GetString();
}

string& OUStringFunc::RightTrim()
{
    while(m_szString[m_szString.length() - 1] == ' ' || m_szString[m_szString.length() - 1] == '\t')
    {
        m_szString.erase(m_szString.length() - 1, 1);
    }

    return GetString();
}

string& OUStringFunc::FormatPath(OUPathType oupt)
{
    string newString = "";
    for(int i = 0; i < m_szString.length(); i++)
    {
        if((char)m_szString[i] < 0)
        {
            newString += m_szString[i];
            i++;
            newString += m_szString[i];
        }
        else
        if(m_szString[i] == '/' || m_szString[i] == '\\')
        {
            /** 若上一个字符已经是路径分隔符 */
            if(newString.length() && newString[newString.length() - 1] == '\\')
            {
                continue;
            }

            newString += '\\';
        }
        else
        {
            newString += m_szString[i];
        }
    }

    m_szString = newString;
    
    switch(oupt)
    {
    case OUPT_UPPERCASE:
        {
            return ToUppercase();
        }

    case OUPT_LOWERCASE:
        {
            return ToLowercase();
        }

    case OUPT_NOCHANGE:
        {
            return GetString();
        }

    default:
        {
            return GetString();
        }
    }

    return GetString();
}

bool OUStringFunc::CaseInsensitiveEqual(string a, string b)
{
    OUStringFunc sa(a), sb(b);

    return sa.ToUppercase() == sb.ToUppercase();
}

string& OUStringFunc::GetExt()
{
    static string str;
    str = "";

    for(int i = m_szString.length() - 1; i >= 0; i--)
    {
        str = m_szString[i] + str;
        if(m_szString[i] == '.') break;
    }

    return str;
}
