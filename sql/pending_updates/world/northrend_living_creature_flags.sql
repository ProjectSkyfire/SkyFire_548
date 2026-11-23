-- Four Northrend creature templates carried the flags of a corpse (immune, unselectable or stunned, feign death), so
-- every spawn lay dead: the Winterskorn Rune-Seers and Focus Wizards could not be fought, the Oil Pools of Fueling
-- the Project could not be selected and the Injured Goblin Miner of Bitter Departure was stunned for good. They get
-- their living flags from the 5.4.8 data; the Focus Wizards also get their hostile faction, without it they stayed
-- friendly to everyone. Safe to run twice.
UPDATE `creature_template` SET `unit_flags` = 33280, `unit_flags2` = 2048, `dynamicflags` = 0 WHERE `entry` = 23667 AND `unit_flags` = 570721024; -- Winterskorn Rune-Seer
UPDATE `creature_template` SET `unit_flags` = 0, `unit_flags2` = 2048, `dynamicflags` = 0 WHERE `entry` = 25781 AND `unit_flags` = 570425344; -- Oil Pool
UPDATE `creature_template` SET `faction_A` = 1985, `faction_H` = 1985, `unit_flags` = 32768, `unit_flags2` = 2048, `dynamicflags` = 0 WHERE `entry` = 26816 AND `unit_flags` = 537165824; -- Focus Wizard
UPDATE `creature_template` SET `unit_flags` = 32768, `unit_flags2` = 2048, `dynamicflags` = 0 WHERE `entry` = 29434 AND `unit_flags` = 537165824; -- Injured Goblin Miner
