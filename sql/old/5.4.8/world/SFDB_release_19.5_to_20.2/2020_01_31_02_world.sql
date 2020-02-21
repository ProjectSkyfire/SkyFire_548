UPDATE `creature` SET `MovementType`=0 WHERE `spawndist`=0 AND `MovementType`<2;
UPDATE `creature` SET `MovementType`=1 WHERE `spawndist`>0 AND `MovementType`<2;
