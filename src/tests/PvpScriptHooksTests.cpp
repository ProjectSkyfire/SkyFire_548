#include "ScriptMgr.h"
#include "DatabaseEnv.h"
#include <memory>
#ifdef CFBG_TEST_MODULE
#include "Battleground.h"
#include "BattlegroundQueue.h"
#include "Config.h"
void AddSC_CFBG();
#endif
#include <cstdlib>
#include <iostream>

// World.cpp normally receives this process identity from worldserver/Main.cpp.
uint32 realmID = 0;
DatabaseWorkerPool<WorldDatabaseConnection> WorldDatabase;
DatabaseWorkerPool<CharacterDatabaseConnection> CharacterDatabase;
DatabaseWorkerPool<LoginDatabaseConnection> LoginDatabase;
std::map<uint32,std::string> realmNameStore;
void Check(bool ok, char const* message)
{
    if (!ok) { std::cerr << message << '\n'; std::exit(1); }
}
class Probe : public PvPScript
{
public:
    Probe() : PvPScript("pvp_extension_probe") { }
    bool SelectBGGroups(BattlegroundQueue*, Battleground*, uint8, uint32, uint32, bool) override { return true; }
    bool CanJoinBG(Player*, Group*) override { return false; }
    void BeforeTeamChat(Player*, ChatMsg, Language& language) override { language = Language::LANG_UNIVERSAL; }
};
struct ScriptRegistryCleanup
{
    void operator()(Probe*) const
    {
        // Probe registers itself on construction. The registry owns its lifetime
        // and Unload deletes it, so deleting the pointer here would free it twice.
        sScriptMgr->Unload();
    }
};

int main()
{
    Check(!sScriptMgr->SelectBGGroups(nullptr,nullptr,0,1,10,false),"Empty registry must use native queue formation");
    Check(!sScriptMgr->SelectBGGroups(nullptr,nullptr,0,0,10,true),"Empty registry must use native reinforcement");
    Check(sScriptMgr->CanJoinBG(nullptr,nullptr),"Empty registry rejected native group entry");
    Language lang=Language::LANG_ORCISH;
    sScriptMgr->BeforeTeamChat(nullptr,ChatMsg::CHAT_MSG_RAID,lang);
    Check(lang==Language::LANG_ORCISH,"Empty registry changed chat language");
    uint32 faction=729;int32 amount=-25;
    sScriptMgr->BeforeReputation(nullptr,faction,amount,true);
    Check(faction==729 && amount==-25,"Empty registry changed reputation");
    TeamId team=TEAM_HORDE;
    sScriptMgr->BeforeBFJoin(nullptr,nullptr,team);
    Check(team==TEAM_HORDE,"Empty registry changed Wintergrasp team");
    sScriptMgr->BeforeBGAddPlayer(nullptr,nullptr);
    sScriptMgr->AfterBGRemovePlayer(nullptr,nullptr);
    sScriptMgr->BeforeBFEnd(nullptr);
    sScriptMgr->AfterBFEnd(nullptr);
    std::unique_ptr<Probe, ScriptRegistryCleanup> probe(std::make_unique<Probe>().release());
    Check(sScriptMgr->SelectBGGroups(nullptr,nullptr,0,1,10,false),"Registered queue owner was not dispatched");
    Check(!sScriptMgr->CanJoinBG(nullptr,nullptr),"Registered group rejection was ignored");
    sScriptMgr->BeforeTeamChat(nullptr,ChatMsg::CHAT_MSG_RAID,lang);
    Check(lang==Language::LANG_UNIVERSAL,"Language reference was not dispatched");
    probe.reset();
    Check(!sScriptMgr->SelectBGGroups(nullptr,nullptr,0,1,10,false),"Unload left a PvP handler registered");
#ifdef CFBG_TEST_MODULE
    AddSC_CFBG();
    Battleground bg;
    BattlegroundQueue queue;
    Check(!sScriptMgr->SelectBGGroups(&queue,&bg,0,1,10,false),"Disabled CFBG intercepted native formation");
    Check(!sScriptMgr->SelectBGGroups(&queue,&bg,0,0,10,true),"Disabled CFBG intercepted native reinforcement");
    Check(sScriptMgr->CanJoinBG(nullptr,nullptr),"Disabled CFBG rejected native group entry");
    Check(sConfigMgr->LoadInitial("cfbg_test_enabled.conf"),"Failed to load isolated enabled test config");
    sScriptMgr->OnConfigLoad(true);
    auto first = new GroupQueueInfo{};
    auto second = new GroupQueueInfo{};
    first->Team = first->NativeTeam = second->Team = second->NativeTeam = ALLIANCE;
    first->Players.emplace(uint64(1001),nullptr); second->Players.emplace(uint64(1002),nullptr);
    queue.m_QueuedGroups[0][BG_QUEUE_NORMAL_ALLIANCE].push_back(first);
    queue.m_QueuedGroups[0][BG_QUEUE_NORMAL_ALLIANCE].push_back(second);
    Check(sScriptMgr->SelectBGGroups(&queue,&bg,0,1,2,false),"Enabled module did not own formation");
    Check(queue.m_SelectionPools[0].GetPlayerCount()==1 && queue.m_SelectionPools[1].GetPlayerCount()==1,"Actual module failed to form two teams from one native faction");
    Check(second->Team==HORDE && queue.m_QueuedGroups[0][BG_QUEUE_NORMAL_HORDE].front()==second,"Assigned team and queue bucket disagree");
    second->IsInvitedToBGInstanceGUID=77;
    auto waiting = new GroupQueueInfo{};
    waiting->Team=HORDE; waiting->NativeTeam=ALLIANCE; waiting->Players.emplace(uint64(1003),nullptr);
    queue.m_QueuedGroups[0][BG_QUEUE_NORMAL_HORDE].push_back(waiting);
    Check(sConfigMgr->LoadInitial("cfbg_test_disabled.conf"),"Failed to load isolated disabled test config");
    sScriptMgr->OnConfigLoad(true);
    Check(!sScriptMgr->SelectBGGroups(&queue,&bg,0,1,2,false),"Live-disabled module did not return native ownership");
    Check(second->Team==HORDE && second->IsInvitedToBGInstanceGUID==77,"Live disable invalidated a committed invitation");
    Check(waiting->Team==ALLIANCE && queue.m_QueuedGroups[0][BG_QUEUE_NORMAL_ALLIANCE].back()==waiting,"Live disable did not restore an uninvited group");
    sScriptMgr->Unload();
#endif
    std::cout << "PvP extension defaults and registration passed\n";
}
