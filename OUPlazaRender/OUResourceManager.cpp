#include "OUResourceManager.h"

OUResourceManager::OUResourceManager(void) :
    m_szFilename(""),
    m_szKey("")
{
}

bool OUResourceManager::OpenPackage(string& szFilename, string& szKey)
{
    m_szFilename = m_szKey = "";
    bool res = m_Pack.SetPackage(szFilename.c_str());
    if(!res) return false;

    m_Pack.SetKey(szKey.c_str());
    m_szFilename = szFilename;
    m_szKey = szKey;

    return true;
}

bool OUResourceManager::IsOpened()
{
    return m_Pack.IsOpened();
}

OUPACKFILEDATA& OUResourceManager::GetFile(string& szFilename)
{
    static OUPACKFILEDATA data;
    data = m_Pack.GetFile(szFilename.c_str());

    return data;
}

////////////////////////////////////////////////////////////////////

OUResourcePool::OUResourcePool()
{
}

#define FORRESMGRMAP for(map<HRESMGR, OUResourceManager*>::iterator it = m_Manager.begin(); it != m_Manager.end(); it++)
OUResourcePool::~OUResourcePool()
{
    FORRESMGRMAP
    {
        delete it->second;
    }
    m_Manager.clear();

    for(map<string, OUPACKFILEDATA>::iterator it = m_DataCache.begin(); it != m_DataCache.end(); it++)
    {
        delete []it->second.m_pBuf;
    }
    m_DataCache.clear();
}

HRESMGR OUResourcePool::OpenPackage(string& szFilename, string& szKey)
{
    OUResourceManager* mgr = new OUResourceManager();
    if(mgr == NULL) return 0;

    if(!mgr->OpenPackage(szFilename, szKey))
    {
        delete mgr;
        return 0;
    }

    m_Manager[(HRESMGR)mgr] = mgr;
    return (HRESMGR)mgr;
}

OUPACKFILEDATA OUResourcePool::GetResource(string& szFilename)
{
    OUPACKFILEDATA data;
    memset(&data, 0, sizeof(OUPACKFILEDATA));

    OUStringFunc fn(szFilename);
    fn.FormatPath(OUStringFunc::OUPathType::OUPT_UPPERCASE);
    if(m_DataCache.find(fn.GetString()) != m_DataCache.end())
    {
        return m_DataCache[fn.GetString()];
    }

    /** 遍历所有包裹找出符合的 */
    FORRESMGRMAP
    {
        data = it->second->GetFile(szFilename);
        if(data.m_dwFileSize != 0)
        {
            m_DataCache[fn.GetString()] = data;
            return data;
        }
    }

    return GetResourceUnpacked(szFilename);
}

OUPACKFILEDATA OUResourcePool::GetResource(HRESMGR hMgr, string& szFilename)
{
    OUPACKFILEDATA data;
    memset(&data, 0, sizeof(OUPACKFILEDATA));

    OUStringFunc fn(szFilename);
    fn.FormatPath(OUStringFunc::OUPathType::OUPT_UPPERCASE);
    if(m_DataCache.find(fn.GetString()) != m_DataCache.end())
    {
        return m_DataCache[fn.GetString()];
    }

    map<HRESMGR, OUResourceManager*>::iterator it;
    it = m_Manager.find(hMgr);
    if(it == m_Manager.end()) return data;

    data = it->second->GetFile(szFilename);
    if(data.m_dwFileSize == 0)
    {
        return GetResourceUnpacked(szFilename);
    }

    m_DataCache[fn.GetString()] = data;
    return data;
}

OUPACKFILEDATA OUResourcePool::GetResourceUnpacked(string& szFilename)
{
    OUPACKFILEDATA data;
    memset(&data, 0, sizeof(OUPACKFILEDATA));

    OUStringFunc fn(szFilename);
    fn.FormatPath(OUStringFunc::OUPathType::OUPT_UPPERCASE);
    if(m_DataCache.find(fn.GetString()) != m_DataCache.end())
    {
        return m_DataCache[fn.GetString()];
    }

    size_t dwFilesize = GetFilesizeUnpacked(szFilename);
    if(0 == dwFilesize) return data;

    FILE* fp = fopen(szFilename.c_str(), "rb");
    if(NULL == fp)
    {
        return data;
    }

    data.m_pBuf = new UCHAR[dwFilesize + 1];
    fread(data.m_pBuf, dwFilesize, 1, fp);
    fclose(fp);
    data.m_pBuf[dwFilesize] = 0;
    data.m_dwFileSize = dwFilesize;

    m_DataCache[fn.GetString()] = data;
    return data;
}

#include <sys\stat.h> 
size_t OUResourcePool::GetFilesizeUnpacked(string& szFilename)
{
    int nRes;
    struct _stat buf;

    nRes = _stat(szFilename.c_str(), &buf);
    if(nRes == 0)
    {
        return buf.st_size;
    }
    else return NULL;
}
