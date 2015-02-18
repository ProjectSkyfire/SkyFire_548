-- Set InhabitType 4 to some mobs that should be allowed to fly.
UPDATE `creature_template` SET `InhabitType`=4 WHERE `entry` IN
(59641,56171, 57422, 59528, 59235, 63973, 59700, 59112, 69815, 59788, 58440, 57445, 
 56201, 58363, 59757, 59785, 73531);
 
 -- Ordos Health and Mana was wrong parsed.
 UPDATE `creature_template` SET `Health_mod`=1500, `Mana_mod`=0 WHERE `entry`=72057;
