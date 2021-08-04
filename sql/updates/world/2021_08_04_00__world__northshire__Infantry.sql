-- Injured Stormwind Infantry Dummy
-- INFO: is a trigger. That see if look at models by wowhead.
-- https://www.wowhead.com/npc=50378/injured-stormwind-infantry-dummy
-- fix: add a trigger flag
UPDATE `creature_template` SET `flags_extra` = '128' WHERE `entry` = 50378;

-- add the guard stay at the point
-- TODO: i don't know why is unit_flags |= 512 doesn't work (wolfs can't find the guard for attack)
-- UNIT_FLAG_DISABLE_MOVE + UNIT_FLAG_IN_COMBAT (4 + 524288) = 524292
UPDATE `creature` SET `unit_flags`=524292 WHERE `guid` IN (168336, 168344, 168356, 168357);

-- stormwind-infantry
SET @ENTRY := 49869;
-- Add text
DELETE FROM `creature_text` WHERE `entry`=@ENTRY AND `groupid` = 0;
INSERT INTO `creature_text`(`entry`, `groupid`, `id`, `text`, `type`, `probability`, `comment`) VALUES 
(@ENTRY, 0, 0, "HELP!", 12, 25, "taken from https://www.wowhead.com/npc=49869/stormwind-infantry"),
(@ENTRY, 0, 1, "I could use a heal, brother!", 12, 25, "taken from https://www.wowhead.com/npc=49869/stormwind-infantry"),
(@ENTRY, 0, 2, "I need a heal!", 12, 25, "taken from https://www.wowhead.com/npc=49869/stormwind-infantry"),
(@ENTRY, 0, 3, "Make yourself useful and heal me, Paxton!", 12, 25, "taken from https://www.wowhead.com/npc=49869/stormwind-infantry");