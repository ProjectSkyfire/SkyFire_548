/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef _ADHOCSTATEMENT_H
#define _ADHOCSTATEMENT_H

#include <ace/Future.h>
#include "SQLOperation.h"

typedef ACE_Future<QueryResult> QueryResultFuture;
/*! Raw, ad-hoc query. */
class BasicStatementTask : public SQLOperation
{
    public:
        BasicStatementTask(const char* sql);
        BasicStatementTask(const char* sql, QueryResultFuture result);
        ~BasicStatementTask();

        bool Execute();

    private:
        const char* m_sql;      //- Raw query to be executed
        bool m_has_result;
        QueryResultFuture m_result;
};

#endif