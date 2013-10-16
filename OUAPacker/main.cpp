////////////////////////////////////////
//
// OU棋牌动画文件打包器
//
////////////////////////////////////////
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include "hge/hge.h"
#include <set>
#include "header.h"
using namespace std;

#pragma comment(lib, "hge.lib")

string dir = "";
int fps = 60;
string out = "output.oga";
string fmt = "%.2d.png";
int start = 0;
int spotx = 0;
int spoty = 0;
HGE* hge = NULL;
int rwidth;
int rheight;
unsigned long trans = 0x00000000;

vector<HTEXTURE> texlist;
set<DWORD> clrset;
vector<DWORD> clrvec;
map<DWORD, int> clrmap;

void showcmd()
{
    printf("** OGame Plaza Animation File Packer **\n");
    printf("             By XadillaX\n\n");
    printf("Please input a directory and its FPS...\n\n");
    printf("Usage:\n");
    printf("    ouapack [-p directory] [-f fps] [-o output] [-fmt xx%d.png] [-s startfrom] [-x spotX] [-y spotY] [-trans color]\n");
    printf("    ouapack --help");
}

void release()
{
    for(int i = 0; i < texlist.size(); i++)
    {
        hge->Texture_Free(texlist[i]);
    }

    hge->Release();
}

int main(int argc, char* argv[])
{
    /** 获取指令 */
    for(int i = 1; i < argc; i++)
    {
        if(!strcmp(argv[i], "--help") && i == 1)
        {
            showcmd();
            return 0;
        }

        if(i == argc - 1) break;
        if(!strcmp(argv[i], "-p"))
        {
            dir = argv[i + 1];
        }
        else
        if(!strcmp(argv[i], "-f"))
        {
            sscanf(argv[i + 1], "%d", &fps);
        }
        else
        if(!strcmp(argv[i], "-o"))
        {
            out = argv[i + 1];
        }
        else
        if(!strcmp(argv[i], "-fmt"))
        {
            fmt = argv[i + 1];
        }
        else
        if(!strcmp(argv[i], "-s"))
        {
            sscanf(argv[i + 1], "%d", &start);
        }
        else
        if(!strcmp(argv[i], "-x"))
        {
            sscanf(argv[i + 1], "%d", &spotx);
        }
        else
        if(!strcmp(argv[i], "-y"))
        {
            sscanf(argv[i + 1], "%d", &spoty);
        }
        else
        if(!strcmp(argv[i], "-trans"))
        {
            sscanf(argv[i + 1], "%lx", &trans);
        }
    }

    hge = hgeCreate(HGE_VERSION);
    hge->System_SetState(HGE_USESOUND, false);
    hge->System_SetState(HGE_WINDOWED, true);
    hge->System_SetState(HGE_SCREENWIDTH, 50);
    hge->System_SetState(HGE_SCREENHEIGHT, 50);
    hge->System_SetState(HGE_SHOWSPLASH, false);
    if(!hge->System_Initiate())
    {
        printf("System error...\n");
        return 0;
    }

    /** 遍历 */
    int cnt = start;
    if(dir != "") fmt = dir + "\\" + fmt;
    do {
        char filename[256];
        sprintf(filename, fmt.c_str(), cnt);
        HTEXTURE hTex = hge->Texture_Load(filename);
        //printf("%s\n", filename);
        if(hTex == 0) break;

        printf("Importing file \"%s\" with handle %.8X...\n", filename, hTex);
        texlist.push_back(hTex);
        if(cnt == start)
        {
            rwidth = hge->Texture_GetWidth(hTex, true);
            rheight = hge->Texture_GetHeight(hTex, true);
        }

        DWORD *mem = hge->Texture_Lock(hTex, true, 0, 0, rwidth, rheight);
        for(int i = 0; i < rheight; i++)
        {
            for(int j = 0; j < rwidth; j++)
            {
                if(mem[j + i * rwidth] != trans) clrset.insert(mem[j + i * rwidth]);
                else clrset.insert(0x00000000);
            }
        }
        hge->Texture_Unlock(hTex);
        
        cnt++;

        /** 不存在编号的图片 */
        if(cnt - 1 == start)
        {
            char filename2[256];
            sprintf(filename2, fmt.c_str(), cnt);
            if(!strcmp(filename, filename2)) break;
        }
    } while(true);

    printf("\nFinishing importing...\n");
    printf("Count of colors: %d...\n", clrset.size());
    printf("Processing color dictionary...\n");

    for(set<DWORD>::iterator it = clrset.begin(); it != clrset.end(); it++)
    {
        clrvec.push_back(*it);
    }
    sort(clrvec.begin(), clrvec.end());
    for(int i = 0; i < clrvec.size(); i++)
    {
        clrmap[clrvec[i]] = i;
    }

    printf("\nCreating packed file...\n");

    FILE* fp = fopen(out.c_str(), "wb");
    if(fp == NULL)
    {
        printf("Can't create the file \"%s\"...\n", out.c_str());
        release();
        return 0;
    }

    ouaheader header;
    memcpy(&header.magic_num, &magic_str, sizeof(DWORD));
    header.fps = fps;
    header.width = rwidth;
    header.height = rheight;
    header.pic_num = texlist.size();
    header.clr_num = clrvec.size();
    header.spotx = spotx;
    header.spoty = spoty;
    header.calc_color_byte();

    printf("Writing header...\n");
    fwrite(&header, sizeof(header), 1, fp);

    printf("Writing dictionary...\n");
    DWORD c;
    for(int i = 0; i < clrvec.size(); i++)
    {
        c = clrvec[i];
        fwrite(&c, sizeof(DWORD), 1, fp);
    }

    DWORD* mem;
    int wh = rwidth * rheight;
    pixinfo pix;
    for(int i = 0; i < texlist.size(); i++)
    {
        char filename[256];
        sprintf(filename, fmt.c_str(), i + start);
        printf("Writing %.8X (%s)...\n", texlist[i], filename);
        mem = hge->Texture_Lock(texlist[i]);
        pix.idx = mem[0] != trans ? clrmap[mem[0]] : clrmap[0x00000000];
        pix.repeat = 1;

        for(int j = 1; j < wh; j++)
        {
            int idx = mem[j] != trans ? clrmap[mem[j]] : clrmap[0x00000000];
            if(pix.idx == idx && pix.repeat < 65535)
            {
                pix.repeat++;
                continue;
            }
            else
            {
                fwrite(&(pix.idx), header.clr_byte, 1, fp);
                fwrite(&(pix.repeat), sizeof(unsigned short), 1, fp);
                pix.idx = idx;
                pix.repeat = 1;
            }
        }
        fwrite(&(pix.idx), header.clr_byte, 1, fp);
        fwrite(&(pix.repeat), sizeof(unsigned short), 1, fp);

        hge->Texture_Unlock(texlist[i]);
    }

    fclose(fp);
    release();

    printf("\nFinished...\n");

    return 0;
}
