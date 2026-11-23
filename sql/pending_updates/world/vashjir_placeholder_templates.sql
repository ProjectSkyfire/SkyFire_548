-- Vashj'ir: creatures that the Vashj'ir spawn update brings in whose template is still a placeholder at level 1 or at the
-- friendly faction 35, so they would stand level 1 and friendly (Kvaldir, Zin'jatar Raiders, Azsh'ir Assailants, the
-- Hellscream's Vanguard, the sea monster tentacles). Level and faction from the 5.4.8 data; melee damage and health follow
-- the level curve.
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `exp` = 3, `faction_A` = 7, `faction_H` = 7 WHERE `entry` = 50051; -- Ghostcrawler
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90, `exp` = 4, `faction_A` = 12, `faction_H` = 12 WHERE `entry` = 43295; -- Salty McTavish
UPDATE `creature_template` SET `minlevel` = 79, `maxlevel` = 84, `exp` = 3, `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 42761; -- Sinister Reflection
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 3, `faction_A` = 14, `faction_H` = 14 WHERE `entry` IN (40237, 40370, 41781, 41805); -- Frenzied Reef Shark, Spiketooth Eel, Zin'jatar Raider
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 0, `faction_A` = 14, `faction_H` = 14 WHERE `entry` IN (42992, 42993, 42994, 42995, 42996, 42997, 42998); -- Sea Monster Tentacle
UPDATE `creature_template` SET `minlevel` = 81, `maxlevel` = 81, `exp` = 0, `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 46392; -- Shimmering Bubble
UPDATE `creature_template` SET `minlevel` = 81, `maxlevel` = 81, `exp` = 3, `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 50005; -- Poseidus
UPDATE `creature_template` SET `minlevel` = 82, `maxlevel` = 82, `exp` = 3, `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 50050; -- Shok'sharak
UPDATE `creature_template` SET `minlevel` = 82, `maxlevel` = 83, `exp` = 0, `faction_A` = 14, `faction_H` = 14 WHERE `entry` = 44373; -- Whirlpool Vehicle
UPDATE `creature_template` SET `minlevel` = 81, `maxlevel` = 81, `exp` = 3, `faction_A` = 16, `faction_H` = 16 WHERE `entry` = 49913; -- Lady La-La
UPDATE `creature_template` SET `minlevel` = 82, `maxlevel` = 82, `exp` = 0, `faction_A` = 16, `faction_H` = 16 WHERE `entry` = 42639; -- Sand Storm
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 3, `faction_A` = 29, `faction_H` = 29 WHERE `entry` IN (42002, 43177); -- Drowning Warrior, Wolf
UPDATE `creature_template` SET `minlevel` = 60, `maxlevel` = 60, `exp` = 0, `faction_A` = 35, `faction_H` = 35 WHERE `entry` IN (40371, 40605, 42099, 42990, 42991); -- Egg-splosive Bunny (Horde), Naga Death Bunny, Tentacle Pacer Bunny (H), Tentacle Summon Bunny (Horde), Turtle Death Bunny
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 0, `faction_A` = 35, `faction_H` = 35 WHERE `entry` IN (41745, 41765, 42678); -- Anchor, Kvaldir Death Bunny, Naga Corpse
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 3, `faction_A` = 35, `faction_H` = 35 WHERE `entry` IN (41794, 43030); -- Captain Samir, Erunak Stonespeaker
UPDATE `creature_template` SET `minlevel` = 81, `maxlevel` = 81, `exp` = 0, `faction_A` = 35, `faction_H` = 35 WHERE `entry` = 44536; -- Camera Bunny Vehicle - HORDE
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 3, `faction_A` = 74, `faction_H` = 74 WHERE `entry` IN (40878, 41764, 41808, 43180); -- Azsh'ir Sentry, Zin'jatar Raider
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 81, `exp` = 3, `faction_A` = 74, `faction_H` = 74 WHERE `entry` = 41579; -- Idra'kess Ambusher
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `exp` = 3, `faction_A` = 85, `faction_H` = 85 WHERE `entry` = 51775; -- Cavern Grunt
UPDATE `creature_template` SET `minlevel` = 70, `maxlevel` = 70, `exp` = 0, `faction_A` = 190, `faction_H` = 190 WHERE `entry` = 44535; -- Cloned Image
UPDATE `creature_template` SET `minlevel` = 81, `maxlevel` = 81, `exp` = 3, `faction_A` = 190, `faction_H` = 190 WHERE `entry` = 47397; -- Nespirah Fluid
UPDATE `creature_template` SET `minlevel` = 82, `maxlevel` = 82, `exp` = 3, `faction_A` = 1733, `faction_H` = 1733 WHERE `entry` IN (42411, 44585); -- Captain McFerris, Captain Taylor
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 3, `faction_A` = 1735, `faction_H` = 1735 WHERE `entry` = 41749; -- Hellscream's Vanguard
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 0, `faction_A` = 1735, `faction_H` = 1735 WHERE `entry` = 41862; -- Gurrok
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `exp` = 3, `faction_A` = 1801, `faction_H` = 1801 WHERE `entry` = 51475; -- Hellscream's Vanguard
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 3, `faction_A` = 1802, `faction_H` = 1802 WHERE `entry` IN (40710, 41598); -- Anemone Frenzy, Captain Taylor
UPDATE `creature_template` SET `minlevel` = 81, `maxlevel` = 81, `exp` = 3, `faction_A` = 1802, `faction_H` = 1802 WHERE `entry` = 48423; -- Captain Glovaal
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `exp` = 3, `faction_A` = 1802, `faction_H` = 1802 WHERE `entry` = 41340; -- Private Pollard
UPDATE `creature_template` SET `minlevel` = 85, `maxlevel` = 85, `exp` = 3, `faction_A` = 1802, `faction_H` = 1802 WHERE `entry` = 51478; -- Cavern Crawler
UPDATE `creature_template` SET `minlevel` = 90, `maxlevel` = 90, `exp` = 4, `faction_A` = 2090, `faction_H` = 2090 WHERE `entry` = 43289; -- Swift Seahorse
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 3, `faction_A` = 2111, `faction_H` = 2111 WHERE `entry` IN (40239, 41760); -- Azsh'ir Scout, Enormous Sand Crab
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 0, `faction_A` = 2205, `faction_H` = 2205 WHERE `entry` = 41809; -- Zin'jatar Abductor
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 3, `faction_A` = 2211, `faction_H` = 2211 WHERE `entry` = 41777; -- Erunak Stonespeaker
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 3, `faction_A` = 2242, `faction_H` = 2242 WHERE `entry` IN (39397, 40162, 40372); -- Zin'jatar Fathom-Stalker
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 3, `faction_A` = 2257, `faction_H` = 2257 WHERE `entry` = 40238; -- Agitated Green Sand Crab
UPDATE `creature_template` SET `minlevel` = 81, `maxlevel` = 81, `exp` = 3, `faction_A` = 2263, `faction_H` = 2263 WHERE `entry` IN (44421, 44422, 44423); -- Idra'kess Harpooner, Sira'kess Tide Priestess, Zin'jatar Fathom-Stalker
UPDATE `creature_template` SET `minlevel` = 80, `maxlevel` = 80, `exp` = 3, `faction_A` = 2268, `faction_H` = 2268 WHERE `entry` IN (41771, 41772, 41773, 41774, 41775, 41796, 41797, 41798, 41799, 41800); -- Hellscream's Vanguard
UPDATE `creature_template` SET `minlevel` = 83, `maxlevel` = 83, `exp` = 3, `faction_A` = 2268, `faction_H` = 2268 WHERE `entry` = 43179; -- Drowning Warrior
UPDATE `creature_template` SET `minlevel` = 81, `maxlevel` = 81, `exp` = 3, `faction_A` = 2276, `faction_H` = 2276 WHERE `entry` = 43843; -- Azsh'ir Assailant
UPDATE `creature_template` SET `minlevel` = 81, `maxlevel` = 81, `exp` = 3, `faction_A` = 2316, `faction_H` = 2316 WHERE `entry` = 51071; -- Captain Florence
