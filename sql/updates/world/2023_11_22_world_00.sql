DELETE FROM `creature` WHERE `id` IN (225338, 225331, 225292);

DELETE FROM `creature_queststarter` WHERE `id`=54975 AND `quest`=29679;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(54975, 29679);

UPDATE `quest_template` SET `PrevQuestId`=29521 WHERE `Id`=29661;
UPDATE `quest_template` SET `PrevQuestId`=29678 WHERE `Id`=29679;
UPDATE `quest_template` SET `PrevQuestId`=29423 WHERE `Id`=29521;

UPDATE `quest_template` SET `OfferRewardText`='$n, that\'s the way to do it! You show up, and problems start getting solved. That\'s just one of many reasons why I like you!' WHERE `Id`=29769;
UPDATE `quest_template` SET `OfferRewardText`='Some of these have mouth-sized chunks bitten out of them! Ah well, we can still make use.$B$BThank you for your help, friend.' WHERE `Id`=29770;
UPDATE `quest_template` SET `RequestItemsText`='Have you done as I\'ve asked?' WHERE `Id`=29771;
UPDATE `quest_template` SET `OfferRewardText`='Good.$B$BMy head, it is harder than the densest wood.$B$BStep back, and let me dispel your doubts.' WHERE `Id`=29771;
UPDATE `quest_template` SET `RequestItemsText`='Well, they\'re as warmed as they\'re going to get. Do you have the mallet?' WHERE `Id`=29768;
UPDATE `quest_template` SET `OfferRewardText`='Hah, $n! Now that\'s a mallet if I\'ve ever seen one! Let\'s make some noise.' WHERE `Id`=29768;
UPDATE `quest_template` SET `OfferRewardText`='Huh... well... that\'s definitely the ringing I expected, but I was hoping for a bit more from Wugou. This guy\'s a deep sleeper, that\'s for sure.$B$BIt\'s alright, $n. I\'ve got another idea, and there\'s not much preparation needed at all - just how I like it.' WHERE `Id`=29772;
UPDATE `quest_template` SET `OfferRewardText`='YEAH!!! $n, we make quite a team. Persistence over planning, I always say!' WHERE `Id`=29774;
UPDATE `quest_template` SET `OfferRewardText`='You have done well, young student. Three of the four spirits have been returned to the temple. We are closer to the answers we seek.' WHERE `Id`=29775;

UPDATE `creature_template` SET `npcflag`=1, `gossip_menu_id`=13140 WHERE  `entry`=55556;
DELETE FROM `npc_text` WHERE `ID`=54024;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`) VALUES
(54024, '<Shu looks at you expectantly.>', '');
DELETE FROM `gossip_menu` WHERE `entry`=13140;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(13140, 54024);

UPDATE `creature_template` SET `gossip_menu_id`=13060 WHERE `entry`=54943;
DELETE FROM `npc_text` WHERE `ID`=53245;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`) VALUES
(53245, 'I came to ask the famous Aysa Cloudsinger for help, but I just can\'t bring myself to intrude on her exercises. She\'s so poised, so practiced... so beautiful.', '');
DELETE FROM `gossip_menu` WHERE `entry`=13060;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(13060, 53245);

UPDATE `creature_template` SET `npcflag`=1, `gossip_menu_id`=14617 WHERE  `entry`=64939;
DELETE FROM `npc_text` WHERE `ID`=65910;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`) VALUES
(65910, 'It is said, "Turn your face towards the sun, and the shadows will fall behind you."$b$bI hope you find many bright days ahead of you, $n!', '');
DELETE FROM `gossip_menu` WHERE `entry`=14617;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(14617, 65910);

UPDATE `creature_template` SET `gossip_menu_id`=12634 WHERE `entry`=42553;
DELETE FROM `npc_text` WHERE `ID`=50616;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`) VALUES
(50616, 'Have you seen my latest invention? It\'s going to be a hit... if it doesn\'t explode.', '');
DELETE FROM `gossip_menu` WHERE `entry`=12634;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(12634, 50616);

UPDATE `creature_template` SET `gossip_menu_id`=11614 WHERE `entry`=42317;
DELETE FROM `npc_text` WHERE `ID`=42230;
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`) VALUES
(42230, 'Operation: Gnomeregan was a success. Sure, that dastardly Thermaplugg had an unforeseen trick up his sleeve, but we have him on the run! It\'s only a matter of time before Gnomeregan is cleaned up and in our hands again!', '');
DELETE FROM `gossip_menu` WHERE `entry`=11614;
INSERT INTO `gossip_menu` (`entry`, `text_id`) VALUES
(11614, 42230);
