-- Opening three different postboxes brings out Postmaster Malown (spell 24627, "Summon Postmaster Malown", is in the
-- 5.4.8 client data and the Stratholme script header says he comes with the third postbox). Each postbox script
-- already summons its three Undead Postmen two seconds after the click; add a row that tells the instance script
-- which of the six boxes was opened (instance data 8, the box number 1 to 6). The instance script counts distinct
-- boxes and summons Malown at the third. Rows 0 to 2 of each script are untouched.
DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `id` = 3 AND `entryorguid` IN (17634600,17634900,17635000,17635100,17635200,17635300);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(17634600, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 34, 8, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'MR Postbox Script - After 2s - Set Instance Data Postbox 1'),
(17634900, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 34, 8, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'CS Postbox Script - After 2s - Set Instance Data Postbox 2'),
(17635000, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 34, 8, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'FL Postbox Script - After 2s - Set Instance Data Postbox 3'),
(17635100, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 34, 8, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'ES Postbox Script - After 2s - Set Instance Data Postbox 4'),
(17635200, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 34, 8, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'KS Postbox Script - After 2s - Set Instance Data Postbox 5'),
(17635300, 9, 3, 0, 0, 0, 100, 0, 2000, 2000, 0, 0, 0, 34, 8, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 'FS Postbox Script - After 2s - Set Instance Data Postbox 6');

-- Postmaster Malown is summoned by the postboxes now (instance script, third distinct postbox), at the spot of his
-- old static spawn, so the always-standing copy goes. Evidence for guid 8447318: not a duplicate but the event's
-- replacement. Same entry 11143 at the same coordinates as the summon position (3639.3, -3201.8, 128.879), phase 0,
-- spawn mask 2, no pool, event, addon, formation, SmartAI or condition row refers to it. The row came from the Legends
-- of Azeroth data (VerifiedBuild 0); TrinityCore 3.3.5 and the Cataclysm Preservation Project 4.3.4 have no spawn
-- for him.
DELETE FROM `creature` WHERE `guid` = 8447318 AND `id` = 11143;
