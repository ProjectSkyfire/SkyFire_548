-- Fix Merchant Square Door (195327) for Quest 14098 "Evacuate the Merchant Square"
-- This adds the knocking spell (67869) to show the cast bar and an event script to spawn the frightened citizen

-- Update door template with spell ID and event ID
UPDATE `gameobject_template` 
SET `data10` = 67869,  -- spellId for "Knocking" cast bar (goober.spellId)
    `data2` = 27000     -- eventId to spawn frightened citizen (goober.eventId)
WHERE `entry` = 195327;

-- Create event script to spawn Frightened Citizen (34981) at the door's position
-- Using relative coordinates (0,0,0) will spawn at the gameobject's position
-- Command 10 = SCRIPT_COMMAND_TEMP_SUMMON_CREATURE
-- datalong = creature entry (34981 = Frightened Citizen)
-- datalong2 = despawn delay in milliseconds (120000 = 2 minutes)
INSERT INTO `event_scripts` (`id`, `delay`, `command`, `datalong`, `datalong2`, `dataint`, `x`, `y`, `z`, `o`) VALUES
(27000, 0, 10, 34981, 120000, 0, 0, 0, 0, 0);

-- Set the ScriptName for creature 34981 (Frightened Citizen) to use our custom AI script
UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = 'npc_frightened_citizen'
WHERE `entry` = 34981;

-- Add creature text for Frightened Citizen (34981)
-- This text is spoken when the citizen is rescued from the door
DELETE FROM `creature_text` WHERE `entry` = 34981;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(34981, 0, 0, 'Thank you! Thank you so much!', 12, 0, 100, 0, 0, 0, 'Frightened Citizen - rescued from door'),
(34981, 0, 1, 'There''s one after me!', 12, 0, 100, 0, 0, 0, 'Frightened Citizen - when Raging Worgen spawns');

-- Set Rampaging Worgen (34884) to be hostile towards Alliance players
-- Faction 14 = Monster - hostile to both Alliance and Horde
UPDATE `creature_template`
SET `faction_A` = 14, `faction_H` = 14
WHERE `entry` = 34884;

