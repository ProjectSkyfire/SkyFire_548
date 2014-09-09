-- Update racemask structure in spell Area
ALTER TABLE `spell_area`
  CHANGE COLUMN `racemask` `racemask` INT(8) UNSIGNED NOT NULL DEFAULT '0' AFTER `aura_spell`;

