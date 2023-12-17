UPDATE creature_template SET modLevel=1 WHERE entry IN
(347, 523, 857, 907, 1572, 2226, 2299, 2302, 2389, 2432, 2804, 2835,
2851, 2861, 2941, 3305, 3890, 4312, 4317, 4319, 4321, 4407, 5118,
6706, 6726, 7410, 7427, 8018, 8019, 8020, 8610, 10360, 11139,
11900, 11901, 11947, 11949, 12197, 12198, 12577, 12616, 12636, 12740,
13177, 14242, 14942, 14981, 14982, 15006, 15007, 15008, 16587, 16694,
16695, 16696, 17554, 18785, 18791, 18808, 18895, 18937, 18953, 19317,
19558, 19581, 19583, 19855, 19858, 19859);

UPDATE creature_template SET minLevel=90, maxLevel=90 WHERE modLevel=1;
DELETE FROM creature_addon WHERE guid IN (138335, 138523, 138526);
-- Add Creature Loot for Quest: 32318 - Regeneration Takes Time - (QuestObjective 269617)
DELETE FROM creature_loot_template WHERE item=92499 AND entry IN (68026, 68027);
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance) VALUES
(68026, 92499, -100),
(68027, 92499, -100);
-- Disable Quest: 9412 - <UNUSED>Crystal Flake Throat Lozenge
DELETE FROM disables WHERE sourceType=1 AND entry=9412;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9412, 'Disable Quest 9412: <UNUSED>Crystal Flake Throat Lozenge');
-- Add Gameobject Loot for Quest: 30064 - Saving the Sutras - (QuestObjective 251805)
DELETE FROM gameobject_loot_template WHERE item=77432 AND entry=40881;
INSERT INTO gameobject_loot_template (entry, item, ChanceOrQuestChance) VALUES
(40881, 77432, -100);
-- Update Creature Template for Heroic Lord Godfrey in Shadowfang
UPDATE creature_template SET minlevel=87, maxlevel=87, faction_A=16, faction_H=16, speed_run=1.42857, baseattacktime=2000, rangeattacktime=2000, unit_class=2, unit_flags=32832 WHERE entry=49712;
-- Change Engines to MyISAM
ALTER TABLE battle_pet_breed ENGINE=MyISAM;
ALTER TABLE battle_pet_quality ENGINE=MyISAM;
ALTER TABLE locales_quest_objective ENGINE=MyISAM;
ALTER TABLE phase_area ENGINE=MyISAM;
ALTER TABLE quest_objective ENGINE=MyISAM;
ALTER TABLE quest_objective_effects ENGINE=MyISAM;
ALTER TABLE terrain_phase_info ENGINE=MyISAM;
ALTER TABLE terrain_swap_defaults ENGINE=MyISAM;
ALTER TABLE terrain_worldmap ENGINE=MyISAM;
-- Add Gameobject Loot for Quest: 30479 - A Gift For Gina - (QuestObjective 251943)
DELETE FROM gameobject_loot_template WHERE entry=41463 AND item=80231;
INSERT INTO gameobject_loot_template (entry, item, ChanceOrQuestChance) VALUES
(41463, 80231, -100);
-- Add Gameobject Loot for Quest: 30476 - A Gift For Old Hillpaw - (QuestObjective 251957)
DELETE FROM gameobject_loot_template WHERE entry=41461 AND item=80229;
INSERT INTO gameobject_loot_template (entry, item, ChanceOrQuestChance) VALUES
(41461, 80229, -100);
-- Add Gameobject Loot for Quest: 30469 - Repossession - (QuestObjective 252081)
DELETE FROM gameobject_loot_template WHERE entry=41423 AND item=79880;
INSERT INTO gameobject_loot_template (entry, item, ChanceOrQuestChance) VALUES
(41423, 79880, -100);
-- Add Gameobject Loot for Quest: 30459 - All of the Arrows - (QuestObjective 252921)
DELETE FROM gameobject_loot_template WHERE entry=41400 AND item=79806;
INSERT INTO gameobject_loot_template (entry, item, ChanceOrQuestChance) VALUES
(41400, 79806, -100);
-- Delete some bad game object loot templates
DELETE FROM gameobject_loot_template WHERE item=0;
-- Add Gameobject Loot for Object: Crate of Candles (1586) - Item: Candle of Beckoning (3080) 
DELETE FROM gameobject_loot_template WHERE entry=28239 AND item=3080;
INSERT INTO gameobject_loot_template (entry, item, ChanceOrQuestChance) VALUES
(28239, 3080, -100);
-- Add Creature Loot for Quest: 14460 - Rite of Honor - (QuestObjective: 256362)
-- Creature: Chief Squealer Thornmantle (36712) - Item: Mane of Thornmantle (50473) 
DELETE FROM creature_loot_template WHERE entry=36712 AND item=50473;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance) VALUES
(36712, 50473, -100);
-- Set lootid in creature_template
UPDATE creature_template SET lootid=36712 WHERE entry=36712;
-- Add Creature Loot for Quest: 14456 - Rite of Courage - (QuestObjective: 255087)
-- Creature: Bristleback Gun Thief (36708) - Item: Stolen Rifle (49535) 
DELETE FROM creature_loot_template WHERE entry=36708 AND item=49535;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance) VALUES
(36708, 49535, -100);
-- Set lootid in creature_template
UPDATE creature_template SET lootid=36708 WHERE entry=36708;
-- Creature Spawns for Camp Sungraze in Mulgore.
SET @CGUID := 3043;
DELETE FROM creature WHERE guid BETWEEN @CGUID-5 AND @CGUID-0;
INSERT INTO creature (guid, id, map, spawnMask, phaseId, phaseGroup, position_x, position_y, position_z, orientation) VALUES
(@CGUID-0, 2957, 1, 1, 0, 0, -734.851, -505.565, -23.9306, 5.83003),
(@CGUID-1, 2987, 1, 1, 0, 0, -807.877, -498.523, -32.1909, 0),
(@CGUID-2, 3052, 1, 1, 0, 0, -794.766, -503.547, -32.1923, 1.65806),
(@CGUID-3, 3233, 1, 1, 0, 0, -789.854, -494.505, -32.1923, 3.33358),
(@CGUID-4, 10636, 1, 1, 0, 0, -808.7, -512.774, -31.948, 0),
(@CGUID-5, 37024, 1, 1, 0, 0, -798.271, -486.757, -32.1923, 1.62316);
-- Update Auras for Sunwalker Reha (43001)
UPDATE creature_template_addon SET auras='79963 79962' WHERE entry=43001;
SET @GGUID := 70100;
DELETE FROM gameobject WHERE guid BETWEEN @GGUID-1 AND @GGUID-0;
INSERT INTO gameobject (guid, id, map, spawnMask, phaseId, phaseGroup, position_x, position_y, position_z, orientation, rotation0, rotation1, rotation2, rotation3, spawntimesecs, animprogress, state) VALUES
(@GGUID-0, 2904, 1, 1, 0, 0, -753.256, -149.158, -29.0568, 3.08918, 0, 0, 0, 1, 120, 255, 1),
(@GGUID-1, 2909, 1, 1, 0, 0, -753.649, -149.057, -29.6249, -3.14159, 0, 0, 0, 1, 120, 255, 1);
-- SAI for Quest: Feed of Evil (14461)
SET @FT := 36727;
SET @ST := 37155;
SET @TT := 37156;

