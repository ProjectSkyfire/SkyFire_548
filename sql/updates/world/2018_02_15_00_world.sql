ALTER TABLE `creature` CHANGE `spawnMask` `spawnMask` int(10) unsigned not null default '1';

ALTER TABLE `gameobject` CHANGE `spawnMask` `spawnMask` int(10) unsigned not null default '1';