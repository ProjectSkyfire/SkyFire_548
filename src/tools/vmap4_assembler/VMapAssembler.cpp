/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifdef _WIN32
  #include "direct.h"
#else
  #include <sys/stat.h>
#endif

#include <string>
#include <iostream>

#include "TileAssembler.h"

void CreateDir(std::string const& path)
{
    if (chdir(path.c_str()) == 0)
    {
        chdir("../");
        return;
    }

#ifdef _WIN32
    _mkdir(path.c_str());
#else
    mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IRWXO); // 0777
#endif
}

int main(int argc, char* argv[])
{
    std::string src = "Buildings";
    std::string dest = "vmaps";

    std::string path = "./" + dest + "/";
    
    CreateDir(path);

    if(argc > 3)
    {
        //printf("\nusage: %s <raw data dir> <vmap dest dir> [config file name]\n", argv[0]);
        std::cout << "usage: " << argv[0] << " <raw data dir> <vmap dest dir>" << std::endl;
        return 1;
    }
    else
    {
        if (argc > 1)
            src = argv[1];
        if (argc > 2)
            dest = argv[2];
    }
    
    std::cout << "using " << src << " as source directory and writing output to " << dest << std::endl;

    VMAP::TileAssembler* ta = new VMAP::TileAssembler(src, dest);

    if(!ta->convertWorld2())
    {
        std::cout << "exit with errors" << std::endl;
        delete ta;
        return 1;
    }

    delete ta;
    std::cout << "Ok, all done" << std::endl;
    return 0;
}
