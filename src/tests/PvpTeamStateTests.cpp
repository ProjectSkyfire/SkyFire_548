#include "PvpTeamState.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <map>
#include <set>
void Check(bool ok, char const* message)
{
    if (!ok) { std::cerr << message << '\n'; std::exit(1); }
}
int main()
{
    std::set<uint64_t> zone[2], queue[2], war[2];
    std::map<uint64_t,unsigned> invites[2], kicks[2];
    zone[0]={1,2}; queue[0]={1}; war[0]={1}; invites[0][1]=123; kicks[0][1]=456;
    Skyfire::TransferTeamRecords(zone,queue,war,invites,kicks,uint64_t(1),0,1);
    Check(zone[0]==std::set<uint64_t>{2} && zone[1].count(1),"Zone membership leaked or another player moved");
    Check(queue[0].empty() && queue[1].count(1) && war[0].empty() && war[1].count(1),"Queue/war membership not moved atomically");
    Check(invites[0].empty() && invites[1][1]==123 && kicks[0].empty() && kicks[1][1]==456,"Invitation or kick deadline changed");
    Skyfire::TransferTeamRecords(zone,queue,war,invites,kicks,uint64_t(1),1,0);
    Check(zone[0]==std::set<uint64_t>({1,2}) && zone[1].empty() && invites[0][1]==123,"Rollback did not restore original records");
    Skyfire::TransferTeamRecords(zone,queue,war,invites,kicks,uint64_t(99),0,1);
    Check(!zone[1].count(99) && !invites[1].count(99),"Absent player acquired phantom reservations");
    Check(!Skyfire::HasTeamVacancy(3,2,5,false),"Pending invites must count against capacity");
    Check(Skyfire::HasTeamVacancy(3,2,5,true),"Candidate's own invitation must not consume a second slot");
    Check(!Skyfire::HasTeamVacancy(0,0,0,false),"Zero-capacity team accepted player");
    std::cout << "PvP team transfer scenarios passed\n";
}
