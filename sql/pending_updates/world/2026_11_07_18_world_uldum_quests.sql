-- Uldum (map 1, zone 5034): quest fixes on top of the
-- spawn update (2026_11_07_16), the Schnottz's Landing chapters and the detection rows.
--
-- Based on Legends of Azeroth 5.4.8 (chain data, extra starters and enders, gossip menus, loot, templates, click rows,
-- the Impending Retribution credits that its C++ gives, the Adarrah spawn and the Easy Money shortcut of block 11),
-- The Cataclysm Preservation Project 4.3.4 (gossip credit rows, click rows, Suspended Starlight and Titan Discs chest
-- loot). Block 8 is derived from the same pattern and is marked. The ender of 28210 "Shaping Up" is not set yet:
-- every database names an entry nobody spawns.
-- Safe to run twice: every block deletes its own keys first or sets absolute values.
--
-- Left out on purpose: every SmartAI cast flag above 8 and event flag 0x200 (the 4.3.4 Asaq row), the 5.4.8 action
-- 91 rows, the vehicle, caravan, camera and chapter scenes, and every combat spell row of the
-- Schnottz mobs (not needed for a quest).

-- 1. Chain data: the 4.3.4 data had no prerequisites, so with every NPC version of a swap pair imported unphased a
-- player could start the chains at any step. The 5.4.8 prerequisite, next-quest and exclusive-group values for the quests of
-- zone 5034, 5396 and 5035 that differ (not the dead duplicates 27925, 27946, 29809). **A gate behind a quest that cannot be
-- finished today is held back** (list below): the Landing, Harrison, Neferset and gnome lines hang on scene or script quests
-- (27003, 27141, 27196, 27511, 27623, 27905 and others), and a gate there would make everything behind them
-- unreachable. Quests whose own next-quest value would add such a quest as a prerequisite are held back the same way.
UPDATE `quest_template` SET `PrevQuestId` = 27520, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27519;
UPDATE `quest_template` SET `PrevQuestId` = 27519, `NextQuestId` = 27602, `ExclusiveGroup` = 0 WHERE `Id` = 27595;
UPDATE `quest_template` SET `PrevQuestId` = 27595, `NextQuestId` = 27623, `ExclusiveGroup` = 0 WHERE `Id` = 27602;
UPDATE `quest_template` SET `PrevQuestId` = 27431, `NextQuestId` = 27669, `ExclusiveGroup` = 0 WHERE `Id` = 27624;
UPDATE `quest_template` SET `PrevQuestId` = 27624, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27627;
UPDATE `quest_template` SET `PrevQuestId` = 27706, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27628;
UPDATE `quest_template` SET `PrevQuestId` = 27838, `NextQuestId` = 27632, `ExclusiveGroup` = 0 WHERE `Id` = 27629;
UPDATE `quest_template` SET `PrevQuestId` = 27628, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27630;
UPDATE `quest_template` SET `PrevQuestId` = 28533, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27631;
UPDATE `quest_template` SET `PrevQuestId` = 27629, `NextQuestId` = 27707, `ExclusiveGroup` = 0 WHERE `Id` = 27632;
UPDATE `quest_template` SET `PrevQuestId` = 28613, `NextQuestId` = 28612, `ExclusiveGroup` = 0 WHERE `Id` = 27748;
UPDATE `quest_template` SET `PrevQuestId` = 27755, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27760;
UPDATE `quest_template` SET `PrevQuestId` = 27760, `NextQuestId` = 27778, `ExclusiveGroup` = 0 WHERE `Id` = 27761;
UPDATE `quest_template` SET `PrevQuestId` = 27760, `NextQuestId` = 27778, `ExclusiveGroup` = 0 WHERE `Id` = 27777;
UPDATE `quest_template` SET `PrevQuestId` = 27630, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27836;
UPDATE `quest_template` SET `PrevQuestId` = 27630, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27837;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 27900, `ExclusiveGroup` = 0 WHERE `Id` = 27899;
UPDATE `quest_template` SET `PrevQuestId` = 27900, `NextQuestId` = 27903, `ExclusiveGroup` = 0 WHERE `Id` = 27901;
UPDATE `quest_template` SET `PrevQuestId` = 27903, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27905;
-- Turned on with block 11 (Easy Money 27003 completes on accept at Adarrah):
UPDATE `quest_template` SET `PrevQuestId` = 27003, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27922;
UPDATE `quest_template` SET `PrevQuestId` = 27922, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27923;
UPDATE `quest_template` SET `PrevQuestId` = 27922, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27924;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 27928, `ExclusiveGroup` = 0 WHERE `Id` = 27926;
UPDATE `quest_template` SET `PrevQuestId` = 27926, `NextQuestId` = 27941, `ExclusiveGroup` = 0 WHERE `Id` = 27928;
UPDATE `quest_template` SET `PrevQuestId` = 27941, `NextQuestId` = 27942, `ExclusiveGroup` = 0 WHERE `Id` = 27939;
UPDATE `quest_template` SET `PrevQuestId` = 27928, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27940;
UPDATE `quest_template` SET `PrevQuestId` = 28002, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27990;
UPDATE `quest_template` SET `PrevQuestId` = 28134, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27993;
UPDATE `quest_template` SET `PrevQuestId` = 27969, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28002;
UPDATE `quest_template` SET `PrevQuestId` = 27922, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28105;
UPDATE `quest_template` SET `PrevQuestId` = 28105, `NextQuestId` = 28134, `ExclusiveGroup` = 0 WHERE `Id` = 28112;
UPDATE `quest_template` SET `PrevQuestId` = 28112, `NextQuestId` = 28135, `ExclusiveGroup` = 0 WHERE `Id` = 28134;
UPDATE `quest_template` SET `PrevQuestId` = 28134, `NextQuestId` = 27520, `ExclusiveGroup` = 0 WHERE `Id` = 28135;
UPDATE `quest_template` SET `PrevQuestId` = 28134, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28141;
UPDATE `quest_template` SET `PrevQuestId` = 28134, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28145;
UPDATE `quest_template` SET `PrevQuestId` = 28187, `NextQuestId` = 28195, `ExclusiveGroup` = 0 WHERE `Id` = 28193;
UPDATE `quest_template` SET `PrevQuestId` = 28193, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28194;
UPDATE `quest_template` SET `PrevQuestId` = 28193, `NextQuestId` = 28267, `ExclusiveGroup` = 0 WHERE `Id` = 28195;
UPDATE `quest_template` SET `PrevQuestId` = 27631, `NextQuestId` = 28210, `ExclusiveGroup` = 0 WHERE `Id` = 28198;
UPDATE `quest_template` SET `PrevQuestId` = 27631, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28200;
UPDATE `quest_template` SET `PrevQuestId` = 27631, `NextQuestId` = 27839, `ExclusiveGroup` = 0 WHERE `Id` = 28201;
UPDATE `quest_template` SET `PrevQuestId` = 28134, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28250;
UPDATE `quest_template` SET `PrevQuestId` = 28267, `NextQuestId` = 28273, `ExclusiveGroup` = 0 WHERE `Id` = 28269;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 28363, `ExclusiveGroup` = 0 WHERE `Id` = 28271;
UPDATE `quest_template` SET `PrevQuestId` = 28269, `NextQuestId` = 28274, `ExclusiveGroup` = 0 WHERE `Id` = 28273;
UPDATE `quest_template` SET `PrevQuestId` = 28273, `NextQuestId` = 28350, `ExclusiveGroup` = 0 WHERE `Id` = 28274;
UPDATE `quest_template` SET `PrevQuestId` = 28210, `NextQuestId` = 28277, `ExclusiveGroup` = 0 WHERE `Id` = 28276;
UPDATE `quest_template` SET `PrevQuestId` = 28276, `NextQuestId` = 28291, `ExclusiveGroup` = 0 WHERE `Id` = 28277;
UPDATE `quest_template` SET `PrevQuestId` = 28274, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28351;
UPDATE `quest_template` SET `PrevQuestId` = 28274, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28352;
UPDATE `quest_template` SET `PrevQuestId` = 28350, `NextQuestId` = 28271, `ExclusiveGroup` = 0 WHERE `Id` = 28353;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 28367, `ExclusiveGroup` = 0 WHERE `Id` = 28363;
UPDATE `quest_template` SET `PrevQuestId` = 28363, `NextQuestId` = 28402, `ExclusiveGroup` = 0 WHERE `Id` = 28367;
UPDATE `quest_template` SET `PrevQuestId` = 28367, `NextQuestId` = 28403, `ExclusiveGroup` = 0 WHERE `Id` = 28402;
UPDATE `quest_template` SET `PrevQuestId` = 28623, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28480;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 28497, `ExclusiveGroup` = 0 WHERE `Id` = 28482;
UPDATE `quest_template` SET `PrevQuestId` = 28623, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28483;
UPDATE `quest_template` SET `PrevQuestId` = 28483, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28486;
UPDATE `quest_template` SET `PrevQuestId` = 28561, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28498;
UPDATE `quest_template` SET `PrevQuestId` = 28561, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28499;
UPDATE `quest_template` SET `PrevQuestId` = 28499, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28500;
UPDATE `quest_template` SET `PrevQuestId` = 28500, `NextQuestId` = 28623, `ExclusiveGroup` = 0 WHERE `Id` = 28501;
UPDATE `quest_template` SET `PrevQuestId` = 28134, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28502;
UPDATE `quest_template` SET `PrevQuestId` = 27738, `NextQuestId` = 28561, `ExclusiveGroup` = 0 WHERE `Id` = 28533;
UPDATE `quest_template` SET `PrevQuestId` = 28533, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28561;
UPDATE `quest_template` SET `PrevQuestId` = 27837, `NextQuestId` = 27838, `ExclusiveGroup` = 0 WHERE `Id` = 28611;
UPDATE `quest_template` SET `PrevQuestId` = 28622, `NextQuestId` = 28654, `ExclusiveGroup` = 0 WHERE `Id` = 28633;
UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28760;
UPDATE `quest_template` SET `PrevQuestId` = 27520, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28845;

