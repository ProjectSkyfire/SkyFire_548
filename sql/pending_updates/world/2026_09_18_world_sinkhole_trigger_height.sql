-- Quest 11713 "Scouting the Sinkholes": the three trigger creatures float about 32 yards
-- above the sinkholes they mark.
--
-- The objectives are credited by the invisible triggers 25664 South Sinkhole, 25665 Northeast
-- Sinkhole and 25666 Northwest Sinkhole, on SMART_EVENT_SPELLHIT of 45853. The item script
-- spell_item_map_of_the_geyser_fields refuses the cast unless one of the three is within
-- 30 yards:
--
--     if (caster->FindNearestCreature(NPC_SOUTH_SINKHOLE, 30.0f, true) || ...)
--         return SPELL_CAST_OK;
--     SetCustomCastResultMessage(SPELL_CUSTOM_ERROR_MUST_BE_CLOSE_TO_SINKHOLE);
--
-- All three sit at z 11.4 / 12.4 / 12.3 while the sinkholes themselves are well below that.
-- The South Sinkhole is the clearest case: gameobject 300177 "TEMP South Sinkhole" (guid 4522)
-- is 1.0 yard away horizontally at z -20.7394, so the trigger hangs 32.18 yards above its own
-- marker. Every other creature spawned within 80 yards of any of the three is between z -15 and
-- z -11; the triggers are the only objects up at +12.
--
-- Only the height changes. x, y and orientation are left exactly as they are.

UPDATE `creature` SET `position_z` = -20.74 WHERE `guid` = 78 AND `id` = 25664;
UPDATE `creature` SET `position_z` = -20.74 WHERE `guid` = 79 AND `id` = 25665;
UPDATE `creature` SET `position_z` = -20.74 WHERE `guid` = 80 AND `id` = 25666;
