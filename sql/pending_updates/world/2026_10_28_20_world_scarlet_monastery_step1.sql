-- Scarlet Monastery: MoP instance on map 1004.
DELETE FROM `instance_template` WHERE `map` = 1004;
INSERT INTO `instance_template` (`map`, `parent`, `script`, `allowMount`) VALUES (1004, 0, 'instance_scarlet_monastery_mop', 0);