UPDATE creature_template SET AIName="SmartAI" WHERE entry IN (@FT, @ST, @TT);

DELETE FROM smart_scripts WHERE entryorguid IN (@FT, @ST, @TT) AND source_type=0 AND id in (0, 1);
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(@FT, 0, 0, 1, 8, 0, 100, 0, 69228, 0, 0, 0, 33, @FT, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "First Trough - On Spellhit - Quest Credit"), 
(@FT, 0, 1, 0, 61, 0, 100, 0, 69228, 0, 0, 0, 11, 42345, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "First Trough - On Spellhit - Cast Cosmetic"), 
(@ST, 0, 0, 1, 8, 0, 100, 0, 69228, 0, 0, 0, 33, @ST, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Second Trough - On Spellhit - Quest Credit"), 
(@ST, 0, 1, 0, 61, 0, 100, 0, 69228, 0, 0, 0, 11, 42345, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Second Trough - On Spellhit - Cast Cosmetic"), 
(@TT, 0, 0, 1, 8, 0, 100, 0, 69228, 0, 0, 0, 33, @TT, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Third Trough - On Spellhit - Quest Credit"), 
(@TT, 0, 1, 0, 61, 0, 100, 0, 69228, 0, 0, 0, 11, 42345, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Third Trough - On Spellhit - Cast Cosmetic");

DELETE FROM conditions WHERE SourceTypeOrReferenceId=13 AND SourceGroup=1 AND SourceEntry=69228;
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ElseGroup, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, ErrorTextId, ScriptName, Comment) VALUES
(13, 1, 69228, 0, 0, 31, 0, 3, @TT, 0, 0, "", "Throw Torch - Target Third Trough"), 
(13, 1, 69228, 0, 1, 31, 0, 3, @ST, 0, 0, "", "Throw Torch - Target Second Trough"), 
(13, 1, 69228, 0, 2, 31, 0, 3, @FT, 0, 0, "", "Throw Torch - Target First Trough");
-- Creature Spawns for Training Dummies on Sunstrider Isle.
SET @CGUID := 5572;
DELETE FROM creature WHERE id=44937;
DELETE FROM creature WHERE guid BETWEEN @CGUID-5 AND @CGUID-0;
INSERT INTO creature (guid, id, map, position_x, position_y, position_z, orientation, curhealth) VALUES
(@CGUID-0, 44937, 530, 10325, -6403.6, 37.23173, 2.932153, 0),
(@CGUID-1, 44937, 530, 10334.4, -6386.11, 36.51433, 2.391101, 0),
(@CGUID-2, 44937, 530, 10373.3, -6377.87, 37.28583, 1.291544, 0),
(@CGUID-3, 44937, 530, 10328.4, -6393.98, 37.14723, 2.635447, 0),
(@CGUID-4, 44937, 530, 10363.9, -6375.25, 37.01423, 1.58825, 0),
(@CGUID-5, 44937, 530, 10381.8, -6381.9, 37.83083, 0.9773844, 0);
-- delete lfg dungeon entrances for obsolete dungeons.
DELETE FROM lfg_entrances WHERE dungeonId IN (18, 165);
-- Quest Starter for Public Relations Agent (37675)
DELETE FROM creature_queststarter WHERE id=37675 AND quest IN (24629, 24635, 24636);
INSERT INTO creature_queststarter (id, quest) VALUES
(37675, 24629),
(37675, 24635),
(37675, 24636);
-- Disable Quest 3581: <UNUSED>
DELETE FROM disables WHERE sourceType=1 AND entry=3581;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 3581, 'Disable Quest 3581: <UNUSED>');

-- Disable Quest 1521: Call of Earth
DELETE FROM disables WHERE sourceType=1 AND entry=1521;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 1521, 'Disable Quest 1521: Call of Earth');

-- Disable Quest 6704: [DEPRECATED] <TXT> SM, RFD, ULD
DELETE FROM disables WHERE sourceType=1 AND entry=6704;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 6704, 'Disable Quest 6704: [DEPRECATED] <TXT> SM, RFD, ULD');

