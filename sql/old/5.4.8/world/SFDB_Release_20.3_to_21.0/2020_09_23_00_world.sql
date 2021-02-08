-- DELETE Hunter, Shaman 'Mail' from Warrior, Paladin, DK
DELETE FROM `playercreateinfo_spell` WHERE `racemask`=0 AND `classmask`=35 AND `Spell`=8737;

-- Learn Warrior, Paladin, DK 'Mail'
DELETE FROM `playercreateinfo_spell` WHERE `racemask`=0 AND `classmask`=35 AND `Spell`=119811;
INSERT INTO `playercreateinfo_spell` (`racemask`, `classmask`, `Spell`, `Note`) VALUES
(0, 35, 119811, 'Mail');
