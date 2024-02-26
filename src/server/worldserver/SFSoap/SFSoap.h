/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef _SFSoap_H
#define _SFSoap_H

#include "Define.h"

#include <ace/Semaphore.h>
#include <ace/Task.h>
#include <Threading.h>

class SFSoapRunnable : public ACE_Based::Runnable
{
    public:
        SFSoapRunnable() : _port(0) { }

        void run() OVERRIDE;

        void SetListenArguments(const std::string& host, uint16 port)
        {
            _host = host;
            _port = port;
        }

    private:
        void process_message(ACE_Message_Block* mb);

        std::string _host;
        uint16 _port;
};

class SOAPCommand
{
    public:
        SOAPCommand():
            pendingCommands(0, USYNC_THREAD, "pendingCommands"), m_success(false)
        {
        }

        ~SOAPCommand()
        {
        }

        void appendToPrintBuffer(std::string msg)
        {
            m_printBuffer += msg;
        }

        ACE_Semaphore pendingCommands;

        void setCommandSuccess(bool val)
        {
            m_success = val;
        }

        bool hasCommandSucceeded() const
        {
            return m_success;
        }

        static void print(void* callbackArg, std::string msg)
        {
            ((SOAPCommand*)callbackArg)->appendToPrintBuffer(msg);
        }

        static void commandFinished(void* callbackArg, bool success);

        bool m_success;
        std::string m_printBuffer;
};

#endif
