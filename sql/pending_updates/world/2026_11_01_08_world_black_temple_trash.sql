-- Black Temple trash and the Spirit of Olum: SmartAI from TrinityCore 3.3.5 (the raid's only mode, 25-player normal, is
-- difficulty bit 0x10 here), and the Angered Soul Fragments' script binding.
UPDATE `creature_template` SET `AIName` = 'SmartAI', `ScriptName` = '' WHERE `entry` IN (22844, 23049, 23172, 23196, 23222, 23232, 23237, 23239, 23411);
UPDATE `creature_template` SET `ScriptName` = 'npc_angered_soul_fragment' WHERE `entry` = 23398;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (22844, 23049, 23172, 23196, 23222, 23232, 23237, 23239, 23411);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(23049, 0, 0, 1, 1, 0, 100, 33, 1000, 1000, 0, 0, 0, 11, 41099, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadowmoon Weapon Master - Out of Combat - Cast \'Battle Stance\' (No Repeat) (Normal Dungeon)'),
(23049, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 41106, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadowmoon Weapon Master - Out of Combat - Cast \'Battle Aura\' (No Repeat) (Normal Dungeon)'),
(23049, 0, 2, 3, 2, 0, 100, 33, 0, 70, 0, 0, 0, 11, 41101, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadowmoon Weapon Master - Between 0-70% Health - Cast \'Defensive Stance\' (No Repeat) (Normal Dungeon)'),
(23049, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 41105, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadowmoon Weapon Master - Between 0-70% Health - Cast \'Defensive Aura\' (No Repeat) (Normal Dungeon)'),
(23049, 0, 4, 5, 2, 0, 100, 33, 0, 40, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadowmoon Weapon Master - Between 0-40% Health - Say Line 0 (No Repeat) (Normal Dungeon)'),
(23049, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 41100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadowmoon Weapon Master - Between 0-40% Health - Cast \'Berserker Stance\' (No Repeat) (Normal Dungeon)'),
(23049, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 41107, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadowmoon Weapon Master - Between 0-40% Health - Cast \'Berserker Aura\' (No Repeat) (Normal Dungeon)'),
(23049, 0, 7, 0, 2, 0, 100, 32, 0, 30, 25000, 30000, 0, 11, 41097, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Shadowmoon Weapon Master - Between 0-30% Health - Cast \'Whirlwind\' (No Repeat) (Normal Dungeon)'),
(23049, 0, 8, 0, 0, 0, 100, 32, 3000, 6000, 8000, 11000, 0, 11, 41103, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shadowmoon Weapon Master - In Combat - Cast \'Mutilate\' (No Repeat) (Normal Dungeon)'),
(23049, 0, 9, 0, 0, 0, 100, 32, 6000, 8000, 12000, 16000, 0, 11, 18813, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Shadowmoon Weapon Master - In Combat - Cast \'Knock Away\' (No Repeat) (Normal Dungeon)'),
(22844, 0, 0, 0, 0, 0, 85, 32, 19000, 19000, 18000, 18000, 0, 11, 15284, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ashtongue Battlelord - In Combat - Cast \'Cleave\' (Phase 1) (No Repeat)'),
(22844, 0, 1, 0, 0, 0, 90, 32, 6000, 6000, 20000, 20000, 0, 11, 32588, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Ashtongue Battlelord - In Combat - Cast \'Concussion Blow\' (Phase 1) (No Repeat)'),
(22844, 0, 2, 0, 0, 0, 85, 32, 15000, 15000, 22000, 22000, 0, 11, 41182, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Ashtongue Battlelord - In Combat - Cast \'Concussive Throw\' (Phase 1) (No Repeat)'),
(22844, 0, 3, 4, 2, 0, 100, 33, 0, 30, 0, 0, 0, 11, 34970, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ashtongue Battlelord - Between 0-30% Health - Cast \'Frenzy\' (No Repeat) (Normal Dungeon)'),
(22844, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ashtongue Battlelord - Between 0-30% Health - Say Line 0 (No Repeat) (Normal Dungeon)'),
(23172, 0, 0, 0, 0, 0, 100, 32, 5000, 16000, 45000, 60000, 0, 11, 38166, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hand of Gorefiend - In Combat - Cast \'Enrage\''),
(23196, 0, 0, 0, 0, 0, 100, 32, 1000, 1000, 25000, 45000, 0, 11, 41277, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Bonechewer Behemoth - In Combat - Cast \'Fiery Comet\''),
(23196, 0, 1, 0, 0, 0, 100, 32, 15000, 15000, 30000, 40000, 0, 11, 41274, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 'Bonechewer Behemoth - In Combat - Cast \'Fel Stomp\''),
(23196, 0, 2, 3, 2, 0, 100, 33, 0, 30, 0, 0, 0, 11, 40743, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bonechewer Behemoth - Between 0-30% Health - Cast \'Frenzy\' (No Repeat) (Normal Dungeon)'),
(23196, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bonechewer Behemoth - Between 0-30% Health - Say Line 0 (No Repeat) (Normal Dungeon)'),
(23196, 0, 4, 0, 4, 0, 100, 32, 0, 0, 0, 0, 0, 11, 41272, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Bonechewer Behemoth - On Aggro - Cast \'Behemoth Charge\' (No Repeat) (Normal Dungeon)'),
(23222, 0, 0, 1, 0, 0, 100, 32, 10000, 10000, 35000, 41500, 0, 11, 41254, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bonechewer Brawler - In Combat - Cast \'Enrage\' (Normal Dungeon)'),
(23222, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bonechewer Brawler - In Combat - Say Line 0 (Normal Dungeon)'),
(23232, 0, 0, 0, 25, 0, 100, 0, 0, 0, 0, 0, 0, 11, 41290, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mutant War Hound - On Reset - Cast Disease Cloud'),
(23232, 0, 1, 0, 6, 0, 100, 0, 0, 0, 0, 0, 0, 11, 41193, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Mutant War Hound - On Death - Cast Cloud of Disease'),
(23237, 0, 0, 0, 4, 0, 100, 32, 0, 0, 0, 0, 0, 11, 41230, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bonechewer Blood Prophet - On Aggro - Cast \'Prophecy of Blood\' (Normal Dungeon)'),
(23237, 0, 1, 0, 0, 0, 100, 32, 7500, 7500, 15000, 22500, 0, 11, 41229, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Bonechewer Blood Prophet - In Combat - Cast \'Bloodbolt\' (Normal Dungeon)'),
(23237, 0, 2, 0, 2, 0, 100, 33, 0, 50, 0, 0, 0, 11, 41238, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Bonechewer Blood Prophet - Between 0-50% Health - Cast \'Blood Drain\' (Normal Dungeon)'),
(23237, 0, 3, 4, 2, 0, 100, 33, 0, 30, 0, 0, 0, 11, 8269, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bonechewer Blood Prophet - Between 0-30% Health - Cast \'Frenzy\' (No Repeat) (Normal Dungeon)'),
(23237, 0, 4, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bonechewer Blood Prophet - Between 0-30% Health - Say Line 0 (No Repeat) (Normal Dungeon)'),
(23239, 0, 0, 1, 2, 0, 100, 33, 0, 30, 0, 0, 0, 11, 8269, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bonechewer Combatant - Between 0-30% Health - Cast \'Frenzy\' (No Repeat) (Normal Dungeon)'),
(23239, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Bonechewer Combatant - Between 0-30% Health - Say Line 0 (No Repeat) (Normal Dungeon)'),
(23411, 0, 0, 1, 62, 0, 100, 0, 8750, 1, 0, 0, 0, 85, 41566, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Olum - On gossip select - cast spell'),
(23411, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Olum - On gossip select - Close gossip'),
(23411, 0, 2, 3, 62, 0, 100, 0, 8750, 2, 0, 0, 0, 85, 41570, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Olum - On gossip select - cast spell'),
(23411, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Spirit of Olum - On gossip select - Close gossip');

DELETE FROM `creature_text` WHERE `entry` IN (22844, 23049, 23196, 23222, 23237, 23239);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(22844, 0, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Ashtongue Battlelord'),
(23049, 0, 0, 'Berserker Stance! Attack them recklessly!', 12, 0, 100, 0, 0, 0, 'Shadowmoon Weapon Master'),
(23196, 0, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Bonechewer Behemoth'),
(23222, 0, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Bonechewer Brawler'),
(23237, 0, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Bonechewer Blood Prophet'),
(23239, 0, 0, '%s becomes enraged!', 16, 0, 100, 0, 0, 0, 'Bonechewer Combatant');
