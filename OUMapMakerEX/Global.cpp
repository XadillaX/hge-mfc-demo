#include "StdAfx.h"
#include "Global.h"

OUIniFile g_IniFile(::OUGetCurrentPath() + "\\config.ini");
string g_szCurSkinName("");
OUMapView* g_pView = NULL;
OUMapMakerEXView* g_pViewDlg = NULL;
bool g_bFirstWillOpen = false;
string g_szFirstWillOpenName = "";

char* FormatSkinFilename(const char* name)
{
    static char filename[512];
    sprintf(filename, "skin\\%s", name);

    return filename;
}

void OUFileList(string path, vector<string>& ext, vector<string>& filenames)
{
    ::CFileFind finder;
    string name = path + "\\*.*";

    bool bWorking = finder.FindFile(name.c_str());
    vector<OUStringFunc> exts;
    for(int i = 0; i < ext.size(); i++)
    {
        OUStringFunc str = ext[i];
        str.ToUppercase();
        exts.push_back(str);
    }

    while(bWorking)
    {
        bWorking = finder.FindNextFile();
        if(finder.IsDots()) continue;
        if(finder.IsDirectory()) continue;

        OUStringFunc str = finder.GetFileName();
        OUStringFunc org = str;
        str.ToUppercase();
        string e = str.GetExt();
        for(int i = 0; i < exts.size(); i++)
        {
            if(exts[i].GetString() == e)
            {
                filenames.push_back(org.GetString());
            }
        }
    }

    finder.Close();
}

CString OUGetCurrentPath()
{
    CString str;
    static char s[MAX_PATH + 1];
    ::GetCurrentDirectory(MAX_PATH, s);
    str = s;

    return str;
}
