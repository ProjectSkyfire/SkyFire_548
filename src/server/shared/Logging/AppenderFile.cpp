/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AppenderFile.h"
#include "Common.h"

#if PLATFORM == PLATFORM_WINDOWS
# include <Windows.h>
#endif

AppenderFile::AppenderFile(uint8 id, std::string const& name, LogLevel level, const char* _filename, const char* _logDir, const char* _mode, AppenderFlags _flags, uint64 fileSize) :
    Appender(id, name, AppenderType::APPENDER_FILE, level, _flags),
    logfile(NULL),
    filename(_filename),
    logDir(_logDir),
    mode(_mode),
    maxFileSize(fileSize),
    fileSize(0)
{
    dynamicName = std::string::npos != filename.find("%s");
    backup = (_flags & APPENDER_FLAGS_MAKE_FILE_BACKUP) != 0;

    logfile = !dynamicName ? OpenFile(_filename, _mode, mode == "w" && backup) : NULL;
}

AppenderFile::~AppenderFile()
{
    CloseFile();
}

void AppenderFile::_write(LogMessage const& message)
{
    bool exceedMaxSize = maxFileSize > 0 && (fileSize.value() + message.Size()) > maxFileSize;

    if (dynamicName)
    {
        char namebuf[SKYFIRE_PATH_MAX];
        snprintf(namebuf, SKYFIRE_PATH_MAX, filename.c_str(), message.param1.c_str());
        logfile = OpenFile(namebuf, mode, backup || exceedMaxSize);
    }
    else if (exceedMaxSize)
        logfile = OpenFile(filename, "w", true);

    if (!logfile)
        return;

    fprintf(logfile, "%s%s", message.prefix.c_str(), message.text.c_str());
    fflush(logfile);
    fileSize += uint64(message.Size());

    if (dynamicName)
        CloseFile();
}

FILE* AppenderFile::OpenFile(std::string const& filename, std::string const& mode, bool backup)
{
    std::string fullName(logDir + filename);
    if (backup)
    {
        CloseFile();
        std::string newName(fullName);
        newName.push_back('.');
        newName.append(LogMessage::getTimeStr(time(NULL)));
        rename(fullName.c_str(), newName.c_str()); // no error handling... if we couldn't make a backup, just ignore
    }

    if (FILE* ret = fopen(fullName.c_str(), mode.c_str()))
    {
        fileSize = ftell(ret);
        return ret;
    }

    return NULL;
}

void AppenderFile::CloseFile()
{
    if (logfile)
    {
        fclose(logfile);
        logfile = NULL;
    }
}
