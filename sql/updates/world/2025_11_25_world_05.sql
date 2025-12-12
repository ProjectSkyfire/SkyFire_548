-- Remove spell 29834 from spell_proc_event and spell_script_names
-- Spell 29834 (Second Wind rank 1) does not exist in MoP DBC, causing DBErrors
DELETE FROM `spell_script_names` WHERE `spell_id` = 29834;
DELETE FROM `spell_proc_event` WHERE `entry` = 29834;

