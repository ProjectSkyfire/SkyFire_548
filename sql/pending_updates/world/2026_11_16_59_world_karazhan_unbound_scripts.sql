-- Karazhan: the Homunculus (16539) and the spell target of the Shade of Aran's circular storm (17161) are
-- bound to scripts that do not exist in the core (the server logs an error for each at startup).
-- TrinityCore 3.3.5 binds no script to them: the Shade of Aran casts the storm itself and the Homunculus
-- fights with its template spell.
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = 16539 AND `ScriptName` = 'npc_homunculus';
UPDATE `creature_template` SET `ScriptName` = '' WHERE `entry` = 17161 AND `ScriptName` = 'npc_aran_blizzard';
