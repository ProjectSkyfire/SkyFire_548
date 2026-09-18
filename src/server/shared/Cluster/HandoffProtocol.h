/* Part of Project SkyFire. See LICENSE.md for copyright information. */
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
    };
    inline Writer Encode(Request const& request)
    {
        Writer out;
        out.U8(1); // Handoff subprotocol version, independent of lifecycle messages.
        out.U8(std::uint8_t(request.Action)); out.U8(std::uint8_t(request.Bind.Use));
        out.U32(request.Bind.Account); out.U32(request.Bind.Realm); out.U32(request.Ttl);
        out.String(request.Bind.Address); out.String(request.Bind.Evidence); out.String(request.Token);
        return out;
    }
    inline bool Decode(std::vector<std::uint8_t> const& bytes, Request& request)
    {
        Reader in(bytes); std::uint8_t version, action, purpose;
        if (!in.U8(version) || version != 1 || !in.U8(action) || action < 1 || action > 4 ||
            !in.U8(purpose) || purpose < 1 || purpose > 2 || !in.U32(request.Bind.Account) ||
            !in.U32(request.Bind.Realm) || !in.U32(request.Ttl) || !in.String(request.Bind.Address,64) ||
            !in.String(request.Bind.Evidence,64) || !in.String(request.Token,64) || !in.End()) return false;
        request.Action = Operation(action); request.Bind.Use = Purpose(purpose);
        request.Bind.Destination = "-"; // The hub assigns this from authenticated registry state.
        return Valid(request.Bind) && (request.Token == "-" || IsToken(request.Token)) &&
            (request.Action == Operation::Issue ? request.Ttl > 0 && request.Ttl <= 900 && request.Token == "-" : !request.Ttl);
    }
}
#endif
