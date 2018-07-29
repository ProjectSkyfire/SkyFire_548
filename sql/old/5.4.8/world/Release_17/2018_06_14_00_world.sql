-- Creature Spawn
SET @SFDBGuid := 2574; -- SFDBGuidAssignment
DELETE FROM creature WHERE id=54855;
INSERT INTO creature (guid, id, map, spawnMask, phaseMask, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, MovementType, npcflag, unit_flags, dynamicflags) VALUES 
(@SFDBGuid, 54855, 860, 1, 1, 0, 0, 1413.67, 3533.79, 86.0637, 2.16421, 120, 0, 0, 0, 0, 0, 0, 0, 0);
-- SAI Assignment
DELETE FROM areatrigger_scripts WHERE entry=6958;
INSERT INTO areatrigger_scripts (entry, ScriptName) VALUES ('6958', 'SmartTrigger');
-- SAI
DELETE FROM smart_scripts WHERE entryorguid=6958;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(6958, 2, 0, 0, 46, 0, 100, 0, 6958, 0, 0, 0, 33, 54855, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Areatrigger The Dawling Valley - On Trigger - Quest Credit');
