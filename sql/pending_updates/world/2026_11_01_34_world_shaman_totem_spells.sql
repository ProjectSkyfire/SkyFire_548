-- Shaman totems cast their effect from their template's spell slots when summoned; these had none, so the totem stood
-- there doing nothing (Stone Bulwark's shield, Windwalk, Healing Tide, Spirit Link). Grounding
-- Totem cast its effect twice. The two script names were bound to scripts that do not exist.
UPDATE `creature_template` SET `spell1` = 114889 WHERE `entry` = 59712;
UPDATE `creature_template` SET `spell1` = 114896, `ScriptName` = '' WHERE `entry` = 59717;
UPDATE `creature_template` SET `spell1` = 114941, `ScriptName` = '' WHERE `entry` = 59764;
UPDATE `creature_template` SET `spell1` = 98007, `spell2` = 98017 WHERE `entry` = 53006;
UPDATE `creature_template` SET `spell2` = 0 WHERE `entry` = 5925;
-- Lesser Scorching Totem: its Scorch, as in TrinityCore 3.3.5 and The Cataclysm Preservation Project 4.3.4.
UPDATE `creature_template` SET `spell1` = 32969 WHERE `entry` = 18795;
