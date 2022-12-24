/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

#ifndef SF_PLAYERTAXI_H
#define SF_PLAYERTAXI_H

class PlayerTaxi
{
    public:
    PlayerTaxi();
    ~PlayerTaxi()
    { }
    // Nodes
    void InitTaxiNodes(uint32 race, uint32 chrClass, uint8 level);
    void InitTaxiNodesForClass(uint32 chrClass);
    void InitTaxiNodesForRace(uint32 race);
    void InitTaxiNodesForFaction(uint32 faction);
    void InitTaxiNodesForLvl(uint8 level);

    void LoadTaxiMask(std::string const& data);

    bool IsTaximaskNodeKnown(uint32 nodeidx) const
    {
        uint8  field = uint8((nodeidx - 1) / 8);
        uint32 submask = 1 << ((nodeidx - 1) % 8);
        return (m_taximask [field] & submask) == submask;
    }
    bool SetTaximaskNode(uint32 nodeidx)
    {
        uint8  field = uint8((nodeidx - 1) / 8);
        uint32 submask = 1 << ((nodeidx - 1) % 8);
        if ((m_taximask [field] & submask) != submask)
        {
            m_taximask [field] |= submask;
            return true;
        }
        else
            return false;
    }
    void AppendTaximaskTo(ByteBuffer& data, bool all);

    // Destinations
    bool LoadTaxiDestinationsFromString(std::string const& values, uint32 team);
    std::string SaveTaxiDestinationsToString();

    void ClearTaxiDestinations()
    {
        m_TaxiDestinations.clear();
    }
    void AddTaxiDestination(uint32 dest)
    {
        m_TaxiDestinations.push_back(dest);
    }
    uint32 GetTaxiSource() const
    {
        return m_TaxiDestinations.empty() ? 0 : m_TaxiDestinations.front();
    }
    uint32 GetTaxiDestination() const
    {
        return m_TaxiDestinations.size() < 2 ? 0 : m_TaxiDestinations [1];
    }
    uint32 GetCurrentTaxiPath() const;
    uint32 NextTaxiDestination()
    {
        m_TaxiDestinations.pop_front();
        return GetTaxiDestination();
    }
    bool empty() const
    {
        return m_TaxiDestinations.empty();
    }

    friend std::ostringstream& operator<< (std::ostringstream& ss, PlayerTaxi const& taxi);
    private:
    TaxiMask m_taximask;
    std::deque<uint32> m_TaxiDestinations;
};

std::ostringstream& operator<< (std::ostringstream& ss, PlayerTaxi const& taxi);

#endif
