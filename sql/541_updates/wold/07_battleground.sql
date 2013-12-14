-- https://github.com/Darkpeninsula/Darkcore/commit/a2a7283893c6f63cd61f657fd1cf7a8cbb47845c
-- my job.
delete from `battleground_template` where `id` =108;
insert into `battleground_template` (`id`, `MinPlayersPerTeam`, `MaxPlayersPerTeam`, `MinLvl`, `MaxLvl`, `AllianceStartLoc`, `AllianceStartO`, `HordeStartLoc`, `HordeStartO`, `StartMaxDist`, `Weight`, `ScriptName`, `Comment`) VALUES 
(108, 5, 10, 81, 85, 1726, 0, 1727, 0, 75, 1, '', 'Twin Peaks');

insert into `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) values('12001','The battle for Twin Peaks begins in 1 minute.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Битва за Два Пика начнется через 1 минуту.');
insert into `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) values('12000','The battle for Twin Peaks begins in 2 minutes.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Сражение в Два Пика начнется через 2 минуты');
insert into `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) values('12002','The battle for Twin Peaks begins in 30 seconds. Prepare yourselves!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Битва за Два Пика начнется через 30 секунд. Приготовьтесь!');
insert into `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) values('12003','Let the battle for Twin Peaks begin!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Битва за Два Пика началась!');
insert into `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) values('12004','$n captured the Horde flag!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'$n $gзахватил:захватила; флаг Орды!');
insert into `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) values('12005','$n captured the Alliance flag!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'$n $gзахватил:захватила; флаг Альянса!');
insert into `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) values('12006','The Horde flag was dropped by $n!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'$n $gпотерял:потеряла; флаг Орды!');
insert into `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) values('12007','The Alliance Flag was dropped by $n!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'$n $gпотерял:потеряла; флаг Альянса!');
insert into `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) values('12008','The Alliance Flag was returned to its base by $n!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'$n $gвернул:вернула; флаг Альянса на свою базу!');
insert into `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) values('12009','The Horde flag was returned to its base by $n!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'$n $gвернул:вернула; флаг Орды на свою базу!');
insert into `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) values('12010','The Horde flag was picked up by $n!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'$n $gподобрал:подобрала; флаг Орды!');
insert into `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) values('12011','The Alliance Flag was picked up by $n!',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'$n $gподобрал:подобрала; флаг Альянса!');
insert into `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) values('12012','The flags are now placed at their bases.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Флаги расположены по своим базам.');
insert into `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) values('12013','The Alliance flag is now placed at its base.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Флаг Альянса теперь расположен на своей базе.');
insert into `trinity_string` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`) values('12014','The Horde flag is now placed at its base.',NULL,NULL,NULL,NULL,NULL,NULL,NULL,'Флаг Орды теперь расположен на своей базе.');

delete from `game_graveyard_zone` where in (1750, 1749, 1726, 1727, 1729, 1728);
insert into `game_graveyard_zone` values
(1750, 5031, 67),
(1749, 5031, 469),
(1726, 5031, 469),
(1727, 5031, 67),
(1729, 5031, 469),
(1728, 5031, 67);