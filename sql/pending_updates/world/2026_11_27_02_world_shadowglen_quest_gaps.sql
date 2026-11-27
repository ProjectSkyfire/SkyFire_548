-- Shadowglen quest follow-ups, monk map markers, and antidote turn-in dialogue (#683).
-- Preserve both Laoxi placements; neither is established to be an erroneous duplicate.
UPDATE `quest_template`
SET `NextQuestId` = CASE `Id`
    WHEN 3106 THEN 26945 WHEN 3117 THEN 26947 WHEN 3118 THEN 26946
    WHEN 3119 THEN 26949 WHEN 3120 THEN 26948 WHEN 26841 THEN 26940
    WHEN 31168 THEN 31169 END,
    `NextQuestIdChain` = CASE `Id`
    WHEN 3106 THEN 26945 WHEN 3117 THEN 26947 WHEN 3118 THEN 26946
    WHEN 3119 THEN 26949 WHEN 3120 THEN 26948 WHEN 26841 THEN 26940
    WHEN 31168 THEN 31169 END
WHERE `Id` IN (3106, 3117, 3118, 3119, 3120, 26841, 31168);

-- Objective index 0 is spell-practice credit. Learning the spell (index 255) has no map location.
DELETE FROM `quest_poi_points` WHERE `questId` IN (31168, 31169);
DELETE FROM `quest_poi` WHERE `questId` IN (31168, 31169);
INSERT INTO `quest_poi` (`questId`, `id`, `objIndex`, `mapid`, `WorldMapAreaId`, `FloorId`, `unk3`, `unk4`) VALUES
(31168, 0, -1, 1, 41, 0, 0, 1),
(31169, 0, -1, 1, 41, 0, 0, 1),
(31169, 1, 0, 1, 41, 0, 0, 7);
INSERT INTO `quest_poi_points` (`questId`, `id`, `idx`, `x`, `y`) VALUES
(31168, 0, 0, 10524, 786),
(31169, 0, 0, 10524, 786),
(31169, 1, 0, 10427, 728),
(31169, 1, 1, 10486, 808),
(31169, 1, 2, 10486, 825),
(31169, 1, 3, 10483, 825),
(31169, 1, 4, 10417, 812),
(31169, 1, 5, 10408, 803),
(31169, 1, 6, 10403, 799),
(31169, 1, 7, 10397, 792),
(31169, 1, 8, 10393, 786),
(31169, 1, 9, 10390, 781),
(31169, 1, 10, 10407, 740);

-- Group 0 already contains the antidote recovery line. Event 20 is quest rewarded.
DELETE FROM `smart_scripts` WHERE `entryorguid` = 49479 AND `source_type` = 0 AND `id` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `action_type`, `action_param1`, `action_param2`, `target_type`, `comment`) VALUES
(49479, 0, 0, 0, 20, 0, 100, 0, 28724, 1, 0, 0, 7, 'Dentaria Silverglade - On Iverron''s Antidote rewarded - Say recovery line');
