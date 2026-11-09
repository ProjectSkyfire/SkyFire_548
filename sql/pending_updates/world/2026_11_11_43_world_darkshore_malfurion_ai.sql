-- Malfurion Stormrage (33091) recast his aura 51835 on himself at every reset through SmartAI, which left him flagged in
-- combat, so players could not talk to him or hand in To the Grove (13940). His auras already come from his template
-- addon, and 5.4.8 gives him no script.
DELETE FROM `smart_scripts` WHERE `entryorguid` = 33091 AND `source_type` = 0;
UPDATE `creature_template` SET `AIName` = '' WHERE `entry` = 33091 AND `AIName` = 'SmartAI';
