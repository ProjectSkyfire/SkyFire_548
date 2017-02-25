-- https://github.com/ProjectSkyfire/SkyFire.548/issues/563
DELETE FROM `instance_template` WHERE `map` = 1098;
INSERT INTO `instance_template` (`map`, `parent`, `allowMount`) VALUES
(1098, 1064, 0); -- Throne of Thunder Raid. AllowMount = true
