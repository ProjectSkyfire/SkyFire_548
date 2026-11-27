-- spell_dbc, spellmisc_dbc and spelleffect_dbc add server-side spells to Spell.dbc, SpellMisc.dbc and SpellEffect.dbc.
-- spell_dbc still had the layout of the older client: the attributes, cast time, duration, range and school now live
-- in spellmisc_dbc, and the ids of the sub tables (aura options, levels and so on) were renumbered by the client, so
-- they are reset. The effect ids collided with SpellEffect.dbc and move to 303020 and up.
-- The old layout is converted inside one procedure. Dropping the Attributes column is its last step and every step
-- before it can run again, so an interrupted run is finished by the next one and a converted database is left alone.

CREATE TABLE IF NOT EXISTS `spellmisc_dbc` (
  `Id` int unsigned NOT NULL,
  `Attributes` int unsigned NOT NULL DEFAULT '0',
  `AttributesEx` int unsigned NOT NULL DEFAULT '0',
  `AttributesEx2` int unsigned NOT NULL DEFAULT '0',
  `AttributesEx3` int unsigned NOT NULL DEFAULT '0',
  `AttributesEx4` int unsigned NOT NULL DEFAULT '0',
  `AttributesEx5` int unsigned NOT NULL DEFAULT '0',
  `AttributesEx6` int unsigned NOT NULL DEFAULT '0',
  `AttributesEx7` int unsigned NOT NULL DEFAULT '0',
  `AttributesEx8` int unsigned NOT NULL DEFAULT '0',
  `AttributesEx9` int unsigned NOT NULL DEFAULT '0',
  `CastingTimeIndex` int unsigned NOT NULL DEFAULT '1',
  `DurationIndex` int unsigned NOT NULL DEFAULT '0',
  `RangeIndex` int unsigned NOT NULL DEFAULT '1',
  `SchoolMask` int unsigned NOT NULL DEFAULT '0',
  `Comment` text NOT NULL,
  PRIMARY KEY (`Id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8mb3 COMMENT='Custom spellmisc.dbc entries';

-- Spells 5302 and 34145 are in the client data and must not be overridden
DELETE FROM `spell_dbc` WHERE `Id` IN (5302, 34145);
DELETE FROM `spelleffect_dbc` WHERE `EffectSpellId` IN (5302, 34145);

DROP PROCEDURE IF EXISTS `convert_spell_dbc`;

DELIMITER //
CREATE PROCEDURE `convert_spell_dbc`()
BEGIN
    IF EXISTS (SELECT 1 FROM `information_schema`.`COLUMNS`
        WHERE `TABLE_SCHEMA` = DATABASE() AND `TABLE_NAME` = 'spell_dbc' AND `COLUMN_NAME` = 'Attributes') THEN
        UPDATE `spelleffect_dbc` SET `Id` = `Id` + 150000 WHERE `Id` < 200000;

        DELETE FROM `spellmisc_dbc` WHERE `Id` IN (SELECT 200000 + `Id` FROM `spell_dbc`);
        INSERT INTO `spellmisc_dbc` (`Id`, `Attributes`, `AttributesEx`, `AttributesEx2`, `AttributesEx3`,
            `AttributesEx4`, `AttributesEx5`, `AttributesEx6`, `AttributesEx7`, `AttributesEx8`, `AttributesEx9`,
            `CastingTimeIndex`, `DurationIndex`, `RangeIndex`, `SchoolMask`, `Comment`)
        SELECT 200000 + `Id`, `Attributes`, `AttributesEx`, `AttributesEx2`, `AttributesEx3`,
            `AttributesEx4`, `AttributesEx5`, `AttributesEx6`, `AttributesEx7`, `AttributesEx8`, `AttributesEx9`,
            `CastingTimeIndex`, `DurationIndex`, `RangeIndex`, `SchoolMask`, `Comment`
        FROM `spell_dbc`;

        IF NOT EXISTS (SELECT 1 FROM `information_schema`.`COLUMNS`
            WHERE `TABLE_SCHEMA` = DATABASE() AND `TABLE_NAME` = 'spell_dbc' AND `COLUMN_NAME` = 'SpellMiscId') THEN
            ALTER TABLE `spell_dbc` ADD COLUMN `SpellMiscId` int unsigned NOT NULL DEFAULT '0'
                AFTER `SpellTargetRestrictionsId`;
        END IF;

        UPDATE `spell_dbc` SET `SpellMiscId` = 200000 + `Id`, `SpellAuraOptionsId` = 0,
            `SpellCastingRequirementsId` = 0, `SpellCategoriesId` = 0, `SpellClassOptionsId` = 0,
            `SpellEquippedItemsId` = 0, `SpellLevelsId` = 0, `SpellTargetRestrictionsId` = 0,
            `SpellInterruptsId` = 0;

        ALTER TABLE `spell_dbc`
            DROP COLUMN `AttributesEx`, DROP COLUMN `AttributesEx2`,
            DROP COLUMN `AttributesEx3`, DROP COLUMN `AttributesEx4`, DROP COLUMN `AttributesEx5`,
            DROP COLUMN `AttributesEx6`, DROP COLUMN `AttributesEx7`, DROP COLUMN `AttributesEx8`,
            DROP COLUMN `AttributesEx9`, DROP COLUMN `AttributesEx10`, DROP COLUMN `CastingTimeIndex`,
            DROP COLUMN `DurationIndex`, DROP COLUMN `RangeIndex`, DROP COLUMN `SchoolMask`,
            MODIFY COLUMN `SpellInterruptsId` int unsigned NOT NULL DEFAULT '0' AFTER `SpellEquippedItemsId`,
            DROP COLUMN `Attributes`;
    END IF;
END//
DELIMITER ;

CALL `convert_spell_dbc`();
DROP PROCEDURE `convert_spell_dbc`;
