-- Restore missing gossip menu links for templates that already expose gossip.

UPDATE `creature_template` child
JOIN `creature_template` parent
    ON child.`entry` IN (parent.`difficulty_entry_1`, parent.`difficulty_entry_2`, parent.`difficulty_entry_3`)
SET child.`gossip_menu_id` = parent.`gossip_menu_id`
WHERE child.`entry` IN (
    20165, 21602, 22527, 22541, 22567, 22575, 22577, 22644, 22647, 22735,
    22938, 30792, 30796, 30797, 30799, 30801, 31366, 32032, 32092, 34254,
    37239, 37278, 37284, 37298, 37369, 37401, 37420, 37422, 37426, 37444,
    37607, 38157, 38640
)
AND (child.`npcflag` & 1) <> 0
AND child.`gossip_menu_id` = 0
AND parent.`gossip_menu_id` > 0;

UPDATE `creature_template`
SET `gossip_menu_id` = 441
WHERE `entry` = 72488
AND (npcflag & 1) <> 0
AND `gossip_menu_id` = 0;

UPDATE `creature_template`
SET `gossip_menu_id` = 9821
WHERE `entry` = 68986
AND (npcflag & 1) <> 0
AND `gossip_menu_id` = 0;

UPDATE `creature_template`
SET `gossip_menu_id` = 14991
WHERE `entry` = 63626
AND (npcflag & 1) <> 0
AND `gossip_menu_id` = 0;
