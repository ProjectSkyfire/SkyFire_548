-- Filter profession-training links independently of the NPC's complete trainer list.
-- Zero preserves ordinary trainers. Apply this schema update before the matching core.
SET @trainer_skill_ddl := IF(
    EXISTS (SELECT 1 FROM information_schema.COLUMNS
        WHERE TABLE_SCHEMA = DATABASE() AND TABLE_NAME = 'gossip_menu_option' AND COLUMN_NAME = 'TrainerSkill'),
    'SELECT 1',
    'ALTER TABLE gossip_menu_option ADD COLUMN TrainerSkill INT UNSIGNED NOT NULL DEFAULT 0 COMMENT ''Optional trainer skill-line filter''');
PREPARE trainer_skill_stmt FROM @trainer_skill_ddl;
EXECUTE trainer_skill_stmt;
DEALLOCATE PREPARE trainer_skill_stmt;

-- Shared profession explanation menus used by Lien Farner and other general trainers.
UPDATE gossip_menu_option
SET TrainerSkill = CASE MenuID
    WHEN 12188 THEN 182 -- Herbalism
    WHEN 12189 THEN 186 -- Mining
    WHEN 12190 THEN 393 -- Skinning
    WHEN 12191 THEN 171 -- Alchemy
    WHEN 12192 THEN 164 -- Blacksmithing
    WHEN 12193 THEN 333 -- Enchanting
    WHEN 12195 THEN 202 -- Engineering
    WHEN 12196 THEN 773 -- Inscription
    WHEN 12197 THEN 755 -- Jewelcrafting
    WHEN 12198 THEN 165 -- Leatherworking
    WHEN 12199 THEN 197 -- Tailoring
END
WHERE MenuID IN (12188,12189,12190,12191,12192,12193,12195,12196,12197,12198,12199)
  AND OptionID = 11 AND OptionType = 5;
