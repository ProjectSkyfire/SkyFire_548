-- Training dummies: bind immortal AI where appropriate, no weapon-skill farming, pacified.
-- Absolute per-entry values (no LIKE / bitwise OR / JOIN guesses).

-- Combat / city practice dummies
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=262144, `unit_flags`=131328 WHERE `entry`=17578; -- Hellfire Training Dummy
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=24792; -- Advanced Training Dummy
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=30527; -- Training Dummy
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=31143; -- Reinforced Training Dummy
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=31144; -- Training Dummy
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=31146; -- Raider's Training Dummy
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=32541; -- Initiate's Training Dummy
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=32542; -- Disciple's Training Dummy
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=32543; -- Veteran's Training Dummy
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=32545; -- Initiate's Training Dummy
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=32546; -- Ebon Knight's Training Dummy
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=2359296, `unit_flags`=4595712 WHERE `entry`=32666; -- Training Dummy
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=32667; -- Training Dummy
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=46647; -- Training Dummy
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=262144, `unit_flags`=131076 WHERE `entry`=60197; -- Training Dummy
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=262144, `unit_flags`=131076 WHERE `entry`=64446; -- Training Dummy
UPDATE `creature_template` SET `ScriptName`='npc_training_dummy', `flags_extra`=262144, `unit_flags`=131076 WHERE `entry`=67127; -- Training Dummy

-- Starter-zone quest dummies: keep SmartAI (empty ScriptName); still no skillgain + pacified
UPDATE `creature_template` SET `ScriptName`='', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=44171; -- Training Dummy
UPDATE `creature_template` SET `ScriptName`='', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=44389; -- Training Dummy
UPDATE `creature_template` SET `ScriptName`='', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=44548; -- Training Dummy
UPDATE `creature_template` SET `ScriptName`='', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=44614; -- Training Dummy
UPDATE `creature_template` SET `ScriptName`='', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=44703; -- Training Dummy
UPDATE `creature_template` SET `ScriptName`='', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=44794; -- Training Dummy
UPDATE `creature_template` SET `ScriptName`='', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=44820; -- Training Dummy
UPDATE `creature_template` SET `ScriptName`='', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=44848; -- Training Dummy
UPDATE `creature_template` SET `ScriptName`='', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=44937; -- Training Dummy
UPDATE `creature_template` SET `ScriptName`='', `flags_extra`=262144, `unit_flags`=131072 WHERE `entry`=48304; -- Training Dummy

-- curhealth=0 + RegenHealth forces max HP on spawn/reload
UPDATE `creature` SET `curhealth`=0 WHERE `id` IN (
17578,24792,30527,31143,31144,31146,32541,32542,32543,32545,32546,32666,32667,
44171,44389,44548,44614,44703,44794,44820,44848,44937,46647,48304,60197,64446,67127
);
