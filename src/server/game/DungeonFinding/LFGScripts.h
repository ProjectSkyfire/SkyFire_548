/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

/*
 * Interaction between core and LFGScripts
 */

#include "Common.h"
#include "ScriptMgr.h"
#include "SharedDefines.h"

class Player;
class Group;

namespace lfg
{

    class LFGPlayerScript : public PlayerScript
    {
    public:
        LFGPlayerScript();

        // Player Hooks
        void OnLogout(Player* player);
        void OnLogin(Player* player, bool loginFirst);
        void OnMapChanged(Player* player);
    };

    class LFGGroupScript : public GroupScript
    {
    public:
        LFGGroupScript();

        // Group Hooks
        void OnAddMember(Group* group, uint64 guid);
        void OnRemoveMember(Group* group, uint64 guid, RemoveMethod method, uint64 kicker, char const* reason);
        void OnDisband(Group* group);
        void OnChangeLeader(Group* group, uint64 newLeaderGuid, uint64 oldLeaderGuid);
        void OnInviteMember(Group* group, uint64 guid);
    };

} // namespace lfg
