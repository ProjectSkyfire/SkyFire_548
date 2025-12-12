-- Fix SmartAI duplicate kill credit errors
-- SmartAI entries are using Action 33 (Kill Credit) but there are already spells that give kill credit:
--   Entry 55054: Action 33 (id=1) gives kill credit for creature 67040, but spell 130723 already does this
--   Entry 66292: Action 33 (id=1) gives kill credit for creature 66292, but spell 131057 already does this
-- 
-- These entries are part of linked chains:
--   55054: id=0 (link=1) -> id=1 (link=2, Action 33) -> id=2 (link=0, Action 85)
--   66292: id=0 (link=1) -> id=1 (link=2, Action 33) -> id=2 (link=0, Action 85)
--
-- Removing the redundant kill credit actions and updating the chain links

-- Entry 55054: Update id=0 to link directly to id=2 (skip id=1)
UPDATE `smart_scripts` 
SET `link` = 2 
WHERE `entryorguid` = 55054 
  AND `source_type` = 0 
  AND `id` = 0;

-- Entry 55054: Remove the kill credit action (id=1)
DELETE FROM `smart_scripts` 
WHERE `entryorguid` = 55054 
  AND `source_type` = 0 
  AND `id` = 1 
  AND `action_type` = 33 
  AND `action_param1` = 67040;

-- Entry 66292: Update id=0 to link directly to id=2 (skip id=1)
UPDATE `smart_scripts` 
SET `link` = 2 
WHERE `entryorguid` = 66292 
  AND `source_type` = 0 
  AND `id` = 0;

-- Entry 66292: Remove the kill credit action (id=1)
DELETE FROM `smart_scripts` 
WHERE `entryorguid` = 66292 
  AND `source_type` = 0 
  AND `id` = 1 
  AND `action_type` = 33 
  AND `action_param1` = 66292;
