-- Jin'do the Godbreaker: the Spirit of Hakkar, Hakkar's Chains, the Spirit Portal and the Gurubashi Spirit had no
-- scripts. The three spell scripts (97152, 97158, 97597) are already bound.
UPDATE `creature_template` SET `ScriptName` = 'npc_jindo_spirit_of_hakkar' WHERE `entry` = 52222 AND `ScriptName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_jindo_chains_of_hakkar' WHERE `entry` = 52430 AND `ScriptName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_jindo_spirit_portal' WHERE `entry` = 52532 AND `ScriptName` = '';
UPDATE `creature_template` SET `ScriptName` = 'npc_jindo_gurubashi_spirit' WHERE `entry` = 52730 AND `ScriptName` = '';
