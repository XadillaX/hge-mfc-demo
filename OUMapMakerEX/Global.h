#ifndef __MAPMAKERGLOBAL_H__
#define __MAPMAKERGLOBAL_H__

/** STL头文件 */
#include <vector>
#include <string>
#include <queue>
#include <map>
#include <list>
#include <algorithm>
#include <deque>
#include <set>

/** 底层头文件 */
#include "../OUPlazaRender/hge.h"
#include "../OUPlazaRender/hgeSprite.h"
#include "../OUPlazaRender/OUIniFile.h"

/** GUI头文件 */
#include "../OUPlazaGUISystem/Global.h"
#include "../OUPlazaGUISystem/OUFont.h"

/** 公共头文件 */
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
