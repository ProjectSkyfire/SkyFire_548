-- Teegan's Expedition: hub gossip/RP, quest gating, and quest 25022 Gaines rescue.

-- ============================================================
-- Quest gating: 25015+25022 unlock 25027/25028; 25027 unlocks 25034
-- ============================================================
UPDATE `quest_template` SET `ExclusiveGroup`=-25027, `NextQuestId`=25027 WHERE `Id` IN (25015, 25022);
UPDATE `quest_template` SET `PrevQuestId`=25015 WHERE `Id`=25028;
UPDATE `quest_template` SET `PrevQuestId`=25027 WHERE `Id`=25034;

-- ============================================================
-- Templates / flags (sniff menus + npcflags)
-- ============================================================
UPDATE `creature_template` SET `gossip_menu_id`=11108, `npcflag`=3, `AIName`='SmartAI' WHERE `entry`=38871;
UPDATE `creature_template` SET `gossip_menu_id`=11128, `npcflag`=4227, `AIName`='SmartAI' WHERE `entry`=38873;
UPDATE `creature_template` SET `gossip_menu_id`=11122, `npcflag`=1, `AIName`='SmartAI' WHERE `entry`=38875;
UPDATE `creature_template` SET `gossip_menu_id`=11129, `npcflag`=3, `AIName`='SmartAI' WHERE `entry`=38876;
UPDATE `creature_template` SET `unit_flags`=768, `AIName`='SmartAI', `ScriptName`='' WHERE `entry`=38872;
UPDATE `gameobject_template` SET `AIName`='SmartGameObjectAI', `ScriptName`='' WHERE `entry` IN (202418, 202419);

-- ============================================================
-- Quest relations
-- ============================================================
DELETE FROM `creature_queststarter` WHERE `id`=38873 AND `quest`=25000;
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(38873, 25000);

-- ============================================================
-- npc_text (MoP Classic sniff IDs)
-- ============================================================
DELETE FROM `npc_text` WHERE `ID` IN (38927,38929,38930,38931,38932,38933,38935,38936,38961,38962,38963);
INSERT INTO `npc_text` (`ID`, `text0_0`, `text0_1`, `lang0`, `prob0`, `WDBVerified`) VALUES
(38927, 'This ain''t the first time I been stuck up to me teef in a jungle that up an'' done ambushed me. I been all over the world, $gson:missy;, an'' I''ll be hog-smacked if this one''s gonna be the end of me.', '', 0, 1, 18414),
(38929, 'Back in the day ''afore the day, me and me mates didn''t have much in the way of drinkin'' money, as is we spent our pay on booze an'' that didn''t leave none left over for more casual drinkin''.$B$BSo we decide we''re gonna comm-an-deer one of them epic-sized kegs of ol'' Barkerstout from the loft of Barky''s Brewpub.$B$BWe chopped the supports out with hatchets and that ol'' boy just up an'' ROLLED on out through the side of the thatch roof like a baby bird leavin'' the nest, but some sort of big round beer bird, that could crush a man.', '', 0, 1, 18414),
(38930, 'You know why they call it stout? ''Cuz once it were rolling down the hill it couldnae be bargained wit'', couldnae be reasoned wit''. It felt no pity, no remorse, no fear.$B$BIt were like a beervalanche, which is a damn fine way to die.$B$BSo that keg explodered soon as it hit the orphanage at the bottom of the hill, and I dove inta the mud puddle wit'' me mouth open, an''... Come to think of it, I don''t ''member how this story ends. Wanna see the tattoo?$B$B<Ol'' Durty Pete drops his pants.>', '', 0, 1, 18414),
(38931, 'You see this here gun, $gboy:missy;?$B$B<Ol'' Durty Pete casually waves his weapon directly at your face.>$B$BThis here gun''s seen action on every continent but for Northrend. It''s saved me life more times than I can count: Seven. One time, "Big Mackie" MacDungal - not the wee MacDungal now, but his pappy - offered me his youngest daughter in barter for me gun.$B$BLet me tell you somethin'' about Edie: she and me gun had a lot in common. Wide-mouthed. Buck-toofed. Dangerous from the biz-ness end. Likin'' to go oft half-cocked.$B$BYou heard about the time wee Edie bet Chuck Brandybeard 25 silver she could kick ''im in the head so hard he would weep snot?', '', 0, 1, 18414),
(38932, 'Ayup, word got around fast and soon the whole of Menethil gathered around to see it through, with bettin'' and fruit vendors and the like, an'' there were some consternation over the rules that had to be ironed out with a big rigamarole, ''til the mayor hisself agreed that weepin'' blood tain''t count fer nothin''.$B$BSo Chuck kneels down an'' she gets up on a stool and REARS back with that big ''ol thick leg of hers and - but I''m digressin''. Point is, I shoulda taken the deal, ''cuz this gun''s a bit dodgy.', '', 0, 1, 18414),
(38933, 'They didn''t always call me Ol'' Durty Pete.$B$BOnce, when I were young an'' durty, I thought I''d impress a sweet buxom lass named Bellie with me chiselin'' styles.$B$BOl'' Bellie! She were somthin'' else. Firm, that one. All big but for the middle, like they was meant to be.$B$BSo I found me a chunk of limestone white like her skin and I set about to chiselin'' and carvin'' and creatin'' what you''d call a masterpiece, but I got me scale all wrong, see? I ended up with an enormous, what would you call it? Butt.', '', 0, 1, 18414),
(38935, 'Back in the war - not that one, the udder one - when them orcs rode dragons? Me and Whistler set out to cross from Southshore to Menethil Harbor inna rowboat we''d built outta knotty pine while soused to our eyeballs on Cherry Grog.$B$BAn'' we got about a third the way by my reckoning, which were mostly far judgin'' by the fact that there were nothing all around us but the sea, when my feet got to start feelin'' wet.', '', 0, 1, 18414),
(38936, 'Now let me tell you somethin'' about knotty pine, it''s got knots, $gson:missy;.$B$BAnd knots is just big holes in trees, which makes for a stupid rowboat and I blamed Whistler but he had his own ideas about my choice of tar (I chose to forget it) so we decided to settle it by drinking.$B$BWhen the grog ran out, that''s when we had the knife fight. For sport.$B$BAnyways, we never made it to Menethil, and I''m pretty sure one or both of us drowned.', '', 0, 1, 18414),
(38961, 'There''s some good huntin'' to be found here in the Barrens, if you''ve the stomach for it.', '', 0, 1, 18414),
(38962, 'We''re still cut off from Forward Command, but thanks to your ''negotiations'' with the quilboar, we should be able to re-open the roads with a little elbow grease.', '', 0, 1, 18414),
(38963, 'The Corporal''s a good man, but he''s got tunnel-vision when it comes to his orders.$B$BSomething unnatural is happening to the land here, $n. This jungle shouldn''t be here!', '', 0, 1, 18414);

