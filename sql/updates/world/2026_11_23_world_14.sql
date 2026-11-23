-- Nested loot references must process the full template, not a nonexistent direct-item group.
-- Follow-up for the recently restored Flood Elemental and Displaced Threshadon loot.
UPDATE `creature_loot_template` SET `groupid` = 0
WHERE `entry` IN (41132, 41137) AND `mincountOrRef` IN (-24104, -24732) AND `groupid` = 1;
