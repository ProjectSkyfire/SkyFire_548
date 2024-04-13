/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef APPENDERDB_H
#define APPENDERDB_H

#include "Appender.h"

class AppenderDB : public Appender
{
public:
    AppenderDB(uint8 _id, std::string const& _name, LogLevel level);
    ~AppenderDB();

    void setRealmId(uint32 realmId);

private:
    uint32 realmId;
    bool enabled;
    void _write(LogMessage const& message);
};

#endif