-- Disable Quest 7363: The Human Condition
DELETE FROM disables WHERE sourceType=1 AND entry=7363;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 7363, 'Disable Quest 7363: The Human Condition');

-- Disable Quest 9382: The Fate of the Clefthoof
DELETE FROM disables WHERE sourceType=1 AND entry=9382;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9382, 'Disable Quest 9382: The Fate of the Clefthoof');

-- Disable Quest 10092: Assault on Mageddon
DELETE FROM disables WHERE sourceType=1 AND entry=10092;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10092, 'Disable Quest 10092: Assault on Mageddon');

-- Disable Quest 10217: Fossil Fuel
DELETE FROM disables WHERE sourceType=1 AND entry=10217;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10217, 'Disable Quest 10217: Fossil Fuel');
-- Remove wrong spell click spells
DELETE FROM npc_spellclick_spells WHERE spell_id IN (46407, 47121, 47416, 82359, 82456);

-- Pomfruit (58767) - Spellclick Spell - Force Pomfruit (111626)
DELETE FROM npc_spellclick_spells WHERE npc_entry=58767;
INSERT INTO npc_spellclick_spells (npc_entry, spell_id, cast_flags) VALUES
(58767, 111626, 0x03);
-- Dawnchaser Retreat - Inn
DELETE FROM areatrigger_tavern WHERE id=8107;
INSERT INTO areatrigger_tavern (id, name) VALUES
(8107, 'Dawnchaser Retreat - Inn');

-- Tavern in the Mists - Inn
DELETE FROM areatrigger_tavern WHERE id=8442;
INSERT INTO areatrigger_tavern (id, name) VALUES
(8442, 'Tavern in the Mists - Inn');
-- Remove Obsolete Spells from skill_extra_item_template
DELETE FROM skill_extra_item_template WHERE spellId IN
(42736, 17638, 11453, 3172, 67025, 62213);
-- Update loot for creature 49871 Blackrock Battle Worg
UPDATE `creature_template` SET `lootid` = 49871 WHERE `entry` = 49871;
DELETE FROM `creature_loot_template` WHERE `Entry` = 49871 ;
INSERT INTO `creature_loot_template` VALUES 
('49871', '117', '5.052265', '1', '0', '1' , '1'),
('49871', '159', '2.61324', '1', '0', '1' , '1'),
('49871', '774', '36.75958', '1', '0', '1' , '1'),
('49871', '805', '32.40418', '1', '0', '1' , '1'),
('49871', '818', '27.35192', '1', '0', '1' , '1'),
('49871', '828', '74.9129', '1', '0', '1' , '1'),
('49871', '1367', '4.006968', '1', '0', '1' , '1'),
('49871', '1374', '1.393728', '1', '0', '1' , '1'),
('49871', '1378', '1.74216', '1', '0', '1' , '1'),
('49871', '1468', '1.393728', '1', '0', '1' , '1'),
('49871', '2211', '0.8710802', '1', '0', '1' , '1'),
('49871', '2589', '7.142857', '1', '0', '1' , '2'),
('49871', '2598', '0.3484321', '1', '0', '1' , '1'),
('49871', '3300', '11048.61', '1', '0', '1' , '1'),
('49871', '4496', '44.94773', '1', '0', '1' , '1'),
('49871', '5571', '53.13589', '1', '0', '1' , '1'),
('49871', '5572', '28.91986', '1', '0', '1' , '1'),
('49871', '23384', '1.916376', '1', '0', '1' , '1'),
('49871', '25467', '1.74216', '1', '0', '1' , '1'),
('49871', '49751', '1.045296', '1', '0', '1' , '1'),
('49871', '62328', '34083.8', '1', '0', '1' , '1'),
('49871', '62512', '1.393728', '1', '0', '1' , '1'),
('49871', '67310', '9.23345', '1', '0', '1' , '1');
-- Update loot for creature 49874 Blackrock Spy
UPDATE `creature_template` SET `lootid` = 49874 WHERE `entry` = 49874;
DELETE FROM `creature_loot_template` WHERE `Entry` = 49874 ;
INSERT INTO `creature_loot_template` VALUES 
('49874', '117', '414.6342', '1', '0', '1' , '1'),
('49874', '159', '217.2474', '1', '0', '1' , '1'),
('49874', '774', '36.75958', '1', '0', '1' , '1'),
('49874', '805', '47.56097', '1', '0', '1' , '1'),
('49874', '818', '22.29965', '1', '0', '1' , '1'),
('49874', '828', '59.93031', '1', '0', '1' , '1'),
('49874', '1369', '4.355401', '1', '0', '1' , '1'),
('49874', '1370', '13.76307', '1', '0', '1' , '1'),
('49874', '1376', '1.74216', '1', '0', '1' , '1'),
('49874', '1377', '0.8710802', '1', '0', '1' , '1'),
('49874', '1378', '8.188153', '1', '0', '1' , '1'),
('49874', '2406', '4.529617', '1', '0', '1' , '1'),
('49874', '2408', '1.393728', '1', '0', '1' , '1'),
('49874', '2589', '12.19512', '1', '0', '1' , '2'),
('49874', '2598', '5.574913', '1', '0', '1' , '1'),
('49874', '2653', '13.76307', '1', '0', '1' , '1'),
('49874', '3300', '52.43903', '1', '0', '1' , '1'),
('49874', '3363', '1.74216', '1', '0', '1' , '1'),
('49874', '3365', '3.832753', '1', '0', '1' , '1'),
('49874', '3609', '10.97561', '1', '0', '1' , '1'),
('49874', '4496', '71.25436', '1', '0', '1' , '1'),
('49874', '4540', '0.5226481', '1', '0', '1' , '1'),
('49874', '5465', '1.393728', '1', '0', '1' , '1'),
('49874', '5571', '34.49477', '1', '0', '1' , '1'),
('49874', '5572', '36.9338', '1', '0', '1' , '1'),
('49874', '49751', '4.355401', '1', '0', '1' , '1'),
('49874', '55973', '2.61324', '1', '0', '1' , '1'),
('49874', '55983', '1.045296', '1', '0', '1' , '1'),
('49874', '56147', '1.393728', '1', '0', '1' , '1'),
('49874', '62328', '171.9512', '1', '0', '1' , '1'),
('49874', '62771', '1.045296', '1', '0', '1' , '1'),
('49874', '62772', '5.749129', '1', '0', '1' , '1'),
('49874', '128644', '0.5226481', '1', '0', '1' , '1');
-- Disable Quest 9941: Tracking Down the Culprits
DELETE FROM disables WHERE sourceType=1 AND entry=9941;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9941, 'Disable Quest 9941: Tracking Down the Culprits');

