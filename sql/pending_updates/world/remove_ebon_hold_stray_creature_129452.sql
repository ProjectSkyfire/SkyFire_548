-- Remove stray Death Knight Initiate spawn in Ebon Hold.
SET @CREATURE_GUID := 129452;

DELETE FROM `creature_addon` WHERE `guid` = @CREATURE_GUID;
DELETE FROM `creature_formations` WHERE `leaderGUID` = @CREATURE_GUID OR `memberGUID` = @CREATURE_GUID;
DELETE FROM `linked_respawn` WHERE `guid` = @CREATURE_GUID OR `linkedGuid` = @CREATURE_GUID;
DELETE FROM `pool_creature` WHERE `guid` = @CREATURE_GUID;
DELETE FROM `game_event_creature` WHERE `guid` = @CREATURE_GUID;
DELETE FROM `game_event_model_equip` WHERE `guid` = @CREATURE_GUID;
DELETE FROM `game_event_npc_vendor` WHERE `guid` = @CREATURE_GUID;
DELETE FROM `game_event_npcflag` WHERE `guid` = @CREATURE_GUID;
DELETE FROM `creature` WHERE `guid` = @CREATURE_GUID;
