-- Training dummies: bind immortal AI, usable HP, no weapon-skill farming.
UPDATE `creature_template`
SET
    `ScriptName` = 'npc_training_dummy',
    `flags_extra` = (`flags_extra` | 0x00040000),
    `unit_flags` = (`unit_flags` | 0x00020000)
WHERE `name` LIKE '%Training Dummy%'
  AND `name` NOT LIKE '%Bunny%';

-- Tiny Health_mod values truncate to ~1 HP; give practice dummies usable pools.
UPDATE `creature_template` SET `Health_mod` = 100 WHERE `entry` IN
(31143, 31144, 32541, 32542, 32543, 32545, 32546, 32666, 32667, 46647, 60197, 64446, 67127);

-- curhealth=0 + RegenHealth forces max HP on spawn/reload.
UPDATE `creature` c
INNER JOIN `creature_template` ct ON c.`id` = ct.`entry`
SET c.`curhealth` = 0
WHERE ct.`name` LIKE '%Training Dummy%'
  AND ct.`name` NOT LIKE '%Bunny%';
