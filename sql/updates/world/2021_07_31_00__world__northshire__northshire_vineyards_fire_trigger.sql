/* set flags_extra like a TRIGGER that we are can make him invisible */
UPDATE `creature_template` SET `flags_extra` = '128' WHERE `creature_template`.`entry` = 42940;