-- The Cosmetic Mottled Raptors (40495) lying dead near Whelgar's Retreat attacked passing players while they could not be
-- selected. They lacked the immunity to players and creatures and the civilian flag, so they took part in combat. Unit
-- flags (immune to players and creatures added) and the civilian flag are as in Legends of Azeroth 5.4.8.

UPDATE `creature_template` SET `unit_flags` = 33555200, `flags_extra` = 2 WHERE `entry` = 40495 AND `ScriptName` = '';
