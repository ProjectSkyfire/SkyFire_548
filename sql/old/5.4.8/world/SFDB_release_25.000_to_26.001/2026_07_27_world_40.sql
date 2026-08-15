-- Align contaminated quest flags and explicit discovery trigger data.

UPDATE `quest_template`
SET `Flags` = `Flags`
        - CASE WHEN (`Flags` & 4096) <> 0 THEN 4096 ELSE 0 END
        - CASE WHEN (`Flags` & 32768) <> 0 THEN 32768 ELSE 0 END,
    `SpecialFlags` = `SpecialFlags`
        - CASE WHEN (`SpecialFlags` & 1) <> 0 THEN 1 ELSE 0 END
WHERE `Id` IN (28617, 28733, 28744, 28747, 28748);

UPDATE `quest_template`
SET `RewardChoiceItemId2` = 0,
    `RewardChoiceItemCount2` = 0
WHERE `Id` = 28748
AND `RewardChoiceItemId2` = 65666;

INSERT IGNORE INTO `skill_discovery_template` (`spellId`, `reqSpell`, `reqSkillValue`, `chance`) VALUES
    (64323, 115063, 0, 100),
    (64323, 123516, 0, 100),
    (64323, 123548, 0, 100),
    (64323, 123549, 0, 100),
    (64323, 129673, 0, 100),
    (64323, 129674, 0, 100),
    (64323, 129675, 0, 100),
    (64323, 129676, 0, 100),
    (64323, 129687, 0, 100),
    (64323, 129705, 0, 100),
    (64323, 129757, 0, 100),
    (64323, 129796, 0, 100),
    (64323, 129814, 0, 100),
    (64323, 129843, 0, 100),
    (64323, 129887, 0, 100),
    (64323, 130025, 0, 100),
    (64323, 130026, 0, 100),
    (64323, 130109, 0, 100),
    (64323, 130140, 0, 100),
    (64323, 130168, 0, 100),
    (64323, 133106, 0, 100);