-- Disable Quest 9950: A Bird's-Eye View
DELETE FROM disables WHERE sourceType=1 AND entry=9950;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9950, 'Disable Quest 9950: A Bird''s-Eye View');

-- Disable Quest 9958: Scouting the Defenses
DELETE FROM disables WHERE sourceType=1 AND entry=9958;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9958, 'Disable Quest 9958: Scouting the Defenses');

-- Disable Quest 9963: Seeking Help from the Source
DELETE FROM disables WHERE sourceType=1 AND entry=9963;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9963, 'Disable Quest 9963: Seeking Help from the Source');

-- Disable Quest 9964: Seeking Help from the Source
DELETE FROM disables WHERE sourceType=1 AND entry=9964;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9964, 'Disable Quest 9964: Seeking Help from the Source');

-- Disable Quest 9965: A Show of Good Faith
DELETE FROM disables WHERE sourceType=1 AND entry=9965;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9965, 'Disable Quest 9965: A Show of Good Faith');

-- Disable Quest 9966: A Show of Good Faith
DELETE FROM disables WHERE sourceType=1 AND entry=9966;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9966, 'Disable Quest 9966: A Show of Good Faith');

-- Disable Quest 9974: The Final Reagents
DELETE FROM disables WHERE sourceType=1 AND entry=9974;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9974, 'Disable Quest 9974: The Final Reagents');

-- Disable Quest 9348: Ravager Roundup
DELETE FROM disables WHERE sourceType=1 AND entry=9348;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9348, 'Disable Quest 9348: Ravager Roundup');

-- Disable Quest 10925: Evil Draws Near
DELETE FROM disables WHERE sourceType=1 AND entry=10925;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10925, 'Disable Quest 10925: Evil Draws Near');

-- Disable Quest 10088: When This Mine's a-Rockin'
DELETE FROM disables WHERE sourceType=1 AND entry=10088;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10088, 'Disable Quest 10088: When This Mine''s a-Rockin''');
-- RequiredSkillId to SKILL_FISHING(356) for QUEST_SORT_FISHING(101) 
UPDATE quest_template SET RequiredSkillId=356 WHERE ZoneOrSort=-101;

-- RequiredSkillId to SKILL_COOKING(185) for QUEST_SORT_COOKING(304) 
UPDATE quest_template SET RequiredSkillId=185 WHERE ZoneOrSort=-304;

-- RequiredSkillId to SKILL_JEWELCRAFTING(755) for QUEST_SORT_JEWELCRAFTING(373) 
UPDATE quest_template SET RequiredSkillId=755 WHERE ZoneOrSort=-373;

-- RequiredSkillId to SKILL_ENGINEERING(202) for QUEST_SORT_ENGINEERING(201) 
UPDATE quest_template SET RequiredSkillId=202 WHERE ZoneOrSort=-201;

-- RequiredSkillId to SKILL_LEATHERWORKING(165) for QUEST_SORT_LEATHERWORKING(182) 
UPDATE quest_template SET RequiredSkillId=165 WHERE ZoneOrSort=-182;

