
DROP TABLE IF EXISTS `locales_npc_broadcast_text_unused`;
CREATE TABLE IF NOT EXISTS `locales_npc_broadcast_text_unused` (
  `entry` int NOT NULL DEFAULT '0',
  `text_0_loc1` text,
  `text_0_loc2` text,
  `text_0_loc3` text,
  `text_0_loc4` text,
  `text_0_loc5` text,
  `text_0_loc6` text,
  `text_0_loc7` text,
  `text_0_loc8` text,
  `text_0_loc9` text,
  `text_0_loc10` text,
  `text_0_loc11` text,
  `text_1_loc1` text,
  `text_1_loc2` text,
  `text_1_loc3` text,
  `text_1_loc4` text,
  `text_1_loc5` text,
  `text_1_loc6` text,
  `text_1_loc7` text,
  `text_1_loc8` text,
  `text_1_loc9` text,
  `text_1_loc10` text,
  `text_1_loc11` text
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

INSERT INTO `locales_npc_broadcast_text_unused` (`entry`, `text_0_loc1`, `text_0_loc2`, `text_0_loc3`, `text_0_loc4`, `text_0_loc5`, `text_0_loc6`, `text_0_loc7`, `text_0_loc8`, `text_0_loc9`, `text_0_loc10`, `text_0_loc11`, `text_1_loc1`, `text_1_loc2`, `text_1_loc3`, `text_1_loc4`, `text_1_loc5`, `text_1_loc6`, `text_1_loc7`, `text_1_loc8`, `text_1_loc9`, `text_1_loc10`, `text_1_loc11`) VALUES
(1, '제발, 제발 좀 도와주시오! 난 여기 갇혀 있소!', 'Au secours, au secours ! On m’opprime !', 'Hilfe, Hilfe! Ich werde bedrängt!', '救命啊！我动不了了！', '救命啊!我動不了了!', '¡Socorro! ¡Socorro! ¡Soy víctima de represión!', '¡Socorro! ¡Socorro! ¡Soy víctima de represión!', 'Помогите! Хулиганы зрения лишают!', 'Aiuto, aiuto! Mi stanno reprimendo!', 'Socorro, socorro! Estão me reprimindo!', 'Socorro, socorro! Estão me reprimindo!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL);

DROP TABLE IF EXISTS `npc_broadcast_text_unused`;
CREATE TABLE IF NOT EXISTS `npc_broadcast_text_unused` (
  `entry` int DEFAULT NULL,
  `lang` int DEFAULT NULL,
  `text_0` text,
  `text_1` text,
  `emote0` int DEFAULT NULL,
  `emote1` int DEFAULT NULL,
  `emote2` int DEFAULT NULL,
  `emotedelay0` int DEFAULT NULL,
  `emotedelay1` int DEFAULT NULL,
  `emotedelay2` int DEFAULT NULL,
  `soundid` int DEFAULT NULL,
  `emotesid` int DEFAULT NULL,
  `flags` int DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_0900_ai_ci;

INSERT INTO `npc_broadcast_text_unused` (`entry`, `lang`, `text_0`, `text_1`, `emote0`, `emote1`, `emote2`, `emotedelay0`, `emotedelay1`, `emotedelay2`, `soundid`, `emotesid`, `flags`) VALUES
(1, 0, 'Help help! I\'m being repressed!', '', 0, 0, 0, 0, 0, 0, 0, 0, 6);
