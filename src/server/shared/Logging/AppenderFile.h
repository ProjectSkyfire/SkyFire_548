/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef APPENDERFILE_H
#define APPENDERFILE_H

#include "ace/Atomic_Op.h"
#include "Appender.h"

class AppenderFile : public Appender
{
public:
    AppenderFile(uint8 _id, std::string const& _name, LogLevel level, const char* filename, const char* logDir, const char* mode, AppenderFlags flags, uint64 maxSize);
    ~AppenderFile();
    FILE* OpenFile(std::string const& _name, std::string const& _mode, bool _backup);

private:
    void CloseFile();
    void _write(LogMessage const& message);
    FILE* logfile;
    std::string filename;
    std::string logDir;
    std::string mode;
    bool dynamicName;
    bool backup;
    uint64 maxFileSize;
    ACE_Atomic_Op<ACE_Thread_Mutex, uint64> fileSize;
};

#endif
