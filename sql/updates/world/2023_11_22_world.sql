DROP TABLE IF EXISTS `lua_private_functions`;
CREATE TABLE IF NOT EXISTS `lua_private_functions` (
  `function_name` varchar(255) CHARACTER SET utf8mb4 COLLATE utf8mb4_bin NOT NULL,
  `enabled` tinyint(4) NOT NULL DEFAULT '1'
) ENGINE=InnoDB DEFAULT CHARSET=latin1;
DELETE FROM `lua_private_functions`;
INSERT INTO `lua_private_functions` (`function_name`, `enabled`) VALUES
	('CastSpellByName', 1),
	('RunMacroText', 1);

DELETE FROM `skyfire_string` WHERE `entry`=11002;
INSERT INTO `skyfire_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`, `content_loc9`, `content_loc10`, `content_loc11`)VALUES 
(11002, '|cFFFFBF00[%s]:|cFFFFFFFF|Hplayer:%s|h[%s]|h|cFF00FFFF potential |r|cFFFFFF00%s|r%s x %u|r', null, null, null, null, null, null, null, null, null, null, null);
