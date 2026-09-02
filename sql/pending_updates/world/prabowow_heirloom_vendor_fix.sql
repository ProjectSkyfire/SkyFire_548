-- mod-prabowow: perbaiki vendor heirloom yang tidak muncul di titik spawn human.
--
-- Dua koreksi terhadap 2026_09_02_world_02.sql:
--
-- 1. flags_extra ditulis sebagai literal heksadesimal 0x200000. Di MySQL
--    literal heksadesimal adalah BINARY STRING, bukan angka, dan konversinya
--    ke kolom integer tidak dijamin menghasilkan 2097152. Kalau nilainya
--    berakhir 0, CREATURE_FLAG_EXTRA_ALL_PHASES tidak aktif dan vendor jadi
--    tidak terlihat di zona yang di-phase -- persis kasus Northshire, tempat
--    karakter human memulai. Sekarang ditulis desimal supaya tidak ambigu.
--
-- 2. GUID spawn dibangkitkan dengan user variable (@GUID := @GUID + 1) di
--    dalam SELECT ber-ORDER BY. Urutan evaluasinya tidak terdefinisi di
--    MySQL 8 dan bisa menghasilkan GUID kembar, yang menggagalkan sebagian
--    baris. ROW_NUMBER() deterministik dan tidak punya masalah itu.

SET @ENTRY := 900001;

UPDATE `creature_template`
SET `flags_extra` = 2097152
WHERE `entry` = @ENTRY;

DELETE FROM `creature` WHERE `id` = @ENTRY;

INSERT INTO `creature`
(`guid`, `id`, `map`, `spawnMask`, `phaseId`, `phaseGroup`, `modelid`, `equipment_id`,
 `position_x`, `position_y`, `position_z`, `orientation`, `spawntimesecs`, `spawndist`,
 `currentwaypoint`, `curhealth`, `curmana`, `MovementType`, `npcflag`, `unit_flags`, `dynamicflags`)
SELECT 8300000 + ROW_NUMBER() OVER (ORDER BY `map`, `position_x`, `position_y`),
       @ENTRY, `map`, 1, 0, 0, 0, 0,
       `position_x` + 4 * COS(`orientation`), `position_y` + 4 * SIN(`orientation`), `position_z`,
       MOD(`orientation` + PI(), 2 * PI()), 120, 0,
       0, 1, 0, 0, 0, 0, 0
FROM (
    SELECT DISTINCT `map`, `position_x`, `position_y`, `position_z`, `orientation`
    FROM `playercreateinfo`
) AS `start`;
