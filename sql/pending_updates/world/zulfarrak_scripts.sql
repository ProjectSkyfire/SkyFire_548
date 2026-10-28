-- Zul'Farrak: bind Witch Doctor Zum'rah and add his texts (instance script and pyramid event need no data)

-- Witch Doctor Zum'rah (7271): bind boss_zum_rah as in the 4.3.4 data.
UPDATE `creature_template` SET `ScriptName` = 'boss_zum_rah' WHERE `entry` = 7271 AND `ScriptName` = '';

-- Zum'rah texts (SAY_SANCT_INVADE = group 0, SAY_WARD = group 1, SAY_KILL = group 2), from 4.3.4.
DELETE FROM `creature_text` WHERE `entry` = 7271;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(7271, 0, 0, 'How dare you enter my sanctum!', 12, 0, 100, 0, 0, 0, 'Witch Doctor Zum''rah'),
(7271, 1, 1, 'Sands consume you!', 14, 14, 100, 0, 0, 5872, 'Witch Doctor Zum''rah'),
(7271, 2, 2, 'Sands consume you!', 14, 14, 100, 0, 0, 5872, 'Witch Doctor Zum''rah'),
(7271, 2, 3, 'Fall!', 14, 14, 100, 0, 0, 5873, 'Witch Doctor Zum''rah');
