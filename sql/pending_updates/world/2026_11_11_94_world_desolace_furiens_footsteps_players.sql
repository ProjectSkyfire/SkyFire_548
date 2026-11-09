-- Furien's Footsteps (14191) and Elune's Gifts (14354): the three ruin bunnies cast their credit spell on every player
-- within 30 yards every 5 seconds. The spells now only land on players who still have one of the two quests
-- open. Safe to run twice.
DELETE FROM `conditions` WHERE `SourceTypeOrReferenceId` = 17 AND `SourceEntry` IN (67342, 67343, 67344);
INSERT INTO `conditions`
(`SourceTypeOrReferenceId`,`SourceGroup`,`SourceEntry`,`SourceId`,`ElseGroup`,`ConditionTypeOrReference`,
 `ConditionTarget`,`ConditionValue1`,`ConditionValue2`,`ConditionValue3`,`NegativeCondition`,`ErrorType`,
 `ErrorTextId`,`ScriptName`,`Comment`)
VALUES
(17,0,67342,0,0,9,1,14191,0,0,0,0,0,'','Elune Cathedral Kill Credit: target has Furien''s Footsteps'),
(17,0,67342,0,1,9,1,14354,0,0,0,0,0,'','Elune Cathedral Kill Credit: target has Elune''s Gifts'),
(17,0,67343,0,0,9,1,14191,0,0,0,0,0,'','Elune Temple Kill Credit: target has Furien''s Footsteps'),
(17,0,67343,0,1,9,1,14354,0,0,0,0,0,'','Elune Temple Kill Credit: target has Elune''s Gifts'),
(17,0,67344,0,0,9,1,14191,0,0,0,0,0,'','Elune Sanctuary Kill Credit: target has Furien''s Footsteps'),
(17,0,67344,0,1,9,1,14354,0,0,0,0,0,'','Elune Sanctuary Kill Credit: target has Elune''s Gifts');