-- RequiredSkillId to SKILL_TAILORING(197) for QUEST_SORT_TAILORING(264) 
UPDATE quest_template SET RequiredSkillId=197 WHERE ZoneOrSort=-264;
-- Portal Effect: Exodar(Trigger:32268) - Portal: Exodar(121850)
DELETE FROM spell_target_position WHERE id IN (32268, 121850);
INSERT INTO spell_target_position (id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(121850, 0, 530, -4029.93, -11572.2, -138.296, 2.43);

-- Portal Effect: Silvermoon(Trigger:32270) - Portal Effect: Silvermoon(121855)
DELETE FROM spell_target_position WHERE id IN (32270, 121855);
INSERT INTO spell_target_position (id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(121855, 0, 530, 9998.49, -7106.78, 47.7055, 2.44);

-- Portal Effect: Shattrath(Trigger:33728) - Portal: Shattrath(121853)
DELETE FROM spell_target_position WHERE id IN (33728, 121853 );
INSERT INTO spell_target_position (id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(121853 , 0, 530, -1823.71, 5416.97, -12.4274, 2.79036);

-- Portal Effect: Shattrath(Trigger:35718) - Portal: Shattrath(121854)
DELETE FROM spell_target_position WHERE id IN (35718, 121854 );
INSERT INTO spell_target_position (id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(121854, 0, 530, -1904.76, 5443.43, -12.4272, 5.96);

-- Portal Effect: Dalaran(Trigger:53141) - Portal: Dalaran(121847)
DELETE FROM spell_target_position WHERE id IN (53141, 121847 );
INSERT INTO spell_target_position (id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(121847, 0, 571, 5807.75, 588.347, 661.505, 1.663);

-- Portal Effect: Tol Barad(Trigger:88341) - Portal: Tol Barad(121861)
DELETE FROM spell_target_position WHERE id IN (88341, 121861 );
INSERT INTO spell_target_position (id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(121861, 0, 732, -603.724, 1387.62, 22.0498, 0.469644);

-- Portal Effect: Ironforge(Trigger:44089) - Portal: Ironforge(121851)
DELETE FROM spell_target_position WHERE id IN (44089, 121851 );
INSERT INTO spell_target_position (id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(121851, 0, 0, -4613.71, -915.287, 501.062, 0);

-- Portal Effect: Stormwind(Trigger:17334) - Portal: Stormwind(121857)
DELETE FROM spell_target_position WHERE id IN (17334, 121857 );
INSERT INTO spell_target_position (id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(121857, 0, 0, -8998.14, 861.254, 29.6206, 2.25);

-- Portal Effect: Theramore(Trigger:49362) - Portal: Theramore(121858)
DELETE FROM spell_target_position WHERE id IN (49362, 121858 );
INSERT INTO spell_target_position (id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(121858, 0, 1, -3748.11, -4440.21, 30.5688, 3.95172);

-- Portal Effect: Orgrimmar(Trigger:17609) - Portal: Orgrimmar(121852)
DELETE FROM spell_target_position WHERE id IN (17609, 121852 );
INSERT INTO spell_target_position (id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(121852, 0, 1, 1776.5, -4338.8, -7.48, 3.55349);

-- Portal Effect: Tol Barad(Trigger:88339) - Portal: Tol Barad(121860)
DELETE FROM spell_target_position WHERE id IN (88339, 121860 );
INSERT INTO spell_target_position (id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(121860, 0, 732, -369.208, 1058.73, 21.7719, 0.634577);

-- Portal Effect: Darnassus(Trigger:17608) - Portal: Darnassus(121849)
DELETE FROM spell_target_position WHERE id IN (17608, 121849 );
INSERT INTO spell_target_position (id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(121849, 0, 1, 9656.54, 2518.26, 1331.66, 0);

-- Portal Effect: Thunder Bluff(Trigger:17610) - Portal: Thunder Bluff(121859)
DELETE FROM spell_target_position WHERE id IN (17610, 121859 );
INSERT INTO spell_target_position (id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(121859, 0, 1, -967.375, 284.82, 110.773, 3.19999);

-- Portal Effect: Undercity(Trigger:17611) - Portal: Undercity(121862)
DELETE FROM spell_target_position WHERE id IN (17611, 121862 );
INSERT INTO spell_target_position (id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(121862, 0, 0, 1773.42, 61.7391, -46.3215, 0);

-- Portal Effect: Stonard(Trigger:49363) - Portal: Stonard(121856)
DELETE FROM spell_target_position WHERE id IN (49363, 121856 );
INSERT INTO spell_target_position (id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(121856, 0, 0, -10469, -3331.54, 25.4716, 0.98606);
UPDATE spell_target_position SET effIndex=1 WHERE effIndex=0 AND id IN
(67835, -- Sholazar Basin
 67836, -- Icecrown
 67837, -- Storm Peaks
 67834, -- Borean Tundra
 68081, -- Underground
 84073, -- Deepholm Intro Teleport
 109984, -- Anti-Exploit LOS Check
 100268, -- Teleport to Abyssal Breach
 100335, -- Teleport to Therazane's Throne
 100508, -- Teleport to Firelands
 100509, -- Teleport to Uldum
 100523); -- Teleport to Hyjal
DELETE FROM spell_script_names WHERE spell_id IN (-55090, 55090) AND ScriptName='spell_dk_scourge_strike';
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES
(55090, 'spell_dk_scourge_strike');
DELETE FROM spell_script_names WHERE spell_id=80353;
INSERT INTO spell_script_names (spell_id, ScriptName) VALUES
(80353, 'spell_mage_time_warp');
-- Disable Quest 9510: Bristlehide Clefthoof Hides
DELETE FROM disables WHERE sourceType=1 AND entry=9510;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9510, 'Disable Quest 9510: Bristlehide Clefthoof Hides');

-- Disable Quest 10084: Assault on Mageddon
DELETE FROM disables WHERE sourceType=1 AND entry=10084;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10084, 'Disable Quest 10084: Assault on Mageddon');

-- Disable Quest 10100: The Mastermind
DELETE FROM disables WHERE sourceType=1 AND entry=10100;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10100, 'Disable Quest 10100: The Mastermind');

-- Disable Quest 9342: Marauding Crust Bursters
DELETE FROM disables WHERE sourceType=1 AND entry=9342;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9342, 'Disable Quest 9342: Marauding Crust Bursters');

-- Disable Quest 9930: The Missing Merchant
DELETE FROM disables WHERE sourceType=1 AND entry=9930;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9930, 'Disable Quest 9930: The Missing Merchant');

-- Disable Quest 7426: One Man's Love
DELETE FROM disables WHERE sourceType=1 AND entry=7426;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 7426, 'Disable Quest 7426: One Man''s Love');

-- Disable Quest 6706: [DEPRECATED] <TXT> ZUL, ST, MAR
DELETE FROM disables WHERE sourceType=1 AND entry=6706;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 6706, 'Disable Quest 6706: [DEPRECATED] <TXT> ZUL, ST, MAR');

-- Disable Quest 6062: Taming the Beast
DELETE FROM disables WHERE sourceType=1 AND entry=6062;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 6062, 'Disable Quest 6062: Taming the Beast');

-- Disable Quest 6067: The Hunter's Path
DELETE FROM disables WHERE sourceType=1 AND entry=6067;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 6067, 'Disable Quest 6067: The Hunter''s Path');

-- Disable Quest 6081: Training the Beast
DELETE FROM disables WHERE sourceType=1 AND entry=6081;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 6081, 'Disable Quest 6081: Training the Beast');

-- Disable Quest 6082: Taming the Beast
DELETE FROM disables WHERE sourceType=1 AND entry=6082;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 6082, 'Disable Quest 6082: Taming the Beast');

-- Disable Quest 6083: Taming the Beast
DELETE FROM disables WHERE sourceType=1 AND entry=6083;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 6083, 'Disable Quest 6083: Taming the Beast');

-- Disable Quest 6708: [DEPRECATED] <TXT> BRD, DM, BRS
DELETE FROM disables WHERE sourceType=1 AND entry=6708;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 6708, 'Disable Quest 6708: [DEPRECATED] <TXT> BRD, DM, BRS');

-- Disable Quest 10083: The Legion's Plans
DELETE FROM disables WHERE sourceType=1 AND entry=10083;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10083, 'Disable Quest 10083: The Legion''s Plans');

-- Disable Quest 9947: Return to Rokag
DELETE FROM disables WHERE sourceType=1 AND entry=9947;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9947, 'Disable Quest 9947: Return to Rokag');

