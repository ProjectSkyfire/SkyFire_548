-- The Klaxxi
DELETE FROM `creature_onkill_reputation` WHERE `creature_id`=62563;
INSERT INTO `creature_onkill_reputation` (`creature_id`, `RewOnKillRepFaction1`, `RewOnKillRepFaction2`, `MaxStanding1`, `RewOnKillRepValue1`, `MaxStanding2`, `RewOnKillRepValue2`) VALUES
(62563, 1337, 1359, 5, 500, 5, 10); -- Shek'zeer Bladesworn + 500 reputation with The Klaxxi (stops at Honored), + 10 reputation with The Black Prince (stops at Honored)
