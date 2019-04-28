-- SmartScript for Quest: To the Surface (27674).
SET @QUESTID := 27674;
SET @QUESTOBJECTIVE := 276692;
SET @TORBEN := 46293;
SET @MENUID := 12104;
SET @SPELLID := 86278;
UPDATE creature_template SET AIName='SmartAI' WHERE entry=@TORBEN;
DELETE FROM gossip_menu_option WHERE menu_id=@MENUID;
INSERT INTO gossip_menu_option (menu_id, id, option_icon, option_text, option_id, npc_option_npcflag, action_menu_id, action_poi_id, box_coded, box_money, box_text) VALUES
(@MENUID, 0, 0, 'Send me to the surface, Torben.', 1, 1, 0, 0, 0, 0, '');
DELETE FROM smart_scripts WHERE entryorguid=@TORBEN;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(@TORBEN, 0, 0, 0, 62, 0, 100, 0, @MENUID, 0, 0, 0, 33, @TORBEN, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Torben Zapblast - On Gossip option select - Give Killcredit'),
(@TORBEN, 0, 1, 0, 62, 0, 100, 0, @MENUID, 0, 0, 0, 85, @SPELLID, 2, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Torben Zapblast - On Gossip option select - Player cast Teleport to Surface on self');
DELETE FROM spell_target_position WHERE id=@SPELLID;
INSERT INTO spell_target_position (id, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(@SPELLID, -5201.42, 477.913, 388.5, 5.2577);
DELETE FROM quest_objective WHERE questId=@QUESTID AND id=@QUESTOBJECTIVE;
INSERT INTO quest_objective (questId, id, index, type, objectId, amount, flags, description) VALUES
(@QUESTID, @QUESTOBJECTIVE, 0, 0, @TORBEN, 1, 0, 'Speak to Torben Zapblast');
