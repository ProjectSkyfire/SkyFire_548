DROP TABLE IF EXISTS  `scene_template`;
CREATE TABLE `scene_template` (
	`SceneId` INT NOT NULL,
	`Flags` INT NOT NULL,
	`ScriptPackageID` INT NOT NULL,
	`ScriptName` TEXT NULL,
	`comment` TEXT NULL
)
COLLATE='utf8mb4_0900_ai_ci'
ENGINE=InnoDB;

DELETE FROM `scene_template` WHERE `SceneId` IN (249, 250, 251, 252);
INSERT INTO `scene_template` (`SceneId`, `Flags`, `ScriptPackageID`, `ScriptName`, `comment`) VALUES 
(249, 2, 535, NULL, 'Legendary 5.4 - Finale - Cloak Reveal - Xuen - ZTO'),
(250, 2, 537, NULL, 'Legendary 5.4 - Finale - Cloak Reveal - Nuizhao - ZTO'),
(251, 2, 538, NULL, 'Legendary 5.4 - Finale - Cloak Reveal - Chiji - ZTO'),
(252, 2, 539, NULL, 'Legendary 5.4 - Finale - Cloak Reveal - Yulon - ZTO');
