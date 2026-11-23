-- Edge of Madness (Gri'lek, Hazza'rah, Renataki, Wushoolay): the four bosses are summoned one at a time from the
-- Cache of Madness. Gri'lek, Renataki, Wushoolay and the Nightmare Illusion were level 1 friendly stubs without
-- scripts, loot or damage, and Hazza'rah had no script. Hazza'rah also stood at the summoning spot all the time.

-- Hazza'rah already had the stats of the other bosses of the dungeon, the other three get the same
UPDATE `creature_template` SET `ScriptName` = 'boss_hazzarah' WHERE `entry` = 52271 AND `ScriptName` = '';
UPDATE `creature_template` SET `minlevel` = 87, `maxlevel` = 87, `exp` = 3, `faction_A` = 14, `faction_H` = 14,
    `mindmg` = 4184, `maxdmg` = 7114, `dmg_multiplier` = 2.25, `baseattacktime` = 2000, `rangeattacktime` = 2000,
    `lootid` = 52258, `mingold` = 20057, `maxgold` = 20057, `ScriptName` = 'boss_grilek'
WHERE `entry` = 52258 AND `minlevel` = 1;
UPDATE `creature_template` SET `minlevel` = 86, `maxlevel` = 87, `exp` = 3, `faction_A` = 14, `faction_H` = 14,
    `mindmg` = 4184, `maxdmg` = 7114, `dmg_multiplier` = 2.25, `baseattacktime` = 2000, `rangeattacktime` = 2000,
    `lootid` = 52269, `mingold` = 20028, `maxgold` = 20028, `ScriptName` = 'boss_renataki'
WHERE `entry` = 52269 AND `minlevel` = 1;
UPDATE `creature_template` SET `minlevel` = 87, `maxlevel` = 87, `exp` = 3, `faction_A` = 14, `faction_H` = 14,
    `mindmg` = 4184, `maxdmg` = 7114, `dmg_multiplier` = 2.25, `baseattacktime` = 2000, `rangeattacktime` = 2000,
    `lootid` = 52286, `mingold` = 19998, `maxgold` = 19998, `ScriptName` = 'boss_wushoolay'
WHERE `entry` = 52286 AND `minlevel` = 1;
UPDATE `creature_template` SET `minlevel` = 84, `maxlevel` = 84, `exp` = 3, `faction_A` = 14, `faction_H` = 14,
    `baseattacktime` = 2000, `rangeattacktime` = 2000, `ScriptName` = 'npc_nightmare_illusion'
WHERE `entry` = 52284 AND `minlevel` = 1;

-- The static Hazza'rah at the Cache of Madness is not wanted, the instance summons the boss
UPDATE `creature` SET `spawnMask` = 0 WHERE `guid` = 219298 AND `id` = 52271;

DELETE FROM `creature_loot_template` WHERE `entry` IN (52258, 52269, 52286);
INSERT INTO `creature_loot_template` (`entry`, `item`, `ChanceOrQuestChance`, `lootmode`, `groupid`, `mincountOrRef`, `maxcount`) VALUES
(52258, 69630, 0, 1, 1, 1, 1),
(52258, 69631, 0, 1, 1, 1, 1),
(52258, 69632, 0, 1, 1, 1, 1),
(52258, 69633, 0, 1, 1, 1, 1),
(52258, 69634, 0, 1, 1, 1, 1),
(52258, 69635, 0, 1, 1, 1, 1),
(52258, 69647, -31, 1, 0, 1, 1),
(52269, 69630, 0, 1, 1, 1, 1),
(52269, 69631, 0, 1, 1, 1, 1),
(52269, 69632, 0, 1, 1, 1, 1),
(52269, 69633, 0, 1, 1, 1, 1),
(52269, 69638, 0, 1, 1, 1, 1),
(52269, 69639, 0, 1, 1, 1, 1),
(52269, 69647, -28, 1, 0, 1, 1),
(52286, 69630, 0, 1, 1, 1, 1),
(52286, 69631, 0, 1, 1, 1, 1),
(52286, 69632, 0, 1, 1, 1, 1),
(52286, 69633, 0, 1, 1, 1, 1),
(52286, 69640, 0, 1, 1, 1, 1),
(52286, 69641, 0, 1, 1, 1, 1),
(52286, 69647, -33, 1, 0, 1, 1);

