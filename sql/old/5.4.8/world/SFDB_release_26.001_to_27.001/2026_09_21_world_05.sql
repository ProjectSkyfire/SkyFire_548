-- Quest 26674 "Mistmantle's Revenge": restore the spell focus spawn that release 26.002 is
-- missing.
--
-- The Mistmantle Family Ring (59363) casts 82029 "Call Stalvan", whose
-- SpellCastingRequirements row gives RequiresSpellFocus 1677 - gameobject 204811 "Manor
-- Mistmantle Spell Focus". 2026_07_09_world_00 added that template and a spawn for it, and
-- everything else from that update is in release 26.002:
--
--   gameobject_template  204811  type 8  displayId 5811  size 0.01  data0 1677   present
--   spell_script_names   82029 -> spell_q26674_call_stalvan                      present
--   creature_text        315 (7 rows), 35124 (5 rows)                            present
--   smart_scripts        315, 31500, 35124, 351240, 351242                       present
--   creature_template    315 and 35124 AIName 'SmartAI'                          present
--   gameobject           id 204811                                               0 rows
--
-- The spawn is the one row that did not survive, and nothing in sql/updates/world since the
-- release adds it back. Without it Spell::CheckCast fails with
-- SPELL_FAILED_REQUIRES_SPELL_FOCUS and the ring does nothing.
--
-- Same position as that update used, this time with an explicit guid: the original INSERT
-- omitted `guid` and left it to AUTO_INCREMENT, which is the likeliest reason the row is not
-- in the dump.

SET @OGUID := 901347;

DELETE FROM `gameobject` WHERE `guid` = @OGUID;
DELETE FROM `gameobject` WHERE `id` = 204811;
INSERT INTO `gameobject` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `position_x`, `position_y`, `position_z`, `orientation`, `rotation0`, `rotation1`, `rotation2`, `rotation3`, `spawntimesecs`, `animprogress`, `state`) VALUES
(@OGUID, 204811, 0, 1, 0, 0, -10368.0, -1255.883, 35.91, 0, 0, 0, 0, 0, 120, 100, 1);
