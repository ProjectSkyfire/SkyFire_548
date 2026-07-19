UPDATE `creature_template` SET `npcflag`=16777216 WHERE `entry`=57301;
UPDATE `creature_template` SET `ScriptName`='npc_glade_glimmer' WHERE  `entry`=57301;

DELETE FROM `npc_spellclick_spells` WHERE `npc_entry`=57301;
INSERT INTO `npc_spellclick_spells` (`npc_entry`, `spell_id`, `cast_flags`) VALUES
(57301, 111543, 1);

DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=18 AND `SourceGroup`=57301 AND `SourceEntry`=111543;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(18, 57301, 111543, 0, 1, 9, 0, 30267, 0, 0, 0, 0, 0, '', 'Required quest Watery Woes active for spellclick'),
(18, 57301, 111543, 0, 1, 2, 0, 18, 1, 0, 1, 0, 0, '', 'required item does not exceeds for spellclick');


UPDATE `quest_template` SET `RequestItemsText`='I refuse to not fish. Fishing\'s who I am!' WHERE `Id`=30267;
UPDATE `quest_template` SET `OfferRewardText`='Finally, someone who understands me.\r\nFishing is what it\'s all about, right?\r\n\r\nMaybe I should leave New Cifera... just pack up my things and join the Anglers, down in the south. Of course, I couldn\'t leave my family behind...\r\n\r\nBut still... a pandaren can dream, right?' WHERE  `Id`=30267;
