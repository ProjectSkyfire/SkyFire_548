SET @GREENTEALEAF      := 72234; -- Green Tea Leaf
SET @GREENTEALEAFREF   := 11919; -- Green Tea Leaf Ref ID
SET @SNOWLILY          := 79010; -- Snow Lily
SET @SNOWLILYREF       := 11920; -- Snow Lily Ref ID
SET @FOOLCAP           := 79011; -- Fool's Cap
SET @FOOLCAPREF        := 11921; -- Fool's Cap Ref ID
SET @SILKWEED          := 72235; -- Silkweed
SET @SILKWEEDREF       := 11922; -- Silkweed Ref ID
SET @RAINPOPPY         := 72237; -- Rain Poppy
SET @RAINPOPPYREF      := 11923; -- Rain Poppy Ref ID
SET @DESECRATEDHERB    := 89639; -- Desecrated Herb
SET @DESECRATEDHERBREF := 11924; -- Desecrated Herb Ref ID

SET @SHADOWPIGMENT     := 79251; -- Shadow Pigment
SET @MISTYPIGMENT      := 79253; -- Misty Pigment

DELETE FROM `milling_loot_template` WHERE `entry`=@GREENTEALEAF;
INSERT INTO `milling_loot_template` (`entry`, `item`, `mincountOrRef`) VALUES
(@GREENTEALEAF, @GREENTEALEAFREF, -@GREENTEALEAFREF);

DELETE FROM `reference_loot_template` WHERE `entry`=@GREENTEALEAFREF;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `mincountOrRef`, `maxcount`) VALUES
(@GREENTEALEAFREF, @SHADOWPIGMENT, 100, 2, 3),
(@GREENTEALEAFREF, @MISTYPIGMENT, 25, 1, 3);

DELETE FROM `milling_loot_template` WHERE `entry`=@SNOWLILY;
INSERT INTO `milling_loot_template` (`entry`, `item`, `mincountOrRef`) VALUES
(@SNOWLILY, @SNOWLILYREF, -@SNOWLILYREF);

DELETE FROM `reference_loot_template` WHERE `entry`=@SNOWLILYREF;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `mincountOrRef`, `maxcount`) VALUES
(@SNOWLILYREF, @SHADOWPIGMENT, 100, 2, 3),
(@SNOWLILYREF, @MISTYPIGMENT, 25, 1, 3);

DELETE FROM `milling_loot_template` WHERE `entry`=@FOOLCAP;
INSERT INTO `milling_loot_template` (`entry`, `item`, `mincountOrRef`) VALUES
(@FOOLCAP, @FOOLCAPREF, -@FOOLCAPREF);

DELETE FROM `reference_loot_template` WHERE `entry`=@FOOLCAPREF;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `mincountOrRef`, `maxcount`) VALUES
(@FOOLCAPREF, @SHADOWPIGMENT, 100, 2, 3),
(@FOOLCAPREF, @MISTYPIGMENT, 25, 1, 3);

DELETE FROM `milling_loot_template` WHERE `entry`=@SILKWEED;
INSERT INTO `milling_loot_template` (`entry`, `item`, `mincountOrRef`) VALUES
(@SILKWEED, @SILKWEEDREF, -@SILKWEEDREF);

DELETE FROM `reference_loot_template` WHERE `entry`=@SILKWEEDREF;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `mincountOrRef`, `maxcount`) VALUES
(@SILKWEEDREF, @SHADOWPIGMENT, 100, 2, 3),
(@SILKWEEDREF, @MISTYPIGMENT, 25, 1, 3);

DELETE FROM `milling_loot_template` WHERE `entry`=@RAINPOPPY;
INSERT INTO `milling_loot_template` (`entry`, `item`, `mincountOrRef`) VALUES
(@RAINPOPPY, @RAINPOPPYREF, -@RAINPOPPYREF);

DELETE FROM `reference_loot_template` WHERE `entry`=@RAINPOPPYREF;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `mincountOrRef`, `maxcount`) VALUES
(@RAINPOPPYREF, @SHADOWPIGMENT, 100, 2, 3),
(@RAINPOPPYREF, @MISTYPIGMENT, 25, 1, 3);

DELETE FROM `milling_loot_template` WHERE `entry`=@DESECRATEDHERB;
INSERT INTO `milling_loot_template` (`entry`, `item`, `mincountOrRef`) VALUES
(@DESECRATEDHERB, @DESECRATEDHERBREF, -@DESECRATEDHERBREF);

