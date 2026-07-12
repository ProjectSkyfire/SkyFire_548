-- Restore Marcus Jensen's battle pet trainer faction templates.
UPDATE `creature_template`
SET `faction_A` = 35,
    `faction_H` = 35
WHERE `entry` = 63014
  AND (`faction_A` = 0 OR `faction_H` = 0);
