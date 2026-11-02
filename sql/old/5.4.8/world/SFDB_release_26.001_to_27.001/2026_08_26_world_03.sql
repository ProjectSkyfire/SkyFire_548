-- Lor'danel hub NPCs for The Last Wave of Survivors (13518) and related Darkshore quests.
-- Dentaria (32973) sniff creature_template_addon has auras=NULL; Glynda (32971) /
-- Serendia (32972) match the same visible hub state. Laird (32974) keeps
-- 49414 + 29266 (Permanent Feign Death) as the dead refugee on the pier.
UPDATE `creature_template_addon`
SET `auras` = NULL
WHERE `entry` IN (32971, 32972, 32973)
  AND `auras` = '49414';
