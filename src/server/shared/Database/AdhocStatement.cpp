/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "AdhocStatement.h"
#include "MySQLConnection.h"

/*! Basic, ad-hoc queries. */
BasicStatementTask::BasicStatementTask(const char* sql) :
    m_has_result(false)
{
    m_sql = _strdup(sql);
}

BasicStatementTask::BasicStatementTask(const char* sql, QueryResultFuture result) :
    m_has_result(true),
    m_result(result)
{
    m_sql = _strdup(sql);
}

BasicStatementTask::~BasicStatementTask()
{
    free((void*)m_sql);
}

bool BasicStatementTask::Execute()
{
    if (m_has_result)
    {
        ResultSet* result = m_conn->Query(m_sql);
        if (!result || !result->GetRowCount())
        {
            delete result;
            m_result.set(QueryResult(NULL));
            return false;
        }
        result->NextRow();
        m_result.set(QueryResult(result));
        return true;
    }

    return m_conn->Execute(m_sql);
}
