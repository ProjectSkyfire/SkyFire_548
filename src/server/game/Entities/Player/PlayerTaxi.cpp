/*
* This file is part of Project SkyFire https://www.projectskyfire.org.
* See LICENSE.md file for Copyright information
*/

#include "Player.h"
#include "PlayerTaxi.h"

// == PlayerTaxi ================================================

PlayerTaxi::PlayerTaxi()
{
    memset(m_taximask, 0, sizeof(m_taximask));
}

void PlayerTaxi::InitTaxiNodes(uint32 race, uint32 chrClass, uint8 level)
{
    InitTaxiNodesForClass(chrClass);
    InitTaxiNodesForRace(race);
    InitTaxiNodesForFaction(Player::TeamForRace(race));
    InitTaxiNodesForLvl(level);
}

void PlayerTaxi::InitTaxiNodesForClass(uint32 chrClass)
{
    // class specific initial known nodes
    switch (chrClass)
    {
        case CLASS_DEATH_KNIGHT:
        {
            for (uint8 i = 0; i < TaxiMaskSize; ++i)
                m_taximask[i] |= sOldContinentsNodesMask[i];
            break;
        }
    }
}

void PlayerTaxi::InitTaxiNodesForRace(uint32 race)
{
    // race specific initial known nodes: capital and taxi hub masks
    switch (Player::TeamForRace(race))
    {
        case ALLIANCE:
            SetTaximaskNode(2);                  // Stormwind, Elwynn
            SetTaximaskNode(582);                // Goldshire, Elwynn
            SetTaximaskNode(589);                // Eastvale Logging Camp, Elwynn
            SetTaximaskNode(6);                  // Ironforge, Dun Morogh
            SetTaximaskNode(619);                // Kharanos, Dun Morogh
            SetTaximaskNode(620);                // Gol'Bolar Quarry, Dun Morogh
            SetTaximaskNode(26);                 // Lor'danel, Darkshore
            SetTaximaskNode(456);                // Dolanaar, Teldrassil
            SetTaximaskNode(457);                // Darnassus, Teldrassil
            SetTaximaskNode(94);                 // The Exodar
            SetTaximaskNode(624);                // Azure Watch, Azuremyst Isle
            SetTaximaskNode(49);                 // Moonglade
            SetTaximaskNode(100);                // Honor Hold, Hellfire Peninsula
            break;
        case HORDE:
            SetTaximaskNode(23);                 // Orgrimmar, Durotar
            SetTaximaskNode(536);                // Sen'jin Village, Durotar
            SetTaximaskNode(537);                // Razor Hill, Durotar
            SetTaximaskNode(11);                 // Undercity, Tirisfal
            SetTaximaskNode(384);                // The Bulwark, Tirisfal
            SetTaximaskNode(460);                // Brill, Tirisfal Glades
            SetTaximaskNode(22);                 // Thunder Bluff, Mulgore
            SetTaximaskNode(402);                // Bloodhoof Village, Mulgore
            SetTaximaskNode(82);                 // Silvermoon City
            SetTaximaskNode(625);                // Fairbreeze Village, Eversong Woods
            SetTaximaskNode(631);                // Falconwing Square, Eversong Woods
            SetTaximaskNode(99);                 // Thrallmar, Hellfire Peninsula
            break;
    }
    SetTaximaskNode(128);                        // Shattrath, Terokkar Forest
    SetTaximaskNode(310);                        // Dalaran
}

void PlayerTaxi::InitTaxiNodesForFaction(uint32 faction)
{
    // new continent starting masks (It will be accessible only at new map)
    switch (faction)
    {
        case ALLIANCE: SetTaximaskNode(100); break;
        case HORDE:    SetTaximaskNode(99);  break;
    }
}

void PlayerTaxi::InitTaxiNodesForLvl(uint8 level)
{
    // level dependent taxi hubs
    if (level >= 68)
        SetTaximaskNode(213);                               //Shattered Sun Staging Area
}

void PlayerTaxi::LoadTaxiMask(std::string const& data)
{
    Tokenizer tokens(data, ' ');

    uint8 index = 0;
    for (Tokenizer::const_iterator iter = tokens.begin(); index < TaxiMaskSize && iter != tokens.end(); ++iter, ++index)
    {
        // load and set bits only for existing taxi nodes
        m_taximask[index] = sTaxiNodesMask[index] & uint32(atol(*iter));
    }
}

void PlayerTaxi::AppendTaximaskTo(ByteBuffer& data, bool all)
{
    if (all)
    {
        for (uint8 i = 0; i < TaxiMaskSize; ++i)
            data << uint8(sTaxiNodesMask[i]);              // all existed nodes
    }
    else
    {
        for (uint8 i = 0; i < TaxiMaskSize; ++i)
            data << uint8(m_taximask[i]);                  // known nodes
    }
}

bool PlayerTaxi::LoadTaxiDestinationsFromString(const std::string& values, uint32 team)
{
    ClearTaxiDestinations();

    Tokenizer Tokenizer(values, ' ');

    for (Tokenizer::const_iterator iter = Tokenizer.begin(); iter != Tokenizer.end(); ++iter)
    {
        uint32 node = uint32(atol(*iter));
        AddTaxiDestination(node);
    }

    if (m_TaxiDestinations.empty())
        return true;

    // Check integrity
    if (m_TaxiDestinations.size() < 2)
        return false;

    for (size_t i = 1; i < m_TaxiDestinations.size(); ++i)
    {
        uint32 cost;
        uint32 path;
        sObjectMgr->GetTaxiPath(m_TaxiDestinations[i - 1], m_TaxiDestinations[i], path, cost);
        if (!path)
            return false;
    }

    // can't load taxi path without mount set (quest taxi path?)
    if (!sObjectMgr->GetTaxiMountDisplayId(GetTaxiSource(), team, true))
        return false;

    return true;
}

std::string PlayerTaxi::SaveTaxiDestinationsToString()
{
    if (m_TaxiDestinations.empty())
        return "";

    std::ostringstream ss;

    for (size_t i = 0; i < m_TaxiDestinations.size(); ++i)
        ss << m_TaxiDestinations[i] << ' ';

    return ss.str();
}

uint32 PlayerTaxi::GetCurrentTaxiPath() const
{
    if (m_TaxiDestinations.size() < 2)
        return 0;

    uint32 path;
    uint32 cost;

    sObjectMgr->GetTaxiPath(m_TaxiDestinations[0], m_TaxiDestinations[1], path, cost);

    return path;
}

std::ostringstream& operator<< (std::ostringstream& ss, PlayerTaxi const& taxi)
{
    for (uint8 i = 0; i < TaxiMaskSize; ++i)
        ss << uint32(taxi.m_taximask[i]) << ' ';
    return ss;
}
