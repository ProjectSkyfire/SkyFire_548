-- An Ancient Enemy (24814): the ending after Zar'jira dies. Zuni falls, Vanira mourns him and offers to take the party
-- back to Darkspear Hold, Vol'jin says his farewell and leaves. While the fires burn, Vol'jin channels Deluge of Shadow
-- at the Sea Witch Channel Bunny.
UPDATE `creature_template` SET `ScriptName` = 'npc_vanira_ancient_enemy', `npcflag` = 0 WHERE `entry` = 38437;

DELETE FROM `creature` WHERE `guid` = 8311012;
INSERT INTO `creature` (`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`, `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`, `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`) VALUES
(8311012, 38452, 1, 1, 0, 0, 0, 0, -713.983, -5587.53, 27.6724, 4.00841, 300, 0, 0, 0, 0, 0, 0, 0, 0);

DELETE FROM `creature_text` WHERE (`entry` = 38225 AND `groupid` IN (2, 3, 4)) OR (`entry` = 38306 AND `groupid` = 3) OR (`entry` = 38437 AND `groupid` IN (1, 2, 3, 4));
INSERT INTO `creature_text` (`entry`, `groupid`, `id`, `text`, `type`, `language`, `probability`, `emote`, `duration`, `sound`, `comment`) VALUES
(38225, 2, 0, 'It be done. Our ancient enemy is defeated.', 12, 0, 100, 1, 0, 20121, 'Vol''jin - Zar''jira is defeated'),
(38225, 3, 0, 'I been waitin'' a long time for a chance to avenge my father. A great weight has been lifted from my shoulders.', 12, 0, 100, 1, 0, 20122, 'Vol''jin - Avenged'),
(38225, 4, 0, 'I must be returnin'' ta Darkspear Hold. Please meet me there once Vanira is done with her healin'' of the boy.', 12, 0, 100, 1, 0, 20123, 'Vol''jin - Leaving'),
(38306, 3, 0, 'Not so fast, little troll!', 14, 0, 100, 1, 0, 21815, 'Zar''jira - Death'),
(38437, 1, 0, 'ZUNI! NOOOO!', 14, 0, 100, 5, 0, 21945, 'Vanira - Zuni falls'),
(38437, 2, 0, 'I''m afraid there''s nothin'' I can do for our brother... her power tore away at his soul.', 12, 0, 100, 1, 0, 21946, 'Vanira - Nothing to be done'),
(38437, 3, 0, 'I''ll send some watchers ta get his body so we can offer a proper farewell... I wish I could do more.', 12, 0, 100, 1, 0, 21947, 'Vanira - Watchers'),
(38437, 4, 0, 'Vol''jin rushed off in eagerness, but I can take us back to safety. Just give me the word when ya ready, mon.', 12, 0, 100, 275, 0, 21948, 'Vanira - Offers the way back');
