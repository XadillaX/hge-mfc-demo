#ifndef __MAPMAKERGLOBAL_H__
#define __MAPMAKERGLOBAL_H__

/** STLͷ�ļ� */
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <list>
#include <algorithm>
#include <deque>
#include <set>

/** �ײ�ͷ�ļ� */
#include "../OUPlazaRender/hge.h"
#include "../OUPlazaRender/hgeSprite.h"
#include "../OUPlazaRender/OUIniFile.h"

/** GUIͷ�ļ� */
#include "../OUPlazaGUISystem/Global.h"
#include "../OUPlazaGUISystem/OUFont.h"

/** ����ͷ�ļ� */
#include "OUMapViewMsg.h"
#include "OUSafeObj.h"
#include "OUMapView.h"
using namespace std;

#define OUMAP_TILE_NAME_FORMAT      "tile/road/tile%d"
#define OUMAP_CARPET_NAME_FORMAT    "tile/carpet/carpet%d"
#define OUMAP_BUILDING_NAME_FORMAT  "tile/building/building%d"

class OUMapMakerEXView;
extern OUIniFile g_IniFile;
extern string g_szCurSkinName;
extern OUMapView* g_pView;
extern OUMapMakerEXView* g_pViewDlg;
extern bool g_bFirstWillOpen;
extern string g_szFirstWillOpenName;

extern char* FormatSkinFilename(const char* name);
extern void OUFileList(string path, vector<string>& ext, vector<string>& filenames);
extern CString OUGetCurrentPath();

#endif
