/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#include "AppenderDB.h"
#include "Database/DatabaseEnv.h"

AppenderDB::AppenderDB(uint8 id, std::string const& name, LogLevel level)
    : Appender(id, name, AppenderType::APPENDER_DB, level), realmId(0), enabled(false) { }

AppenderDB::~AppenderDB() { }

void AppenderDB::_write(LogMessage const& message)
{
    // Avoid infinite loop, PExecute triggers Logging with "sql.sql" type
    if (!enabled || !message.type.find("sql"))
        return;

    PreparedStatement* stmt = LoginDatabase.GetPreparedStatement(LOGIN_INS_LOG);
    stmt->setUInt64(0, message.mtime);
    stmt->setUInt32(1, realmId);
    stmt->setString(2, message.type);
    stmt->setUInt8(3, uint8(message.level));
    stmt->setString(4, message.text);
    LoginDatabase.Execute(stmt);
}

void AppenderDB::setRealmId(uint32 _realmId)
{
    enabled = true;
    realmId = _realmId;
}
