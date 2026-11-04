-- Valley of Trials (Orc starting zone): data fixes from the Legends of Azeroth 5.4.8 data (the zone audit findings
-- VT-1, VT-3, VT-8 and VT-9). No new spawns, so no new guids.

-- ---------------------------------------------------------------------------------------------------------------
-- VT-1: the 12 class starter quests were Level 2 / MinLevel 2; LoA and 4.3.4 have 3 / 3 and the quest texts say
-- "Reach level 3 ...". The two Monk quests (31156, 31157) are 3 / 2 in LoA (no 4.3.4 row), so their MinLevel stays 2.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `quest_template` SET `Level` = 3, `MinLevel` = 3 WHERE `Id` IN (2383, 3087, 3088, 3089, 3090, 25138, 25139, 25141, 25143, 25145, 25147, 25149);
UPDATE `quest_template` SET `Level` = 3, `MinLevel` = 2 WHERE `Id` IN (31156, 31157);

-- ---------------------------------------------------------------------------------------------------------------
-- VT-3: "Welcome!" (5843, Razor Hill Gift Voucher) had no turn-in; LoA has Magga (11943), who is spawned.
-- ---------------------------------------------------------------------------------------------------------------
DELETE FROM `creature_questender` WHERE `id` = 11943 AND `quest` = 5843;
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(11943, 5843);

-- ---------------------------------------------------------------------------------------------------------------
-- VT-8: Lo'Shall (39224) had minlevel 1; both references have 2 (maxlevel is already 2). The two Den Grunt templates
-- (5952, 51787) are left as they are: the references have the scaled level 90 Orgrimmar version.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `creature_template` SET `minlevel` = 2 WHERE `entry` = 39224; -- Lo'Shall

-- ---------------------------------------------------------------------------------------------------------------
-- VT-9: NextQuestIdChain (the quest offered automatically on turn-in) set to the value LoA and 4.3.4 agree on. The six
-- class letters offer their class quest; Cutting Teeth (25126), Vile Familiars (25131), Report to Sen'jin Village
-- (25133) and the Burning Blade Medallion quest (25134) offer nothing (home had 25172, 25132, 25167 and 25135). The
-- quests stay obtainable from their givers; only the automatic popup changes.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `quest_template` SET `NextQuestIdChain` = 25147 WHERE `Id` = 2383;
UPDATE `quest_template` SET `NextQuestIdChain` = 25139 WHERE `Id` = 3087;
UPDATE `quest_template` SET `NextQuestIdChain` = 25141 WHERE `Id` = 3088;
UPDATE `quest_template` SET `NextQuestIdChain` = 25143 WHERE `Id` = 3089;
UPDATE `quest_template` SET `NextQuestIdChain` = 25145 WHERE `Id` = 3090;
UPDATE `quest_template` SET `NextQuestIdChain` = 25149 WHERE `Id` = 25138;
UPDATE `quest_template` SET `NextQuestIdChain` = 0 WHERE `Id` IN (25126, 25131, 25133, 25134);