-- ============================================================
-- Gossip menus (point to sniff TextIDs)
-- ============================================================
DELETE FROM `gossip_menu` WHERE `MenuID` IN (11108,11113,11115,11116,11117,11118,11119,11121,11122,11128,11129);
INSERT INTO `gossip_menu` (`MenuID`, `TextID`) VALUES
(11108, 38962),
(11113, 38927),
(11115, 38929),
(11116, 38930),
(11117, 38932),
(11118, 38931),
(11119, 38933),
(11121, 38935),
(11122, 38936),
(11128, 38961),
(11129, 38963);

-- Pete story chain + Goucho vendor (ActionMenuID reconstructed from sniff option order)
DELETE FROM `gossip_menu_option` WHERE `MenuID` IN (11113,11115,11116,11117,11118,11119,11121,11122,11128);
INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`) VALUES
(11113, 0, 0, 'Tell me a story, Ol'' Durty Pete!', 0, 1, 1, 11115, 0, 0, 0, '', 0),
(11115, 0, 0, 'That sounds serious.', 0, 1, 1, 11116, 0, 0, 0, '', 0),
(11115, 1, 0, 'I''ve heard this one. Tell me another...', 0, 1, 1, 11118, 0, 0, 0, '', 0),
(11116, 0, 0, 'Tell me another story, Ol'' Durty Pete!', 0, 1, 1, 11118, 0, 0, 0, '', 0),
(11118, 0, 0, 'Weep SNOT!?', 0, 1, 1, 11117, 0, 0, 0, '', 0),
(11118, 1, 0, 'I''ve heard this one. Tell me another...', 0, 1, 1, 11119, 0, 0, 0, '', 0),
(11117, 0, 0, 'Tell me another story, Ol'' Durty Pete!', 0, 1, 1, 11119, 0, 0, 0, '', 0),
(11119, 0, 0, 'A butt?', 0, 1, 1, 11121, 0, 0, 0, '', 0),
(11119, 1, 0, 'I''ve heard this one. Tell me another...', 0, 1, 1, 11121, 0, 0, 0, '', 0),
(11121, 0, 0, 'That''s not good.', 0, 1, 1, 11122, 0, 0, 0, '', 0),
(11121, 1, 0, 'I''ve heard this one. Tell me another...', 0, 1, 1, 11113, 0, 0, 0, '', 0),
(11122, 0, 0, 'Tell me another story, Ol'' Durty Pete!', 0, 1, 1, 11113, 0, 0, 0, '', 0),
(11128, 0, 1, 'Let me browse your goods.', 0, 3, 128, 0, 0, 0, 0, '', 0);

-- Also allow starting Pete stories from default idle menu 11122 via 11113 invite
-- (11122 already links to 11113 above)

-- ============================================================
-- Creature text
-- ============================================================
DELETE FROM `creature_text` WHERE `entry` IN (38871,38872,38873,38875,38876);
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(38871, 0, 0, 'Now then, $n. Let''s see what this what this crazy stick is capable of...', 12, 0, 100, 0, 0, 0, 'Corporal Teegan'),
(38871, 1, 0, 'Interesting. Very ... interesting.', 12, 0, 100, 0, 0, 0, 'Corporal Teegan'),
(38872, 0, 0, '$n - help! The ropes ... please...', 12, 0, 100, 0, 0, 0, 'Ambassador Gaines'),
(38872, 1, 0, 'Ah, thank you! Suffice it to say... Negotiations did proceed as planned.', 12, 0, 100, 0, 0, 0, 'Ambassador Gaines'),
(38872, 2, 0, 'I should be able to get back to camp on my own.', 12, 0, 100, 0, 0, 0, 'Ambassador Gaines'),
(38873, 0, 0, 'Hang in there ol'' buddy!', 12, 0, 100, 0, 0, 0, 'Goucho'),
(38875, 0, 0, 'Gol-Crawdad!?', 12, 0, 100, 0, 0, 0, 'Ol'' Durty Pete'),
(38875, 1, 0, 'Frappin'' cratchet! Sonnova HEY what!?', 12, 0, 100, 0, 0, 0, 'Ol'' Durty Pete'),
(38875, 2, 0, '''magonna - git! Git! Horkin'' nit, unfh, DABBIT!', 12, 0, 100, 0, 0, 0, 'Ol'' Durty Pete'),
(38876, 0, 0, 'Stop shooting you senile idiot! You''re going to kill somebody!', 12, 0, 100, 0, 0, 0, 'Hannah Bridgewater');

-- ============================================================
-- Ambassador Gaines spawn + waypoints (restraints already present)
-- ============================================================
DELETE FROM `creature` WHERE `guid`=8202170;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8202170, 38872, 1, 1, 0, 0, 0, 0, -2101.96, -2581.554, 93.46812, 4.066617, 300, 0, 0, 1, 0, 0, 0, 0, 0);

DELETE FROM `waypoints` WHERE `entry`=38872;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(38872, 1, -2104.89, -2583.25, 94.0367, 'Ambassador Gaines'),
(38872, 2, -2111.89, -2585.00, 94.5367, 'Ambassador Gaines'),
(38872, 3, -2118.89, -2588.25, 94.7867, 'Ambassador Gaines'),
(38872, 4, -2123.89, -2597.00, 94.5367, 'Ambassador Gaines'),
(38872, 5, -2123.32, -2611.96, 93.6054, 'Ambassador Gaines');

-- ============================================================
-- SmartAI: Quilboar Restraints → Gaines (both poles required)
-- ============================================================
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (202418, 202419) AND `source_type`=1;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(202418, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 38872, 15, 0, 0, 0, 0, 0, 'Quilboar Restraint - On LootState GO_ACTIVATED - Set Data 1 1 on Gaines'),
(202419, 1, 0, 0, 70, 0, 100, 0, 2, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 38872, 15, 0, 0, 0, 0, 0, 'Quilboar Restraint - On LootState GO_ACTIVATED - Set Data 1 1 on Gaines');

DELETE FROM `smart_scripts` WHERE `entryorguid`=38872 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=3887200 AND `source_type`=9;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
-- Bound state: phase 1 = zero poles, phase 2 = one pole, phase 3 = freed
(38872, 0, 0, 1, 11, 0, 100, 0, 0, 0, 0, 0, 22, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - On Respawn - Set Phase 1'),
(38872, 0, 1, 0, 61, 0, 100, 0, 0, 0, 0, 0, 11, 72866, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Linked - Cast Cosmetic Stun'),
(38872, 0, 2, 3, 10, 1, 100, 0, 1, 20, 60000, 60000, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - OOC LoS (phase 1) - Store Invoker'),
(38872, 0, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Linked - Say Line 0'),
(38872, 0, 4, 0, 38, 1, 100, 0, 1, 1, 0, 0, 22, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - On Data 1 1 (phase 1) - Set Phase 2'),
(38872, 0, 5, 6, 38, 2, 100, 0, 1, 1, 0, 0, 64, 1, 0, 0, 0, 0, 0, 21, 15, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - On Data 1 1 (phase 2) - Store Closest Player'),
(38872, 0, 6, 0, 61, 0, 100, 0, 0, 0, 0, 0, 80, 3887200, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Linked - Run Free Script'),
-- Free script (sniff timings after second Opening)
(3887200, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 22, 3, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Set Phase 3'),
(3887200, 9, 1, 0, 0, 0, 100, 0, 0, 0, 0, 0, 45, 1, 1, 0, 0, 0, 0, 11, 38924, 30, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Set Data on Rope Bunnies'),
(3887200, 9, 2, 0, 0, 0, 100, 0, 0, 0, 0, 0, 28, 72866, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Remove Stun'),
(3887200, 9, 3, 0, 0, 0, 100, 0, 0, 0, 0, 0, 33, 38872, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Kill Credit Stored'),
(3887200, 9, 4, 0, 0, 0, 100, 0, 0, 0, 0, 0, 11, 73118, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Cast Kill Credit'),
(3887200, 9, 5, 0, 0, 0, 100, 0, 100, 100, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Say Line 1'),
(3887200, 9, 6, 0, 0, 0, 100, 0, 3200, 3200, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Say Line 2'),
(3887200, 9, 7, 0, 0, 0, 100, 0, 500, 500, 0, 0, 53, 1, 38872, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - Script - Start Waypoints'),
(38872, 0, 7, 0, 58, 0, 100, 0, 5, 38872, 0, 0, 41, 2000, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ambassador Gaines - On WP Ended - Despawn');

-- LoS help only while on quest 25022 (SourceGroup = event_id+1)
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId`=22 AND `SourceEntry`=38872 AND `SourceId`=0 AND `SourceGroup`=3;
INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(22, 3, 38872, 0, 0, 9, 0, 25022, 0, 0, 0, 0, 0, '', 'Gaines LoS help only if quest 25022 taken');