-- Disable Quest 9949: A Bird's-Eye View
DELETE FROM disables WHERE sourceType=1 AND entry=9949;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9949, 'Disable Quest 9949: A Bird''s-Eye View');

-- Disable Quest 9346: When Helboars Fly
DELETE FROM disables WHERE sourceType=1 AND entry=9346;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9346, 'Disable Quest 9346: When Helboars Fly');

-- Disable Quest 9969: The Final Reagents
DELETE FROM disables WHERE sourceType=1 AND entry=9969;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9969, 'Disable Quest 9969: The Final Reagents');

-- Disable Quest 9929: The Missing Merchant
DELETE FROM disables WHERE sourceType=1 AND entry=9929;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9929, 'Disable Quest 9929: The Final Reagents');

-- Disable Quest 9959: Scouting the Defenses
DELETE FROM disables WHERE sourceType=1 AND entry=9959;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9959, 'Disable Quest 9959: Scouting the Defenses');

-- Disable Quest 9953: Lookout Nodak
DELETE FROM disables WHERE sourceType=1 AND entry=9953;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9953, 'Disable Quest 9953: Lookout Nodak');

-- Disable Quest 9943: Return to Thander
DELETE FROM disables WHERE sourceType=1 AND entry=9943;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9943, 'Disable Quest 9943: Return to Thander');

-- Disable Quest 9344: A Hasty Departure
DELETE FROM disables WHERE sourceType=1 AND entry=9344;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9344, 'Disable Quest 9344: A Hasty Departure');

-- Disable Quest 10089: Forge Camps of the Legion
DELETE FROM disables WHERE sourceType=1 AND entry=10089;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10089, 'Disable Quest 10089: Forge Camps of the Legion');

-- Disable Quest 10149: Mission: End All, Be All
DELETE FROM disables WHERE sourceType=1 AND entry=10149;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10149, 'Disable Quest 10149: Mission: End All, Be All');

-- Disable Quest 9952: Mission: End All, Be All
DELETE FROM disables WHERE sourceType=1 AND entry=9952;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 9952, 'Disable Quest 9952: Prospector Balmoral');

-- Disable Quest 10090: The Legion's Plans
DELETE FROM disables WHERE sourceType=1 AND entry=10090;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10090, 'Disable Quest 10090: The Legion''s Plans');

-- Disable Quest 10219: Walk the Dog
DELETE FROM disables WHERE sourceType=1 AND entry=10219;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10219, 'Disable Quest 10219: Walk the Dog');

-- Disable Quest 10398: Return to Honor Hold
DELETE FROM disables WHERE sourceType=1 AND entry=10398;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10398, 'Disable Quest 10398: Return to Honor Hold');

-- Disable Quest 10401: Mission: End All, Be All
DELETE FROM disables WHERE sourceType=1 AND entry=10401;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10401, 'Disable Quest 10401: Mission: End All, Be All');

-- Disable Quest 10841: The Vengeful Harbinger
DELETE FROM disables WHERE sourceType=1 AND entry=10841;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10841, 'Disable Quest 10841: The Vengeful Harbinger');

-- Disable Quest 10631: Back In Business
DELETE FROM disables WHERE sourceType=1 AND entry=10631;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10631, 'Disable Quest 10631: Back In Business');

-- Disable Quest 13150: ZZOLDLost: Widget
DELETE FROM disables WHERE sourceType=1 AND entry=13150;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 13150, 'Disable Quest 13150: ZZOLDLost: Widget');

