/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "ChatDelivery.h"
#include "Chat.h"
#include "CellImpl.h"
#include "Group.h"
#include "GridNotifiersImpl.h"
#include "ObjectAccessor.h"
#include "Player.h"
#include "WorldPacket.h"
#include "WorldSession.h"
#include <map>
#include <mutex>
#include <memory>

namespace Skyfire::Chat::Delivery
{
    namespace
    {
        struct Pending
        {
            std::uint32_t Account;
            std::uint64_t Sender, Incarnation;
            WorldPacket Packet;
            Check Validate;
            std::map<std::uint64_t, std::uint64_t> Recipients;
            std::string Prefix;
            bool Addon;
            std::function<void(WorldSession*)> Execute;
            std::function<void(Player*)> Prepare;
        };
        // World-thread owned; no Player/Group/Guild/Channel pointer survives a tick.
        std::map<std::uint64_t, Pending> Requests;
        std::mutex RequestsLock;
        thread_local bool ExecutingControl = false;
        struct ControlScope
        {
            ControlScope() { ExecutingControl = true; }
            ~ControlScope() { ExecutingControl = false; }
        };
    }
    bool Submit(Player* sender, AudienceKind kind, std::string const& text, std::uint32_t language,
        std::string const& prefix, WorldPacket const& packet, std::vector<Player*> const& recipients, Check check,
        std::function<void(Player*)> prepare)
    {
        if (!RoutingEnabled()) return false;
        if (!sender || !sender->IsInWorld() || !sender->GetSession()) return true;
        auto* session = sender->GetSession();
        AudienceProjection audience;
        audience.Kind = kind;
        audience.Key = "player-" + std::to_string(session->GetAccountId()) + "-" + std::to_string(unsigned(kind));
        std::map<std::uint64_t, std::uint64_t> selected;
        for (auto* recipient : recipients)
            if (recipient && recipient->IsInWorld() && recipient->GetSession() && check(sender, recipient))
                selected[recipient->GetGUID()] = recipient->GetSession()->GetChatIncarnation();
        for (auto const& member : selected)
            audience.Members.push_back({member.first, member.second, member.first == sender->GetGUID(), true});
        if (!selected.count(sender->GetGUID()))
            audience.Members.push_back({sender->GetGUID(), session->GetChatIncarnation(), true, false});
        RoutedMessage message;
        message.Account = session->GetAccountId(); message.Sender = sender->GetGUID();
        message.Incarnation = session->GetChatIncarnation(); message.Text = text;
        message.Language = language; message.Prefix = prefix;
        std::lock_guard<std::mutex> lock(RequestsLock);
        auto sequence = Requests.size() < 128 ? QueueRoute(std::move(audience), std::move(message)) : 0;
        if (!sequence)
        { session->SendNotification("Chat service unavailable or busy. Message was not sent."); return true; }
        Requests.emplace(sequence, Pending{session->GetAccountId(), sender->GetGUID(), session->GetChatIncarnation(),
            packet, std::move(check), std::move(selected), prefix, language == std::uint32_t(Language::LANG_ADDON), {}, std::move(prepare)});
        return true;
    }
    bool GroupMessage(Player* sender, Group* group, WorldPacket const& packet, std::string const& text,
        std::uint32_t language, std::string const& prefix, AudienceKind kind, int subgroup,
        bool ignoreOtherGroups, std::uint64_t ignore)
    {
        if (!RoutingEnabled()) return false;
        std::vector<Player*> recipients;
        for (GroupReference* member = group->GetFirstMember(); member; member = member->next())
            if (auto* player = member->GetSource()) recipients.push_back(player);
        auto groupGuid = group->GetGUID();
        return Submit(sender, kind, text, language, prefix, packet, recipients,
            [groupGuid, subgroup, ignoreOtherGroups, ignore, kind, prefix](Player* currentSender, Player* recipient)
        {
            Group* current = currentSender->GetGroup();
            if (!current || current->GetGUID() != groupGuid) current = currentSender->GetOriginalGroup();
            if (!current || current->GetGUID() != groupGuid || !current->IsMember(currentSender->GetGUID()) || !current->IsMember(recipient->GetGUID()) ||
                recipient->GetGUID() == ignore || (ignoreOtherGroups && recipient->GetGroup() != current) ||
                (subgroup != -1 && (current->GetMemberGroup(recipient->GetGUID()) != subgroup ||
                    current->GetMemberGroup(currentSender->GetGUID()) != subgroup))) return false;
            if (kind == AudienceKind::RaidWarning &&
                (!current->isRaidGroup() || current->isBGGroup() ||
                 !(current->IsLeader(currentSender->GetGUID()) || current->IsAssistant(currentSender->GetGUID())))) return false;
            return prefix.empty() || recipient->GetSession()->IsAddonRegistered(prefix);
        });
    }
    bool SpatialMessage(Player* sender, WorldPacket const& packet, std::string const& text,
        std::uint32_t language, float range, bool ownTeamOnly)
    {
        if (!RoutingEnabled()) return false;
        std::vector<Player*> recipients{sender};
        Skyfire::MessageDistDeliverer collector(sender, nullptr, range, ownTeamOnly);
        collector.recipients = &recipients;
        sender->VisitNearbyWorldObject(range, collector);
        // The original spatial visitor also accounts for vehicles, shared vision,
        // phase and farsight. Reuse it at delivery rather than approximating range.
        auto visible = std::make_shared<std::set<std::uint64_t>>();
        for (auto* player : recipients) visible->insert(player->GetGUID());
        return Submit(sender, AudienceKind::Spatial, text, language, "", packet, recipients,
            [visible](Player*, Player* recipient) { return visible->count(recipient->GetGUID()) != 0; },
            [visible, range, ownTeamOnly](Player* currentSender)
        {
            visible->clear(); visible->insert(currentSender->GetGUID());
            std::vector<Player*> current;
            Skyfire::MessageDistDeliverer check(currentSender, nullptr, range, ownTeamOnly);
            check.recipients = &current; currentSender->VisitNearbyWorldObject(range, check);
            for (auto* player : current) visible->insert(player->GetGUID());
        });
    }
    void Update()
    {
        for (auto const& result : TakeRouteResults())
        {
            std::unique_lock<std::mutex> lock(RequestsLock);
            auto found = Requests.find(result.Sequence);
            if (found == Requests.end()) continue;
            Pending pending = std::move(found->second); Requests.erase(found);
            lock.unlock();
            auto* sender = ObjectAccessor::FindPlayer(pending.Sender);
            if (!sender || !sender->IsInWorld() || !sender->GetSession() ||
                sender->GetSession()->GetAccountId() != pending.Account ||
                sender->GetSession()->GetChatIncarnation() != pending.Incarnation) continue;
            if (!result.Success || result.Account != pending.Account)
            {
                sender->GetSession()->SendNotification(pending.Execute ?
                    "Chat service unavailable or request expired. Operation was not executed." :
                    "Chat service unavailable or request expired. Message was not sent.");
                continue;
            }
            if (pending.Execute)
            {
                if (result.Recipients.size() != 1 || result.Recipients.front().Guid != pending.Sender ||
                    result.Recipients.front().Incarnation != pending.Incarnation) continue;
                ControlScope scope;
                pending.Execute(sender->GetSession());
                continue;
            }
            if (!pending.Addon && (!sender->CanSpeak() || sender->HasAura(1852))) continue;
            if (pending.Prepare) pending.Prepare(sender);
            for (auto const& member : result.Recipients)
            {
                auto expected = pending.Recipients.find(member.Guid);
                if (expected == pending.Recipients.end() || expected->second != member.Incarnation) continue;
                auto* recipient = ObjectAccessor::FindPlayer(member.Guid);
                if (!recipient || !recipient->IsInWorld() || !recipient->GetSession() ||
                    recipient->GetSession()->GetChatIncarnation() != member.Incarnation ||
                    !pending.Validate(sender, recipient)) continue;
                recipient->GetSession()->SendPacket(&pending.Packet);
            }
        }
    }
    namespace
    {
        bool SubmitControl(WorldSession* session, AudienceKind kind, char const* action,
            std::function<void(WorldSession*)> execute)
        {
            if (!RoutingEnabled() || ExecutingControl) return false;
            auto* player = session ? session->GetPlayer() : nullptr;
            if (!player || !player->IsInWorld()) return true;
            AudienceProjection projection;
            projection.Kind = kind;
            projection.Key = "control-" + std::to_string(session->GetAccountId()) + "-" + std::to_string(unsigned(kind));
            projection.Members.push_back({player->GetGUID(), session->GetChatIncarnation(), true, true});
            RoutedMessage message;
            message.Account = session->GetAccountId(); message.Sender = player->GetGUID();
            message.Incarnation = session->GetChatIncarnation(); message.Text = action;
            std::lock_guard<std::mutex> lock(RequestsLock);
            auto sequence = Requests.size() < 128 ? QueueRoute(std::move(projection), std::move(message)) : 0;
            if (!sequence)
            { session->SendNotification("Chat service unavailable or busy. Operation was not executed."); return true; }
            Requests.emplace(sequence, Pending{session->GetAccountId(), player->GetGUID(), session->GetChatIncarnation(),
                WorldPacket(), {}, {}, "", false, std::move(execute), {}});
            return true;
        }
    }
    bool Control(WorldSession* session, WorldPacket& packet, void (WorldSession::*handler)(WorldPacket&),
        AudienceKind kind, char const* action)
    {
        if (!RoutingEnabled() || ExecutingControl) return false;
        if (packet.size() > 4096)
        { packet.rfinish(); session->SendNotification("Chat administration request is too large."); return true; }
        auto guild = session->GetPlayer() ? session->GetPlayer()->GetGuildId() : 0;
        auto invite = session->GetPlayer() ? session->GetPlayer()->GetGuildIdInvited() : 0;
        bool queued = SubmitControl(session, kind, action, [copy = WorldPacket(packet), handler, kind, guild, invite](WorldSession* current) mutable
        {
            if (kind == AudienceKind::GuildControl && (current->GetPlayer()->GetGuildId() != guild ||
                current->GetPlayer()->GetGuildIdInvited() != invite))
            { current->SendNotification("Guild membership changed. Please retry the operation."); return; }
            // The existing authoritative handler performs current rank/RBAC checks
            // and scoped persistence. Raw client packets and passwords stay local.
            try { (current->*handler)(copy); }
            catch (ByteBufferException const&) { current->KickPlayer(); }
        });
        if (queued) packet.rfinish();
        return queued;
    }
    bool Command(WorldSession* session, std::string const& command)
    {
        if (!RoutingEnabled() || ExecutingControl || !session) return false;
        if (command.size() > 511)
        { session->SendNotification("Command is too long."); return true; }
        return SubmitControl(session, AudienceKind::Command, "gm-command", [command](WorldSession* current)
        {
            // Preserve the player identity; never turn a routed command into a
            // console command. ParseCommands rechecks RBAC at execution time.
            if (!ChatHandler(current).ParseCommands(command.c_str()))
                current->SendNotification("Unknown command or insufficient permission.");
        });
    }
}