-- Hazza'rah lacked the kill line and the announcement for the player left awake
DELETE FROM `creature_text` WHERE `entry` IN (52258, 52269, 52286) OR (`entry` = 52271 AND `groupid` IN (4, 5));
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(52271, 4, 0, 'There there... close your eyes...', 14, 0, 100, 0, 0, 24253, 'Hazza''rah - kill'),
(52271, 5, 0, 'Attack the nightmare illusions and defend your sleeping allies!', 42, 0, 100, 0, 0, 0, 'Hazza''rah - dreamstate announce'),
(52258, 0, 0, 'You seek me? We fight now!', 14, 0, 100, 0, 0, 24232, 'Gri''lek - aggro'),
(52258, 1, 0, 'Gri''lek wanders... no more...', 14, 0, 100, 0, 0, 24233, 'Gri''lek - death'),
(52258, 2, 0, 'I COME FOR YOU, LITTLE MORTAL!', 14, 0, 100, 0, 0, 24234, 'Gri''lek - fixate 1'),
(52258, 2, 1, 'I will catch you, small one!', 14, 0, 100, 0, 0, 24235, 'Gri''lek - fixate 2'),
(52258, 3, 0, 'Crushed!', 14, 0, 100, 0, 0, 24236, 'Gri''lek - kill 1'),
(52258, 3, 1, 'You are weak!', 14, 0, 100, 0, 0, 24237, 'Gri''lek - kill 2'),
(52258, 4, 0, 'Coward! You cannot run!', 14, 0, 100, 0, 0, 24241, 'Gri''lek - root 1'),
(52258, 4, 1, 'No! You do not escape the rage of the Gurubashi!', 14, 0, 100, 0, 0, 24242, 'Gri''lek - root 2'),
(52258, 5, 0, '%s is chasing you. Run!', 42, 0, 100, 0, 0, 0, 'Gri''lek - fixate announce'),
(52269, 0, 0, 'Feel the sting of a thousand cuts!', 14, 0, 100, 0, 0, 24307, 'Renataki - blades 1'),
(52269, 0, 1, 'One thousand blades!', 14, 0, 100, 0, 0, 24308, 'Renataki - blades 2'),
(52269, 1, 0, 'Ahhh... I shall add a new blade to my collection today...', 14, 0, 100, 0, 0, 24309, 'Renataki - aggro'),
(52269, 2, 0, 'Not even a thousand deaths could put my soul to rest...', 14, 0, 100, 0, 0, 24310, 'Renataki - death'),
(52269, 3, 0, 'A swift slice to the throat, and this one falls like any other.', 14, 0, 100, 0, 0, 24311, 'Renataki - kill 1'),
(52269, 3, 1, 'Learn your place.', 14, 0, 100, 0, 0, 24312, 'Renataki - kill 2'),
(52269, 4, 0, 'Perhaps a quick stab will let out all that tension in your shoulders...', 14, 0, 100, 0, 0, 24314, 'Renataki - mocking'),
(52269, 4, 1, 'Rogues do it from behind...', 14, 0, 100, 0, 0, 24315, 'Renataki - mocking'),
(52269, 4, 2, 'It only hurts this much the first time.  The second time, you''re dead.', 14, 0, 100, 0, 0, 24316, 'Renataki - mocking'),
(52286, 0, 0, 'You shall regret disturbing my homeland!', 14, 0, 100, 0, 0, 24329, 'Wushoolay - aggro'),
(52286, 1, 0, 'The end of our empire... is only the beginning...', 14, 0, 100, 0, 0, 24330, 'Wushoolay - death'),
(52286, 2, 0, 'Shocking. I know.', 14, 0, 100, 0, 0, 24331, 'Wushoolay - kill 1'),
(52286, 2, 1, 'Feel the might of my storm!', 14, 0, 100, 0, 0, 24332, 'Wushoolay - kill 2'),
(52286, 3, 0, 'Can you feel that tingling sensation? It may be your last.', 14, 0, 100, 0, 0, 24333, 'Wushoolay - lightning nuke 1'),
(52286, 3, 1, 'You''re in for a stunning surprise...', 14, 0, 100, 0, 0, 24334, 'Wushoolay - lightning nuke 2'),
(52286, 4, 0, 'Are you quicker than lightning?', 14, 0, 100, 0, 0, 24335, 'Wushoolay - lightning rush 1'),
(52286, 4, 1, 'In a flash, you''ll be nothing more than dust...', 14, 0, 100, 0, 0, 24403, 'Wushoolay - lightning rush 2'),
(52286, 4, 2, 'The storm feeds my power!', 14, 0, 100, 0, 0, 24336, 'Wushoolay - lightning rush 3'),
(52286, 5, 0, '%s is about to blast you with lightning! Get away!', 42, 0, 100, 0, 0, 0, 'Wushoolay - emote - lightning nuke');