-- Held back (5.4.8 values, turn on a line when its prerequisite quest works; home value in the comment):
-- UPDATE `quest_template` SET `PrevQuestId` = 27141, `NextQuestId` = 27196, `ExclusiveGroup` = 0 WHERE `Id` = 27176;   [home prev 0 next 27196]
-- UPDATE `quest_template` SET `PrevQuestId` = 27141, `NextQuestId` = 27196, `ExclusiveGroup` = 0 WHERE `Id` = 27179;   [home prev 0 next 27196]
-- UPDATE `quest_template` SET `PrevQuestId` = 27141, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27187;   [home prev 0 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 27511, `ExclusiveGroup` = 0 WHERE `Id` = 27196;   [home prev 0 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 27624, `ExclusiveGroup` = 0 WHERE `Id` = 27431;   [home prev 0 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 27511, `NextQuestId` = 27431, `ExclusiveGroup` = 0 WHERE `Id` = 27517;   [home prev 0 next 27431]
-- UPDATE `quest_template` SET `PrevQuestId` = 27511, `NextQuestId` = 27549, `ExclusiveGroup` = 0 WHERE `Id` = 27541;   [home prev 0 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 27602, `NextQuestId` = 27706, `ExclusiveGroup` = 0 WHERE `Id` = 27623;   [home prev 27602 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 27632, `NextQuestId` = 27738, `ExclusiveGroup` = 0 WHERE `Id` = 27707;   [home prev 27632 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 27707, `NextQuestId` = 28533, `ExclusiveGroup` = 0 WHERE `Id` = 27738;   [home prev 27707 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 27905, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27755;   [home prev 0 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 27778, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27779;   [home prev 0 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 27669, `NextQuestId` = 27900, `ExclusiveGroup` = 0 WHERE `Id` = 27899;   [home prev 0 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 27950, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 27969;   [home prev 0 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 28002, `NextQuestId` = 28187, `ExclusiveGroup` = 0 WHERE `Id` = 27990;   [home prev 0 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 28134, `NextQuestId` = 27141, `ExclusiveGroup` = 0 WHERE `Id` = 27993;   [home prev 0 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 27990, `NextQuestId` = 28193, `ExclusiveGroup` = 0 WHERE `Id` = 28187;   [home prev 27990 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 28198, `NextQuestId` = 28276, `ExclusiveGroup` = 0 WHERE `Id` = 28210;   [home prev 28198 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 28195, `NextQuestId` = 28269, `ExclusiveGroup` = 0 WHERE `Id` = 28267;   [home prev 28195 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 0, `NextQuestId` = 28273, `ExclusiveGroup` = 0 WHERE `Id` = 28270;   [home prev 0 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 28402, `NextQuestId` = 28404, `ExclusiveGroup` = 0 WHERE `Id` = 28403;   [home prev 28402 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 28404, `NextQuestId` = 28497, `ExclusiveGroup` = 0 WHERE `Id` = 28482;   [home prev 0 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 28482, `NextQuestId` = 28613, `ExclusiveGroup` = 0 WHERE `Id` = 28497;   [home prev 28482 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 28486, `NextQuestId` = 0, `ExclusiveGroup` = 0 WHERE `Id` = 28520;   [home prev 0 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 27511, `NextQuestId` = 27431, `ExclusiveGroup` = 0 WHERE `Id` = 28602;   [home prev 0 next 27431]
-- UPDATE `quest_template` SET `PrevQuestId` = 28612, `NextQuestId` = 28622, `ExclusiveGroup` = 0 WHERE `Id` = 28621;   [home prev 28612 next 0]
-- UPDATE `quest_template` SET `PrevQuestId` = 28621, `NextQuestId` = 28633, `ExclusiveGroup` = 0 WHERE `Id` = 28622;   [home prev 28621 next 0]

-- 2. Starters and enders: 5.4.8 extra givers (the later NPC versions of Harrison Jones, Sullah, Schnottz and others, all
-- spawned) and the three enders home lacks (27187 and 27627 Harrison Jones, 27707 Vizier Tanotep). The Mangy Hyena
-- (45202) is not a quest giver in 5.4.8: its starter row and quest flag go, Harrison Jones 44860 gives 27187.
DELETE FROM `creature_queststarter` WHERE (`id`, `quest`) IN ((45202, 27187), (44860, 27187), (45180, 27141), (45180, 27176),
(45180, 27179), (45296, 27196), (45296, 27627), (46135, 27630), (46134, 27631), (48034, 27669), (46978, 27899), (48082, 27905),
(47940, 27990), (48162, 28267), (48186, 28269), (48203, 28273), (48431, 28274), (48237, 28276), (48186, 28353), (48431, 28363),
(48528, 28402), (48558, 28403), (48698, 28482), (49248, 28622), (48564, 28623));
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(44860, 27187), (45180, 27141), (45180, 27176), (45180, 27179), (45296, 27196), (45296, 27627), (46135, 27630), (46134, 27631),
(48034, 27669), (46978, 27899), (48082, 27905), (47940, 27990), (48162, 28267), (48186, 28269), (48203, 28273), (48431, 28274),
(48237, 28276), (48186, 28353), (48431, 28363), (48528, 28402), (48558, 28403), (48698, 28482), (49248, 28622), (48564, 28623);
DELETE FROM `creature_questender` WHERE (`id`, `quest`) IN ((44860, 27187), (45296, 27627), (46136, 27707));
INSERT INTO `creature_questender` (`id`, `quest`) VALUES (44860, 27187), (45296, 27627), (46136, 27707);
UPDATE `creature_template` SET `npcflag` = `npcflag` & ~2 WHERE `entry` = 45202;

-- 3. Gossip menus of the Landing conversation NPCs. Home points them at the last menu of each conversation (or none),
-- so the first option never shows and the conversation credits of 27926 and 28002 cannot be given. Menu ids and the
-- gossip flag as in 4.3.4 and 5.4.8; the Impending Retribution NPCs use the 5.4.8 ids (home has the option on the
-- neighbouring menu).
UPDATE `creature_template` SET `gossip_menu_id` = 12211, `npcflag` = `npcflag` | 1 WHERE `entry` = 47176;
UPDATE `creature_template` SET `gossip_menu_id` = 12214, `npcflag` = `npcflag` | 1 WHERE `entry` = 47185;
UPDATE `creature_template` SET `gossip_menu_id` = 12217, `npcflag` = `npcflag` | 1 WHERE `entry` = 47187;
UPDATE `creature_template` SET `gossip_menu_id` = 12220, `npcflag` = `npcflag` | 1 WHERE `entry` = 47189;
UPDATE `creature_template` SET `gossip_menu_id` = 12238, `npcflag` = `npcflag` | 1 WHERE `entry` = 47292;
UPDATE `creature_template` SET `gossip_menu_id` = 12284, `npcflag` = `npcflag` | 1 WHERE `entry` = 47516;
UPDATE `creature_template` SET `gossip_menu_id` = 12288, `npcflag` = `npcflag` | 1 WHERE `entry` = 47519;
UPDATE `creature_template` SET `gossip_menu_id` = 12314, `npcflag` = `npcflag` | 1 WHERE `entry` = 47520;
UPDATE `creature_template` SET `gossip_menu_id` = 12317, `npcflag` = `npcflag` | 1 WHERE `entry` = 47707;
UPDATE `creature_template` SET `gossip_menu_id` = 12358 WHERE `entry` = 47715;
UPDATE `creature_template` SET `gossip_menu_id` = 12361 WHERE `entry` = 47930;
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (46603, 47292, 47516, 47519, 47520, 47715, 45715) AND `ScriptName` = '';
-- Ramkahen Laborer (28201 "Ploughshares to Swords"): its credit row (gossip hello) is live but the template has no gossip
-- flag, so it can never be clicked; 5.4.8 has the gossip flag.
UPDATE `creature_template` SET `npcflag` = `npcflag` | 1 WHERE `entry` = 46333;

-- 4. Conversation credits (28002 Crisis Management: 4.3.4 rows, gossip select then the Kill Credit spell cast by the
-- player on itself, as the existing rows of 47176 to 47189; 28134 Impending Retribution: the three credits that the 5.4.8
-- C++ gives in OnGossipSelect, as quest credit to the player; the option only shows while 28134 is taken, as in that C++).
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (47516, 47519, 47520, 47707, 46603, 47715, 47930, 47292);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(47516, 0, 0, 0, 62, 0, 100, 0, 12286, 0, 0, 0, 0, 85, 88618, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Prolific Writer - On Gossip Option Select - Invoker Cast Kill Credit'),
(47519, 0, 0, 0, 62, 0, 100, 0, 12290, 0, 0, 0, 0, 85, 88617, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Privileged Socialite - On Gossip Option Select - Invoker Cast Kill Credit'),
(47520, 0, 0, 0, 62, 0, 100, 0, 12315, 0, 0, 0, 0, 85, 88817, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Pretentious Businessman - On Gossip Option Select - Invoker Cast Kill Credit'),
(47707, 0, 0, 0, 62, 0, 100, 0, 12318, 0, 0, 0, 0, 85, 88818, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Budding Artist - On Gossip Option Select - Invoker Cast Kill Credit'),
(46603, 0, 0, 1, 62, 0, 100, 0, 12356, 0, 0, 0, 0, 33, 47932, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Nomarch Teneth - On Gossip Option Select - Quest Credit Nomarch Conversation'),
(46603, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Nomarch Teneth - Linked - Close Gossip'),
(47715, 0, 0, 1, 62, 0, 100, 0, 12358, 0, 0, 0, 0, 33, 47933, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sun Priest Asaris - On Gossip Option Select - Quest Credit Asaris Conversation'),
(47715, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Sun Priest Asaris - Linked - Close Gossip'),
(47930, 0, 0, 1, 62, 0, 100, 0, 12361, 0, 0, 0, 0, 33, 47936, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Asaq - On Gossip Option Select - Quest Credit Asaq Conversation'),
(47930, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Asaq - Linked - Close Gossip');
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` IN (12356, 12358, 12361) AND `SourceEntry` = 0 AND `ConditionTypeOrReference` = 9;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 12356, 0, 0, 0, 9, 0, 28134, 0, 0, 0, 0, 0, '', 'Nomarch Teneth conversation option only while quest 28134 is taken'),
(15, 12358, 0, 0, 0, 9, 0, 28134, 0, 0, 0, 0, 0, '', 'Sun Priest Asaris conversation option only while quest 28134 is taken'),
(15, 12361, 0, 0, 0, 9, 0, 28134, 0, 0, 0, 0, 0, '', 'Asaq conversation option only while quest 28134 is taken');

-- 5. Spell clicks: the Desert Fox (27939) and the Dead Troopers (27941) have their SmartAI rows live (event 73) but no
-- spell click row and no click flag, and this core fires event 73 only for a matched row on a flagged creature, so the
-- uniforms and the fox could never be taken. Rows as in 4.3.4 (the clicker casts the dummy "cover" spell, the SmartAI
-- rows create the item); the click only works while the quest is taken. The Orsis Survivor (27519): 5.4.8 click row 85372,
-- credit and despawn from its action list 4571500 (the Sand Burst cast with flag 16, the random say and the data set to
-- the Orsis Survivor Vehicle are left out).
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (47201, 47207, 47213, 47216, 47219, 45715);
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`, `user_type`) VALUES
(47201, 87961, 1, 0), (47207, 87975, 1, 0), (47213, 87975, 1, 0), (47216, 87975, 1, 0), (47219, 87975, 1, 0), (45715, 85372, 1, 0);
UPDATE `creature_template` SET `npcflag` = `npcflag` | 16777216 WHERE `entry` IN (47201, 47207, 47213, 47216, 47219, 45715);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` IN (47201, 47207, 47213, 47216, 47219, 45715);
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 47201, 87961, 0, 0, 9, 0, 27939, 0, 0, 0, 0, 0, '', 'Desert Fox spell click only while quest 27939 is taken'),
(18, 47207, 87975, 0, 0, 9, 0, 27941, 0, 0, 0, 0, 0, '', 'Dead Trooper spell click only while quest 27941 is taken'),
(18, 47213, 87975, 0, 0, 9, 0, 27941, 0, 0, 0, 0, 0, '', 'Dead Trooper spell click only while quest 27941 is taken'),
(18, 47216, 87975, 0, 0, 9, 0, 27941, 0, 0, 0, 0, 0, '', 'Dead Trooper spell click only while quest 27941 is taken'),
(18, 47219, 87975, 0, 0, 9, 0, 27941, 0, 0, 0, 0, 0, '', 'Dead Trooper spell click only while quest 27941 is taken'),
(18, 45715, 85372, 0, 0, 9, 0, 27519, 0, 0, 0, 0, 0, '', 'Orsis Survivor spell click only while quest 27519 is taken');
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 45715;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(45715, 0, 0, 1, 73, 0, 100, 0, 0, 0, 0, 0, 0, 33, 45715, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Orsis Survivor - On Spellclick - Quest Credit Orsis Survivor'),
(45715, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 16777216, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orsis Survivor - Linked - Remove Spellclick Flag'),
(45715, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 5000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orsis Survivor - Linked - Despawn');

-- 6. Quest drops (negative chance). Tormented Tomb-Robber (45765, two spawns, "Be Prepared" 27517 and 28602) is a
-- hostile level 83 mob in 5.4.8 with a rope drop; home has it friendly with no loot. Myzerian (48428, "Myzerian's Head"
-- 28376) has no loot id in home. The Suspended Starlight and Coffer of Promise chests have no loot id in home
-- (4.3.4: 34726 and 36093, quest required); items 61376 (27549, 7 needed) and 64667 (28633).
UPDATE `creature_template` SET `faction_A` = 14, `faction_H` = 14, `lootid` = 45765, `unit_flags` = 512 WHERE `entry` = 45765 AND `faction_A` = 35;
UPDATE `creature_template` SET `lootid` = 48428 WHERE `entry` = 48428 AND `lootid` = 0;
DELETE FROM `creature_loot_template` WHERE (`entry`, `item`) IN ((45765, 61321), (45765, 61376), (48428, 63700));
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(45765, 61321, -100, 1, 0, 1, 1),
(45765, 61376, -0.0437, 1, 0, 1, 1),
(48428, 63700, -88.8754, 1, 0, 1, 1);
UPDATE `gameobject_template` SET `data1` = 34726 WHERE `entry` = 205582 AND `data1` = 0;
UPDATE `gameobject_template` SET `data1` = 36093 WHERE `entry` = 207294 AND `data1` = 0;
DELETE FROM `gameobject_loot_template` WHERE (`entry`, `item`) IN ((34726, 61376), (36093, 64667));
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(34726, 61376, -100, 1, 0, 1, 1),
(36093, 64667, -100, 1, 0, 1, 1);

-- 7. Kill credit link: 28277 "Salhet the Tactician" counts credit 48211 for 30 kills; 5.4.8 links the Bloodsnarl
-- Scavenger (48199, 3 spawns here) to it, home does not.
UPDATE `creature_template` SET `KillCredit1` = 48211 WHERE `entry` = 48199 AND `KillCredit1` = 0;

-- 8. Slacking Laborers (27969 "Make Yourself Useful"). DERIVED, not in either reference: both give the laborer the
-- gossip "Enough slacking! You're behind schedule." (menu 12238, now set) and the Kill Credit spell 88241 (credit 47127),
-- but neither has the row that casts it. Same pattern as the guests above: the player casts the credit spell on itself, the
-- laborer loses the gossip flag until it respawns, so one laborer gives one credit. Drop this block if it is unwanted.
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(47292, 0, 0, 1, 62, 0, 100, 0, 12238, 0, 0, 0, 0, 85, 88241, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Slacking Laborer - On Gossip Option Select - Invoker Cast Kill Credit'),
(47292, 0, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Slacking Laborer - Linked - Close Gossip'),
(47292, 0, 2, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Slacking Laborer - Linked - Remove Gossip Flag');

-- 9. Neferset Frond (27922 "Traitors!"). 5.4.8 does it in C++ (go_neferset_frond completes the quest when the frond is
-- used while the quest is incomplete); the quest's only objective is the credit 47466, so the same result as a SmartAI row
-- on the object: using it gives that credit to the player (it only counts while the quest is taken).
UPDATE `gameobject_template` SET `AIName` = 'SmartAI' WHERE `entry` = 206579 AND `AIName` = '' AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` = 206579;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(206579, 1, 0, 0, 64, 0, 100, 0, 0, 0, 0, 0, 0, 33, 47466, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Neferset Frond - On Gossip Hello - Quest Credit Betrayal');

-- 10. Area Trigger Bunny (47971, chapter 4 of the Landing; 28193 "Lockdown!"). 5.4.8 and 4.3.4 both have the bunny (4.3.4 on
-- the registry 49528) cast the Kill Credit spell 89193 (credit 47970 "Schnottz Guests") on the players within 20 yards every
-- one to two seconds while it is out of combat; home has no AI on it. The credit counts only while the quest is taken.
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 47971 AND `AIName` = '' AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 47971;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(47971, 0, 0, 0, 1, 0, 100, 0, 1000, 2000, 1000, 1500, 0, 11, 89193, 0, 0, 0, 0, 0, 17, 0, 20, 0, 0, 0, 0, 0, 'Area Trigger Bunny - Out Of Combat - Cast Kill Credit on players within 20 yards');

-- 11. Easy Money (27003): the 5.4.8 shortcut instead of the caravan cinematic (phase 298), which cannot be rebuilt from
-- the references. Based on Legends of Azeroth 5.4.8: Adarrah (44833) stands in Tanaris (its spawn is
-- guid 274425 there), his C++ script casts spell 89328 on the player when a quest is accepted from him (27003 is the only
-- quest he starts), and the spell script completes 27003 and teleports the player to the Landing next to Adarrah 46873,
-- the quest's ender. SmartAI does the same with the accepted-quest event: credit 44833 (the quest's only objective,
-- "Caravan Escorted") to the invoker, then the teleport (map 1, -10994.88, -1255.51, 13.24, 4.65). Difference to 5.4.8: the
-- teleport here does not carry the "do not unsummon pet" option, so a pet is called back after the jump.
-- The guid 9126040 is in the range reserved for Uldum. Adarrah is already the starter of 27003 and carries the quest giver
-- flag in home, so only the spawn and the AI are new.
DELETE FROM `creature` WHERE `guid` = 9126040;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(9126040, 44833, 1, 1, 0, 0, 22403, 0, -8920.7, -2261.42, 8.96093, 5.84685, 60, 0, 0, 35967, 0, 0, 0, 0, 0);
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` = 44833 AND `AIName` = '' AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 44833;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(44833, 0, 0, 1, 19, 0, 100, 0, 27003, 0, 0, 0, 0, 33, 44833, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Adarrah - On Quest Accept (Easy Money) - Quest Credit Caravan Escorted'),
(44833, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 62, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, -10994.88, -1255.51, 13.24, 4.65, 'Adarrah - Linked - Teleport Invoker to Schnottz''s Landing');
