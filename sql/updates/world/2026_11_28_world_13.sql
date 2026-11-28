-- Thundermar assault: Colin starts Narkrall only for an active quest.
UPDATE `creature_template` SET `AIName`='' , `ScriptName`='npc_thundermar_colin', `npcflag`=`npcflag` | 3 WHERE `entry`=48173;
UPDATE `creature_template` SET `AIName`='' , `ScriptName`='npc_thundermar_narkrall', `InhabitType`=1 WHERE `entry`=48257;
UPDATE `creature_template` SET `AIName`='' , `ScriptName`='npc_thundermar_assault_drake', `InhabitType`=5 WHERE `entry`=48197;
UPDATE `creature_template` SET `InhabitType`=5 WHERE `entry`=48256;

-- The script supplies the ready option and revalidates the quest on selection.
DELETE FROM `gossip_menu_option` WHERE `MenuID`=12409 AND `OptionID`=0;

INSERT INTO `creature_text` (`entry`,`groupid`,`id`,`text`,`type`,`language`,`probability`,`emote`,`duration`,`sound`,`comment`) VALUES
(48173,1,0,'Here he comes, $n! Flying in from the west. Get your weapons ready!',12,0,100,25,0,0,'Colin Thundermar - Narkrall arrival')
ON DUPLICATE KEY UPDATE `text`=VALUES(`text`),`type`=VALUES(`type`),`language`=VALUES(`language`),`probability`=VALUES(`probability`),`emote`=VALUES(`emote`),`duration`=VALUES(`duration`),`sound`=VALUES(`sound`),`comment`=VALUES(`comment`);

-- Validate the item target and retain the native drake-dropper damage.
INSERT INTO `spell_script_names` (`spell_id`,`ScriptName`) VALUES (89557,'spell_thundermar_drake_dropper')
ON DUPLICATE KEY UPDATE `ScriptName`=VALUES(`ScriptName`);

-- Restore the English completion dialogue for the assault and summit handoff.
UPDATE `quest_template` SET `OfferRewardText`='You did it, $n! Alliance ingenuity to the rescue. Of course, I don''t have anything to help you fight the dismounted riders now that they''re on the ground. You''re a $c; improvise!' WHERE `Id`=28280;
UPDATE `quest_template` SET `OfferRewardText`='You sure know your way around a battlefield, $c. It looks like they''re about to fall back. All we need is one decisive blow...' WHERE `Id`=28281;
UPDATE `quest_template` SET `OfferRewardText`='We did it, $n! Narkrall Rakeclaw lies dead at my feet, and Thundermar has weathered a storm of drakes! This wouldn''t have been possible if it weren''t for the help of the Alliance. I owe you a debt that I don''t think can ever be repaid.' WHERE `Id`=28282;
UPDATE `quest_template` SET `OfferRewardText`='Hello, $c! Keegan Firebeard here has told me all about your heroics down south. Thank you for all of your efforts on behalf of the Wildhammer. I see Colin Thundermar is right behind you. The summit is ready to begin! Please stay and let''s see how things unfold...' WHERE `Id`=28294;
