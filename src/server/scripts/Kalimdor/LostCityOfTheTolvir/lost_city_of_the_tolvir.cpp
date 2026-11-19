/*
* This file is part of the Pandaria 5.4.8 Project. See THANKS file for Copyright information
*
* This program is free software; you can redistribute it and/or modify it
* under the terms of the GNU General Public License as published by the
* Free Software Foundation; either version 2 of the License, or (at your
* option) any later version.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
* more details.
*
* You should have received a copy of the GNU General Public License along
* with this program. If not, see <http://www.gnu.org/licenses/>.
*/

#include "ScriptMgr.h"
#include "InstanceScript.h"
#include "ScriptedCreature.h"
#include "Player.h"
#include "SpellAuras.h"
#include "Vehicle.h"
#include "lost_city_of_the_tolvir.h"

enum WindTunnelMisc
{
    SPELL_SLIPSTREAM            = 85016,
    SPELL_RIDE_VEHICLE_HARDCODED = 46598,

    NPC_WIND_TUNNEL_LANDING_ZONE = 48097,

    WIND_TUNNEL_RIDE_TIME       = 2000,
    LANDING_RIDE_TIME           = 1000,
    LANDING_ZONE_RANGE          = 100,
    UNLOCK_CHECK_TIME           = 1000
};

// The wind tunnels around the ruins take the party up to the platform of Siamat once the three other bosses are dead:
// a passenger rides for two seconds, then the slipstream throws him on the landing zone
struct npc_wind_tunnel : public ScriptedAI
{
    explicit npc_wind_tunnel(Creature* creature) : ScriptedAI(creature), _unlockTimer(0)
    {
        me->setActive(true);
    }

    void Reset() OVERRIDE
    {
        // The core takes the click flag off a vehicle that has no usable seat
        me->SetFlag(UNIT_FIELD_NPC_FLAGS, UNIT_NPC_FLAG_SPELLCLICK);
        UpdateVisibility();
    }

    void PassengerBoarded(Unit* passenger, int8 /*seatId*/, bool apply) OVERRIDE
    {
        if (passenger->GetTypeId() != TypeID::TYPEID_PLAYER)
            return;

        _passengers.remove_if(PassengerCheck(passenger->GetGUID()));
        if (apply)
            _passengers.push_back(std::make_pair(passenger->GetGUID(), uint32(WIND_TUNNEL_RIDE_TIME)));
    }

    void UpdateAI(uint32 diff) OVERRIDE
    {
        if (_unlockTimer <= diff)
        {
            UpdateVisibility();
            _unlockTimer = UNLOCK_CHECK_TIME;
        }
        else
            _unlockTimer -= diff;

        for (std::list<std::pair<uint64, uint32> >::iterator itr = _passengers.begin(); itr != _passengers.end();)
        {
            if (itr->second > diff)
            {
                itr->second -= diff;
                ++itr;
                continue;
            }

            // Only a player who still sits in this tunnel is taken on, and one that finds no landing zone gets out
            Player* player = Unit::GetPlayer(*me, itr->first);
            if (player && player->GetVehicleBase() == me)
            {
                Creature* landing = me->FindNearestCreature(NPC_WIND_TUNNEL_LANDING_ZONE, float(LANDING_ZONE_RANGE));
                player->ExitVehicle();
                if (landing)
                {
                    player->CastSpell(player, SPELL_SLIPSTREAM, true);
                    player->CastSpell(landing, SPELL_RIDE_VEHICLE_HARDCODED, true);

                    if (Aura* aura = landing->GetAura(SPELL_RIDE_VEHICLE_HARDCODED, player->GetGUID()))
                        aura->SetDuration(LANDING_RIDE_TIME);
                }
            }

            itr = _passengers.erase(itr);
        }
    }

private:
    void UpdateVisibility()
    {
        InstanceScript* instance = me->GetInstanceScript();
        bool unlocked = instance && instance->GetBossState(DATA_GENERAL_HUSAM) == DONE && instance->GetBossState(DATA_LOCKMAW) == DONE
            && instance->GetBossState(DATA_HIGH_PROPHET_BARIM) == DONE;

        if (me->IsVisible() != unlocked)
            me->SetVisible(unlocked);
    }

    struct PassengerCheck
    {
        explicit PassengerCheck(uint64 guid) : _guid(guid) { }
        bool operator()(std::pair<uint64, uint32> const& passenger) const { return passenger.first == _guid; }
        uint64 _guid;
    };

    std::list<std::pair<uint64, uint32> > _passengers;
    uint32 _unlockTimer;
};

void AddSC_lost_city_of_the_tolvir()
{
    RegisterCreatureAI(npc_wind_tunnel);
}
