/* The aura was not on all creatures, so we delete this line. Instead, we will make this part as scripts. */
DELETE FROM `creature_template_addon` WHERE `entry` = 49874;
/* Add the script of blackrock_spy */
UPDATE `creature_template` SET `ScriptName` = 'npc_blackrock_spy' WHERE `creature_template`.`entry` = 49874;