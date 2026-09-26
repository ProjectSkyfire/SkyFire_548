/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/
#include "Cluster/ChatRouting.h"
#include <stdexcept>

int main()
{
    using namespace Skyfire::Chat;
    auto check = [](bool ok) { if (!ok) throw std::runtime_error("Chat routing invariant failed"); };
    std::string generation(64, 'a');
    // Login channel requests require the new session's presence first; accepting
    // unknown sessions would bypass the service's identity checks.
    {
        PresenceDirectory loginPresence;
        MessageRouter loginRouter;
        PresenceSnapshot login{generation, 1, {{10, 100, 1, "Sender"}}};
        AudienceProjection join{generation, "channel-control", AudienceKind::ChannelControl, 1,
            {{100, 1, true, true}}};
        RoutedMessage request;
        request.Generation = generation; request.Audience = join.Key;
        request.Sequence = request.Revision = 1;
        request.Sender = 100; request.Incarnation = 1; request.Account = 10;
        request.Text = "HandleJoinChannel";
        check(!loginRouter.Project(loginPresence, 1, "world-a", join, 100));
        check(loginPresence.Replace(1, "world-a", login, 101));
        check(loginRouter.Project(loginPresence, 1, "world-a", join, 102));
        check(loginRouter.Route(loginPresence, 1, "world-a", request, 103));
        check(loginRouter.Take(1, "world-a", generation, 104).size() == 1);
    }
    PresenceDirectory presence;
    PresenceSnapshot players{generation, 1, {{10, 100, 1, "Sender"}, {20, 200, 2, "Officer"}, {30, 300, 3, "Member"}}};
    check(presence.Replace(1, "world-a", players, 100));
    check(presence.Replace(2, "world-b", players, 100));
    AudienceProjection officers{generation, "guild-officers", AudienceKind::Officer, 1,
        {{100, 1, true, true}, {200, 2, false, true}, {300, 3, false, false}}};
    RoutedMessage message;
    message.Generation = generation; message.Audience = officers.Key; message.Sequence = message.Revision = 1;
    message.Sender = 100; message.Incarnation = 1; message.Account = 10; message.Text = "Officers only";
    auto bytes = EncodeRoute(officers, message).Bytes;
    AudienceProjection decodedProjection; RoutedMessage decodedMessage;
    check(DecodeRoute(bytes, decodedProjection, decodedMessage) && decodedMessage.Text == message.Text);
    for (std::size_t n = 0; n < bytes.size(); ++n)
        check(!DecodeRoute(std::vector<std::uint8_t>(bytes.begin(), bytes.begin() + n), decodedProjection, decodedMessage));
    bytes.push_back(0); check(!DecodeRoute(bytes, decodedProjection, decodedMessage));

    MessageRouter router;
    check(router.Project(presence, 1, "world-a", officers, 101));
    check(!router.Project(presence, 1, "world-b", officers, 101));
    check(!router.Route(presence, 2, "world-a", message, 102));
    auto wrong = message; wrong.Account = 20;
    check(!router.Route(presence, 1, "world-a", wrong, 102));
    wrong = message; wrong.Sender = 300; wrong.Incarnation = 3; wrong.Account = 30;
    check(!router.Route(presence, 1, "world-a", wrong, 102));
    check(router.Route(presence, 1, "world-a", message, 102));
    check(!router.Route(presence, 1, "world-a", message, 103)); // duplicate cannot fan out twice
    check(router.Take(2, "world-a", generation, 104).empty());
    check(router.Take(1, "world-a", std::string(64, 'b'), 104).empty());
    auto deliveries = router.Take(1, "world-a", generation, 104);
    check(deliveries.size() == 1 && deliveries.front().Recipients.size() == 2);
    check(deliveries.front().Recipients[1].Guid == 200);
    check(router.Take(1, "world-a", generation, 105).empty());
    std::vector<RouteMember> recipients;
    check(DecodeRecipients(EncodeRecipients(deliveries.front().Recipients).Bytes, recipients) && recipients.size() == 2);
    check(!DecodeRecipients(EncodeRecipients({{100, 1}, {100, 1}}).Bytes, recipients));

    // Demotion invalidates an already queued audience revision.
    message.Sequence = 2;
    check(router.Route(presence, 1, "world-a", message, 106));
    officers.Revision = 2; officers.Members[1].Listen = false;
    check(router.Project(presence, 1, "world-a", officers, 107));
    check(router.Take(1, "world-a", generation, 108).empty());
    check(!router.Route(presence, 1, "world-a", message, 108));
    // Reconnection cannot inherit a former character session's route.
    players.Sequence = 2; players.Players[1].Incarnation = 4;
    check(presence.Replace(1, "world-a", players, 109));
    officers.Revision = 3;
    check(!router.Project(presence, 1, "world-a", officers, 110));
    officers.Members[1].Incarnation = 4;
    check(router.Project(presence, 1, "world-a", officers, 111));
    message.Revision = 3; message.Sequence = 3;
    check(router.Route(presence, 1, "world-a", message, 112));
    check(router.Take(1, "world-a", generation, 5112).empty()); // deadline, no replay

    // Binary addon payloads survive the wire; ordinary chat rejects control bytes.
    message.Text = std::string("x\0\xff", 3); message.Prefix = "TEST"; message.Language = 0xffffffffu;
    check(DecodeRoute(EncodeRoute(officers, message).Bytes, decodedProjection, decodedMessage));
    check(decodedMessage.Text == message.Text && decodedMessage.Prefix == "TEST");
    message.Sequence = 4; check(router.Route(presence, 1, "world-a", message, 5113));
    message.Sequence = 5; message.Language = 0;
    check(!router.Route(presence, 1, "world-a", message, 5114));

    // Administrative work can only be returned to the submitting session.
    AudienceProjection control{generation, "guild-control", AudienceKind::GuildControl, 1, {{100, 1, true, true}}};
    check(router.Project(presence, 1, "world-a", control, 5115));
    message.Audience = control.Key; message.Revision = 1; message.Text = "guild-invite"; message.Prefix.clear();
    check(router.Route(presence, 1, "world-a", message, 5116));
    control.Revision = 2; control.Members.push_back({300, 3, false, true});
    check(router.Project(presence, 1, "world-a", control, 5117));
    message.Sequence = 6; message.Revision = 2;
    check(!router.Route(presence, 1, "world-a", message, 5118));
    check(!router.Route(presence, 1, "world-a", message, 20118));

    MessageRouter bounded;
    check(!bounded.Project(presence, 2, "world-b", officers, 200));
    message.Generation = generation; message.Audience = officers.Key;
    message.Revision = officers.Revision; message.Text = "bounded";
    // Realm 2 still has incarnation 2, unlike realm 1. Its own projection is required.
    officers.Revision = 4; officers.Members[1].Incarnation = 2;
    check(bounded.Project(presence, 2, "world-b", officers, 201));
    message.Revision = 4;
    for (std::uint64_t i = 1; i <= MessageRouter::MaxQueued; ++i)
    { message.Sequence = i; check(bounded.Route(presence, 2, "world-b", message, 202)); }
    message.Sequence = MessageRouter::MaxQueued + 1;
    check(!bounded.Route(presence, 2, "world-b", message, 202));
    check(bounded.Take(2, "world-b", generation, 203).size() == 32);
    check(bounded.Route(presence, 2, "world-b", message, 204));
}
