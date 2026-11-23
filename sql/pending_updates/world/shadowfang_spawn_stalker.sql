-- Springvale Spawn Stalker (Shadowfang Keep) is an unselectable spawn marker, but it took part in combat and hit
-- players standing on it every 2 seconds, which kept them in combat for minutes. It is sessile, uninteractible and
-- outside combat, so it gets IMMUNE_TO_PC and NON_ATTACKABLE next to its NOT_SELECTABLE flag.
UPDATE `creature_template` SET `unit_flags` = `unit_flags` | 0x00000100 | 0x00000002 WHERE `entry` = 50611;
