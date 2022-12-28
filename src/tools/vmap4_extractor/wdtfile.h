/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/
 
#ifndef WDTFILE_H
#define WDTFILE_H

#include "mpqfile.h"
#include "wmo.h"
#include <string>
#include <vector>
#include "stdlib.h"

class ADTFile;

class WDTFile
{
private:
    MPQFile WDT;
    std::string filename;
public:
    WDTFile(char* file_name, char* file_name1);
    ~WDTFile(void);
    bool init(char* map_id, unsigned int mapID);

    std::vector<std::string> gWmoInstansName;
    int gnWMO;

    ADTFile* GetMap(int x, int z);
};

#endif
