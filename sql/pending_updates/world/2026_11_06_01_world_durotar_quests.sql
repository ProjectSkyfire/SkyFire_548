-- Durotar quest fixes outside the Valley of Trials and the Echo Isles: a prerequisite that locks the Sen'jin Village
-- chain for Trolls, and four quest objectives that nothing can complete. Data from the 4.3.4 dump where it has the
-- row, otherwise from the Legends of Azeroth 5.4.8 data (the 4.3.4 dump has no script for these quests).

-- ---------------------------------------------------------------------------------------------------------------
-- "Breaking the Chain" (25167) required "Report to Sen'jin Village" (25133), a Valley of Trials quest, so Trolls
-- (who arrive with "Sen'jin Village", 25073) could never take it and the whole chain behind it. 4.3.4 and 5.4.8 both
-- have no previous quest on 25167; "Report to Sen'jin Village" also listed it as next quest, which adds the same
-- prerequisite, so that link goes too. Master Gadrin still offers 25167 to Orcs after 25133, as in 4.3.4.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `quest_template` SET `PrevQuestId` = 0 WHERE `Id` = 25167;
UPDATE `quest_template` SET `NextQuestId` = 0 WHERE `Id` = 25133;

-- ---------------------------------------------------------------------------------------------------------------
-- "Watershed Patrol" (25188) needs one credit for each of the four quests of the patrol, given when the quest is
-- handed in: Spirits Be Praised (25189) and Raggaran's Fury (25192) to Raggaran, Lost But Not Forgotten (25193) to
-- Misha Tor'kren, That's the End of That Raptor (25195) to Zen'Taji. Nothing gave the credits, so the quest could not
-- be completed. Rows from the 5.4.8 data.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `creature_template` SET `AIName` = 'SmartAI' WHERE `entry` IN (3193, 39324, 39326) AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (3193, 39324, 39326);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3193, 0, 0, 0, 20, 0, 100, 0, 25193, 0, 0, 0, 0, 33, 39333, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Misha Tor''kren - On Quest Reward - Kill Credit Help Misha Tor''kren'),
(39324, 0, 0, 0, 20, 0, 100, 0, 25195, 0, 0, 0, 0, 33, 39334, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Zen''Taji - On Quest Reward - Kill Credit Help Zen''Taji'),
(39326, 0, 0, 0, 20, 0, 100, 0, 25192, 0, 0, 0, 0, 33, 39332, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Raggaran - On Quest Reward - Kill Credit Help Raggaran'),
(39326, 0, 1, 0, 20, 0, 100, 0, 25189, 0, 0, 0, 0, 33, 39331, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Raggaran - On Quest Reward - Kill Credit Help Grandmatron Tekla');

-- ---------------------------------------------------------------------------------------------------------------
-- "Fizzled" (25260) asks for Fizzle's Orb (52580), which no loot table or script gave. Fizzle Darkclaw (3203) has the
-- gossip menu 11240 in the 4.3.4 and 5.4.8 data, but the template had no menu and no gossip flag. The 5.4.8 data
-- adds the orb when "<Attempt to remove the orb.>" is chosen and closes the window, so the follow-up menu 11239 is
-- not used there; the option's link to it is cleared the same way. Fizzle's existing combat rows (ids 0 to 3) stay.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `creature_template` SET `gossip_menu_id` = 11240, `npcflag` = `npcflag` | 1 WHERE `entry` = 3203;
UPDATE `gossip_menu_option` SET `ActionMenuID` = 0 WHERE `MenuID` = 11240 AND `OptionID` = 0;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 3203 AND `id` IN (4, 5);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(3203, 0, 4, 5, 62, 0, 100, 0, 11240, 0, 0, 0, 0, 56, 52580, 1, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Fizzle Darkclaw - On Gossip Option 0 Selected - Add Item Fizzle''s Orb'),
(3203, 0, 5, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Fizzle Darkclaw - Linked To Id 4 - Close Gossip');

-- ---------------------------------------------------------------------------------------------------------------
-- "The Burning Blade" (25232) asks for 6 Burning Blade Spellscrolls (52564), which the Burning Blade Thug (3195),
-- Neophyte (3196) and Cultist (3199) should drop for players on the quest. The drops were missing, so the quest
-- could not be completed. Chances from the 4.3.4 data (quest drops, shown as negative chances here).
-- ---------------------------------------------------------------------------------------------------------------
DELETE FROM `creature_loot_template` WHERE `item` = 52564 AND `entry` IN (3195, 3196, 3199);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(3195, 52564, -43, 1, 0, 1, 1),
(3196, 52564, -86, 1, 0, 1, 1),
(3199, 52564, -64, 1, 0, 1, 1);

-- ---------------------------------------------------------------------------------------------------------------
-- "The Wolf and the Kodo" (25205) is completed by asking Shin Stonepillar (39380) for his fable. The gossip menu
-- 11209 exists in the 4.3.4 and 5.4.8 data but the template had no menu, gossip flag or script, so the objective
-- (credit 39365) could not be reached. Script rows from the 5.4.8 data.
-- ---------------------------------------------------------------------------------------------------------------
UPDATE `creature_template` SET `gossip_menu_id` = 11209, `npcflag` = `npcflag` | 1, `AIName` = 'SmartAI' WHERE `entry` = 39380 AND `ScriptName` = '';
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` = 39380;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(39380, 0, 0, 1, 62, 0, 100, 0, 11209, 0, 0, 0, 0, 33, 39365, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shin Stonepillar - On Gossip Option 0 Selected - Kill Credit The Kodo'),
(39380, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Shin Stonepillar - Linked To Id 0 - Close Gossip');
