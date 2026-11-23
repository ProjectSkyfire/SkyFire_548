-- Magisters' Terrace: the Scrying Orb (187578) and the Escape to the Isle of Quel'Danas orb (188173) are
-- bound to scripts that do not exist in the core; they work by their own gameobject data (a camera and a
-- spellcaster), exactly as in TrinityCore 3.3.5 where they have no ScriptName.
UPDATE `gameobject_template` SET `ScriptName` = '' WHERE `entry` = 187578 AND `ScriptName` = 'go_movie_orb';
UPDATE `gameobject_template` SET `ScriptName` = '' WHERE `entry` = 188173 AND `ScriptName` = 'go_kael_orb';
