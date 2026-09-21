/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#ifndef _TRANSACTION_H
#define _TRANSACTION_H

#include "Platform/Threading.h"
#include "SQLOperation.h"

//- Forward declare (don't include header to prevent circular includes)
class PreparedStatement;

/*! Transactions, high level class. */
class Transaction
{
    friend class CharacterServiceClient;
    friend class TransactionTask;
    friend class MySQLConnection;

    template <typename T>
    friend class DatabaseWorkerPool;

public:
    Transaction() : _cleanedUp(false) { }
    ~Transaction() { Cleanup(); }

    void Append(PreparedStatement* statement);
    void Append(const char* sql);
    void PAppend(const char* sql, ...);
    // Own a character-state value buffer; the service selects the persistence SQL.
    void SetCharacterSave(uint32 guid, uint32 account, bool create, PreparedStatement* state);

    size_t GetSize() const { return m_queries.size() + (m_characterState ? 1 : 0); }

protected:
    void Cleanup();
    std::list<SQLElementData> m_queries;

private:
    bool _cleanedUp;
    uint32 m_characterGuid = 0;
    uint32 m_characterAccount = 0;
    bool m_characterCreate = false;
    PreparedStatement* m_characterState = nullptr;
};

typedef Skyfire::AutoPtr<Transaction, Skyfire::Mutex> SQLTransaction;

/*! Low level class*/
class TransactionTask : public SQLOperation
{
    template <class T> friend class DatabaseWorkerPool;
    friend class DatabaseWorker;

public:
    TransactionTask(SQLTransaction trans) : m_trans(trans) { };
    ~TransactionTask() { };

protected:
    bool Execute();

    SQLTransaction m_trans;
};

#endif
