-- Fix game_tele wrong position errors for Scarlet Monastery
-- Map 189 (Scarlet Monastery) is missing from instance_template table
-- This causes IsValidMAP to return false for instance maps without a template
-- Adding instance_template entry for map 189 to fix validation

INSERT INTO `instance_template` (`map`, `parent`, `script`, `allowMount`) VALUES
(189, 0, '', 0)
ON DUPLICATE KEY UPDATE `parent` = 0, `script` = '', `allowMount` = 0;

