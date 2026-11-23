-- Grimaxe's Demise (26102): General Grimaxe (42010) stands on the Dark Iron Golem (42008) in the middle of the frozen lake and
-- has to be killed for the quest, but his template makes him immune to players as well as to creatures, so he could not be
-- attacked (upstream issue 1480). Legends of Azeroth 5.4.8 keeps him immune to creatures only, so the defenders cannot kill him
-- and the players can. His seat on the golem is not touched.

UPDATE `creature_template` SET `unit_flags` = 33280 WHERE `entry` = 42010 AND `ScriptName` = '';