-- Disable Quest 13299: ZZOLDWisp Cloak
DELETE FROM disables WHERE sourceType=1 AND entry=13299;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 13299, 'Disable Quest 13299: ZZOLDWisp Cloak');

-- Disable Quest 10080: Forge Camps of the Legion
DELETE FROM disables WHERE sourceType=1 AND entry=10080;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 10080, 'Disable Quest 10080: Forge Camps of the Legion');

-- Disable Quest 13303: The Panchurian Candidate
DELETE FROM disables WHERE sourceType=1 AND entry=13303;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 13303, 'Disable Quest 13303: The Panchurian Candidate');

-- Disable Quest 29260: The Elements Cry Out
DELETE FROM disables WHERE sourceType=1 AND entry=29260;
INSERT INTO disables (sourceType, entry, comment) VALUES
(1, 29260, 'Disable Quest 29260: The Elements Cry Out');
UPDATE creature_equip_template SET itemEntry2=0 WHERE itemEntry2=512;

DELETE FROM creature_equip_template WHERE itemEntry1 IN
(155857, 118409, 141604);
-- SmartScript for Quest: Paint it Black (26342).
SET @CRUSHCOG := 42291;
SET @PIBCREDIT := 42796;
SET @SIRENSPELL := 84152;
SET @QITEMSPELL := 79781;
UPDATE creature_template SET AIName='SmartAI' WHERE entry=@CRUSHCOG;
DELETE FROM smart_scripts WHERE entryorguid=@CRUSHCOG;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(@CRUSHCOG, 0, 0, 0, 9, 0, 100, 0, 0, 8, 15000, 25000, 11, @SIRENSPELL, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog Sentry-Bot - Cast Wailing Siren on Close'),
(@CRUSHCOG, 0, 1, 2, 8, 0, 100, 0, @QITEMSPELL, 0, 0, 0, 33, @PIBCREDIT, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Crushcog Sentry-Bot - Give Credit on Spell Hit'),
(@CRUSHCOG, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Crushcog Sentry-Bot - Despawn on Spell Hit');
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
-- SmartScript for Quest: Saving the Young
SET @HATCHLING := 39157;
SET @WHISTLESPELL := 70874;
UPDATE creature_template SET AIName='SmartAI' WHERE entry=@HATCHLING;
DELETE FROM smart_scripts WHERE entryorguid=@HATCHLING;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(@HATCHLING, 0, 0, 1, 8, 0, 100, 0, @WHISTLESPELL, 0, 0, 0, 33, @HATCHLING, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Lost Bloodtalon Hatchling - Give Credit On SpellHit'),
(@HATCHLING, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Lost Bloodtalon Hatchling - Despawn on Spell Hit');
-- CreatureLoot for Multiple Quests named "A Rough Start"
SET @QITEM := 50222;
SET @WILDMANECAT := 38046;
UPDATE creature_template SET lootid=@WILDMANECAT WHERE entry=@WILDMANECAT;
DELETE FROM creature_loot_template WHERE entry=@WILDMANECAT;
INSERT INTO creature_loot_template (entry, item, ChanceOrQuestChance, lootmode, groupid, mincountOrRef, maxcount) VALUES
(@WILDMANECAT, @QITEM, -100, 1, 0, 1, 1);
-- Added missing page_text for The Emperor's Burden - Part 8
DELETE FROM page_text WHERE entry=4546;
INSERT INTO page_text (entry, text, next_page, WDBVerified) VALUES
(4546, 'Ten thousand years ago, Emperor Shaohao, the Last Emperor of Pandaria, used the power of these sacred waters to spare Pandaria from the devastation of the Sundering that destroyed the rest of the world.', 0, 1);
-- Remove Obsolete spell_pet_auras
DELETE FROM spell_pet_auras WHERE spell IN
(19028, 19578, 35691, 35692, 35693, 56314, 56314, 23785, 23822, 23823, 23824, 23825,
 23785, 23822, 23823, 23824, 23825, 23785, 23822, 23823, 23824, 23825, 23785, 23822,
 23823, 23824, 23825, 23785, 23822, 23823, 23824, 23825, 34460);
DELETE FROM spell_pet_auras WHERE aura IN
(57452, 57484);

-- Remove Obsolete spell_scripts
DELETE FROM spell_scripts WHERE id IN
(34448, 34452);

-- Remove Obsolete player_factionchange_spells
DELETE FROM player_factionchange_spells WHERE horde_id=53736;

-- Remove Obsolete player_factionchange_achievement
DELETE FROM player_factionchange_achievement WHERE alliance_id IN
(970, 1681, 1684, 1692, 1707, 2797);
DELETE FROM player_factionchange_achievement WHERE horde_id IN
(1360, 1168, 1170, 1173, 1274, 1784, 1657, 1677, 1680, 2776, 2145, 2195, 3656);
-- Added Teleport: Moonglade TARGET_DEST_DB
-- no delete on purpose to not execute next code twice
INSERT INTO spell_target_position (id, effIndex, target_map, target_position_x, target_position_y, target_position_z, target_orientation) VALUES
(18960, 0, 1, 7995.56, -2679.75, 512.49, 0);

-- Increment npcflags by UNIT_NPC_FLAG_GOSSIP for creatures with gossipmenus
-- WARNING: ONLY SFDB 18!
SET @UNIT_NPC_FLAG_GOSSIP := 1;
UPDATE creature_template SET npcflag=(npcflag+@UNIT_NPC_FLAG_GOSSIP) WHERE entry IN
(2239, 38043, 3143, 37223, 3433, 37671, 4258, 4752, 4772, 4794, 5119, 5164, 5191,
 5637, 48617, 5835, 6491, 6729, 7772, 7937, 7953, 7954, 35344, 37592, 10162, 36993,
 11104, 11117, 11146, 11177, 11486, 14369, 14846, 38764, 15708, 35611, 16261, 16280,
 16610, 17015, 17772, 17827, 17852, 17948, 17982, 18530, 19471, 19476, 19858, 19923,
 20977, 21859, 21950, 22258, 22433, 22468, 5627, 56103, 23052, 23053, 23316, 23139,
 23140, 23149, 17764, 22823, 23089, 23101, 23145, 23191, 23376, 25319, 25924, 55500,
 27817, 27215, 28067, 28209, 28210, 27216, 26879, 29327, 23895, 24037, 24066, 24713,
 24788, 24807, 24956, 25841, 26206, 26218, 26547, 26664, 26666, 27657, 27658, 27659,
 27810, 28042, 28044, 28099, 28283, 28284, 29145, 29319, 29344, 29396, 29778, 29804,
 30007, 30399, 30401, 30708, 30944, 30997, 31044, 37589, 31115, 31135, 31154, 31261,
 31333, 31776, 32301, 33235, 33648, 33674, 33956, 35766, 35093, 35603, 36517, 36990,
 37120, 37554, 37688, 37696, 38054, 38316, 36955, 37221, 37675, 37887, 38041, 37715,
 38039, 38040, 38042, 38295, 36471, 36631, 38124, 38465, 38553, 38738, 38745, 38794,
 38989, 39442, 39660, 40825, 42383, 44352, 44353, 49409, 49941, 49956, 52212, 54852,
 57107);

-- Remove Obsolete aura from Training Dummy
UPDATE creature_template_addon SET auras='83470' WHERE entry=44614;

DELETE FROM spell_script_names WHERE ScriptName='spell_hun_disengage';
DELETE FROM spell_script_names WHERE spell_id=781;
DELETE FROM spell_script_names WHERE ScriptName='spell_pri_glyph_of_prayer_of_healing';
DELETE FROM spell_script_names WHERE spell_id=55680;
DELETE FROM spell_script_names WHERE ScriptName='spell_rog_prey_on_the_weak';
DELETE FROM spell_script_names WHERE spell_id=51685;
-- SAI for Quest: "See to the Survivors (27671)"
SET @SURVIVOR := 46268;
SET @ATB := 86264;
SET @EMOTE := 26;

DELETE FROM conditions WHERE SourceEntry = @ATB;
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ElseGroup, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, NegativeCondition, ErrorType, ErrorTextId, ScriptName, Comment) VALUES
(17, 0, @ATB, 0, 0, 31, 1, 3, @SURVIVOR, 0, 0, 0, 0, '', 'Item Teleport Beacon can only be used if target is Survivor');

DELETE FROM creature_template_addon WHERE entry= @ATB;
INSERT INTO creature_template_addon (entry, path_id, mount, bytes1, bytes2, emote, auras) VALUES
(@ATB, 0, 0, 0, 1, 431, NULL);

UPDATE creature_template SET AIName = "SmartAI" WHERE entry = @SURVIVOR;

DELETE FROM smart_scripts WHERE entryorguid = @SURVIVOR;
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(@SURVIVOR, 0, 0, 0, 1, 0, 50, 0, 60000, 300000, 60000, 300000, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Survivor - Say Text 0'),
(@SURVIVOR, 0, 1, 2, 8, 0, 100, 1, @ATB, 0, 0, 0, 17, @EMOTE, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Survivor - On Spell Hit "Activate Teleport Beacon" - Set Emote State (EMOTE_CONGRATULATE)'),
(@SURVIVOR, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 33, @SURVIVOR, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Survivor - linked - Give Killcredit (46268)'),
(@SURVIVOR, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Survivor - linked - Despawn after 5 seconds');

DELETE FROM creature_text WHERE entry = @SURVIVOR;
INSERT INTO creature_text (entry, groupid, id, text, type, language, probability, emote, duration, sound, comment) VALUES
(@SURVIVOR, 0, 0, 'Don''t let them eat me!', 12, 0, 100, 0, 0, 0, 'Survivor'),
(@SURVIVOR, 0, 1, 'Help! I''m too weak to make it on my own.', 12, 0, 100, 0, 0, 0, 'Survivor'),
(@SURVIVOR, 0, 2, 'I don''t want to die down here!', 12, 0, 100, 0, 0, 0, 'Survivor'),
(@SURVIVOR, 0, 3, 'I''m really going to die down here, aren''t I?', 12, 0, 100, 0, 0, 0, 'Survivor'),
(@SURVIVOR, 0, 4, 'I''m too young to die!', 12, 0, 100, 0, 0, 0, 'Survivor'),
(@SURVIVOR, 0, 5, 'Please help me!', 12, 0, 100, 0, 0, 0, 'Survivor'),
(@SURVIVOR, 0, 6, 'Some help! Anyone!', 12, 0, 100, 0, 0, 0, 'Survivor');
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
-- GameObjectLoot for Quest: "Stealing is Bad... Re-Stealing is OK"
SET @QITEM := 210768;
SET @GD1 := 41405;
DELETE FROM gameobject_loot_template WHERE entry=@GD1;
INSERT INTO gameobject_loot_template (entry, item, ChanceOrQuestChance) VALUES
(@GD1, @QITEM, -100);
