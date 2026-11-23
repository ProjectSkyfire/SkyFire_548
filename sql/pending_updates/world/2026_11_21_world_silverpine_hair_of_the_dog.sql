-- Silverpine Forest: "Hair of the Dog" (27226). The Orc Sea Dogs lie mortally wounded until a Barrel of Explosive Ale
-- (spell 84517) is thrown at them; they then credit the player, shout, and run around for a minute. They had no script
-- and no text, so the quest could not be completed. Based on Legends of Azeroth 5.4.8.
UPDATE `creature_template` SET `AIName`='SmartAI' WHERE `entry`=45196;
DELETE FROM `smart_scripts` WHERE `entryorguid`=45196 AND `source_type`=0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(45196, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 75, 84511, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orc Sea Dog - On Respawn - Add Aura Mortally Wounded'),
(45196, 0, 1, 2, 8, 0, 100, 0, 84517, 0, 60000, 60000, 0, 33, 45196, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Orc Sea Dog - On SpellHit Barrel of Explosive Ale - Quest Credit'),
(45196, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 28, 84511, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orc Sea Dog - Linked To Id 1 - Remove Aura Mortally Wounded'),
(45196, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orc Sea Dog - Linked To Id 2 - Say Line Random'),
(45196, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 8, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orc Sea Dog - Linked To Id 3 - Set React Aggressive'),
(45196, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 83, 512, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orc Sea Dog - Linked To Id 4 - Remove NpcFlag 512'),
(45196, 0, 6, 7, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 84528, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orc Sea Dog - Linked To Id 5 - Cast Ale Frenzy'),
(45196, 0, 7, 8, 61, 0, 100, 0, 0, 0, 0, 0, 0, 59, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orc Sea Dog - Linked To Id 6 - Set Run'),
(45196, 0, 8, 9, 61, 0, 100, 0, 0, 0, 0, 0, 0, 89, 10, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orc Sea Dog - Linked To Id 7 - Random Move'),
(45196, 0, 9, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 41, 60000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Orc Sea Dog - Linked To Id 8 - Despawn after 60 sec');

DELETE FROM `creature_text` WHERE `entry`=45196;
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(45196, 0, 0, 'AAAAAAAAAALLLLLLLLLLEEEEEEEEE!!! ARRRRRRRRGGGHHHH!', 12, 0, 100, 0, 0, 0, 'Orc Sea Dog'),
(45196, 0, 1, 'Crush and clobber! Bobber slobber!', 12, 0, 100, 0, 0, 0, 'Orc Sea Dog'),
(45196, 0, 2, 'HELL OF A KICK TO THAT ALE!', 12, 0, 100, 0, 0, 0, 'Orc Sea Dog'),
(45196, 0, 3, 'My power has returned!', 12, 0, 100, 0, 0, 0, 'Orc Sea Dog'),
(45196, 0, 4, 'ORC NEEDS PEANUTS!', 12, 0, 100, 0, 0, 0, 'Orc Sea Dog');
