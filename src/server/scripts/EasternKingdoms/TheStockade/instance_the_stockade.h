/*
* This file is part of Project SkyFire https://www.projectskyfire.org. 
* See LICENSE.md file for Copyright information
*/

/*
This placeholder for the instance is needed for dungeon finding to be able
to give credit after the boss defined in lastEncounterDungeon is killed.
Without it, the party doing random dungeon won't get satchel of spoils and
gets instead the deserter debuff.
*/

#define TheStockadeScriptName "instance_the_stockade"

uint32 const EncounterCount = 5;

enum DataTypes
{
    // Encounter States/Boss GUIDs
    DATA_HOGGER = 3
};

enum CreatureIds
{
    NPC_HOGGER = 46254
};

template<class AI>
AI* GetTheStockadeAI(Creature* creature)
{
    return GetInstanceAI<AI>(creature, TheStockadeScriptName);
}
