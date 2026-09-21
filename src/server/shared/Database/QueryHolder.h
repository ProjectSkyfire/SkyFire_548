/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _QUERYHOLDER_H
#define _QUERYHOLDER_H

#include "Threading/Future.h"

class SQLQueryHolder
{
    friend class SQLQueryHolderTask;
private:
    typedef std::pair<SQLElementData, SQLResultSetUnion> SQLResultPair;
    std::vector<SQLResultPair> m_queries;
    uint32 m_characterGuid = 0;
    uint32 m_characterAccount = 0;
    bool m_declinedNames = false;
public:
    SQLQueryHolder() { }
    ~SQLQueryHolder();
    bool SetQuery(size_t index, const char* sql);
    bool SetPQuery(size_t index, const char* format, ...) ATTR_PRINTF(3, 4);
    bool SetPreparedQuery(size_t index, PreparedStatement* stmt);
    void SetSize(size_t size);
    void SetCharacterLoad(uint32 guid, uint32 account, bool declinedNames);
    QueryResult GetResult(size_t index);
    PreparedQueryResult GetPreparedResult(size_t index);
    void SetResult(size_t index, ResultSet* result);
    void SetPreparedResult(size_t index, PreparedResultSet* result);
};

typedef Skyfire::Future<SQLQueryHolder*> QueryResultHolderFuture;

class SQLQueryHolderTask : public SQLOperation
{
private:
    SQLQueryHolder* m_holder;
    QueryResultHolderFuture m_result;

public:
    SQLQueryHolderTask(SQLQueryHolder* holder, QueryResultHolderFuture res)
        : m_holder(holder), m_result(res) { };
    bool Execute();
};

#endif
