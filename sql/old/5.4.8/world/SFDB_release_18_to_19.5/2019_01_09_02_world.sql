-- SAI for Quest: Feed of Evil (14461)
SET @FT := 36727;
SET @ST := 37155;
SET @TT := 37156;

UPDATE creature_template SET AIName="SmartAI" WHERE entry IN (@FT, @ST, @TT);

DELETE FROM smart_scripts WHERE entryorguid IN (@FT, @ST, @TT) AND source_type=0 AND id in (0, 1);
INSERT INTO smart_scripts (entryorguid, source_type, id, link, event_type, event_phase_mask, event_chance, event_flags, event_param1, event_param2, event_param3, event_param4, action_type, action_param1, action_param2, action_param3, action_param4, action_param5, action_param6, target_type, target_param1, target_param2, target_param3, target_x, target_y, target_z, target_o, comment) VALUES
(@FT, 0, 0, 1, 8, 0, 100, 0, 69228, 0, 0, 0, 33, @FT, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "First Trough - On Spellhit - Quest Credit"), 
(@FT, 0, 1, 0, 61, 0, 100, 0, 69228, 0, 0, 0, 11, 42345, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "First Trough - On Spellhit - Cast Cosmetic"), 
(@ST, 0, 0, 1, 8, 0, 100, 0, 69228, 0, 0, 0, 33, @ST, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Second Trough - On Spellhit - Quest Credit"), 
(@ST, 0, 1, 0, 61, 0, 100, 0, 69228, 0, 0, 0, 11, 42345, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Second Trough - On Spellhit - Cast Cosmetic"), 
(@TT, 0, 0, 1, 8, 0, 100, 0, 69228, 0, 0, 0, 33, @TT, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, "Third Trough - On Spellhit - Quest Credit"), 
(@TT, 0, 1, 0, 61, 0, 100, 0, 69228, 0, 0, 0, 11, 42345, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, "Third Trough - On Spellhit - Cast Cosmetic");

DELETE FROM conditions WHERE SourceTypeOrReferenceId=13 AND SourceGroup=1 AND SourceEntry=69228;
INSERT INTO conditions (SourceTypeOrReferenceId, SourceGroup, SourceEntry, SourceId, ElseGroup, ConditionTypeOrReference, ConditionTarget, ConditionValue1, ConditionValue2, ConditionValue3, ErrorTextId, ScriptName, Comment) VALUES
(13, 1, 69228, 0, 0, 31, 0, 3, @TT, 0, 0, "", "Throw Torch - Target Third Trough"), 
(13, 1, 69228, 0, 1, 31, 0, 3, @ST, 0, 0, "", "Throw Torch - Target Second Trough"), 
(13, 1, 69228, 0, 2, 31, 0, 3, @FT, 0, 0, "", "Throw Torch - Target First Trough");
