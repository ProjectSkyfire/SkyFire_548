-- Clear stale spell display/focus references reported by DBErrors.

UPDATE `quest_template`
SET `RewardSpell` = 0
WHERE (`Id`, `RewardSpell`) IN
(
    (1652, 5502),
    (9673, 23357),
    (7463, 10140),
    (6089, 23357),
    (6103, 23357),
    (9451, 8075),
    (9675, 23357),
    (9691, 5502),
    (25902, 8071),
    (25083, 23357),
    (28720, 92107)
);

UPDATE `quest_template`
SET `RewardSpellCast` = 0
WHERE (`Id`, `RewardSpellCast`) IN
(
    (9451, 8073),
    (25902, 8073)
);

UPDATE `gameobject_template`
SET `data0` = 0
WHERE `type` = 8
  AND (`entry`, `data0`) IN
(
    (212582, 5705760),
    (212587, 5705760),
    (212594, 5705760),
    (212595, 5705760),
    (213868, 5749504),
    (213869, 5749504),
    (213870, 5749504),
    (213871, 5749504),
    (213874, 5749504),
    (213875, 5749504),
    (213876, 5749504),
    (213877, 5749504)
);

DELETE FROM `conditions`
WHERE `SourceTypeOrReferenceId` = 17
  AND `SourceGroup` = 0
  AND `SourceEntry` = 40200
  AND `SourceId` = 0
  AND `ElseGroup` = 0
  AND `ConditionTypeOrReference` = 9
  AND `ConditionTarget` = 0
  AND `ConditionValue1` IN (11010, 11023, 11102)
  AND `ConditionValue2` = 0
  AND `ConditionValue3` = 0
  AND `NegativeCondition` = 1;
