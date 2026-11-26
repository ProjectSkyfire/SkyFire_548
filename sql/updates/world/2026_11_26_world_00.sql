-- Shore recovery points, NOT ship stop positions (which are over water).
-- stop_node is TaxiPathNode.index within the transport's DBC path.
-- 4294967295 is an explicit shore recovery point for a route with NO stop frames.
CREATE TABLE IF NOT EXISTS `transport_arrival_dock` (
  `entry` int unsigned NOT NULL,
  `stop_node` int unsigned NOT NULL,
  `name` varchar(100) NOT NULL DEFAULT '' COMMENT 'Readable arrival destination',
  `map` int unsigned NOT NULL,
  `position_x` float NOT NULL,
  `position_y` float NOT NULL,
  `position_z` float NOT NULL,
  `orientation` float NOT NULL,
  PRIMARY KEY (`entry`, `stop_node`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- Covers every continent transport currently listed in transports (27 entries).
-- Ordinary destinations use existing game_tele shore/hub coordinates.
-- Moa'ki and Sandy Beach use land-based TaxiNodes 294 and 606.
-- Keel Harbor uses ground NPC Lorna Crowley (43727) on phased map 654.
-- Scripted battle/patrol stops use named shore recovery hubs; these are not docks.
INSERT INTO `transport_arrival_dock`
  (`entry`, `stop_node`, `name`, `map`, `position_x`, `position_y`, `position_z`, `orientation`)
VALUES
-- Ship (The Maiden's Fancy), DBC path 241.
  (20808, 6, 'Ratchet', 1, -956.664, -3754.71, 5.33239, 0.996637),
  (20808, 16, 'Booty Bay', 0, -14297.2, 530.993, 8.77916, 3.98863),
-- Zeppelin (The Thundercaller), DBC path 302.
  (164871, 4, 'Orgrimmar', 1, 1569.97, -4397.41, 16.0472, 0.543025),
  (164871, 19, 'Brill', 0, 2259.25, 290.43, 34.1137, 0.987414),
-- Zeppelin (The Iron Eagle), DBC path 285.
  (175080, 3, 'Grom''gol Base Camp', 0, -12388.9, 172.578, 2.83358, 1.91753),
  (175080, 12, 'Orgrimmar', 1, 1569.97, -4397.41, 16.0472, 0.543025),
-- Ship (The Lady Mehley), DBC path 292.
  (176231, 5, 'Menethil Harbor', 0, -3769.32, -744.26, 8.01027, 1.95752),
  (176231, 14, 'Theramore', 1, -3641.3, -4358.93, 8.35467, 3.81559),
-- Ship (The Bravery), DBC path 967.
  (176310, 5, 'Stormwind Harbor', 0, -8419.34, 1212.14, 5.17277, 1.55232),
  (176310, 16, 'Rut''theran Village', 1, 8704.53, 944.683, 13.5177, 2.99385),
-- Zeppelin (The Purple Princess), DBC path 301.
  (176495, 3, 'Grom''gol Base Camp', 0, -12388.9, 172.578, 2.83358, 1.91753),
  (176495, 19, 'Brill', 0, 2259.25, 290.43, 34.1137, 0.987414),
-- Ship, Night Elf (Feathermoon Ferry), DBC path 777.
  (177233, 3, 'The Forgotten Coast', 1, -4347.46, 2415.11, 8.00515, 1.52603),
  (177233, 13, 'Feathermoon Stronghold', 1, -4317.47, 3287.35, 18.2864, 3.12825),
-- Ship, Night Elf (Elune's Blessing), DBC path 503.
  (181646, 6, 'Azuremyst Isle', 530, -4216.87, -12336.9, 4.34247, 6.02008),
  (181646, 17, 'Rut''theran Village', 1, 8704.53, 944.683, 13.5177, 2.99385),
-- Ship, Icebreaker (Northspear), DBC path 964.
  (181688, 5, 'Menethil Harbor', 0, -3769.32, -744.26, 8.01027, 1.95752),
  (181688, 36, 'Valgarde', 571, 564.401, -4944.94, 18.5962, 5.36544),
-- Zeppelin, Horde (Cloudkisser), DBC path 737.
  (181689, 3, 'Vengeance Landing', 571, 1942.86, -6167.11, 23.724, 2.64258),
  (181689, 10, 'Brill', 0, 2259.25, 290.43, 34.1137, 0.987414),
-- Zeppelin, Horde (The Mighty Wind), DBC path 712.
  (186238, 4, 'Warsong Hold', 571, 2741.29, 6097.16, 76.9055, 0.731543),
  (186238, 20, 'Orgrimmar', 1, 1569.97, -4397.41, 16.0472, 0.543025),
-- Zeppelin, DBC path 727.
  (186371, 1, 'Westguard Keep (zeppelin recovery)', 571, 1391.04, -3284.63, 163.929, 1.59391),
-- Sister Mercy, DBC path 778.
  (187038, 1, 'Valgarde (Sister Mercy recovery)', 571, 564.401, -4944.94, 18.5962, 5.36544),
  (187038, 2, 'Valgarde (Sister Mercy recovery)', 571, 564.401, -4944.94, 18.5962, 5.36544),
  (187038, 3, 'Valgarde (Sister Mercy recovery)', 571, 564.401, -4944.94, 18.5962, 5.36544),
  (187038, 4, 'Valgarde (Sister Mercy recovery)', 571, 564.401, -4944.94, 18.5962, 5.36544),
  (187038, 5, 'Valgarde (Sister Mercy recovery)', 571, 564.401, -4944.94, 18.5962, 5.36544),
  (187038, 10, 'Valgarde (Sister Mercy recovery)', 571, 564.401, -4944.94, 18.5962, 5.36544),
-- Turtle (Walker of Waves), DBC path 799.
  (187568, 4, 'Moa''ki Harbor', 571, 2792.45, 908.96, 22.33, 0),
  (187568, 18, 'Unu''pe', 571, 2925.02, 4065.63, 1.46737, 3.56739),
-- Turtle (Green Island), DBC path 1079.
  (188511, 4, 'Moa''ki Harbor', 571, 2792.45, 908.96, 22.33, 0),
  (188511, 24, 'Kamagua', 571, 774.043, -2940.65, 7.36477, 1.41719),
-- Ship, Icebreaker (Stormwind's Pride), DBC path 965.
  (190536, 4, 'Stormwind Harbor', 0, -8419.34, 1212.14, 5.17277, 1.55232),
  (190536, 16, 'Valiance Keep', 571, 2213.95, 5273.15, 11.2565, 5.89294),
-- The Zephyr, DBC path 1221.
  (190549, 4, 'Thunder Bluff', 1, -1277.37, 124.804, 131.287, 5.22274),
  (190549, 27, 'Orgrimmar', 1, 1569.97, -4397.41, 16.0472, 0.543025),
-- Orgrim's Hammer, DBC path 1095.
  (192241, 1, 'Argent Tournament Grounds (gunship recovery)', 571, 8515.89, 629.25, 547.396, 1.5747),
  (192241, 4, 'Argent Tournament Grounds (gunship recovery)', 571, 8515.89, 629.25, 547.396, 1.5747),
  (192241, 7, 'Argent Tournament Grounds (gunship recovery)', 571, 8515.89, 629.25, 547.396, 1.5747),
  (192241, 14, 'Argent Tournament Grounds (gunship recovery)', 571, 8515.89, 629.25, 547.396, 1.5747),
-- The Skybreaker, DBC path 1094.
  (192242, 5, 'Argent Tournament Grounds (gunship recovery)', 571, 8515.89, 629.25, 547.396, 1.5747),
  (192242, 8, 'Argent Tournament Grounds (gunship recovery)', 571, 8515.89, 629.25, 547.396, 1.5747),
  (192242, 11, 'Argent Tournament Grounds (gunship recovery)', 571, 8515.89, 629.25, 547.396, 1.5747),
-- Ship to Vashj'ir, DBC path 1368.
  (197195, 4, 'Stormwind Harbor', 0, -8419.34, 1212.14, 5.17277, 1.55232),
  (197195, 11, 'Sandy Beach, Vashj''ir (recovery)', 0, -5310.85, 3914.42, 3.22433, 0),
-- Orc Gunship, DBC path 2338.
  (203428, 1, 'Keel Harbor (gunship recovery)', 654, -1299.28, 2129.4, 5.62959, 3.31613),
  (203428, 2, 'Keel Harbor (gunship recovery)', 654, -1299.28, 2129.4, 5.62959, 3.31613),
-- Ship to Vashj'ir, DBC path 2204.
  (203466, 1, 'Sandy Beach, Vashj''ir (recovery)', 0, -5310.85, 3914.42, 3.22433, 0),
  (203466, 4, 'Sandy Beach, Vashj''ir (recovery)', 0, -5310.85, 3914.42, 3.22433, 0),
  (203466, 7, 'Orgrimmar', 1, 1569.97, -4397.41, 16.0472, 0.543025),
  (203466, 12, 'Orgrimmar', 1, 1569.97, -4397.41, 16.0472, 0.543025),
-- The Spear of Durotar, DBC path 2265.
  (203626, 1, 'Orgrimmar', 1, 1569.97, -4397.41, 16.0472, 0.543025),
  (203626, 15, 'Orgrimmar', 1, 1569.97, -4397.41, 16.0472, 0.543025),
  (203626, 27, 'Dragonmaw Port', 0, -4024.3, -6380.98, 37.2517, 6.24448),
  (203626, 119, 'Dragonmaw Port', 0, -4024.3, -6380.98, 37.2517, 6.24448),
-- Alliance Gunship, DBC path 2222.
  (204018, 4294967295, 'Deepholm', 646, 915.004, 503.842, -49.23, 0.196921),
-- Krazzworks to Dragonmaw Port, DBC path 2477.
  (206328, 2, 'The Krazzworks', 0, -2748.68, -6359.73, 199.013, 3.34637),
  (206328, 8, 'Dragonmaw Port', 0, -4024.3, -6380.98, 37.2517, 6.24448),
-- Dragonmaw Port to Krazzworks, DBC path 2480.
  (206329, 2, 'Dragonmaw Port', 0, -4024.3, -6380.98, 37.2517, 6.24448),
  (206329, 13, 'The Krazzworks', 0, -2748.68, -6359.73, 199.013, 3.34637),
-- Krazzworks Attack Zeppelin, DBC path 2600.
  (207227, 4294967295, 'The Krazzworks', 0, -2748.68, -6359.73, 199.013, 3.34637)
ON DUPLICATE KEY UPDATE `name`=VALUES(`name`), `map`=VALUES(`map`), `position_x`=VALUES(`position_x`),
  `position_y`=VALUES(`position_y`), `position_z`=VALUES(`position_z`), `orientation`=VALUES(`orientation`);
