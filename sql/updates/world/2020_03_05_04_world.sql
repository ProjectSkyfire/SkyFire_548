UPDATE `creature` SET `MovementType`=1, `spawndist`=15 WHERE `MovementType`<2 AND `id`=66514;
UPDATE `creature_template` SET `MovementType`=1, `InhabitType`=4 WHERE entry=66514;
