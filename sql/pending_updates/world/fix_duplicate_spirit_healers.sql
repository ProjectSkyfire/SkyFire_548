-- Three graveyards had two Spirit Healers. Keep the one that stands at the graveyard and remove the second one, which is
-- 26 to 57 yards away (or 570 yards too high in Vashj'ir) and which the 4.3.4 data does not have.
-- Silverpine Forest, Shadowfang Keep graveyard: keep guid 16760, remove guid 16632.
-- Silithus, Scarab Wall graveyard: keep guid 16774, remove guid 16659.
-- Vashj'ir, The Abyssal Maw graveyard: keep guid 16762, remove guid 16745.
DELETE FROM `creature` WHERE `guid` IN (16632, 16659, 16745) AND `id` = 6491;
