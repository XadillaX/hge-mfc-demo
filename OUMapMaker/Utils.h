#ifndef __UTILS_H__
#define __UTILS_H__
#include <windows.h>
#include <vector>
#include <string>
using namespace std;

#define OUMAP_TILE_NAME_FORMAT      "Assets/tile/road/tile%d"
#define OUMAP_MAP_NAME_FORMAT       "Assets/map/%s.map"
#define OUMAP_BUILDING_NAME_FORMAT  "Assets/tile/building/wall%d"
#define OUMAP_BMAP_NAME_FORMAT      "Assets/map/%sbuilding.map"

extern void OUFileList(string path, vector<string>& ext, vector<string>& filenames);
extern CString OUGetCurrentPath();

#endif
