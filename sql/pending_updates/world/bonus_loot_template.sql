-- Bonus rolls of 5.4.8 raid and world bosses: a player holding the boss's currency gets the roll
-- prompt once per boss and difficulty a week. Names and columns follow alexkulya's 5.4.8 table.
-- entry: base creature entry of the boss. spell: the prompt aura (aura 394), whose triggered spell
-- does the roll. currency: spent by the roll (Elder Charm 697, Mogu Rune of Fate 752, Warforged Seal 776).
-- loot_id_normal / loot_id_heroic: the creature or gameobject loot the roll draws from on normal and
-- heroic; world bosses, raid finder and flex draw from the boss's personal loot list instead.
-- loot_source: LOOT_SOURCE_GO for bosses whose loot sits in a chest; the prompt comes when the chest opens.
CREATE TABLE IF NOT EXISTS `bonus_loot_template` (
  `entry` int(10) unsigned NOT NULL,
  `spell` int(10) unsigned NOT NULL,
  `currency` smallint(5) unsigned NOT NULL,
  `loot_id_normal` int(10) unsigned NOT NULL,
  `loot_id_heroic` int(10) unsigned NOT NULL,
  `loot_source` enum('LOOT_SOURCE_CREATURE','LOOT_SOURCE_GO') NOT NULL,
  `comment` text,
  PRIMARY KEY (`entry`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8;

-- Rows from the alexkulya/pandaria_5.4.8 world DB (2026_08_16 release).
-- Rerunnable: the rows of these bosses are deleted by entry and rewritten.
-- 37 sources (31 creatures, 6 chests).

DELETE FROM `bonus_loot_template` WHERE `entry` IN (59915,60009,60143,60400,60410,60491,60583,60585,60709,60999,62164,62346,62397,62442,62511,62543,62837,62980,62983,69161,71454,71466,71475,71504,71515,71529,71543,71734,71859,71865,71889,71952,71953,71954,71955,72057,72249);
INSERT INTO `bonus_loot_template` (`entry`,`spell`,`currency`,`loot_id_normal`,`loot_id_heroic`,`loot_source`,`comment`) VALUES
(59915,125144,697,59915,59915,'LOOT_SOURCE_CREATURE','Stone Guard'),
(60009,132189,697,60009,60009,'LOOT_SOURCE_CREATURE','Feng the Accursed'),
(60143,132190,697,60143,60143,'LOOT_SOURCE_CREATURE','Gara''jal the Spiritbinder'),
(60400,132193,697,60400,60400,'LOOT_SOURCE_CREATURE','Will of the Emperor'),
(60410,132192,697,43504,44053,'LOOT_SOURCE_GO','Elegon'),
(60491,132205,697,0,0,'LOOT_SOURCE_CREATURE','Sha of Anger'),
(60583,132204,697,60583,60583,'LOOT_SOURCE_CREATURE','Protectors of the Endless (Elite) Bonus'),
(60585,132200,697,60585,60585,'LOOT_SOURCE_CREATURE','Protectors of the Endless Bonus'),
(60709,132191,697,60709,60709,'LOOT_SOURCE_CREATURE','Spirit Kings'),
(60999,132203,697,60999,60999,'LOOT_SOURCE_CREATURE','Sha of Fear'),
(62164,132196,697,62164,62164,'LOOT_SOURCE_CREATURE','Garalon'),
(62346,132206,697,0,0,'LOOT_SOURCE_CREATURE','Galleon'),
(62397,132197,697,62397,62397,'LOOT_SOURCE_CREATURE','Wind Lord Mel''jarak'),
(62442,132201,697,43579,44088,'LOOT_SOURCE_GO','Tsulong'),
(62511,132198,697,62511,62511,'LOOT_SOURCE_CREATURE','Amber-Shaper Un''sok'),
(62543,132195,697,62543,62543,'LOOT_SOURCE_CREATURE','Blade Lord Tay''ak'),
(62837,132199,697,62837,62837,'LOOT_SOURCE_CREATURE','Grand Empress Shek''zeer'),
(62980,132194,697,62980,62980,'LOOT_SOURCE_CREATURE','Imperial Vizier Zor''lok'),
(62983,132202,697,43584,44089,'LOOT_SOURCE_GO','Lei Shi'),
(69161,137554,752,0,0,'LOOT_SOURCE_CREATURE','Oondasta'),
(71454,145917,776,71454,71454,'LOOT_SOURCE_CREATURE','Malkorok'),
(71466,145914,776,71466,71466,'LOOT_SOURCE_CREATURE','Iron Juggernaut'),
(71475,145910,776,71475,71475,'LOOT_SOURCE_CREATURE','The Fallen Protectors'),
(71504,145918,776,71504,71504,'LOOT_SOURCE_CREATURE','Siegecrafter Blackfuse'),
(71515,145916,776,71515,71515,'LOOT_SOURCE_CREATURE','General Nazgrim'),
(71529,145920,776,71529,71529,'LOOT_SOURCE_CREATURE','Thok the Bloodthirsty'),
(71543,145909,776,232092,54233,'LOOT_SOURCE_GO','Immerseus'),
(71734,145912,776,232164,54236,'LOOT_SOURCE_GO','Sha of Pride'),
(71859,145915,776,71859,71859,'LOOT_SOURCE_CREATURE','Dark Shaman'),
(71865,145922,776,71865,71865,'LOOT_SOURCE_CREATURE','Garrosh Hellscream'),
(71889,145919,776,232166,54244,'LOOT_SOURCE_GO','Spoils of Pandaria'),
(71952,148317,776,0,0,'LOOT_SOURCE_CREATURE','Chi-Ji'),
(71953,148317,776,0,0,'LOOT_SOURCE_CREATURE','Xuen'),
(71954,148317,776,0,0,'LOOT_SOURCE_CREATURE','Niuzao'),
(71955,148317,776,0,0,'LOOT_SOURCE_CREATURE','Yu''lon'),
(72057,148316,776,0,0,'LOOT_SOURCE_CREATURE','Ordos'),
(72249,145913,776,72249,72249,'LOOT_SOURCE_CREATURE','Galakras');
