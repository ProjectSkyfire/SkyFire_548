-- Quest 13569 (The Ritual Bond): Grovekeeper's Incense trance, spirit phase, Karithus turn-in.

DELETE FROM `smart_scripts` WHERE `source_type` = 1 AND `entryorguid` = 194771;
DELETE FROM `smart_scripts` WHERE `source_type` = 0 AND `entryorguid` IN (33048, 33053, 33131, 33132, 33133, 33296);
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` = 3304800;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 15 AND `SourceGroup` = 10410;
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 18 AND `SourceGroup` IN (33131, 33132, 33133);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` IN (19, 20) AND `SourceEntry` IN (13567, 13568, 13597);
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 22 AND `SourceEntry` IN (33131, 33132, 33133);
DELETE FROM `npc_spellclick_spells` WHERE `npc_entry` IN (33131, 33132, 33133);
DELETE FROM `creature_queststarter` WHERE `id` IN (33131, 33132, 33133);
DELETE FROM `creature_questender` WHERE `id` IN (33131, 33132, 33133);
DELETE FROM `creature` WHERE `guid` IN (308261, 308262, 308263, 308264);
DELETE FROM `creature_addon` WHERE `guid` = 188276;
DELETE FROM `creature_text` WHERE `entry` = 33048 AND `groupid` IN (0, 1);

INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
-- Grovekeeper's Incense: SMART_ACTION_CAST (11) only works on creatures; forward player to Karithus.
(194771, 1, 0, 1, 62, 0, 100, 0, 10410, 0, 0, 0, 0, 64, 1, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Grovekeeper''s Incense - On Gossip Select - Store Target List'),
(194771, 1, 1, 2, 61, 0, 100, 0, 0, 0, 0, 0, 0, 100, 1, 0, 0, 0, 0, 0, 10, 188275, 33048, 0, 0, 0, 0, 0, 'Grovekeeper''s Incense - Linked - Send Target To Keeper Karithus'),
(194771, 1, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 10, 188275, 33048, 0, 0, 0, 0, 0, 'Grovekeeper''s Incense - Linked - Set Data 0 1 On Keeper Karithus'),
(194771, 1, 3, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 72, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Grovekeeper''s Incense - Linked - Close Gossip'),
-- Ritual Bond Bunny: cloud VFX on turn-in ceremony.
(33296, 0, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 11, 64362, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Ritual Bond Bunny - On Data Set 0 1 - Cast Ritual Bond Cloud'),
-- Grimclaw: injured bear resting at the thicket (Feign Death from template/sniff auras).
(33053, 0, 0, 0, 11, 0, 100, 0, 0, 0, 0, 0, 0, 11, 51329, 2, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Grimclaw - On Respawn - Cast Feign Death'),
-- Keeper Karithus: incense gossip forwards here to cast Grovekeeper's Trance (64198) on the player.
(33048, 0, 0, 0, 38, 0, 100, 0, 0, 1, 0, 0, 0, 11, 64198, 0, 0, 0, 0, 0, 12, 1, 0, 0, 0, 0, 0, 0, 'Keeper Karithus - On Data Set 0 1 - Cast Grovekeeper''s Trance'),
-- Keeper Karithus turn-in ceremony for quest 13569.
(33048, 0, 1, 2, 20, 0, 100, 0, 13569, 0, 0, 0, 0, 28, 64198, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Keeper Karithus - On Reward Quest 13569 - Remove Grovekeeper''s Trance'),
(33048, 0, 2, 3, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 64359, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Keeper Karithus - Linked - Cast Ritual Bond'),
(33048, 0, 3, 4, 61, 0, 100, 0, 0, 0, 0, 0, 0, 11, 62182, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Keeper Karithus - Linked - Cast Nature Channeling'),
(33048, 0, 4, 5, 61, 0, 100, 0, 0, 0, 0, 0, 0, 45, 0, 1, 0, 0, 0, 0, 19, 33296, 20, 0, 0, 0, 0, 0, 'Keeper Karithus - Linked - Trigger Ritual Bond Bunny'),
(33048, 0, 5, 6, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 0, 7000, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Keeper Karithus - Linked - Say Line 0'),
(33048, 0, 6, 7, 52, 0, 100, 0, 0, 33048, 0, 0, 0, 28, 62182, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Keeper Karithus - On Text Over Line 0 - Remove Nature Channeling'),
(33048, 0, 7, 0, 61, 0, 100, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Keeper Karithus - Linked - Say Line 1');

-- Sniff export left OptionType at 0; SkyFire only shows GO gossip options with OptionType 1 (GOSSIP_OPTION_GOSSIP).
UPDATE `gossip_menu_option`
SET `OptionType` = 1
WHERE `MenuID` = 10410
  AND `OptionID` = 0
  AND `OptionType` <> 1;

UPDATE `gameobject_template`
SET `AIName` = 'SmartGameObjectAI', `ScriptName` = '', `size` = 0.5
WHERE `entry` = 194771;

-- Great Spirits: phase 170 questgivers during trance (npcflag=2, not spellclick).
UPDATE `creature_template`
SET `AIName` = '', `ScriptName` = '', `npcflag` = 2, `unit_flags` = 32768
WHERE `entry` IN (33131, 33132, 33133);

UPDATE `creature_template`
SET `AIName` = 'SmartAI', `ScriptName` = ''
WHERE `entry` IN (33048, 33053, 33296)
  AND (`AIName` IS NULL OR `AIName` = '' OR `AIName` <> 'SmartAI' OR `ScriptName` IS NOT NULL AND `ScriptName` <> '');

-- Ritual Bond Bunny over the incense mound: invisible trigger (model 11686 + TRIGGER flag).
UPDATE `creature_template`
SET `modelid1` = 11686, `modelid2` = 0, `flags_extra` = `flags_extra` | 128
WHERE `entry` = 33296
  AND (`modelid1` <> 11686 OR (`flags_extra` & 128) = 0);

-- Keeper Karithus: visible in default (169) and trance (170). PhaseXPhaseGroup 371 = {169, 170}.
UPDATE `creature`
SET `phaseId` = 0, `phaseGroup` = 371
WHERE `guid` = 188275
  AND `id` = 33048
  AND (`phaseId` <> 0 OR `phaseGroup` <> 371);

-- Grimclaw: lay down injured (UNIT_STAND_STATE_DEAD + Feign Death aura).
UPDATE `creature_template_addon`
SET `bytes1` = 7, `bytes2` = 1, `emote` = 0, `auras` = '51329'
WHERE `entry` = 33053
  AND (`bytes1` <> 7 OR `auras` IS NULL OR `auras` NOT LIKE '%51329%');

INSERT INTO `creature_addon` (`guid`, `path_id`, `mount`, `bytes1`, `bytes2`, `emote`, `auras`) VALUES
(188276, 0, 0, 7, 1, 0, '51329');

-- Great Spirits in phase 170 around the thicket.
-- Grovekeeper's Trance (64198) applies SPELL_AURA_PHASE via MiscValueB=170.
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `MovementType`) VALUES
(308261, 33133, 1, 1, 170, 0, 6474.35, 198.16, 5.82357, 2.28221, 120, 0, 0),
(308262, 33132, 1, 1, 170, 0, 6561.43, 272.294, 11.2011, 3.22886, 120, 0, 0),
(308263, 33131, 1, 1, 170, 0, 6491.19, 250.34, 6.02786, 2.47837, 120, 0, 0);

-- Spirit blessing quests: accept/turn-in at spirit; RewardSpellCast 62803 completes 13569.
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(33133, 13567),
(33131, 13568),
(33132, 13597);

INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(33133, 13567),
(33131, 13568),
(33132, 13597);

-- Spirit quest dialogue.
UPDATE `quest_template`
SET `Details` = 'I am the caretaker for my kind, leading not through force, but through wisdom and swift action. Wisdom is what carries us into the deeper forest when danger beckons and swiftness is what leaves our enemies basking in our absence.$B$BIn this wisdom, I see through to your true intentions, and I know that you are deserving of my blessing, the blessing of my swiftness. It can be yours, but know that the other spirits will not welcome you once you proceed - consider this carefully before accepting my boon.',
    `OfferRewardText` = 'I am the caretaker for my kind, leading not through force, but through wisdom and swift action. Wisdom is what carries us into the deeper forest when danger beckons and swiftness is what leaves our enemies basking in our absence.$B$BIn this wisdom, I see through to your true intentions, and I know that you are deserving of my blessing, the blessing of my swiftness. It can be yours, but know that the other spirits will not welcome you once you proceed - consider this carefully before accepting my boon.'
WHERE `Id` = 13567;

UPDATE `quest_template`
SET `Details` = 'I share these forests willingly, accepting night elf kind into my home, and on occasion calling them friend. Though they have been tainted greatly in recent memory, I can see in you the same strength, honor, and spirit that first endeared them to me. If you place value in the ferocity necessary to overcome all odds, then I am willing to call you my champion. Know now that if you side with my kind, the other spirits will not accept you, so consider your choice carefully.',
    `OfferRewardText` = 'I share these forests willingly, accepting night elf kind into my home, and on occasion calling them friend. Though they have been tainted greatly in recent memory, I can see in you the same strength, honor, and spirit that first endeared them to me. If you place value in the ferocity necessary to overcome all odds, then I am willing to call you my champion. Know now that if you side with my kind, the other spirits will not accept you, so consider your choice carefully.'
WHERE `Id` = 13568;

UPDATE `quest_template`
SET `Details` = 'For countless ages I have walked these forests and lived in these mountains. Through snow and fire, I have watched civilizations crumble and nature press on. Now, the very earth turns against us, and it becomes very apparent that we must fight if our legacy is to continue. Your heart is true, and I would choose you as our champion. If you value the thick hide and resilience of my kind, then accept my boon. Know that the other spirits will not accept you once this choice is made, so choose wisely.',
    `OfferRewardText` = 'For countless ages I have walked these forests and lived in these mountains. Through snow and fire, I have watched civilizations crumble and nature press on. Now, the very earth turns against us, and it becomes very apparent that we must fight if our legacy is to continue. Your heart is true, and I would choose you as our champion. If you value the thick hide and resilience of my kind, then accept my boon. Know that the other spirits will not accept you once this choice is made, so choose wisely.'
WHERE `Id` = 13597;

INSERT INTO `conditions` (`SourceTypeOrReferenceId`, `SourceGroup`, `SourceEntry`, `SourceId`, `ElseGroup`, `ConditionTypeOrReference`, `ConditionTarget`, `ConditionValue1`, `ConditionValue2`, `ConditionValue3`, `NegativeCondition`, `ErrorType`, `ErrorTextId`, `ScriptName`, `Comment`) VALUES
(15, 10410, 0, 0, 0, 9, 0, 13569, 0, 0, 0, 0, 0, '', 'Grovekeeper''s Incense - quest 13569 taken'),
-- Spirit of the Stag (13567): only during trance while Ritual Bond is active.
(19, 0, 13567, 0, 0, 9, 0, 13569, 0, 0, 0, 0, 0, '', 'Spirit of the Stag - offer during Ritual Bond'),
(19, 0, 13567, 0, 0, 1, 0, 64198, 0, 0, 0, 0, 0, '', 'Spirit of the Stag - offer during Grovekeeper''s Trance'),
(19, 0, 13567, 0, 0, 8, 0, 13568, 0, 0, 1, 0, 0, '', 'Spirit of the Stag - not if Moonstalker already chosen'),
(19, 0, 13567, 0, 0, 8, 0, 13597, 0, 0, 1, 0, 0, '', 'Spirit of the Stag - not if Bear already chosen'),
(19, 0, 13567, 0, 0, 14, 0, 13568, 0, 0, 0, 0, 0, '', 'Spirit of the Stag - Moonstalker quest not taken'),
(19, 0, 13567, 0, 0, 14, 0, 13597, 0, 0, 0, 0, 0, '', 'Spirit of the Stag - Bear quest not taken'),
(20, 0, 13567, 0, 0, 9, 0, 13569, 0, 0, 0, 0, 0, '', 'Spirit of the Stag - quest mark during Ritual Bond'),
(20, 0, 13567, 0, 0, 1, 0, 64198, 0, 0, 0, 0, 0, '', 'Spirit of the Stag - quest mark during trance'),
(20, 0, 13567, 0, 0, 8, 0, 13568, 0, 0, 1, 0, 0, '', 'Spirit of the Stag - hide mark if Moonstalker chosen'),
(20, 0, 13567, 0, 0, 8, 0, 13597, 0, 0, 1, 0, 0, '', 'Spirit of the Stag - hide mark if Bear chosen'),
-- Spirit of the Moonstalker (13568).
(19, 0, 13568, 0, 0, 9, 0, 13569, 0, 0, 0, 0, 0, '', 'Spirit of the Moonstalker - offer during Ritual Bond'),
(19, 0, 13568, 0, 0, 1, 0, 64198, 0, 0, 0, 0, 0, '', 'Spirit of the Moonstalker - offer during trance'),
(19, 0, 13568, 0, 0, 8, 0, 13567, 0, 0, 1, 0, 0, '', 'Spirit of the Moonstalker - not if Stag already chosen'),
(19, 0, 13568, 0, 0, 8, 0, 13597, 0, 0, 1, 0, 0, '', 'Spirit of the Moonstalker - not if Bear already chosen'),
(19, 0, 13568, 0, 0, 14, 0, 13567, 0, 0, 0, 0, 0, '', 'Spirit of the Moonstalker - Stag quest not taken'),
(19, 0, 13568, 0, 0, 14, 0, 13597, 0, 0, 0, 0, 0, '', 'Spirit of the Moonstalker - Bear quest not taken'),
(20, 0, 13568, 0, 0, 9, 0, 13569, 0, 0, 0, 0, 0, '', 'Spirit of the Moonstalker - quest mark during Ritual Bond'),
(20, 0, 13568, 0, 0, 1, 0, 64198, 0, 0, 0, 0, 0, '', 'Spirit of the Moonstalker - quest mark during trance'),
(20, 0, 13568, 0, 0, 8, 0, 13567, 0, 0, 1, 0, 0, '', 'Spirit of the Moonstalker - hide mark if Stag chosen'),
(20, 0, 13568, 0, 0, 8, 0, 13597, 0, 0, 1, 0, 0, '', 'Spirit of the Moonstalker - hide mark if Bear chosen'),
-- Spirit of the Thistle Bear (13597).
(19, 0, 13597, 0, 0, 9, 0, 13569, 0, 0, 0, 0, 0, '', 'Spirit of the Thistle Bear - offer during Ritual Bond'),
(19, 0, 13597, 0, 0, 1, 0, 64198, 0, 0, 0, 0, 0, '', 'Spirit of the Thistle Bear - offer during trance'),
(19, 0, 13597, 0, 0, 8, 0, 13567, 0, 0, 1, 0, 0, '', 'Spirit of the Thistle Bear - not if Stag already chosen'),
(19, 0, 13597, 0, 0, 8, 0, 13568, 0, 0, 1, 0, 0, '', 'Spirit of the Thistle Bear - not if Moonstalker already chosen'),
(19, 0, 13597, 0, 0, 14, 0, 13567, 0, 0, 0, 0, 0, '', 'Spirit of the Thistle Bear - Stag quest not taken'),
(19, 0, 13597, 0, 0, 14, 0, 13568, 0, 0, 0, 0, 0, '', 'Spirit of the Thistle Bear - Moonstalker quest not taken'),
(20, 0, 13597, 0, 0, 9, 0, 13569, 0, 0, 0, 0, 0, '', 'Spirit of the Thistle Bear - quest mark during Ritual Bond'),
(20, 0, 13597, 0, 0, 1, 0, 64198, 0, 0, 0, 0, 0, '', 'Spirit of the Thistle Bear - quest mark during trance'),
(20, 0, 13597, 0, 0, 8, 0, 13567, 0, 0, 1, 0, 0, '', 'Spirit of the Thistle Bear - hide mark if Stag chosen'),
(20, 0, 13597, 0, 0, 8, 0, 13568, 0, 0, 1, 0, 0, '', 'Spirit of the Thistle Bear - hide mark if Moonstalker chosen');

INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(33048, 0, 0, 'Please hold still for a moment.', 12, 0, 100, 0, 0, 0, 'Keeper Karithus'),
(33048, 1, 0, 'It is done. The spirit is bound with you and you will carry it with you anytime you are in Darkshore. May you serve nature well.', 12, 0, 100, 0, 0, 0, 'Keeper Karithus');
