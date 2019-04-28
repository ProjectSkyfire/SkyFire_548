SET @VEHICLE := 46185;

UPDATE creature_template SET AIName = "SmartAI" WHERE entry = @VEHICLE;

DELETE FROM npc_spellclick_spells WHERE npc_entry =  @VEHICLE;
INSERT INTO npc_spellclick_spells (npc_entry, spell_id, cast_flags, user_type) VALUES
(@VEHICLE, 125095, 1, 1);

DELETE FROM smart_scripts WHERE entryorguid = 46185 AND source_type=0;
DELETE FROM smart_scripts WHERE entryorguid = 46185*100 AND source_type=9;
INSERT INTO smart_scripts (entryorguid,source_type,id,link,event_type,event_phase_mask,event_chance,event_flags,event_param1,event_param2,event_param3,event_param4,action_type,action_param1,action_param2,action_param3,action_param4,action_param5,action_param6,target_type,target_param1,target_param2,target_param3,target_x,target_y,target_z,target_o,comment) VALUES
(@VEHICLE,0,0,1,11,0,100,0,0,0,0,0, 81,16777216,0,0,0,0,0, 1,0,0,0,0,0,0,0, "Sanitron 500 - On Respawn - Set npc flag"),
(@VEHICLE,0,1,2,25,0,100,0,0,0,0,0, 66,0,0,0,0,0,0, 1,0,0,0,0,0,0,0, "Sanitron 500 - On Reset - Set Home Position"),
(@VEHICLE,0,2,3,25,0,100,0,0,0,0,0, 22,1,0,0,0,0,0, 1,0,0,0,0,0,0,0, "Sanitron 500 - On Reset - Set Phase 1"),
(@VEHICLE,0,3,0,27,1,100,0,0,0,0,0, 80,@VEHICLE*100,0,0,0,0,0, 1,0,0,0,0,0,0,0, "Sanitron 500 - Start Decontamination Script"),
(@VEHICLE*100,9,0,1,0,1,100,0,0,0,0,0, 22,2,0,0,0,0,0, 1,0,0,0,0,0,0,0, "Sanitron 500 - Script - Set Phase 2"),
(@VEHICLE*100,9,1,2,0,2,100,0,0,0,0,0, 1,0,0,0,0,0,0, 1,0,0,0,0,0,0,0, "Sanitron 500 - Script - Say 0"),
(@VEHICLE*100,9,2,3,0,2,100,0,2000,2000,0,0, 69,0,0,0,0,0,0, 8,0,0,0,-5173.94,725.62,291.387,0, "Sanitron 500 - Script - Move To Waypoint 1"),
(@VEHICLE*100,9,3,4,0,2,100,0,3000,3000,0,0, 86,0,0,19,0,0,0, 0,0,0,0,0,0,0,0, "Sanitron 500 - Script - Animation 1 [delay]"),
(@VEHICLE*100,9,4,5,0,2,100,0,0,0,0,0, 11,86075,0,0,0,0,0, 7,0,0,0,0,0,0,0, "Sanitron 500 - Script - Cast Decontaminate Stage 1'"),
(@VEHICLE*100,9,5,6,0,2,100,0,1000,1000,0,0, 69,0,0,0,0,0,0, 8,0,0,0,-5174.56,716.36,289.387,0, "Sanitron 500 - Script - Move To Waypoint 2"),
(@VEHICLE*100,9,6,7,0,2,100,0,3000,3000,0,0, 86,0,0,19,0,0,0, 0,0,0,0,0,0,0,0, "Sanitron 500 - Script - Animation 2 [delay]"),
(@VEHICLE*100,9,7,8,0,2,100,0,0,0,0,0, 11,86084,0,0,0,0,0, 7,0,0,0,0,0,0,0, "Sanitron 500 - Script - Cast Decontaminate Stage 2'"),
(@VEHICLE*100,9,8,9,0,2,100,0,2000,2000,0,0, 69,0,0,0,0,0,0, 8,0,0,0,-5175.21,707.18,291.887,0, "Sanitron 500 - Script - Move To Waypoint 3"),
(@VEHICLE*100,9,9,10,0,2,100,0,3000,3000,0,0, 86,0,0,19,0,0,0, 0,0,0,0,0,0,0,0, "Sanitron 500 - Script - Animation 3 [delay]"),
(@VEHICLE*100,9,10,11,0,2,100,0,1500,1500,0,0, 1,1,0,0,0,0,0, 1,0,0,0,0,0,0,0, "Sanitron 500 - Script - Say 1"),
(@VEHICLE*100,9,11,12,0,2,100,0,0,0,0,0, 11,86086,0,0,0,0,0, 7,0,0,0,0,0,0,0, "Sanitron 500 - Script - Cast Decontaminate Stage 3'"),
(@VEHICLE*100,9,12,13,0,2,100,0,1000,1000,0,0, 69,0,0,0,0,0,0, 8,0,0,0,-5175.98,700.18,291.387,0, "Sanitron 500 - Script - Move To Waypoint 4"),
(@VEHICLE*100,9,13,14,0,2,100,0,2000,2000,0,0, 1,2,0,0,0,0,0, 1,0,0,0,0,0,0,0, "Sanitron 500 - Script - Say 2"),
(@VEHICLE*100,9,14,15,0,2,100,0,0,0,0,0, 37,0,0,0,0,0,0, 1,0,0,0,0,0,0,0,"Sanitron 500 - Script - Die"),
(@VEHICLE*100,9,15,16,0,2,100,0,0,0,0,0, 41,2000,0,0,0,0,0, 1,0,0,0,0,0,0,0,"Sanitron 500 - Script - Despawn After 2 Secounds"),
(@VEHICLE*100,9,16,0,0,2,100,0,0,0,0,0, 22,1,0,0,0,0,0, 1,0,0,0,0,0,0,0, "Sanitron 500 - Script - Set Phase 1");

DELETE FROM creature_text  WHERE entry = @VEHICLE;
INSERT INTO creature_text (entry, groupid, id, text, type, language, probability, emote, duration, sound, comment) VALUES 
(@VEHICLE, 0, 0, 'Commencing decontamination sequence...', 12, 0, 100, 0, 0, 0, NULL),
(@VEHICLE, 1, 0, 'Decontamination complete. Standby for delivery.', 12, 0, 100, 0, 0, 0, NULL),
(@VEHICLE, 2, 0, 'Warning, system overload. Malfunction imminent!', 12, 0, 100, 0, 0, 0, NULL);
