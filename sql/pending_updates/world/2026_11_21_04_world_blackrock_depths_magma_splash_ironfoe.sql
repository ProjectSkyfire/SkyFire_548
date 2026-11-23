-- Bael'Gar's Magma Splash and Emperor Dagran Thaurissan's Ironfoe are passive auras that trigger on melee hits.
-- Neither creature carried them. The spawn addon rows replace the template addon, so the auras go there.
UPDATE `creature_addon` SET `auras` = '23378' WHERE `guid` = 45885;
UPDATE `creature_addon` SET `auras` = '15641' WHERE `guid` = 47613;
