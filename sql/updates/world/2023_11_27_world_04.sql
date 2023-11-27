UPDATE `creature_template` SET `unit_flags`=0x100|0x200|0x8000 WHERE `entry`=55595;
DELETE FROM `creature_queststarter` WHERE `id`=55583 AND `quest`=29784;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(55583, 29784);
