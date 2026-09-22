/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#ifndef SKYFIRE_CHAT_WHISPER_H
#define SKYFIRE_CHAT_WHISPER_H
#include "ChatPresence.h"
namespace Skyfire::Chat
{
    constexpr std::size_t MaxWhisperBytes = 512;
    struct Whisper
    {
        std::string Generation;
        std::uint32_t Account = 0;
        std::uint64_t Sender = 0, SenderIncarnation = 0, Receiver = 0, ReceiverIncarnation = 0;
        std::string Text;
        std::string ReceiverName; // Local completion context; not sent on the service wire.
    };
    inline Cluster::Writer EncodeWhisper(Whisper const& message)
    {
        Cluster::Writer out; out.String(message.Generation); out.U32(message.Account);
        Write64(out, message.Sender); Write64(out, message.SenderIncarnation);
        Write64(out, message.Receiver); Write64(out, message.ReceiverIncarnation);
        out.String(message.Text); return out;
    }
    inline bool DecodeWhisper(std::vector<std::uint8_t> const& bytes, Whisper& message)
    {
        if (bytes.size() > MaxWhisperBytes) return false;
        Cluster::Reader in(bytes); Whisper decoded;
        if (!in.String(decoded.Generation, 64) || decoded.Generation.size() != 64 ||
            decoded.Generation.find_first_not_of("0123456789abcdef") != std::string::npos ||
            !in.U32(decoded.Account) || !decoded.Account || !Read64(in, decoded.Sender) || !decoded.Sender ||
            !Read64(in, decoded.SenderIncarnation) || !decoded.SenderIncarnation ||
            !Read64(in, decoded.Receiver) || !decoded.Receiver ||
            !Read64(in, decoded.ReceiverIncarnation) || !decoded.ReceiverIncarnation ||
            !in.String(decoded.Text, 255) || !in.End()) return false;
        message = std::move(decoded); return true;
    }
    inline bool AuthorizeWhisper(PresenceDirectory const& directory, std::uint32_t realm,
        std::string const& node, Whisper const& message, std::uint64_t now)
    {
        auto const* sender = directory.Find(realm, node, message.Generation, message.Sender, message.SenderIncarnation, now);
        return sender && sender->Account == message.Account &&
            directory.Find(realm, node, message.Generation, message.Receiver, message.ReceiverIncarnation, now);
    }
}
#endif
