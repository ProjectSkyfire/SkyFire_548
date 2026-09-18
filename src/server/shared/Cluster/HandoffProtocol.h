/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_HANDOFF_PROTOCOL_H
#define SKYFIRE_HANDOFF_PROTOCOL_H
#include "HandoffStore.h"
namespace Skyfire::Cluster::Handoff
{
    constexpr Message RequestType = Message(6), ReplyType = Message(0x8001);
    struct Request
    {
        Operation Action = Operation::Inspect;
        Binding Bind;
        std::uint32_t Ttl = 0;
        std::string Token = "-";
        std::string ExpectedDestination = "-";
    };
    inline Writer Encode(Request const& request)
    {
        Writer out;
        out.U8(request.ExpectedDestination == "-" ? 1 : 2); // v2 binds the advertised world owner.
        out.U8(std::uint8_t(request.Action)); out.U8(std::uint8_t(request.Bind.Use));
        out.U32(request.Bind.Account); out.U32(request.Bind.Realm); out.U32(request.Ttl);
        out.String(request.Bind.Address); out.String(request.Bind.Evidence); out.String(request.Token);
        if (request.ExpectedDestination != "-") out.String(request.ExpectedDestination);
        return out;
    }
    inline bool Decode(std::vector<std::uint8_t> const& bytes, Request& request)
    {
        Reader in(bytes); std::uint8_t version, action, purpose;
        if (!in.U8(version) || (version != 1 && version != 2) || !in.U8(action) || action < 1 || action > 4 ||
            !in.U8(purpose) || purpose < 1 || purpose > 2 || !in.U32(request.Bind.Account) ||
            !in.U32(request.Bind.Realm) || !in.U32(request.Ttl) || !in.String(request.Bind.Address,64) ||
            !in.String(request.Bind.Evidence,64) || !in.String(request.Token,64)) return false;
        request.ExpectedDestination = "-";
        if (version == 2 && (!in.String(request.ExpectedDestination,96) || request.ExpectedDestination == "-")) return false;
        if (!in.End()) return false;
        request.Action = Operation(action); request.Bind.Use = Purpose(purpose);
        request.Bind.Destination = "-"; // The hub assigns this from authenticated registry state.
        return Valid(request.Bind) && (request.Token == "-" || IsToken(request.Token)) &&
            (request.Action == Operation::Issue ? request.Ttl > 0 && request.Ttl <= 900 && request.Token == "-" : !request.Ttl);
    }
}
#endif