DELETE FROM `reference_loot_template` WHERE `entry`=@DESECRATEDHERBREF;
INSERT INTO `reference_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `mincountOrRef`, `maxcount`) VALUES
(@DESECRATEDHERBREF, @SHADOWPIGMENT, 100, 2, 3),
(@DESECRATEDHERBREF, @MISTYPIGMENT, 25, 1, 3);
SET @SOURCETYPE := 1; -- Quest
SET @QUESTID    := 14; -- The People's Militia

DELETE FROM `disables` WHERE `sourceType`=@SOURCETYPE AND `entry`=@QUESTID;
INSERT INTO `disables` (`sourceType`, `entry`, `comment`) VALUES
(@SOURCETYPE, @QUESTID, 'Disable Quest 14: The People\'s Militia');
SET @ISDQPOOL := 12002; -- Instructor Skythorn - Daily Quests
SET @QUESTID1 := 30155; -- Restoring the Balance
SET @QUESTID2 := 30158; -- Disarming the Enemy
SET @QUESTID3 := 31698; -- Thinning The Pack
SET @QUESTID4 := 31706; -- Weeping Widows
SET @QUESTID5 := 31707; -- A Tangled Web
SET @QUESTID6 := 31712; -- Monkey Mischief

DELETE FROM `pool_template` WHERE `entry`=@ISDQPOOL;
INSERT INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES
(@ISDQPOOL, 1, 'Instructor Skythorn - Daily Quests');

DELETE FROM `pool_quest` WHERE `pool_entry`=@ISDQPOOL;
INSERT INTO `pool_quest` (`entry`, `pool_entry`, `description`) VALUES
(@QUESTID1, @ISDQPOOL, 'Restoring the Balance'),
(@QUESTID2, @ISDQPOOL, 'Disarming the Enemy'),
(@QUESTID3, @ISDQPOOL, 'Thinning The Pack'),
(@QUESTID4, @ISDQPOOL, 'Weeping Widows'),
(@QUESTID5, @ISDQPOOL, 'A Tangled Web'),
(@QUESTID6, @ISDQPOOL, 'Monkey Mischief');
SET @RDDQPOOL := 12003; -- Rik'kal the Dissector - Daily Quests
SET @QUESTID1 := 31234; -- Putting An Eye Out
SET @QUESTID2 := 31271; -- Bad Genes
SET @QUESTID3 := 31502; -- Wing Clip
SET @QUESTID4 := 31503; -- Shortcut to Ruin
SET @QUESTID5 := 31508; -- Specimen Request
SET @QUESTID6 := 31509; -- Fear Takes Root

DELETE FROM `pool_template` WHERE `entry`=@RDDQPOOL;
INSERT INTO `pool_template` (`entry`, `max_limit`, `description`) VALUES
(@RDDQPOOL, 1, 'Rik''kal the Dissector - Daily Quests');

DELETE FROM `pool_quest` WHERE `pool_entry`=@RDDQPOOL;
INSERT INTO `pool_quest` (`entry`, `pool_entry`, `description`) VALUES
(@QUESTID1, @RDDQPOOL, 'Putting An Eye Out'),
(@QUESTID2, @RDDQPOOL, 'Bad Genes'),
(@QUESTID3, @RDDQPOOL, 'Wing Clip'),
(@QUESTID4, @RDDQPOOL, 'Shortcut to Ruin'),
(@QUESTID5, @RDDQPOOL, 'Specimen Request'),
(@QUESTID6, @RDDQPOOL, 'Fear Takes Root');
SET @AQUESTID := 32578; -- Among the Bones (A)
SET @HQUESTID := 32293; -- Among the Bones (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32571; -- A Wing to Fly On (A)
SET @HQUESTID := 32506; -- A Wing to Fly On (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32525; -- Ashes of the Enemy (A)
SET @HQUESTID := 32218; -- Ashes of the Enemy (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 31376; -- Attack At The Temple of the Jade Serpent (A)
SET @HQUESTID := 31377; -- Attack At The Temple of the Jade Serpent (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32485; -- Bolstering the Defenses (A)
SET @HQUESTID := 32288; -- Bolstering the Defenses (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32634; -- Breaking Down the Defenses (A)
SET @HQUESTID := 32269; -- Breaking Down the Defenses (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 31378; -- Challenge At The Temple of the Red Crane (A)
SET @HQUESTID := 31379; -- Challenge At The Temple of the Red Crane (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32627; -- Charged Moganite (A)
SET @HQUESTID := 32265; -- Charged Moganite (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32551; -- Compy Stomp (A)
SET @HQUESTID := 32282; -- Compy Stomp (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32543; -- Dangers of Za'Tual (A)
SET @HQUESTID := 32200; -- Dangers of Za'Tual (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32539; -- Dark Offerings (A)
SET @HQUESTID := 32217; -- Dark Offerings (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32537; -- De-Constructed (A)
SET @HQUESTID := 32255; -- De-Constructed (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32639; -- Deconstruction (A)
SET @HQUESTID := 32302; -- Deconstruction (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 31382; -- Defense At Niuzao Temple (A)
SET @HQUESTID := 31383; -- Defense At Niuzao Temple (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32554; -- Dino Might (A)
SET @HQUESTID := 32298; -- Dino Might (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32553; -- Direhorn vs Devilsaur (A)
SET @HQUESTID := 32297; -- Direhorn or Devilsaur (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32573; -- Enemies Beneath the Tower (A)
SET @HQUESTID := 32287; -- Enemies Beneath the Tower (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32607; -- Extended Shore Leave (A)
SET @HQUESTID := 32676; -- Extended Shore Leave (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32527; -- Grave Circumstances (A)
SET @HQUESTID := 32201; -- Grave Circumstances (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32540; -- Harbingers of the Loa (A)
SET @HQUESTID := 32252; -- Harbingers of the Loa (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32538; -- Heinous Sacrifice (A)
SET @HQUESTID := 32215; -- Heinous Sacrifice (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32631; -- High Recognition (A)
SET @HQUESTID := 32304; -- High Recognition (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32732; -- Ihgaluk Crag (A)
SET @HQUESTID := 32730; -- Ihgaluk Crag (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32528; -- Into the Crypts (A)
SET @HQUESTID := 32226; -- Into the Crypts (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32546; -- Just Some Light Clean-Up Work (A)
SET @HQUESTID := 32299; -- Just Some Light Clean-Up Work (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32548; -- Left To Rot (A)
SET @HQUESTID := 32491; -- Left To Rot (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32552; -- Loa-saur (A)
SET @HQUESTID := 32283; -- Loa-saur (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32632; -- Made for War (A)
SET @HQUESTID := 32303; -- Made for War (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32638; -- Mana Scavengers (A)
SET @HQUESTID := 32266; -- Mana Manifestations (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32536; -- Manipulating the Saurok (A)
SET @HQUESTID := 32254; -- Manipulating the Saurok (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32586; -- Maximum Capacitor (A)
SET @HQUESTID := 32208; -- Maximum Capacitor (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32301; -- Old Enemies (A)
SET @HQUESTID := 32213; -- Old Enemies (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32637; -- Overpowered (A)
SET @HQUESTID := 32305; -- Overpowered (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32541; -- Preventing a Future Threat (A)
SET @HQUESTID := 32227; -- Preventing a Future Threat (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32544; -- Pterrible Ptorment (A)
SET @HQUESTID := 32216; -- Pterrible Ptorment (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32608; -- Raiding the Vault (A)
SET @HQUESTID := 32677; -- Raiding the Vault (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32582; -- Raining Bones (A)
SET @HQUESTID := 32294; -- Raining Bones (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32532; -- Rise No More! (A)
SET @HQUESTID := 32224; -- Rise No More! (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32550; -- Saur Loser (A)
SET @HQUESTID := 32207; -- Saur Loser (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32526; -- Soul Surrender (A)
SET @HQUESTID := 32220; -- Soul Surrender (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32633; -- Spellbound (A)
SET @HQUESTID := 32264; -- Spellbound (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32533; -- Stone Cold (A)
SET @HQUESTID := 32219; -- Stone Cold (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32606; -- Subtle Encouragement (A)
SET @HQUESTID := 32605; -- Subtle Encouragement (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32542; -- Surgical Strike (A)
SET @HQUESTID := 32275; -- Surgical Death (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32628; -- Tactical Mana Bombs (A)
SET @HQUESTID := 32268; -- Tactical Mana Bombs (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32567; -- The Beast Pens (A)
SET @HQUESTID := 32523; -- The Beast Pens (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32530; -- The Bloodletter (A)
SET @HQUESTID := 32495; -- The Bloodletter (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32529; -- The Call of Thunder (A)
SET @HQUESTID := 32225; -- The Call of Thunder (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32531; -- The Conquest of Stone (A)
SET @HQUESTID := 32517; -- The Conquest of Stone (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32731; -- The Court of Bones (A)
SET @HQUESTID := 32728; -- The Court of Bones (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32547; -- The Creeping Carpet of Ihgaluk (A)
SET @HQUESTID := 32489; -- The Creeping Carpet of Ihgaluk (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32545; -- The Residents of Ihgaluk (A)
SET @HQUESTID := 32274; -- The Residents of Ihgaluk (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32574; -- The Shuddering Moor (A)
SET @HQUESTID := 32228; -- The Shuddering Moor (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32535; -- The Skumblade Threat (A)
SET @HQUESTID := 32204; -- The Skumblade Threat (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32572; -- The Sleepless Legion (A)
SET @HQUESTID := 32285; -- The Sleepless Legion (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 32575; -- The Zandalari Colossus (A)
SET @HQUESTID := 32230; -- The Zandalari Colossus (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);

SET @AQUESTID := 31380; -- Trial At The Temple of the White Tiger (A)
SET @HQUESTID := 31381; -- Trial At The Temple of the White Tiger (H)

DELETE FROM `player_factionchange_quests` WHERE `alliance_id`=@AQUESTID AND `horde_id`=@HQUESTID;
INSERT INTO `player_factionchange_quests` (`alliance_id`, `horde_id`) VALUES
(@AQUESTID, @HQUESTID);
SET @MFFEVENT := 1;    -- Midsummer Fire Festival
SET @DWQUEST := 32497; -- Honor the Flame (Dread Wastes)
SET @JFQUEST := 32498; -- Honor the Flame (Jade Forest)
SET @KWQUEST := 32499; -- Honor the Flame (Krasarang Wilds)
SET @KLQUEST := 32500; -- Honor the Flame (Kun-Lai Summit)
SET @TSQUEST := 32501; -- Honor the Flame (Townlong Steppes)
SET @VWQUEST := 32502; -- Honor the Flame (Valley of the Four Winds)

DELETE FROM `game_event_seasonal_questrelation` WHERE `questId` IN (@DWQUEST, @JFQUEST, @KWQUEST, @KLQUEST, @TSQUEST, @VWQUEST) AND `eventEntry`=@MFFEVENT;
INSERT INTO `game_event_seasonal_questrelation` (`questId`, `eventEntry`) VALUES
(@DWQUEST, @MFFEVENT),
(@JFQUEST, @MFFEVENT),
(@KWQUEST, @MFFEVENT),
(@KLQUEST, @MFFEVENT),
(@TSQUEST, @MFFEVENT),
(@VWQUEST, @MFFEVENT);
-- Quest: A Shade of Dread (31210)
SET @OLOOT :=42807;
SET @QITEM :=85573;

DELETE FROM `gameobject_loot_template` WHERE `entry`=@OLOOT AND `item`=@QITEM;
INSERT INTO `gameobject_loot_template` (`entry`, `item`, `ChanceOrQuestChance`) VALUES
(@OLOOT, @QITEM, -100);
-- Human starting zone updates
UPDATE creature_template SET ainame="", scriptname="npc_blackrock_battle_worg" WHERE entry=49871;
UPDATE creature_template SET ainame="", scriptname="npc_stormwind_infantry" WHERE entry=49869;
-- Human starting zone updates
DELETE FROM creature_template_addon WHERE entry = 49869;
INSERT INTO creature_template_addon VALUES
(49869,0,0,0,1,0,"");
-- Human starting zone updates
DELETE FROM creature_text WHERE entry=49869 AND groupid=1 LIMIT 5;
INSERT INTO creature_text VALUES
(49869,1,0,"Come, monsters! We will crush you!",14,0,100,0,0,0,"Stormwind Infantry - Yell"),
(49869,1,1,"I'll kill a hundred more of your battle worgs!",14,0,100,0,0,0,"Stormwind Infantry - Yell"),
(49869,1,2,"Look alive, men! There are orcs and worgs about!",14,0,100,0,0,0,"Stormwind Infantry - Yell"),
(49869,1,3,"Your filthy dogs won't be enough!",14,0,100,0,0,0,"Stormwind Infantry - Yell"),
(49869,1,4,"Your worgs are no match for the might of Stormwind!",14,0,100,0,0,0,"Stormwind Infantry - Yell");

UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 15361;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 69818;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 69819;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 69891;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 71033;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 71876;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 72151;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 72464;
UPDATE `creature_template` SET `faction_A` = 16 WHERE  `entry` = 72771;
UPDATE `creature_template` SET `faction_A` = 2497 WHERE  `entry` = 73041;
UPDATE `creature_template` SET `faction_A` = 2497 WHERE  `entry` = 73043;
UPDATE `creature_template` SET `faction_A` = 14 WHERE  `entry` = 73167;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 73648;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 73657;
UPDATE `creature_template` SET `faction_A` = 230 WHERE  `entry` = 73718;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 73812;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 77137;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 7380;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 7382;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 7384;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 7386;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 7389;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 7390;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 7391;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 7395;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 7543;
UPDATE `creature_template` SET `faction_A` = 5 WHERE  `entry` = 7544;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 7547;
UPDATE `creature_template` SET `faction_A` = 4 WHERE  `entry` = 7550;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 7554;
UPDATE `creature_template` SET `faction_A` = 2395 WHERE  `entry` = 7560;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 7561;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 7567;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 8376;
UPDATE `creature_template` SET `faction_A` = 1629 WHERE  `entry` = 9656;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 9657;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 9662;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 11325;
UPDATE `creature_template` SET `faction_A` = 3 WHERE  `entry` = 11326;
UPDATE `creature_template` SET `faction_A` = 4 WHERE  `entry` = 11327;
UPDATE `creature_template` SET `faction_A` = 115 WHERE  `entry` = 12419;
UPDATE `creature_template` SET `faction_A` = 7 WHERE  `entry` = 14421;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 15186;
UPDATE `creature_template` SET `faction_A` = 1610 WHERE  `entry` = 15358;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 15429;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 16445;
UPDATE `creature_template` SET `faction_A` = 4 WHERE  `entry` = 30379;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 63647;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 63649;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 63756;
UPDATE `creature_template` SET `faction_A` = 2392 WHERE  `entry` = 63810;
UPDATE `creature_template` SET `faction_A` = 14 WHERE  `entry` = 64523;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 64608;
UPDATE `creature_template` SET `faction_A` = 14 WHERE  `entry` = 64687;
UPDATE `creature_template` SET `faction_A` = 14 WHERE  `entry` = 64692;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 65006;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 65009;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 65060;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 65065;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 65071;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 65076;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 65078;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 65282;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 65428;
UPDATE `creature_template` SET `faction_A` = 2558 WHERE  `entry` = 66391;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 66564;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 66567;
UPDATE `creature_template` SET `faction_A` = 14 WHERE  `entry` = 66668;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 73305;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 15698;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 15699;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 15705;
UPDATE `creature_template` SET `faction_A` = 4 WHERE  `entry` = 15706;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 15710;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 16456;
UPDATE `creature_template` SET `faction_A` = 1629 WHERE  `entry` = 16547;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 16548;
UPDATE `creature_template` SET `faction_A` = 115 WHERE  `entry` = 16549;
UPDATE `creature_template` SET `faction_A` = 4 WHERE  `entry` = 16701;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 17255;
UPDATE `creature_template` SET `faction_A` = 6 WHERE  `entry` = 18381;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 20408;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 21008;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 21009;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 21010;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 21018;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 21055;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 21056;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 21063;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 21064;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 21076;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 22445;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 22943;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 23231;
UPDATE `creature_template` SET `faction_A` = 534 WHERE  `entry` = 23258;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 23909;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 24388;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 24389;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 24480;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 24753;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 25109;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 25110;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 25146;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 25147;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 25706;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 26050;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 26056;
UPDATE `creature_template` SET `faction_A` = 1610 WHERE  `entry` = 26119;
UPDATE `creature_template` SET `faction_A` = 2395 WHERE  `entry` = 27217;
UPDATE `creature_template` SET `faction_A` = 14 WHERE  `entry` = 28470;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 28513;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 28883;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 29089;
UPDATE `creature_template` SET `faction_A` = 5 WHERE  `entry` = 29726;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 31575;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 32590;
UPDATE `creature_template` SET `faction_A` = 2395 WHERE  `entry` = 32591;
UPDATE `creature_template` SET `faction_A` = 2395 WHERE  `entry` = 32592;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 32595;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 32791;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 32841;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 32939;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 33188;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 33194;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 63197;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 63647;
UPDATE `creature_template` SET `faction_A` = 2203 WHERE  `entry` = 7387;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 33194;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 33197;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 33198;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 33200;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 33205;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 33219;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 33238;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 33529;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 33578;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 33810;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 34364;
UPDATE `creature_template` SET `faction_A` = 2395 WHERE  `entry` = 34587;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 34694;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 34724;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 34930;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 35394;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 35395;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 35396;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 35397;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 35398;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 35399;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 35400;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 35468;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 36482;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 36511;
UPDATE `creature_template` SET `faction_A` = 1629 WHERE  `entry` = 36607;
UPDATE `creature_template` SET `faction_A` = 2204 WHERE  `entry` = 36871;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 36909;
UPDATE `creature_template` SET `faction_A` = 2395 WHERE  `entry` = 36911;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 36979;
UPDATE `creature_template` SET `faction_A` = 2203 WHERE  `entry` = 40295;
UPDATE `creature_template` SET `faction_A` = 2203 WHERE  `entry` = 40624;
UPDATE `creature_template` SET `faction_A` = 2203 WHERE  `entry` = 40703;
UPDATE `creature_template` SET `faction_A` = 1629 WHERE  `entry` = 42078;
UPDATE `creature_template` SET `faction_A` = 2203 WHERE  `entry` = 43916;
UPDATE `creature_template` SET `faction_A` = 2204 WHERE  `entry` = 45128;
UPDATE `creature_template` SET `faction_A` = 2204 WHERE  `entry` = 45247;
UPDATE `creature_template` SET `faction_A` = 2395 WHERE  `entry` = 47944;
UPDATE `creature_template` SET `faction_A` = 2203 WHERE  `entry` = 48107;
UPDATE `creature_template` SET `faction_A` = 2204 WHERE  `entry` = 48242;
UPDATE `creature_template` SET `faction_A` = 2203 WHERE  `entry` = 48609;
UPDATE `creature_template` SET `faction_A` = 2203 WHERE  `entry` = 48641;
UPDATE `creature_template` SET `faction_A` = 2203 WHERE  `entry` = 48982;
UPDATE `creature_template` SET `faction_A` = 2203 WHERE  `entry` = 49586;
UPDATE `creature_template` SET `faction_A` = 2203 WHERE  `entry` = 49587;
UPDATE `creature_template` SET `faction_A` = 2204 WHERE  `entry` = 49588;
UPDATE `creature_template` SET `faction_A` = 2204 WHERE  `entry` = 49590;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 50468;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 50545;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 50586;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 51090;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 51122;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 51600;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 51601;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 51632;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 51635;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 52226;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 52343;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 52344;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 52831;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 53048;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 53232;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 53283;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 53623;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 53658;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 53661;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 53884;
UPDATE `creature_template` SET `faction_A` = 1629 WHERE  `entry` = 54027;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 54227;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 54438;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 54539;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 54541;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61604;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61714;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61809;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62106;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62376;
UPDATE `creature_template` SET `faction_A` = 2482 WHERE  `entry` = 72993;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 73668;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 7381;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 7555;
UPDATE `creature_template` SET `faction_A` = 6 WHERE  `entry` = 14878;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 18839;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 20472;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 54730;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 54984;
UPDATE `creature_template` SET `faction_A` = 14 WHERE  `entry` = 55236;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 55367;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 55386;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 55574;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 56031;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 56266;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 56280;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 56382;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 58163;
UPDATE `creature_template` SET `faction_A` = 14 WHERE  `entry` = 58456;
UPDATE `creature_template` SET `faction_A` = 190 WHERE  `entry` = 58941;
UPDATE `creature_template` SET `faction_A` = 2203 WHERE  `entry` = 58960;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 58963;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 58964;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 59020;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 59101;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 59399;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 59454;
UPDATE `creature_template` SET `faction_A` = 16 WHERE  `entry` = 59580;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 59712;
UPDATE `creature_template` SET `faction_A` = 2492 WHERE  `entry` = 59830;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 60698;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 60763;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 60800;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61071;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 61080;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 61081;
UPDATE `creature_template` SET `faction_A` = 190 WHERE  `entry` = 61088;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61142;
UPDATE `creature_template` SET `faction_A` = 4 WHERE  `entry` = 61160;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61165;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61168;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61169;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61253;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61255;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61257;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61258;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61312;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61314;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61317;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61318;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61319;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61321;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61323;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61324;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61328;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61367;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61369;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61370;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61372;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61384;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61385;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61386;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61425;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61438;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61440;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61441;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61472;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61689;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61690;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61691;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61718;
UPDATE `creature_template` SET `faction_A` = 1629 WHERE  `entry` = 61757;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61758;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 66150;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 68805;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 68839;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 69748;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 69794;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 70025;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 70026;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 71016;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 71021;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 73010;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 73052;
UPDATE `creature_template` SET `faction_A` = 16 WHERE  `entry` = 73157;
UPDATE `creature_template` SET `faction_A` = 7 WHERE  `entry` = 73297;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 73338;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 73357;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 73366;
UPDATE `creature_template` SET `faction_A` = 4 WHERE  `entry` = 2671;
UPDATE `creature_template` SET `faction_A` = 116 WHERE  `entry` = 7383;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 7385;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 7394;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 7545;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 7549;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 7553;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 7562;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 7565;
UPDATE `creature_template` SET `faction_A` = 4 WHERE  `entry` = 10598;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 16069;
UPDATE `creature_template` SET `faction_A` = 4 WHERE  `entry` = 16085;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 23198;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 23234;
UPDATE `creature_template` SET `faction_A` = 2395 WHERE  `entry` = 23266;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 23274;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 24968;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 25062;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 27346;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 29147;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 40054;
UPDATE `creature_template` SET `faction_A` = 2203 WHERE  `entry` = 40819;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 43050;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 46754;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 46755;
UPDATE `creature_template` SET `faction_A` = 2204 WHERE  `entry` = 47244;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 48636;
UPDATE `creature_template` SET `faction_A` = 7 WHERE  `entry` = 49565;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61758;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61826;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61830;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62019;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62020;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62022;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62034;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62051;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62114;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62115;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62116;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62117;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62118;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62119;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62120;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62121;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62129;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62177;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62184;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62187;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62189;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62190;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62201;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62246;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62250;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62257;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62258;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62312;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62314;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62316;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62370;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62373;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62896;
UPDATE `creature_template` SET `faction_A` = 14 WHERE  `entry` = 65580;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 68662;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 69648;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 70104;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 70154;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 72007;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 72676;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 73023;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 73024;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 73042;
UPDATE `creature_template` SET `faction_A` = 2497 WHERE  `entry` = 73049;
UPDATE `creature_template` SET `faction_A` = 2497 WHERE  `entry` = 73053;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 73306;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 73364;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 73702;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 73780;
UPDATE `creature_template` SET `faction_A` = 2399 WHERE  `entry` = 73821;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 73967;
UPDATE `creature_template` SET `faction_A` = 2395 WHERE  `entry` = 10259;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 15666;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 20225;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 22510;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 22512;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 22513;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 22514;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 22958;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 22975;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 22978;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 23408;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 24488;
UPDATE `creature_template` SET `faction_A` = 16 WHERE  `entry` = 25721;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 26131;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 27893;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 27914;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 28017;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 29238;
UPDATE `creature_template` SET `faction_A` = 1712 WHERE  `entry` = 30164;
UPDATE `creature_template` SET `faction_A` = 2107 WHERE  `entry` = 30320;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 31216;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 31698;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 31717;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 31778;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 31852;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 32158;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 32207;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 32336;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 32640;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 33029;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 33274;
UPDATE `creature_template` SET `faction_A` = 2395 WHERE  `entry` = 33530;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 34155;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 34770;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 36908;
UPDATE `creature_template` SET `faction_A` = 1629 WHERE  `entry` = 38374;
UPDATE `creature_template` SET `faction_A` = 2204 WHERE  `entry` = 42183;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 45340;
UPDATE `creature_template` SET `faction_A` = 2204 WHERE  `entry` = 46898;
UPDATE `creature_template` SET `faction_A` = 14 WHERE  `entry` = 50159;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 51649;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 52894;
UPDATE `creature_template` SET `faction_A` = 4 WHERE  `entry` = 53225;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 54128;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 54487;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 54745;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 55571;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 59243;
UPDATE `creature_template` SET `faction_A` = 1711 WHERE  `entry` = 59534;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 60649;
UPDATE `creature_template` SET `faction_A` = 190 WHERE  `entry` = 60700;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61086;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61143;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61167;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61171;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61322;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61325;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61326;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61329;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61368;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61383;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61443;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61459;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61677;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61686;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61704;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61711;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61713;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61751;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61753;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61755;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61829;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61889;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61890;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62050;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62127;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62178;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62181;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62182;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62185;
UPDATE `creature_template` SET `faction_A` = 7 WHERE  `entry` = 62242;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62255;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62256;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62364;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62395;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62454;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62523;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62524;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62526;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62555;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62564;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62620;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62625;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62638;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62641;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62664;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62669;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62693;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62695;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62697;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62815;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62816;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62818;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62819;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62820;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62835;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62854;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62864;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62884;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62885;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62886;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62892;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62893;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62895;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62899;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62900;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62905;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62914;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62921;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62922;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62925;
UPDATE `creature_template` SET `faction_A` = 190 WHERE  `entry` = 62994;
UPDATE `creature_template` SET `faction_A` = 31 WHERE  `entry` = 63057;
UPDATE `creature_template` SET `faction_A` = 31 WHERE  `entry` = 63096;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 63097;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 63365;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 63555;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 63559;
UPDATE `creature_template` SET `faction_A` = 2395 WHERE  `entry` = 63832;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 63838;
UPDATE `creature_template` SET `faction_A` = 31 WHERE  `entry` = 63953;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 64232;
UPDATE `creature_template` SET `faction_A` = 31 WHERE  `entry` = 64248;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 65058;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 65061;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 65063;
UPDATE `creature_template` SET `faction_A` = 31 WHERE  `entry` = 65190;
UPDATE `creature_template` SET `faction_A` = 14 WHERE  `entry` = 68553;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 68657;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 68820;
UPDATE `creature_template` SET `faction_A` = 14 WHERE  `entry` = 72775;
UPDATE `creature_template` SET `faction_A` = 2482 WHERE  `entry` = 73000;
UPDATE `creature_template` SET `faction_A` = 2482 WHERE  `entry` = 73002;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 73037;
UPDATE `creature_template` SET `faction_A` = 2497 WHERE  `entry` = 73040;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 73819;
UPDATE `creature_template` SET `faction_A` = 954 WHERE  `entry` = 89;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 356;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 358;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 510;
UPDATE `creature_template` SET `faction_A` = 4 WHERE  `entry` = 1964;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 2523;
UPDATE `creature_template` SET `faction_A` = 16 WHERE  `entry` = 3425;
UPDATE `creature_template` SET `faction_A` = 16 WHERE  `entry` = 3475;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 4271;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 4272;
UPDATE `creature_template` SET `faction_A` = 16 WHERE  `entry` = 4514;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 6075;
UPDATE `creature_template` SET `faction_A` = 18 WHERE  `entry` = 6250;
UPDATE `creature_template` SET `faction_A` = 2358 WHERE  `entry` = 6505;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 7707;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 7708;
UPDATE `creature_template` SET `faction_A` = 16 WHERE  `entry` = 10741;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 11689;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 12149;
UPDATE `creature_template` SET `faction_A` = 59 WHERE  `entry` = 12922;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 14333;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 14539;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 14540;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 14541;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 14542;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 14543;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 14544;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 14545;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 14549;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 14550;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 14755;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 14756;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 15438;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 15439;
UPDATE `creature_template` SET `faction_A` = 38 WHERE  `entry` = 15652;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 15714;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 15715;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 15716;
UPDATE `creature_template` SET `faction_A` = 24 WHERE  `entry` = 17280;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 18363;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 18364;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 18365;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 18377;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 18378;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 18379;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 18380;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 19280;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 19281;
UPDATE `creature_template` SET `faction_A` = 14 WHERE  `entry` = 19349;
UPDATE `creature_template` SET `faction_A` = 1 WHERE  `entry` = 19668;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 20072;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 20150;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 20151;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 20152;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 20217;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 20220;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 20222;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 20223;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 20224;
UPDATE `creature_template` SET `faction_A` = 2 WHERE  `entry` = 33239;
UPDATE `creature_template` SET `faction_A` = 2395 WHERE  `entry` = 35387;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 36910;
UPDATE `creature_template` SET `faction_A` = 2395 WHERE  `entry` = 37865;
UPDATE `creature_template` SET `faction_A` = 2204 WHERE  `entry` = 40198;
UPDATE `creature_template` SET `faction_A` = 2203 WHERE  `entry` = 42177;
UPDATE `creature_template` SET `faction_A` = 2204 WHERE  `entry` = 43800;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 45322;
UPDATE `creature_template` SET `faction_A` = 2204 WHERE  `entry` = 46896;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 53276;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 54374;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 54383;
UPDATE `creature_template` SET `faction_A` = 7 WHERE  `entry` = 54491;
UPDATE `creature_template` SET `faction_A` = 2395 WHERE  `entry` = 55187;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 55356;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 59170;
UPDATE `creature_template` SET `faction_A` = 31 WHERE  `entry` = 59702;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 60561;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 60941;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61141;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61313;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61439;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61827;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61828;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61877;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 61883;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62313;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62315;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62829;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62852;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62887;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62894;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62907;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62915;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 62916;
UPDATE `creature_template` SET `faction_A` = 31 WHERE  `entry` = 62954;
UPDATE `creature_template` SET `faction_A` = 31 WHERE  `entry` = 63057;
UPDATE `creature_template` SET `faction_A` = 190 WHERE  `entry` = 63060;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 63098;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 63370;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 63508;
UPDATE `creature_template` SET `faction_A` = 31 WHERE  `entry` = 63549;
UPDATE `creature_template` SET `faction_A` = 31 WHERE  `entry` = 63557;
UPDATE `creature_template` SET `faction_A` = 31 WHERE  `entry` = 63585;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 63847;
UPDATE `creature_template` SET `faction_A` = 31 WHERE  `entry` = 63849;
UPDATE `creature_template` SET `faction_A` = 31 WHERE  `entry` = 63954;
UPDATE `creature_template` SET `faction_A` = 31 WHERE  `entry` = 63957;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 64632;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 65072;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 65074;
UPDATE `creature_template` SET `faction_A` = 188 WHERE  `entry` = 65124;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 65313;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 65314;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 66104;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 66105;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 66491;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 67022;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 67230;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 68211;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 71163;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 71486;
UPDATE `creature_template` SET `faction_A` = 16 WHERE  `entry` = 72769;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 73004;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 73011;
UPDATE `creature_template` SET `faction_A` = 2497 WHERE  `entry` = 73044;
UPDATE `creature_template` SET `faction_A` = 2497 WHERE  `entry` = 73048;
UPDATE `creature_template` SET `faction_A` = 35 WHERE  `entry` = 73050;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 73060;
UPDATE `creature_template` SET `faction_A` = 14 WHERE  `entry` = 73277;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 73352;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 73424;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 73534;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 74410;
UPDATE `creature_template` SET `faction_A` = 0 WHERE  `entry` = 77178;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 15361;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 69818;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 69819;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 69891;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 71033;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 71876;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 72151;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 72464;
UPDATE `creature_template` SET `faction_H` = 16 WHERE  `entry` = 72771;
UPDATE `creature_template` SET `faction_H` = 2497 WHERE  `entry` = 73041;
UPDATE `creature_template` SET `faction_H` = 2497 WHERE  `entry` = 73043;
UPDATE `creature_template` SET `faction_H` = 14 WHERE  `entry` = 73167;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 73648;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 73657;
UPDATE `creature_template` SET `faction_H` = 230 WHERE  `entry` = 73718;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 73812;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 77137;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 7380;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 7382;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 7384;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 7386;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 7389;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 7390;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 7391;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 7395;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 7543;
UPDATE `creature_template` SET `faction_H` = 5 WHERE  `entry` = 7544;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 7547;
UPDATE `creature_template` SET `faction_H` = 4 WHERE  `entry` = 7550;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 7554;
UPDATE `creature_template` SET `faction_H` = 2395 WHERE  `entry` = 7560;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 7561;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 7567;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 8376;
UPDATE `creature_template` SET `faction_H` = 1629 WHERE  `entry` = 9656;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 9657;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 9662;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 11325;
UPDATE `creature_template` SET `faction_H` = 3 WHERE  `entry` = 11326;
UPDATE `creature_template` SET `faction_H` = 4 WHERE  `entry` = 11327;
UPDATE `creature_template` SET `faction_H` = 115 WHERE  `entry` = 12419;
UPDATE `creature_template` SET `faction_H` = 7 WHERE  `entry` = 14421;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 15186;
UPDATE `creature_template` SET `faction_H` = 1610 WHERE  `entry` = 15358;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 15429;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 16445;
UPDATE `creature_template` SET `faction_H` = 4 WHERE  `entry` = 30379;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 63647;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 63649;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 63756;
UPDATE `creature_template` SET `faction_H` = 2392 WHERE  `entry` = 63810;
UPDATE `creature_template` SET `faction_H` = 14 WHERE  `entry` = 64523;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 64608;
UPDATE `creature_template` SET `faction_H` = 14 WHERE  `entry` = 64687;
UPDATE `creature_template` SET `faction_H` = 14 WHERE  `entry` = 64692;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 65006;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 65009;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 65060;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 65065;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 65071;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 65076;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 65078;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 65282;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 65428;
UPDATE `creature_template` SET `faction_H` = 2558 WHERE  `entry` = 66391;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 66564;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 66567;
UPDATE `creature_template` SET `faction_H` = 14 WHERE  `entry` = 66668;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 73305;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 15698;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 15699;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 15705;
UPDATE `creature_template` SET `faction_H` = 4 WHERE  `entry` = 15706;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 15710;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 16456;
UPDATE `creature_template` SET `faction_H` = 1629 WHERE  `entry` = 16547;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 16548;
UPDATE `creature_template` SET `faction_H` = 115 WHERE  `entry` = 16549;
UPDATE `creature_template` SET `faction_H` = 4 WHERE  `entry` = 16701;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 17255;
UPDATE `creature_template` SET `faction_H` = 6 WHERE  `entry` = 18381;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 20408;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 21008;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 21009;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 21010;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 21018;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 21055;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 21056;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 21063;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 21064;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 21076;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 22445;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 22943;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 23231;
UPDATE `creature_template` SET `faction_H` = 534 WHERE  `entry` = 23258;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 23909;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 24388;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 24389;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 24480;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 24753;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 25109;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 25110;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 25146;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 25147;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 25706;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 26050;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 26056;
UPDATE `creature_template` SET `faction_H` = 1610 WHERE  `entry` = 26119;
UPDATE `creature_template` SET `faction_H` = 2395 WHERE  `entry` = 27217;
UPDATE `creature_template` SET `faction_H` = 14 WHERE  `entry` = 28470;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 28513;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 28883;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 29089;
UPDATE `creature_template` SET `faction_H` = 5 WHERE  `entry` = 29726;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 31575;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 32590;
UPDATE `creature_template` SET `faction_H` = 2395 WHERE  `entry` = 32591;
UPDATE `creature_template` SET `faction_H` = 2395 WHERE  `entry` = 32592;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 32595;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 32791;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 32841;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 32939;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 33188;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 33194;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 63197;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 63647;
UPDATE `creature_template` SET `faction_H` = 2203 WHERE  `entry` = 7387;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 33194;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 33197;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 33198;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 33200;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 33205;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 33219;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 33238;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 33529;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 33578;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 33810;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 34364;
UPDATE `creature_template` SET `faction_H` = 2395 WHERE  `entry` = 34587;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 34694;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 34724;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 34930;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 35394;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 35395;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 35396;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 35397;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 35398;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 35399;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 35400;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 35468;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 36482;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 36511;
UPDATE `creature_template` SET `faction_H` = 1629 WHERE  `entry` = 36607;
UPDATE `creature_template` SET `faction_H` = 2204 WHERE  `entry` = 36871;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 36909;
UPDATE `creature_template` SET `faction_H` = 2395 WHERE  `entry` = 36911;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 36979;
UPDATE `creature_template` SET `faction_H` = 2203 WHERE  `entry` = 40295;
UPDATE `creature_template` SET `faction_H` = 2203 WHERE  `entry` = 40624;
UPDATE `creature_template` SET `faction_H` = 2203 WHERE  `entry` = 40703;
UPDATE `creature_template` SET `faction_H` = 1629 WHERE  `entry` = 42078;
UPDATE `creature_template` SET `faction_H` = 2203 WHERE  `entry` = 43916;
UPDATE `creature_template` SET `faction_H` = 2204 WHERE  `entry` = 45128;
UPDATE `creature_template` SET `faction_H` = 2204 WHERE  `entry` = 45247;
UPDATE `creature_template` SET `faction_H` = 2395 WHERE  `entry` = 47944;
UPDATE `creature_template` SET `faction_H` = 2203 WHERE  `entry` = 48107;
UPDATE `creature_template` SET `faction_H` = 2204 WHERE  `entry` = 48242;
UPDATE `creature_template` SET `faction_H` = 2203 WHERE  `entry` = 48609;
UPDATE `creature_template` SET `faction_H` = 2203 WHERE  `entry` = 48641;
UPDATE `creature_template` SET `faction_H` = 2203 WHERE  `entry` = 48982;
UPDATE `creature_template` SET `faction_H` = 2203 WHERE  `entry` = 49586;
UPDATE `creature_template` SET `faction_H` = 2203 WHERE  `entry` = 49587;
UPDATE `creature_template` SET `faction_H` = 2204 WHERE  `entry` = 49588;
UPDATE `creature_template` SET `faction_H` = 2204 WHERE  `entry` = 49590;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 50468;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 50545;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 50586;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 51090;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 51122;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 51600;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 51601;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 51632;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 51635;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 52226;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 52343;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 52344;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 52831;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 53048;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 53232;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 53283;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 53623;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 53658;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 53661;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 53884;
UPDATE `creature_template` SET `faction_H` = 1629 WHERE  `entry` = 54027;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 54227;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 54438;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 54539;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 54541;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61604;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61714;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61809;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62106;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62376;
UPDATE `creature_template` SET `faction_H` = 2482 WHERE  `entry` = 72993;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 73668;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 7381;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 7555;
UPDATE `creature_template` SET `faction_H` = 6 WHERE  `entry` = 14878;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 18839;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 20472;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 54730;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 54984;
UPDATE `creature_template` SET `faction_H` = 14 WHERE  `entry` = 55236;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 55367;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 55386;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 55574;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 56031;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 56266;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 56280;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 56382;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 58163;
UPDATE `creature_template` SET `faction_H` = 14 WHERE  `entry` = 58456;
UPDATE `creature_template` SET `faction_H` = 190 WHERE  `entry` = 58941;
UPDATE `creature_template` SET `faction_H` = 2203 WHERE  `entry` = 58960;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 58963;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 58964;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 59020;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 59101;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 59399;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 59454;
UPDATE `creature_template` SET `faction_H` = 16 WHERE  `entry` = 59580;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 59712;
UPDATE `creature_template` SET `faction_H` = 2492 WHERE  `entry` = 59830;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 60698;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 60763;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 60800;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61071;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 61080;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 61081;
UPDATE `creature_template` SET `faction_H` = 190 WHERE  `entry` = 61088;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61142;
UPDATE `creature_template` SET `faction_H` = 4 WHERE  `entry` = 61160;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61165;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61168;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61169;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61253;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61255;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61257;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61258;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61312;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61314;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61317;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61318;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61319;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61321;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61323;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61324;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61328;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61367;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61369;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61370;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61372;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61384;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61385;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61386;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61425;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61438;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61440;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61441;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61472;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61689;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61690;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61691;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61718;
UPDATE `creature_template` SET `faction_H` = 1629 WHERE  `entry` = 61757;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61758;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 66150;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 68805;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 68839;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 69748;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 69794;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 70025;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 70026;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 71016;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 71021;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 73010;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 73052;
UPDATE `creature_template` SET `faction_H` = 16 WHERE  `entry` = 73157;
UPDATE `creature_template` SET `faction_H` = 7 WHERE  `entry` = 73297;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 73338;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 73357;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 73366;
UPDATE `creature_template` SET `faction_H` = 4 WHERE  `entry` = 2671;
UPDATE `creature_template` SET `faction_H` = 116 WHERE  `entry` = 7383;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 7385;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 7394;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 7545;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 7549;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 7553;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 7562;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 7565;
UPDATE `creature_template` SET `faction_H` = 4 WHERE  `entry` = 10598;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 16069;
UPDATE `creature_template` SET `faction_H` = 4 WHERE  `entry` = 16085;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 23198;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 23234;
UPDATE `creature_template` SET `faction_H` = 2395 WHERE  `entry` = 23266;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 23274;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 24968;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 25062;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 27346;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 29147;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 40054;
UPDATE `creature_template` SET `faction_H` = 2203 WHERE  `entry` = 40819;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 43050;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 46754;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 46755;
UPDATE `creature_template` SET `faction_H` = 2204 WHERE  `entry` = 47244;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 48636;
UPDATE `creature_template` SET `faction_H` = 7 WHERE  `entry` = 49565;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61758;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61826;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61830;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62019;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62020;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62022;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62034;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62051;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62114;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62115;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62116;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62117;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62118;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62119;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62120;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62121;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62129;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62177;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62184;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62187;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62189;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62190;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62201;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62246;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62250;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62257;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62258;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62312;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62314;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62316;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62370;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62373;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62896;
UPDATE `creature_template` SET `faction_H` = 14 WHERE  `entry` = 65580;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 68662;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 69648;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 70104;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 70154;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 72007;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 72676;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 73023;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 73024;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 73042;
UPDATE `creature_template` SET `faction_H` = 2497 WHERE  `entry` = 73049;
UPDATE `creature_template` SET `faction_H` = 2497 WHERE  `entry` = 73053;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 73306;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 73364;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 73702;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 73780;
UPDATE `creature_template` SET `faction_H` = 2399 WHERE  `entry` = 73821;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 73967;
UPDATE `creature_template` SET `faction_H` = 2395 WHERE  `entry` = 10259;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 15666;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 20225;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 22510;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 22512;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 22513;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 22514;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 22958;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 22975;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 22978;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 23408;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 24488;
UPDATE `creature_template` SET `faction_H` = 16 WHERE  `entry` = 25721;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 26131;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 27893;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 27914;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 28017;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 29238;
UPDATE `creature_template` SET `faction_H` = 1712 WHERE  `entry` = 30164;
UPDATE `creature_template` SET `faction_H` = 2107 WHERE  `entry` = 30320;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 31216;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 31698;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 31717;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 31778;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 31852;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 32158;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 32207;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 32336;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 32640;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 33029;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 33274;
UPDATE `creature_template` SET `faction_H` = 2395 WHERE  `entry` = 33530;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 34155;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 34770;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 36908;
UPDATE `creature_template` SET `faction_H` = 1629 WHERE  `entry` = 38374;
UPDATE `creature_template` SET `faction_H` = 2204 WHERE  `entry` = 42183;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 45340;
UPDATE `creature_template` SET `faction_H` = 2204 WHERE  `entry` = 46898;
UPDATE `creature_template` SET `faction_H` = 14 WHERE  `entry` = 50159;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 51649;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 52894;
UPDATE `creature_template` SET `faction_H` = 4 WHERE  `entry` = 53225;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 54128;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 54487;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 54745;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 55571;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 59243;
UPDATE `creature_template` SET `faction_H` = 1711 WHERE  `entry` = 59534;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 60649;
UPDATE `creature_template` SET `faction_H` = 190 WHERE  `entry` = 60700;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61086;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61143;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61167;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61171;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61322;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61325;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61326;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61329;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61368;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61383;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61443;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61459;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61677;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61686;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61704;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61711;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61713;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61751;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61753;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61755;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61829;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61889;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61890;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62050;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62127;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62178;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62181;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62182;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62185;
UPDATE `creature_template` SET `faction_H` = 7 WHERE  `entry` = 62242;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62255;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62256;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62364;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62395;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62454;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62523;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62524;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62526;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62555;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62564;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62620;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62625;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62638;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62641;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62664;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62669;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62693;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62695;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62697;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62815;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62816;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62818;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62819;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62820;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62835;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62854;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62864;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62884;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62885;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62886;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62892;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62893;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62895;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62899;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62900;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62905;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62914;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62921;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62922;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62925;
UPDATE `creature_template` SET `faction_H` = 190 WHERE  `entry` = 62994;
UPDATE `creature_template` SET `faction_H` = 31 WHERE  `entry` = 63057;
UPDATE `creature_template` SET `faction_H` = 31 WHERE  `entry` = 63096;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 63097;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 63365;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 63555;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 63559;
UPDATE `creature_template` SET `faction_H` = 2395 WHERE  `entry` = 63832;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 63838;
UPDATE `creature_template` SET `faction_H` = 31 WHERE  `entry` = 63953;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 64232;
UPDATE `creature_template` SET `faction_H` = 31 WHERE  `entry` = 64248;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 65058;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 65061;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 65063;
UPDATE `creature_template` SET `faction_H` = 31 WHERE  `entry` = 65190;
UPDATE `creature_template` SET `faction_H` = 14 WHERE  `entry` = 68553;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 68657;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 68820;
UPDATE `creature_template` SET `faction_H` = 14 WHERE  `entry` = 72775;
UPDATE `creature_template` SET `faction_H` = 2482 WHERE  `entry` = 73000;
UPDATE `creature_template` SET `faction_H` = 2482 WHERE  `entry` = 73002;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 73037;
UPDATE `creature_template` SET `faction_H` = 2497 WHERE  `entry` = 73040;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 73819;
UPDATE `creature_template` SET `faction_H` = 954 WHERE  `entry` = 89;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 356;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 358;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 510;
UPDATE `creature_template` SET `faction_H` = 4 WHERE  `entry` = 1964;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 2523;
UPDATE `creature_template` SET `faction_H` = 16 WHERE  `entry` = 3425;
UPDATE `creature_template` SET `faction_H` = 16 WHERE  `entry` = 3475;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 4271;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 4272;
UPDATE `creature_template` SET `faction_H` = 16 WHERE  `entry` = 4514;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 6075;
UPDATE `creature_template` SET `faction_H` = 18 WHERE  `entry` = 6250;
UPDATE `creature_template` SET `faction_H` = 2358 WHERE  `entry` = 6505;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 7707;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 7708;
UPDATE `creature_template` SET `faction_H` = 16 WHERE  `entry` = 10741;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 11689;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 12149;
UPDATE `creature_template` SET `faction_H` = 59 WHERE  `entry` = 12922;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 14333;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 14539;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 14540;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 14541;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 14542;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 14543;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 14544;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 14545;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 14549;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 14550;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 14755;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 14756;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 15438;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 15439;
UPDATE `creature_template` SET `faction_H` = 38 WHERE  `entry` = 15652;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 15714;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 15715;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 15716;
UPDATE `creature_template` SET `faction_H` = 24 WHERE  `entry` = 17280;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 18363;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 18364;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 18365;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 18377;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 18378;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 18379;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 18380;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 19280;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 19281;
UPDATE `creature_template` SET `faction_H` = 14 WHERE  `entry` = 19349;
UPDATE `creature_template` SET `faction_H` = 1 WHERE  `entry` = 19668;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 20072;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 20150;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 20151;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 20152;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 20217;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 20220;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 20222;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 20223;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 20224;
UPDATE `creature_template` SET `faction_H` = 2 WHERE  `entry` = 33239;
UPDATE `creature_template` SET `faction_H` = 2395 WHERE  `entry` = 35387;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 36910;
UPDATE `creature_template` SET `faction_H` = 2395 WHERE  `entry` = 37865;
UPDATE `creature_template` SET `faction_H` = 2204 WHERE  `entry` = 40198;
UPDATE `creature_template` SET `faction_H` = 2203 WHERE  `entry` = 42177;
UPDATE `creature_template` SET `faction_H` = 2204 WHERE  `entry` = 43800;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 45322;
UPDATE `creature_template` SET `faction_H` = 2204 WHERE  `entry` = 46896;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 53276;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 54374;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 54383;
UPDATE `creature_template` SET `faction_H` = 7 WHERE  `entry` = 54491;
UPDATE `creature_template` SET `faction_H` = 2395 WHERE  `entry` = 55187;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 55356;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 59170;
UPDATE `creature_template` SET `faction_H` = 31 WHERE  `entry` = 59702;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 60561;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 60941;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61141;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61313;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61439;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61827;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61828;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61877;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 61883;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62313;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62315;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62829;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62852;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62887;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62894;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62907;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62915;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 62916;
UPDATE `creature_template` SET `faction_H` = 31 WHERE  `entry` = 62954;
UPDATE `creature_template` SET `faction_H` = 31 WHERE  `entry` = 63057;
UPDATE `creature_template` SET `faction_H` = 190 WHERE  `entry` = 63060;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 63098;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 63370;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 63508;
UPDATE `creature_template` SET `faction_H` = 31 WHERE  `entry` = 63549;
UPDATE `creature_template` SET `faction_H` = 31 WHERE  `entry` = 63557;
UPDATE `creature_template` SET `faction_H` = 31 WHERE  `entry` = 63585;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 63847;
UPDATE `creature_template` SET `faction_H` = 31 WHERE  `entry` = 63849;
UPDATE `creature_template` SET `faction_H` = 31 WHERE  `entry` = 63954;
UPDATE `creature_template` SET `faction_H` = 31 WHERE  `entry` = 63957;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 64632;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 65072;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 65074;
UPDATE `creature_template` SET `faction_H` = 188 WHERE  `entry` = 65124;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 65313;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 65314;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 66104;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 66105;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 66491;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 67022;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 67230;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 68211;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 71163;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 71486;
UPDATE `creature_template` SET `faction_H` = 16 WHERE  `entry` = 72769;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 73004;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 73011;
UPDATE `creature_template` SET `faction_H` = 2497 WHERE  `entry` = 73044;
UPDATE `creature_template` SET `faction_H` = 2497 WHERE  `entry` = 73048;
UPDATE `creature_template` SET `faction_H` = 35 WHERE  `entry` = 73050;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 73060;
UPDATE `creature_template` SET `faction_H` = 14 WHERE  `entry` = 73277;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 73352;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 73424;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 73534;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 74410;
UPDATE `creature_template` SET `faction_H` = 0 WHERE  `entry` = 77178;
DELETE FROM `creature_queststarter` WHERE  `id`= 37070;
DELETE FROM `creature_queststarter` WHERE  `id`= 50039;
DELETE FROM `creature_queststarter` WHERE  `id`= 2031;
DELETE FROM `creature_queststarter` WHERE  `id`= 46363;
DELETE FROM `creature_queststarter` WHERE  `id`= 46268;
DELETE FROM `creature_queststarter` WHERE  `id`= 42563;
DELETE FROM `creature_queststarter` WHERE  `id`= 42645;
DELETE FROM `creature_queststarter` WHERE  `id`= 42563;
DELETE FROM `creature_queststarter` WHERE  `id`= 42223;
DELETE FROM `creature_queststarter` WHERE  `id`= 42224;
DELETE FROM `creature_queststarter` WHERE  `id`= 35830;
DELETE FROM `creature_queststarter` WHERE  `id`= 10556;
DELETE FROM `creature_queststarter` WHERE  `id`= 1501;
DELETE FROM `creature_queststarter` WHERE  `id`= 49340;
DELETE FROM `creature_queststarter` WHERE  `id`= 38038;
DELETE FROM `creature_queststarter` WHERE  `id`= 38142;
DELETE FROM `creature_queststarter` WHERE  `id`= 38300;
DELETE FROM `creature_queststarter` WHERE  `id`= 38306;
DELETE FROM `creature_queststarter` WHERE  `id`= 38560;
DELETE FROM `creature_queststarter` WHERE  `id`= 15274;
DELETE FROM `creature_queststarter` WHERE  `id`= 34830;
DELETE FROM `creature_queststarter` WHERE  `id`= 34865;
DELETE FROM `creature_queststarter` WHERE  `id`= 34877;
DELETE FROM `creature_queststarter` WHERE  `id`= 34876;
DELETE FROM `creature_queststarter` WHERE  `id`= 34878;
DELETE FROM `creature_queststarter` WHERE  `id`= 34835;-- Delete stray stuff in booty bay
DELETE FROM gameobject WHERE guid IN (9412, 9413, 9414, 9415, 9416, 9417, 9418, 9419, 9420, 9438, 9439, 9440, 9441, 9442, 9443, 9444, 9445, 9446);

DELETE FROM creature WHERE id IN (25094, 25111, 25082, 25089, 25078, 25098, 25096, 25097, 25093, 25095);
ALTER TABLE `creature_template` CHANGE `minlevel` `minlevel` SMALLINT(5) NOT NULL DEFAULT '1' AFTER `gossip_menu_id`;
ALTER TABLE `creature_template` CHANGE `maxlevel` `maxlevel` SMALLINT(5) NOT NULL DEFAULT '1' AFTER `minlevel`;

UPDATE `creature_template` SET `exp`=-1 WHERE `entry` IN (68, 309, 347, 352, 523, 603, 853, 857, 907, 931, 1233, 1284, 1387, 1571, 1572, 1573, 1642, 1652, 1735, 1736, 1737, 1739, 1740, 1741, 1756, 1976, 2041, 2225, 2226, 2299, 2302, 2389, 2409, 2432, 2804, 2835, 2851, 2858, 2859, 2861, 2941, 2995, 3083, 3084, 3210, 3211, 3213, 3214, 3296, 3305, 3310, 3343, 3468, 3469, 3502, 3575, 3615, 3625, 3838, 3841, 3890, 3936, 4255, 4257, 4262, 4267, 4309, 4310, 4312, 4314, 4317, 4319, 4321, 4407, 4423, 4551, 4624, 4968, 5118, 5134, 5135, 5139, 5595, 5597, 5603, 5624, 5952, 6026, 6526, 6706, 6726, 7410, 7427, 7823, 7824, 7975, 7980, 7999, 8018, 8019, 8020, 8055, 8096, 8609, 8610, 8670, 8719, 9297, 9460, 9521, 9526, 9527, 10038, 10181, 10360, 10364, 10367, 10583, 10897, 10981, 10982, 10985, 10986, 10987, 10989, 10990, 10991, 11099, 11102, 11138, 11139, 11190, 11194, 11600, 11602, 11603, 11604, 11605, 11606, 11657, 11675, 11676, 11677, 11678, 11822, 11837, 11838, 11839, 11840, 11899, 11900, 11901, 11946, 11947, 11948, 11949, 11997, 11998, 12047, 12048, 12050, 12051, 12052, 12053, 12096, 12097, 12121, 12122, 12127, 12159, 12160, 12197, 12198, 12577, 12578, 12596, 12616, 12617, 12636, 12740, 13078, 13079, 13080, 13081, 13086, 13087, 13088, 13089, 13096, 13097, 13098, 13099, 13116, 13117, 13137, 13138, 13139, 13140, 13143, 13144, 13145, 13146, 13147, 13149, 13150, 13151, 13152, 13153, 13154, 13155, 13161, 13176, 13177, 13178, 13179, 13180, 13181, 13216, 13218, 13221, 13236, 13256, 13257, 13281, 13284, 13296, 13297, 13298, 13299, 13300, 13316, 13317, 13318, 13319, 13320, 13324, 13325, 13326, 13327, 13328, 13329, 13330, 13331, 13332, 13333, 13334, 13335, 13336, 13337, 13356, 13357, 13358, 13359, 13377, 13378, 13396, 13397, 13416, 13419, 13421, 13422, 13424, 13425, 13426, 13427, 13428, 13437, 13438, 13439, 13440, 13441, 13442, 13443, 13446, 13447, 13448, 13449, 13496, 13516, 13517, 13518, 13519, 13520, 13521, 13522, 13523, 13524, 13525, 13526, 13527, 13528, 13529, 13530, 13531, 13534, 13535, 13536, 13537, 13538, 13539, 13540, 13541, 13542, 13543, 13544, 13545, 13546, 13547, 13548, 13549, 13550, 13551, 13552, 13553, 13554, 13555, 13556, 13557, 13576, 13577, 13597, 13598, 13616, 13617, 13618, 13676, 13777, 13797, 13798, 13839, 13959, 13977, 14016, 14018, 14019, 14026, 14027, 14028, 14029, 14030, 14031, 14141, 14142, 14185, 14186, 14187, 14188, 14242, 14282, 14283, 14284, 14285, 14304, 14363, 14365, 14367, 14375, 14376, 14377, 14378, 14379, 14380, 14402, 14403, 14404, 14423, 14438, 14439, 14440, 14441, 14442, 14622, 14717, 14762, 14763, 14764, 14765, 14766, 14767, 14768, 14769, 14770, 14771, 14772, 14773, 14774, 14775, 14776, 14777, 14832, 14848, 14913, 14942, 14943, 14944, 14945, 14946, 14947, 14948, 14981, 14982, 15006, 15007, 15008, 15088, 15139, 15177, 15178, 15184, 15193, 15195, 15371, 15466, 15467, 15518, 15519, 15659, 15961, 16096, 16189, 16192, 16222, 16227, 16378, 16392, 16587, 16694, 16695, 16696, 16711, 16733, 16800, 16801, 16802, 16822, 16921, 17468, 17506, 17507, 17554, 17555, 17855, 18103, 18139, 18785, 18788, 18789, 18791, 18807, 18808, 18809, 18895, 18930, 18931, 18937, 18938, 18939, 18940, 18942, 18953, 18989, 19317, 19504, 19529, 19541, 19558, 19581, 19583, 19855, 19858, 19859, 19905, 19906, 19907, 19908, 19909, 19910, 19911, 19912, 19915, 19923, 19925, 20118, 20119, 20120, 20234, 20269, 20271, 20272, 20273, 20274, 20276, 20362, 20374, 20381, 20382, 20383, 20384, 20385, 20386, 20388, 20390, 20447, 20497, 20499, 20515, 20556, 20674, 20762, 21107, 21235, 21766, 21984, 21986, 22216, 22407, 22455, 22485, 22494, 22522, 22931, 22935, 22937, 23090, 23115, 23612, 23619, 23623, 23624, 23625, 23626, 23636, 23736, 23775, 23859, 24032, 24061, 24155, 24366, 24470, 24795, 24851, 25040, 25145, 25148, 25149, 25150, 25151, 25152, 25288, 25499, 25991, 26016, 26017, 26019, 26020, 26021, 26024, 26027, 26028, 26029, 26030, 26031, 26032, 26033, 26037, 26038, 26253, 26560, 26566, 26602, 26844, 26845, 26847, 26848, 26850, 26851, 26852, 26853, 26876, 26877, 26878, 26879, 26880, 26881, 27046, 27170, 27175, 27344, 27881, 27894, 27946, 27953, 28037, 28094, 28195, 28196, 28197, 28312, 28319, 28366, 28574, 28613, 28615, 28618, 28621, 28623, 28624, 28674, 28781, 28801, 28818, 28863, 28865, 29260, 29262, 29480, 29533, 29568, 29611, 29617, 29712, 29721, 29749, 29750, 29757, 29762, 29910, 29950, 29951, 30231, 30269, 30271, 30280, 30312, 30314, 30400, 30433, 30488, 30489, 30499, 30560, 30569, 30578, 30579, 30580, 30581, 30582, 30583, 30584, 30586, 30587, 30590, 30610, 30611, 30626, 30737, 30739, 30740, 30853, 30869, 30870, 31036, 31051, 31052, 31053, 31054, 31069, 31078, 31091, 31101, 31102, 31106, 31107, 31108, 31109, 31146, 31151, 31153, 31250, 31416, 31426, 31430, 31708, 31715, 31841, 31842, 32169, 32170, 32294, 32296, 32307, 32308, 32330, 32332, 32333, 32571, 32615, 32616, 32617, 32618, 32619, 32620, 32621, 32622, 32623, 32624, 32625, 32626, 32627, 32629, 33253, 33254, 33502, 33504, 33505, 33506, 33508, 33510, 33511, 33849, 34018, 34019, 34021, 34022, 34024, 34025, 34026, 34027, 34028, 34029, 34374, 34378, 34429, 34775, 34776, 34777, 34778, 34793, 34802, 34918, 34919, 34922, 34924, 34927, 34929, 34935, 34943, 34944, 34955, 34971, 34972, 34973, 34976, 34978, 34983, 34985, 34986, 34987, 34988, 34989, 34991, 34993, 34997, 34998, 34999, 35000, 35001, 35002, 35007, 35008, 35017, 35019, 35020, 35021, 35022, 35023, 35024, 35025, 35026, 35027, 35069, 35136, 35137, 35138, 35139, 35140, 35141, 35273, 35315, 35322, 35345, 35346, 35450, 35478, 35480, 35481, 35556, 35562, 35596, 35597, 35600, 35601, 35602, 35603, 35611, 35612, 35819, 36151, 36152, 36162, 36164, 36165, 36166, 36213, 36217, 36224, 36225, 36226, 36273, 36355, 36356, 36481, 36648, 36728, 37005, 37058, 37790, 37825, 37860, 37869, 37888, 37915, 37944, 38217, 38831, 38898, 38899, 38966, 39172, 39173, 39175, 39196, 39210, 39211, 39212, 39330, 39340, 39605, 39656, 39898, 39934, 40138, 40333, 40358, 40367, 40413, 40473, 40552, 40553, 40558, 40768, 40769, 40809, 40827, 40851, 40852, 40866, 40867, 40871, 40873, 40966, 41140, 41142, 41214, 41215, 41240, 41246, 41321, 41322, 41323, 41325, 41332, 41383, 41580, 41605, 41860, 41861, 41896, 42129, 42131, 42316, 42317, 42319, 42406, 42426, 42623, 42928, 42974, 42983, 43000, 43042, 43043, 43045, 43049, 43052, 43053, 43072, 43073, 43079, 43085, 43086, 43087, 43088, 43104, 43107, 43114, 43121, 43124, 43216, 43220, 43225, 43287, 43289, 43290, 43293, 43295, 43328, 43371, 43375, 43427, 43440, 43481, 43549, 43570, 43573, 43690, 43695, 43697, 43701, 43702, 43841, 43842, 43845, 43867, 43889, 43931, 43991, 44036, 44059, 44060, 44230, 44231, 44232, 44233, 44244, 44399, 44407, 44408, 44409, 44410, 44440, 44441, 44451, 44787, 44825, 44865, 44866, 44867, 44868, 44871, 44872, 44874, 44876, 44877, 44878, 45015, 45042, 45082, 45111, 45230, 45250, 45253, 45344, 45382, 45479, 45564, 45659, 45814, 45822, 45827, 45829, 46004, 46006, 46011, 46071, 46077, 46142, 46180, 46181, 46190, 46236, 46552, 46637, 46638, 46639, 46640, 46667, 46717, 46757, 46833, 46844, 46986, 47060, 47061, 47116, 47118, 47119, 47121, 47133, 47147, 47154, 47155, 47156, 47174, 47434, 47581, 47582, 47584, 47586, 47587, 47588, 47589, 47644, 47655, 47661, 47663, 47665, 47688, 47718, 47736, 47745, 47844, 47875, 47927, 48075, 48273, 48274, 48275, 48318, 48321, 48546, 48572, 48736, 48738, 48874, 48883, 49038, 49420, 49533, 49573, 49809, 49820, 50043, 50083, 50084, 50369, 50440, 50441, 50442, 50443, 50445, 50463, 50546, 50548, 50549, 50550, 50551, 50552, 50553, 50668, 50670, 50674, 50676, 50678, 50683, 50684, 51257, 51309, 51310, 51323, 51334, 51346, 51348, 51355, 51371, 51372, 51383, 51384, 51429, 51450, 51451, 51468, 51471, 51472, 51473, 51474, 51475, 51476, 51478, 51507, 51508, 51509, 51510, 51511, 51513, 51514, 51515, 51516, 51517, 51518, 51519, 51520, 51521, 51522, 51523, 51524, 51525, 51532, 51533, 51534, 51535, 51536, 51537, 51538, 51539, 51540, 51541, 51542, 51543, 51544, 51545, 51546, 51547, 51548, 51549, 51554, 51571, 51605, 51610, 51666, 51732, 51768, 51769, 51770, 51771, 51772, 51773, 51774, 51775, 51776, 51777, 51778, 51779, 51780, 51781, 51782, 51783, 51784, 51785, 51786, 51787, 51788, 51789, 51790, 51791, 51792, 51793, 51794, 51795, 51796, 51797, 51798, 51799, 51800, 51801, 51802, 51803, 51804, 51805, 51806, 51807, 51808, 51809, 51810, 51811, 51812, 51813, 51814, 51815, 51816, 51817, 51818, 51819, 51820, 51821, 51822, 51823, 51824, 51825, 51827, 51828, 51829, 51830, 51831, 51832, 51834, 51835, 51837, 51838, 51839, 51840, 51841, 51842, 51843, 51844, 51845, 51846, 51847, 51848, 51849, 51850, 51851, 51852, 51853, 51854, 51855, 51856, 51857, 51858, 51859, 51860, 51861, 51862, 51863, 51864, 51867, 51868, 51869, 51870, 51871, 51872, 51873, 51874, 51875, 51876, 51877, 51878, 51879, 51880, 51881, 51882, 51883, 51884, 51885, 51886, 51887, 51888, 51889, 51890, 51891, 51892, 51893, 51894, 51895, 51896, 51897, 51898, 51899, 51900, 51901, 51902, 51903, 51904, 51905, 51906, 51907, 51908, 51909, 51910, 51911, 51912, 51913, 51914, 51915, 51916, 51917, 51918, 51919, 51920, 51921, 51922, 51923, 51924, 51925, 51938, 51986, 52009, 52010, 52011, 52012, 52013, 52018, 52060, 52084, 52184, 52280, 52404, 52410, 52646, 52670, 52700, 52753, 52767, 52792, 52817, 52821, 52983, 52996, 53000, 53008, 53352, 53518, 53537, 53562, 53563, 53598, 53649, 53657, 53660, 53663, 53667, 53669, 53676, 53736, 53755, 53783, 53905, 53956, 53957, 53958, 53959, 53960, 54116, 54168, 54208, 54209, 54226, 54260, 54297, 54300, 54301, 54302, 54306, 54313, 54334, 54344, 54345, 54346, 54392, 54393, 54441, 54442, 54443, 54471, 54472, 54473, 54499, 54502, 54509, 54519, 54523, 54524, 54548, 54601, 54788, 55003, 55382, 55396, 55402, 55520, 55561, 55568, 55577, 55578, 55657, 55660, 55778, 55779, 55805, 55810, 55842, 55844, 55860, 55861, 55904, 55911, 55912, 55913, 55914, 55967, 55968, 55969, 55971, 55996, 56041, 56139, 56140, 56141, 56142, 56143, 56144, 56160, 56737, 56771, 57281, 57287, 57288, 57289, 57473, 57474, 57475, 57629, 57630, 57800, 57801, 58181, 58255, 58336, 58505, 58612, 58765, 58770, 58777, 58782, 58784, 58787, 58788, 58792, 58816, 58840, 58855, 58870, 58877, 58878, 58912, 58913, 58914, 58929, 58934, 58936, 58939, 58948, 59048, 59086, 59087, 59088, 59089, 59317, 59375, 59471, 59595, 59596, 59604, 59654, 59658, 59812, 59922, 59923, 59952, 60152, 60219, 60226, 60227, 60228, 60969, 60971, 61138, 61478, 61744, 61745, 61759, 62087, 62092, 62334, 62742, 62808, 62824, 62827, 62841, 62911, 62912, 62913, 62926, 63351, 63357, 63364, 63372, 63373, 64044, 64045, 64250, 64479, 64519, 64526, 64529, 64560, 64564, 64565, 64573, 64574, 64584, 64586, 64587, 64600, 64615, 64629, 64709, 64727, 64728, 64729, 64732, 64733, 64734, 64735, 64736, 64900, 64923, 64956, 64957, 64997, 64998, 65138, 65151, 65152, 65154, 65227, 65228, 65232, 65236, 65237, 65238, 65239, 65240, 65241, 65310, 65442, 65444, 65474, 65494, 65507, 65510, 65539, 65540, 65548, 65549, 65609, 65610, 65611, 65643, 65654, 65680, 65785, 65786, 65801, 65807, 65863, 65865, 65867, 66091, 66458, 66666, 66913, 67013, 67014, 67149, 67150, 67195, 67196, 67197, 67198, 67199, 67200, 67260, 67267, 67418, 67560, 67580, 67581, 67754, 67785, 67790, 67796, 67840, 67864, 67867, 67923, 67944, 67945, 67947, 67953, 67965, 67968, 67973, 67979, 67987, 67989, 67992, 68003, 68008, 68023, 68025, 68053, 68054, 68059, 68072, 68084, 68085, 68086, 68088, 68108, 68144, 68216, 68223, 68226, 68235, 68236, 68237, 68242, 68278, 68287, 68293, 68315, 68323, 68329, 68336, 68339, 68357, 68363, 68364, 68365, 68370, 68372, 68392, 68399, 68403, 68404, 68405, 68406, 68408, 68424, 68425, 68429, 68430, 68496, 68514, 68538, 68544, 68586, 68589, 68590, 68592, 68593, 68609, 68631, 68633, 68634, 68637, 68642, 68643, 68644, 68645, 68651, 68652, 68677, 68678, 68679, 68689, 68690, 68749, 68764, 68765, 68767, 68768, 68777, 68792, 68794, 68842, 68882, 68920, 68921, 68922, 68924, 68930, 68941, 68955, 68966, 68970, 68972, 68975, 68979, 68980, 68981, 68982, 68990, 68991, 68992, 68994, 68995, 68996, 69026, 69359, 69360, 69422, 69797, 69799, 69838, 70085, 70086, 70087, 70088, 70131, 70132, 70245, 70253, 70309, 70337, 70354, 70362, 70363, 70370, 70371, 70381, 70498, 70643, 70682, 70690, 70691, 70705, 70706, 70741, 70799, 70822, 70824, 70877, 70959, 70978, 71042, 71111, 71112, 71123, 71147, 71148, 71149, 71151, 71177, 71210, 71246, 71249, 71250, 71265, 71280, 71302, 71303, 71340, 71344, 71358, 71359, 71360, 71369, 71487, 71490, 71491, 71492, 71589, 71592, 71718, 71719, 71794, 71837, 71972, 71973, 71974, 71982, 71992, 72005, 72038, 72227, 72302, 72311, 72312, 72313, 72318, 72366, 72371, 72376, 72392, 72394, 72422, 72423, 72424, 72425, 72501, 72515, 72534, 72548, 72549, 72559, 72560, 72561, 72570, 72579, 72580, 72581, 72614, 72774, 72886, 72911, 72912, 73082, 73120, 73121, 73122, 73343, 73347, 73361, 73362, 73385, 73386, 73554, 73558, 73561, 73562, 73563, 73584, 73585, 73586, 73587, 73588, 73600, 73602, 73614, 73664, 73709, 73710, 73711, 73712, 73713, 73714, 73715, 73726, 73727, 73728, 73811, 73818, 73820, 73909, 73910, 73927, 73929, 73941, 73942, 73957, 73962, 74060, 74061, 74089, 74217, 74219, 76357);

UPDATE `creature_template` SET `minlevel`=-10 WHERE `entry` IN (10981, 10990, 13618, 13676, 25499);
UPDATE `creature_template` SET `maxlevel`=-10 WHERE `entry` IN (10981, 10990);
UPDATE `creature_template` SET `maxlevel`=-9 WHERE `entry` IN (13618, 13676);
UPDATE `creature_template` SET `minlevel`=-6 WHERE `entry` IN (11602, 11605, 13080, 13081, 13087, 13089, 13096, 13097, 13098, 13099);
UPDATE `creature_template` SET `minlevel`=-5 WHERE `entry` IN (2225, 3343, 3625, 4255, 4257, 5134, 5135, 5139, 10364, 10367, 12047, 12048, 12052, 12096, 12097, 13149);
UPDATE `creature_template` SET `maxlevel`=-5 WHERE `entry` IN (2225, 3343, 3625, 4255, 4257, 5134, 5135, 5139, 10364, 10367, 11602, 11605, 12047, 12048, 12052, 12096, 12097, 13080, 13081, 13087, 13089, 13096, 13097, 13098, 13099);
UPDATE `creature_template` SET `minlevel`=-4 WHERE `entry` IN (10989, 11838, 11839, 13325, 13327, 13330, 13448, 13534, 13537, 13540, 13543, 13546, 13549, 13552, 13555);
UPDATE `creature_template` SET `maxlevel`=-4 WHERE `entry` IN (13149, 13325, 13327, 13330);
UPDATE `creature_template` SET `minlevel`=-3 WHERE `entry` IN (12051, 12127, 13150, 13335, 13336, 13337);
UPDATE `creature_template` SET `maxlevel`=-3 WHERE `entry` IN (11838, 11839, 12051, 12127, 13335, 13336, 13337, 13448, 13534, 13537, 13540, 13543, 13546, 13549, 13552, 13555);
UPDATE `creature_template` SET `minlevel`=-2 WHERE `entry` IN (11657, 11677, 11840, 12050, 12053, 13078, 13079, 13086, 13088, 13180, 13216, 13218, 13221, 13284, 13324, 13329, 13426, 13427, 13428, 13438, 13440, 13447, 13516, 13520, 13524, 13528, 13535, 13538, 13541, 13544, 13547, 13550, 13553, 13556, 13576, 13777, 14141, 14142, 30739, 30740);
UPDATE `creature_template` SET `maxlevel`=-2 WHERE `entry` IN (10989, 11657, 11677, 12050, 12053, 13078, 13079, 13086, 13088, 13150, 13180, 13216, 13218, 13221, 13284, 13324, 13329, 13426, 13427, 13428, 13438, 13440, 13447, 13516, 13520, 13524, 13528, 13576, 14141, 14142, 30739, 30740);
UPDATE `creature_template` SET `minlevel`=-1 WHERE `entry` IN (13137, 13138, 13143, 13144, 13145, 13146, 13147, 13179, 13296, 13297, 13298, 13299, 13300, 13326, 13328, 13333, 13334, 13358, 13359, 13439, 13517, 13521, 13525, 13529, 14185, 14188, 15466, 56160, 58181, 58255, 64584, 65867, 67979, 70253, 70309, 70337, 70682, 72312, 72313, 72318, 72371, 72534, 72548, 72549, 72579, 72580, 72581, 72911, 72912, 73561, 73562, 73563, 73726, 73727, 73728, 73929, 74089);
UPDATE `creature_template` SET `maxlevel`=-1 WHERE `entry` IN (11840, 13137, 13138, 13143, 13144, 13145, 13146, 13147, 13179, 13296, 13297, 13298, 13299, 13300, 13326, 13328, 13333, 13334, 13439, 13517, 13521, 13525, 13529, 13535, 13538, 13541, 13544, 13547, 13550, 13553, 13556, 13777, 14185, 14188, 58181, 58255, 64584, 65867, 67979, 70253, 70309, 70337, 70682, 72312, 72313, 72318, 72534, 72548, 72549, 72579, 72580, 72581, 72911, 72912, 73561, 73562, 73563, 73726, 73727, 73728, 73929, 74089);
UPDATE `creature_template` SET `minlevel`=0 WHERE `entry` IN (68, 309, 347, 352, 523, 603, 857, 907, 931, 1233, 1387, 1571, 1572, 1573, 1652, 1735, 1756, 1976, 2041, 2226, 2299, 2302, 2389, 2409, 2432, 2804, 2835, 2851, 2858, 2859, 2861, 2941, 2995, 3083, 3084, 3296, 3305, 3310, 3468, 3469, 3502, 3575, 3615, 3838, 3841, 3890, 4262, 4267, 4309, 4310, 4312, 4314, 4317, 4319, 4321, 4407, 4423, 4551, 4624, 5118, 5595, 5597, 5603, 5624, 6026, 6526, 6706, 6726, 7410, 7427, 7823, 7824, 7980, 8018, 8019, 8020, 8055, 8096, 8609, 8610, 8670, 8719, 9297, 9460, 9521, 9526, 9527, 10038, 10360, 10583, 10897, 11099, 11102, 11138, 11139, 11190, 11194, 11822, 11899, 11900, 11901, 11947, 11949, 11997, 11998, 12121, 12122, 12197, 12198, 12577, 12578, 12596, 12616, 12617, 12636, 12740, 13116, 13117, 13151, 13155, 13161, 13176, 13177, 13178, 13181, 13236, 13257, 13281, 13331, 13332, 13377, 13378, 13416, 13424, 13425, 13437, 13441, 13442, 13443, 13518, 13522, 13526, 13530, 13536, 13539, 13542, 13545, 13548, 13551, 13554, 13557, 13577, 13616, 13617, 13797, 13798, 13839, 14026, 14027, 14028, 14029, 14030, 14031, 14186, 14187, 14242, 14284, 14285, 14304, 14363, 14365, 14367, 14375, 14376, 14377, 14378, 14379, 14380, 14402, 14403, 14404, 14423, 14438, 14439, 14440, 14441, 14442, 14622, 14717, 14762, 14763, 14764, 14765, 14766, 14767, 14768, 14769, 14770, 14771, 14772, 14773, 14774, 14775, 14776, 14777, 14832, 14848, 14913, 14942, 14943, 14944, 14945, 14946, 14947, 14948, 14981, 14982, 15006, 15007, 15008, 15139, 15177, 15178, 15184, 15195, 15518, 15519, 15659, 15961, 16096, 16189, 16192, 16222, 16227, 16378, 16392, 16587, 16694, 16695, 16696, 16711, 16733, 16822, 17506, 17507, 17554, 17555, 17855, 18103, 18139, 18785, 18788, 18789, 18791, 18807, 18808, 18809, 18895, 18930, 18931, 18937, 18938, 18939, 18940, 18942, 18953, 18989, 19317, 19504, 19529, 19541, 19558, 19581, 19583, 19855, 19858, 19859, 19905, 19906, 19907, 19908, 19909, 19910, 19911, 19912, 19915, 19923, 19925, 20118, 20119, 20120, 20234, 20269, 20271, 20272, 20273, 20274, 20276, 20362, 20374, 20381, 20382, 20383, 20384, 20385, 20386, 20388, 20390, 20447, 20497, 20499, 20515, 20556, 20674, 20762, 21107, 21235, 21766, 21986, 22216, 22407, 22455, 22485, 22494, 22931, 22935, 22937, 23090, 23115, 23612, 23619, 23623, 23624, 23625, 23626, 23636, 23736, 23859, 24032, 24061, 24155, 24366, 24470, 24795, 24851, 25040, 25145, 25148, 25149, 25150, 25151, 25152, 25288, 25991, 26016, 26017, 26019, 26020, 26021, 26024, 26027, 26028, 26029, 26030, 26031, 26032, 26033, 26037, 26038, 26253, 26560, 26566, 26602, 26844, 26845, 26847, 26848, 26850, 26851, 26852, 26853, 26876, 26877, 26878, 26879, 26880, 26881, 27046, 27170, 27175, 27344, 27881, 27894, 27946, 27953, 28037, 28094, 28195, 28196, 28197, 28312, 28319, 28366, 28574, 28613, 28615, 28618, 28621, 28623, 28624, 28674, 28781, 28801, 28818, 28863, 28865, 29260, 29262, 29480, 29533, 29568, 29617, 29712, 29721, 29749, 29750, 29757, 29762, 29910, 29950, 29951, 30231, 30269, 30271, 30280, 30312, 30314, 30400, 30433, 30488, 30489, 30499, 30560, 30569, 30578, 30579, 30580, 30581, 30582, 30583, 30584, 30586, 30587, 30590, 30610, 30611, 30626, 30737, 30853, 30869, 30870, 31036, 31051, 31052, 31053, 31054, 31069, 31078, 31091, 31101, 31102, 31106, 31107, 31108, 31109, 31151, 31153, 31250, 31416, 31426, 31430, 31708, 31715, 31841, 31842, 32169, 32170, 32294, 32296, 32307, 32308, 32330, 32332, 32333, 32571, 32615, 32616, 32617, 32618, 32619, 32620, 32621, 32622, 32623, 32624, 32625, 32626, 32627, 32629, 33253, 33254, 33502, 33504, 33505, 33506, 33508, 33510, 33511, 33849, 34018, 34019, 34021, 34022, 34024, 34025, 34026, 34027, 34028, 34029, 34374, 34378, 34429, 34775, 34776, 34777, 34778, 34793, 34802, 34918, 34919, 34927, 34929, 34935, 34943, 34944, 34955, 34971, 34972, 34973, 34976, 34978, 34983, 34985, 34986, 34987, 34988, 34989, 34991, 34993, 34997, 34998, 34999, 35000, 35001, 35002, 35007, 35008, 35017, 35019, 35020, 35021, 35022, 35023, 35024, 35025, 35026, 35027, 35069, 35136, 35137, 35138, 35139, 35140, 35141, 35273, 35315, 35322, 35345, 35346, 35450, 35478, 35480, 35481, 35556, 35562, 35596, 35597, 35600, 35601, 35602, 35603, 35611, 35612, 35819, 36151, 36152, 36162, 36164, 36165, 36166, 36213, 36217, 36224, 36225, 36226, 36273, 36355, 36356, 36481, 36728, 37005, 37790, 37825, 37860, 37869, 37888, 37915, 37944, 38831, 39172, 39173, 39175, 39210, 39211, 39212, 39330, 39340, 39898, 39934, 40138, 40333, 40358, 40367, 40413, 40473, 40552, 40553, 40558, 40768, 40769, 40809, 40827, 40851, 40852, 40866, 40867, 40871, 40873, 40966, 41140, 41142, 41214, 41215, 41240, 41246, 41321, 41322, 41323, 41325, 41332, 41383, 41580, 41605, 41860, 41861, 41896, 42319, 42406, 42426, 42623, 42974, 42983, 43000, 43042, 43043, 43045, 43049, 43052, 43053, 43072, 43073, 43079, 43085, 43086, 43087, 43088, 43104, 43107, 43114, 43121, 43124, 43216, 43220, 43225, 43287, 43289, 43290, 43293, 43295, 43328, 43371, 43375, 43427, 43440, 43481, 43549, 43570, 43573, 43690, 43695, 43697, 43701, 43702, 43841, 43842, 43889, 43991, 44036, 44059, 44060, 44230, 44231, 44232, 44233, 44244, 44399, 44407, 44408, 44409, 44410, 44440, 44787, 44825, 44865, 44866, 44867, 44868, 44871, 44872, 44874, 44876, 44877, 44878, 45015, 45082, 45230, 45344, 45479, 45564, 45659, 45814, 45822, 45827, 45829, 46004, 46006, 46011, 46071, 46077, 46142, 46180, 46181, 46190, 46552, 46637, 46638, 46639, 46640, 46667, 46986, 47060, 47061, 47116, 47118, 47119, 47121, 47133, 47147, 47154, 47155, 47156, 47174, 47434, 47581, 47582, 47584, 47586, 47587, 47588, 47589, 47644, 47655, 47661, 47663, 47665, 47688, 47844, 47875, 47927, 48075, 48273, 48274, 48275, 48318, 48321, 48874, 48883, 49038, 49573, 50083, 50084, 50440, 50441, 50442, 50443, 50445, 50463, 50546, 50548, 50549, 50550, 50551, 50552, 50553, 50668, 50670, 50674, 50676, 50678, 50683, 50684, 51257, 51309, 51310, 51323, 51334, 51346, 51348, 51371, 51372, 51383, 51384, 51429, 51450, 51451, 51468, 51471, 51472, 51473, 51474, 51475, 51476, 51478, 51507, 51508, 51509, 51510, 51511, 51513, 51514, 51515, 51516, 51517, 51518, 51519, 51520, 51521, 51522, 51523, 51524, 51525, 51532, 51533, 51534, 51535, 51536, 51537, 51538, 51539, 51540, 51541, 51542, 51543, 51544, 51545, 51546, 51547, 51548, 51549, 51554, 51605, 51610, 51732, 51768, 51769, 51770, 51771, 51772, 51773, 51774, 51775, 51776, 51777, 51778, 51779, 51780, 51781, 51782, 51783, 51784, 51785, 51786, 51789, 51790, 51791, 51792, 51793, 51794, 51795, 51796, 51797, 51798, 51799, 51800, 51801, 51802, 51803, 51804, 51805, 51806, 51807, 51808, 51809, 51810, 51811, 51812, 51813, 51814, 51815, 51816, 51817, 51818, 51819, 51820, 51821, 51822, 51823, 51824, 51825, 51827, 51828, 51829, 51830, 51831, 51832, 51834, 51835, 51837, 51838, 51839, 51840, 51841, 51842, 51843, 51844, 51845, 51846, 51847, 51848, 51849, 51850, 51851, 51852, 51853, 51854, 51855, 51856, 51857, 51858, 51859, 51860, 51861, 51862, 51863, 51864, 51867, 51868, 51869, 51870, 51871, 51872, 51873, 51874, 51875, 51876, 51877, 51878, 51879, 51880, 51881, 51882, 51883, 51884, 51885, 51886, 51887, 51888, 51889, 51890, 51891, 51892, 51893, 51894, 51895, 51896, 51897, 51898, 51899, 51900, 51901, 51902, 51903, 51904, 51905, 51906, 51907, 51908, 51909, 51910, 51911, 51912, 51913, 51914, 51915, 51916, 51917, 51918, 51919, 51920, 51921, 51922, 51923, 51924, 51925, 51986, 52009, 52010, 52011, 52012, 52013, 52018, 52060, 52084, 52184, 52280, 52404, 52410, 52646, 52670, 52753, 52817, 52821, 52983, 52996, 53000, 53008, 53352, 53562, 53563, 53598, 53783, 54208, 54209, 54334, 54345, 54346, 54392, 54393, 54441, 54442, 54443, 54471, 54472, 54473, 54502, 54509, 54519, 54523, 54524, 54548, 54601, 54788, 55003, 55382, 55396, 55402, 55568, 55577, 55578, 55657, 55660, 55778, 55779, 55805, 55810, 55842, 55844, 55860, 55861, 55904, 55996, 56041, 56737, 56771, 57287, 57288, 57289, 57473, 57474, 57475, 57629, 57630, 57800, 57801, 58505, 58612, 58770, 58782, 58784, 58788, 58792, 58816, 58855, 58877, 58912, 58913, 58914, 58929, 58934, 58939, 59048, 59317, 59471, 59595, 59596, 59604, 59654, 59658, 59812, 59922, 59923, 59952, 60152, 60969, 60971, 61744, 61745, 61759, 62334, 62824, 62913, 62926, 63351, 63357, 63364, 63372, 63373, 64044, 64045, 64519, 64573, 64574, 64586, 64587, 64615, 64709, 64728, 64729, 64732, 64733, 64734, 64735, 64736, 64923, 64956, 64957, 64997, 64998, 65138, 65232, 65236, 65237, 65238, 65239, 65240, 65241, 65474, 65494, 65507, 65510, 65548, 65549, 65610, 65611, 65643, 65654, 65680, 65785, 65786, 65801, 65807, 65863, 65865, 66666, 66913, 67013, 67014, 67149, 67150, 67195, 67196, 67197, 67198, 67199, 67200, 67260, 67267, 67418, 67754, 67785, 67790, 67796, 67923, 67944, 67945, 67965, 67987, 68008, 68053, 68054, 68216, 68226, 68293, 68323, 68363, 68364, 68365, 68372, 68403, 68404, 68405, 68406, 68408, 68424, 68425, 68678, 68764, 68765, 68767, 68768, 68792, 68794, 68979, 68980, 68981, 68982, 68990, 68991, 68992, 68994, 68995, 68996, 69359, 69360, 69422, 70085, 70086, 70087, 70088, 70354, 70381, 70498, 70643, 70690, 70705, 70706, 70741, 70824, 70877, 71111, 71112, 71210, 71246, 71265, 71280, 71302, 71303, 71340, 71358, 71359, 71369, 71487, 71490, 71491, 71589, 71718, 71719, 71794, 71837, 71972, 71973, 71974, 72005, 72038, 72366, 72376, 72422, 72423, 72424, 72425, 72501, 72515, 72559, 72570, 72614, 72774, 72886, 73082, 73120, 73121, 73122, 73343, 73361, 73362, 73385, 73386, 73600, 73602, 73664, 73811, 73818, 73820, 74219);
UPDATE `creature_template` SET `maxlevel`=0 WHERE `entry` IN (68, 309, 347, 352, 523, 603, 857, 907, 931, 1233, 1387, 1571, 1572, 1573, 1652, 1735, 1756, 1976, 2041, 2226, 2299, 2302, 2389, 2409, 2432, 2804, 2835, 2851, 2858, 2859, 2861, 2941, 2995, 3083, 3084, 3296, 3305, 3310, 3468, 3469, 3502, 3575, 3615, 3838, 3841, 3890, 4262, 4267, 4309, 4310, 4312, 4314, 4317, 4319, 4321, 4407, 4423, 4551, 4624, 5118, 5595, 5597, 5603, 5624, 6026, 6526, 6706, 6726, 7410, 7427, 7823, 7824, 7980, 8018, 8019, 8020, 8055, 8096, 8609, 8610, 8670, 8719, 9297, 9460, 9521, 9526, 9527, 10038, 10360, 10583, 10897, 11099, 11102, 11138, 11139, 11190, 11194, 11822, 11899, 11900, 11901, 11947, 11949, 11997, 11998, 12121, 12122, 12197, 12198, 12577, 12578, 12596, 12616, 12617, 12636, 12740, 13116, 13117, 13151, 13155, 13161, 13176, 13177, 13178, 13181, 13236, 13257, 13281, 13331, 13332, 13358, 13359, 13377, 13378, 13416, 13424, 13425, 13437, 13441, 13442, 13443, 13518, 13522, 13526, 13530, 13536, 13539, 13542, 13545, 13548, 13551, 13554, 13557, 13577, 13616, 13617, 13839, 14026, 14027, 14028, 14029, 14030, 14031, 14186, 14187, 14242, 14284, 14285, 14304, 14363, 14365, 14367, 14375, 14376, 14377, 14378, 14379, 14380, 14402, 14403, 14404, 14423, 14438, 14439, 14440, 14441, 14442, 14622, 14717, 14762, 14763, 14764, 14765, 14766, 14767, 14768, 14769, 14770, 14771, 14772, 14773, 14774, 14775, 14776, 14777, 14832, 14848, 14913, 14942, 14943, 14944, 14945, 14946, 14947, 14948, 14981, 14982, 15006, 15007, 15008, 15139, 15177, 15178, 15184, 15195, 15466, 15518, 15519, 15659, 15961, 16096, 16189, 16192, 16222, 16227, 16378, 16392, 16587, 16694, 16695, 16696, 16711, 16733, 16822, 17506, 17507, 17554, 17555, 17855, 18103, 18139, 18785, 18788, 18789, 18791, 18807, 18808, 18809, 18895, 18930, 18931, 18937, 18938, 18939, 18940, 18942, 18953, 18989, 19317, 19504, 19529, 19541, 19558, 19581, 19583, 19855, 19858, 19859, 19905, 19906, 19907, 19908, 19909, 19910, 19911, 19912, 19915, 19923, 19925, 20118, 20119, 20120, 20234, 20269, 20271, 20272, 20273, 20274, 20276, 20362, 20374, 20381, 20382, 20383, 20384, 20385, 20386, 20388, 20390, 20447, 20497, 20499, 20515, 20556, 20674, 20762, 21107, 21235, 21766, 21986, 22216, 22407, 22455, 22485, 22494, 22931, 22935, 22937, 23090, 23115, 23612, 23619, 23623, 23624, 23625, 23626, 23636, 23736, 23859, 24032, 24061, 24155, 24366, 24470, 24795, 24851, 25040, 25145, 25148, 25149, 25150, 25151, 25152, 25288, 25499, 25991, 26016, 26017, 26019, 26020, 26021, 26024, 26027, 26028, 26029, 26030, 26031, 26032, 26033, 26037, 26038, 26253, 26560, 26566, 26602, 26844, 26845, 26847, 26848, 26850, 26851, 26852, 26853, 26876, 26877, 26878, 26879, 26880, 26881, 27046, 27170, 27175, 27344, 27881, 27894, 27946, 27953, 28037, 28094, 28195, 28196, 28197, 28312, 28319, 28366, 28574, 28613, 28615, 28618, 28621, 28623, 28624, 28674, 28781, 28801, 28818, 28863, 28865, 29260, 29262, 29480, 29533, 29568, 29617, 29712, 29721, 29749, 29750, 29757, 29762, 29910, 29950, 29951, 30231, 30269, 30271, 30280, 30312, 30314, 30400, 30433, 30488, 30489, 30499, 30560, 30569, 30578, 30579, 30580, 30581, 30582, 30583, 30584, 30586, 30587, 30590, 30610, 30611, 30626, 30737, 30853, 30869, 30870, 31036, 31051, 31052, 31053, 31054, 31069, 31078, 31091, 31101, 31102, 31106, 31107, 31108, 31109, 31151, 31153, 31250, 31416, 31426, 31430, 31708, 31715, 31841, 31842, 32169, 32170, 32294, 32296, 32307, 32308, 32330, 32332, 32333, 32571, 32615, 32616, 32617, 32618, 32619, 32620, 32621, 32622, 32623, 32624, 32625, 32626, 32627, 32629, 33253, 33254, 33502, 33504, 33505, 33506, 33508, 33510, 33511, 33849, 34018, 34019, 34021, 34022, 34024, 34025, 34026, 34027, 34028, 34029, 34374, 34378, 34429, 34775, 34776, 34777, 34778, 34793, 34802, 34918, 34919, 34927, 34929, 34935, 34943, 34944, 34955, 34971, 34972, 34973, 34976, 34978, 34983, 34985, 34986, 34987, 34988, 34989, 34991, 34993, 34997, 34998, 34999, 35000, 35001, 35002, 35007, 35008, 35017, 35019, 35020, 35021, 35022, 35023, 35024, 35025, 35026, 35027, 35069, 35136, 35137, 35138, 35139, 35140, 35141, 35273, 35315, 35322, 35345, 35346, 35450, 35478, 35480, 35481, 35556, 35562, 35596, 35597, 35600, 35601, 35602, 35603, 35611, 35612, 35819, 36151, 36152, 36162, 36164, 36165, 36166, 36213, 36217, 36224, 36225, 36226, 36273, 36355, 36356, 36481, 36728, 37005, 37790, 37825, 37860, 37869, 37888, 37915, 37944, 38831, 39172, 39173, 39175, 39210, 39211, 39212, 39330, 39340, 39898, 39934, 40138, 40333, 40358, 40367, 40413, 40473, 40552, 40553, 40558, 40768, 40769, 40809, 40827, 40851, 40852, 40866, 40867, 40871, 40873, 40966, 41140, 41142, 41214, 41215, 41240, 41246, 41321, 41322, 41323, 41325, 41332, 41383, 41580, 41605, 41860, 41861, 41896, 42319, 42406, 42426, 42623, 42974, 42983, 43000, 43042, 43043, 43045, 43049, 43052, 43053, 43072, 43073, 43079, 43085, 43086, 43087, 43088, 43104, 43107, 43114, 43121, 43124, 43216, 43220, 43225, 43287, 43289, 43290, 43293, 43295, 43328, 43371, 43375, 43427, 43440, 43481, 43549, 43570, 43573, 43690, 43695, 43697, 43701, 43702, 43841, 43842, 43889, 43991, 44036, 44059, 44060, 44230, 44231, 44232, 44233, 44244, 44399, 44407, 44408, 44409, 44410, 44440, 44787, 44825, 44865, 44866, 44867, 44868, 44871, 44872, 44874, 44876, 44877, 44878, 45015, 45082, 45230, 45344, 45479, 45564, 45659, 45814, 45822, 45827, 45829, 46004, 46006, 46011, 46071, 46077, 46142, 46180, 46181, 46190, 46552, 46637, 46638, 46639, 46640, 46667, 46986, 47060, 47061, 47116, 47118, 47119, 47121, 47133, 47147, 47154, 47155, 47156, 47174, 47434, 47581, 47582, 47584, 47586, 47587, 47588, 47589, 47644, 47655, 47661, 47663, 47665, 47688, 47844, 47875, 47927, 48075, 48273, 48274, 48275, 48318, 48321, 48874, 48883, 49038, 49573, 50083, 50084, 50440, 50441, 50442, 50443, 50445, 50463, 50546, 50548, 50549, 50550, 50551, 50552, 50553, 50668, 50670, 50674, 50676, 50678, 50683, 50684, 51257, 51309, 51310, 51323, 51334, 51346, 51348, 51371, 51372, 51383, 51384, 51429, 51450, 51451, 51468, 51471, 51472, 51473, 51474, 51475, 51476, 51478, 51507, 51508, 51509, 51510, 51511, 51513, 51514, 51515, 51516, 51517, 51518, 51519, 51520, 51521, 51522, 51523, 51524, 51525, 51532, 51533, 51534, 51535, 51536, 51537, 51538, 51539, 51540, 51541, 51542, 51543, 51544, 51545, 51546, 51547, 51548, 51549, 51554, 51605, 51610, 51732, 51768, 51769, 51770, 51771, 51772, 51773, 51774, 51775, 51776, 51777, 51778, 51779, 51780, 51781, 51782, 51783, 51784, 51785, 51786, 51789, 51790, 51791, 51792, 51793, 51794, 51795, 51796, 51797, 51798, 51799, 51800, 51801, 51802, 51803, 51804, 51805, 51806, 51807, 51808, 51809, 51810, 51811, 51812, 51813, 51814, 51815, 51816, 51817, 51818, 51819, 51820, 51821, 51822, 51823, 51824, 51825, 51827, 51828, 51829, 51830, 51831, 51832, 51834, 51835, 51837, 51838, 51839, 51840, 51841, 51842, 51843, 51844, 51845, 51846, 51847, 51848, 51849, 51850, 51851, 51852, 51853, 51854, 51855, 51856, 51857, 51858, 51859, 51860, 51861, 51862, 51863, 51864, 51867, 51868, 51869, 51870, 51871, 51872, 51873, 51874, 51875, 51876, 51877, 51878, 51879, 51880, 51881, 51882, 51883, 51884, 51885, 51886, 51887, 51888, 51889, 51890, 51891, 51892, 51893, 51894, 51895, 51896, 51897, 51898, 51899, 51900, 51901, 51902, 51903, 51904, 51905, 51906, 51907, 51908, 51909, 51910, 51911, 51912, 51913, 51914, 51915, 51916, 51917, 51918, 51919, 51920, 51921, 51922, 51923, 51924, 51925, 51986, 52009, 52010, 52011, 52012, 52013, 52018, 52060, 52084, 52184, 52280, 52404, 52410, 52646, 52670, 52753, 52817, 52821, 52983, 52996, 53000, 53008, 53352, 53562, 53563, 53598, 53783, 54208, 54209, 54334, 54345, 54346, 54392, 54393, 54441, 54442, 54443, 54471, 54472, 54473, 54502, 54509, 54519, 54523, 54524, 54548, 54601, 54788, 55003, 55382, 55396, 55402, 55568, 55577, 55578, 55657, 55660, 55778, 55779, 55805, 55810, 55842, 55844, 55860, 55861, 55904, 55996, 56041, 56160, 56737, 56771, 57287, 57288, 57289, 57473, 57474, 57475, 57629, 57630, 57800, 57801, 58505, 58612, 58770, 58782, 58784, 58788, 58792, 58816, 58855, 58877, 58912, 58913, 58914, 58929, 58934, 58939, 59048, 59317, 59471, 59595, 59596, 59604, 59654, 59658, 59812, 59922, 59923, 59952, 60152, 60969, 60971, 61744, 61745, 61759, 62334, 62824, 62913, 62926, 63351, 63357, 63364, 63372, 63373, 64044, 64045, 64519, 64573, 64574, 64586, 64587, 64615, 64709, 64728, 64729, 64732, 64733, 64734, 64735, 64736, 64923, 64956, 64957, 64997, 64998, 65138, 65232, 65236, 65237, 65238, 65239, 65240, 65241, 65474, 65494, 65507, 65510, 65548, 65549, 65610, 65611, 65643, 65654, 65680, 65785, 65786, 65801, 65807, 65863, 65865, 66666, 66913, 67013, 67014, 67149, 67150, 67195, 67196, 67197, 67198, 67199, 67200, 67260, 67267, 67418, 67754, 67785, 67790, 67796, 67923, 67944, 67945, 67965, 67987, 68008, 68053, 68054, 68216, 68226, 68293, 68323, 68363, 68364, 68365, 68372, 68403, 68404, 68405, 68406, 68408, 68424, 68425, 68678, 68764, 68765, 68767, 68768, 68792, 68794, 68979, 68980, 68981, 68982, 68990, 68991, 68992, 68994, 68995, 68996, 69359, 69360, 69422, 70085, 70086, 70087, 70088, 70354, 70381, 70498, 70643, 70690, 70705, 70706, 70741, 70824, 70877, 71111, 71112, 71210, 71246, 71265, 71280, 71302, 71303, 71340, 71358, 71359, 71369, 71487, 71490, 71491, 71589, 71718, 71719, 71794, 71837, 71972, 71973, 71974, 72005, 72038, 72366, 72371, 72376, 72422, 72423, 72424, 72425, 72501, 72515, 72559, 72570, 72614, 72774, 72886, 73082, 73120, 73121, 73122, 73343, 73361, 73362, 73385, 73386, 73600, 73602, 73664, 73811, 73818, 73820, 74219);
UPDATE `creature_template` SET `minlevel`=1 WHERE `entry` IN (10985, 11946, 11948, 13139, 13140, 13152, 13153, 13154, 13256, 13318, 13319, 13320, 13356, 13357, 13419, 13421, 13422, 13446, 13449, 13496, 13519, 13523, 13527, 13531, 13597, 13598, 13959, 13977, 34922, 34924);
UPDATE `creature_template` SET `maxlevel`=1 WHERE `entry` IN (10985, 11946, 11948, 13139, 13140, 13152, 13153, 13154, 13256, 13318, 13319, 13320, 13356, 13357, 13419, 13421, 13422, 13446, 13449, 13496, 13519, 13523, 13527, 13531, 13597, 13598, 13797, 13798, 13959, 13977, 34922, 34924);

UPDATE `creature_template` SET `minlevel`=-84, `maxlevel`=-84 WHERE `entry` IN (60226, 60227, 60228);
UPDATE `creature_template` SET `minlevel`=-8, `maxlevel`=-7 WHERE `entry` IN (10982, 10986, 10987, 10991, 11603, 11606, 11678, 13316, 13317, 13396, 13397, 76357);
UPDATE `creature_template` SET `minlevel`=-7, `maxlevel`=-6 WHERE `entry` IN (11600, 11604, 11675, 11837, 14282, 14283);
UPDATE `creature_template` SET `minlevel`=2, `maxlevel`=2 WHERE `entry` IN (11676, 12159, 23775);
UPDATE `creature_template` SET `minlevel`=3, `maxlevel`=3 WHERE `entry` IN (1284, 3936, 4968, 7999, 10181, 15088, 15193, 15467, 16800, 16801, 16802, 17468, 21984, 22522, 29611, 31146, 36648, 37058, 38966, 39605, 39656, 42129, 42131, 42317, 42928, 43845, 43867, 43931, 44441, 44451, 45042, 45111, 45250, 45253, 45382, 46236, 46717, 46757, 46833, 46844, 47718, 47736, 47745, 48546, 48572, 48736, 48738, 49420, 49533, 49809, 49820, 50043, 50369, 51355, 51571, 51666, 52700, 52767, 52792, 53518, 53537, 53649, 53657, 53660, 53663, 53667, 53669, 53676, 53736, 53755, 53905, 53956, 53957, 53958, 53959, 53960, 54131, 54132, 54168, 54226, 54260, 54297, 54300, 54301, 54302, 54306, 54313, 54344, 54499, 54611, 54631, 55148, 55150, 55189, 55561, 56730, 57164, 57205, 58336, 60219, 61138, 61170, 62087, 62092, 62163, 62742, 62808, 62827, 62841, 62911, 62912, 64402, 64526, 64529, 64560, 64564, 64565, 64600, 64629, 64727, 65227, 65228, 65539, 65540, 65659, 65660, 65661, 66154, 66155, 66156, 66458, 67560, 67580, 67581, 67840, 67864, 67867, 67947, 67953, 67968, 67973, 67989, 67992, 68003, 68023, 68025, 68059, 68072, 68084, 68085, 68086, 68088, 68108, 68144, 68223, 68235, 68236, 68237, 68242, 68278, 68287, 68315, 68329, 68336, 68339, 68357, 68370, 68392, 68399, 68429, 68496, 68514, 68538, 68544, 68567, 68568, 68569, 68570, 68571, 68572, 68573, 68575, 68586, 68589, 68590, 68592, 68593, 68609, 68631, 68633, 68634, 68637, 68642, 68643, 68644, 68645, 68651, 68652, 68677, 68679, 68689, 68690, 68749, 68777, 68842, 68882, 68920, 68921, 68922, 68924, 68930, 68941, 68955, 68966, 68970, 68972, 68975, 69026, 69797, 69799, 70131, 70132, 70245, 70362, 70363, 70370, 70371, 70799, 70978, 71042, 71147, 71148, 71149, 71151, 71177, 71249, 71250, 71344, 71592, 71992, 72302, 72311, 72392, 72394, 72560, 72561, 73554, 73558, 73709, 73710, 73909, 73910, 73927, 73941, 73942, 73957, 73962, 74217);
UPDATE `creature_template` SET `minlevel`=5, `maxlevel`=5 WHERE `entry` IN (3210, 3211, 3213, 3214, 5952, 7975, 12160, 15371, 16921, 38217, 38898, 38899, 39196, 42316);