-- ============================================================
-- SmartAI: Quest 25027 turn-in RP (Teegan → Pete → Hannah → Goucho)
-- ============================================================
DELETE FROM `smart_scripts` WHERE `entryorguid`=38871 AND `source_type`=0;
DELETE FROM `smart_scripts` WHERE `entryorguid`=3887100 AND `source_type`=9;
DELETE FROM `smart_scripts` WHERE `entryorguid`=38875 AND `source_type`=0 AND `id` IN (0,1,2,3,4);
DELETE FROM `smart_scripts` WHERE `entryorguid`=3887500 AND `source_type`=9;
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (38873,38876) AND `source_type`=0 AND `id`=0;

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(38871, 0, 0, 0, 20, 0, 100, 0, 25027, 0, 0, 0, 80, 3887100, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Corporal Teegan - On Quest 25027 Rewarded - Run RP Script'),
(3887100, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Teegan - Script - Say Line 0'),
(3887100, 9, 1, 0, 0, 0, 100, 0, 3200, 3200, 0, 0, 11, 73093, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Teegan - Script - Cast Nature Channeling'),
(3887100, 9, 2, 0, 0, 0, 100, 0, 3700, 3700, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 38875, 30, 0, 0, 0, 0, 0, 'Teegan - Script - Set Data on Pete'),
(3887100, 9, 3, 0, 0, 0, 100, 0, 1300, 1300, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Teegan - Script - Say Line 1'),
(3887100, 9, 4, 0, 0, 0, 100, 0, 3200, 3200, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 38876, 30, 0, 0, 0, 0, 0, 'Teegan - Script - Set Data on Hannah'),
(3887100, 9, 5, 0, 0, 0, 100, 0, 1600, 1600, 0, 0, 45, 1, 1, 0, 0, 0, 0, 19, 38873, 30, 0, 0, 0, 0, 0, 'Teegan - Script - Set Data on Goucho'),
(38875, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 80, 3887500, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ol'' Durty Pete - On Data 1 1 - Run Panic Script'),
(3887500, 9, 0, 0, 0, 0, 100, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Pete - Script - Say Line 0'),
(3887500, 9, 1, 0, 0, 0, 100, 0, 1200, 1200, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Pete - Script - Say Line 1'),
(3887500, 9, 2, 0, 0, 0, 100, 0, 3200, 3200, 0, 0, 1, 2, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Pete - Script - Say Line 2'),
(38876, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Hannah - On Data 1 1 - Say Line 0'),
(38873, 0, 0, 0, 38, 0, 100, 0, 1, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Goucho - On Data 1 1 - Say Line 0');
