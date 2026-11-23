-- Sunstrider Isle (Blood Elf starting zone): second pass over the quest flow. The kill targets of "Aggression" (8334) and
-- "Felendren the Banished" (8335) were flagged for SmartAI but had no rows.
-- Rows are the 4.3.4 ones (5.4.8 has the combat casts only). No new spawns, so no new guids.

-- ---------------------------------------------------------------------------------------------------------------
-- Arcane Wraith (15273) and Tainted Arcane Wraith (15298), kill objectives of 8335, and Feral Tender (15294),
-- kill objective of 8334, have AIName SmartAI and no rows: no combat spells and no "Thirst Unending" (8346) credit
-- when a Blood Elf casts Arcane Torrent on them. Mana Wyrm (15274) already gives that credit with the same spell
-- (61314) and stays the main source. The credit rows use the two Arcane Torrent spells (mana 28730, energy 25046) of
-- the 4.3.4 rows; the other classes are covered by the wyrm. The spell-hit rows run every time (flags 0, as the wyrm's).
-- The combat casts are Arcane Bolt (wraith), Slow (tainted wraith) and Renew below 45 percent health (feral tender).
-- ---------------------------------------------------------------------------------------------------------------
DELETE FROM `smart_scripts` WHERE `entryorguid` IN (15273, 15294, 15298) AND `source_type` = 0;
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(15273, 0, 0, 0, 8, 0, 100, 0, 28730, 0, 0, 0, 0, 11, 61314, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Arcane Wraith - On Spellhit ''Arcane Torrent'' (mana) - Cast ''Quest Credit 8346'''),
(15273, 0, 1, 0, 8, 0, 100, 0, 25046, 0, 0, 0, 0, 11, 61314, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Arcane Wraith - On Spellhit ''Arcane Torrent'' (energy) - Cast ''Quest Credit 8346'''),
(15273, 0, 2, 0, 0, 0, 100, 0, 6000, 15000, 6000, 15000, 0, 11, 37361, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Arcane Wraith - In Combat - Cast ''Arcane Bolt'''),
(15294, 0, 0, 0, 8, 0, 100, 0, 28730, 0, 0, 0, 0, 11, 61314, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Feral Tender - On Spellhit ''Arcane Torrent'' (mana) - Cast ''Quest Credit 8346'''),
(15294, 0, 1, 0, 8, 0, 100, 0, 25046, 0, 0, 0, 0, 11, 61314, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Feral Tender - On Spellhit ''Arcane Torrent'' (energy) - Cast ''Quest Credit 8346'''),
(15294, 0, 2, 0, 2, 0, 100, 0, 0, 45, 16000, 18000, 0, 11, 31325, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 'Feral Tender - Between 0-45% Health - Cast ''Renew'''),
(15298, 0, 0, 0, 8, 0, 100, 0, 28730, 0, 0, 0, 0, 11, 61314, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tainted Arcane Wraith - On Spellhit ''Arcane Torrent'' (mana) - Cast ''Quest Credit 8346'''),
(15298, 0, 1, 0, 8, 0, 100, 0, 25046, 0, 0, 0, 0, 11, 61314, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0, 0, 'Tainted Arcane Wraith - On Spellhit ''Arcane Torrent'' (energy) - Cast ''Quest Credit 8346'''),
(15298, 0, 2, 0, 0, 0, 100, 0, 8000, 16000, 8000, 16000, 0, 11, 25603, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 'Tainted Arcane Wraith - In Combat - Cast ''Slow''');
