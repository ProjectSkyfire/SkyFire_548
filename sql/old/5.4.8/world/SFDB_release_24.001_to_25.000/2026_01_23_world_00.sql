DELETE FROM `skyfire_string` WHERE `entry`=401;
INSERT INTO `skyfire_string` (`entry`, `content_default`) VALUES (401, 'You change security level of account %s to %i.');
DELETE FROM `skyfire_string` WHERE `entry`=402;
INSERT INTO `skyfire_string` (`entry`, `content_default`) VALUES (402, '%s changed your security level to %i.');
DELETE FROM `skyfire_string` WHERE `entry`=403;
INSERT INTO `skyfire_string` (`entry`, `content_default`) VALUES (403, 'You have low security level for this.');
DELETE FROM `skyfire_string` WHERE `entry`=597;
INSERT INTO `skyfire_string` (`entry`, `content_default`) VALUES (597, 'Current gamemasters:');
DELETE FROM `skyfire_string` WHERE `entry`=598;
INSERT INTO `skyfire_string` (`entry`, `content_default`) VALUES (598, '|    Account    |  GM  |');
DELETE FROM `skyfire_string` WHERE `entry`=599;
INSERT INTO `skyfire_string` (`entry`, `content_default`) VALUES (599, 'No gamemasters.');

DELETE FROM `command` WHERE `name`='account set gmlevel';
INSERT INTO `command` (`name`, `permission`, `help`) VALUES ('account set gmlevel', 228, 'Syntax: .account set gmlevel [$account] #level [#realmid]  Set the security level for targeted player (can\'t be used at self) or for account $name to a level of #level on the realm #realmID.  #level may range from 0 to 3.  #reamID may be -1 for all realms.');
DELETE FROM `command` WHERE `name`='gm list';
INSERT INTO `command` (`name`, `permission`, `help`) VALUES ('gm list', 375, 'Syntax: .gm list  Display a list of all Game Masters accounts and security levels.');

