-- "Let's do battle!" gossip option (type 22, GOSSIP_OPTION_PET_BATTLE) on the
-- pet battle tamers, with the gossip npcflag bit (1); other npcflag bits stay.
-- Tamers without a gossip menu get menu 990001 (text 68). Jeremy Feasel (67370)
-- keeps menu 15172 and gets the option appended there.
-- No 5.4.8 source for the option text was found: "Let's do battle!" is the
-- wording of a later client.

DELETE FROM `gossip_menu` WHERE `MenuID` = 990001;
DELETE FROM `gossip_menu_option` WHERE `MenuID` = 990001 OR (`MenuID` = 15172 AND `OptionType` = 22);

INSERT INTO `gossip_menu` (`MenuID`, `TextID`, `VerifiedBuild`) VALUES
(990001, 68, 0);

INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`) VALUES
(990001, 0, 9, 'Let\'s do battle!', 0, 22, 1, 0, 0, 0, 0, NULL, 0, 0);

INSERT INTO `gossip_menu_option` (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`)
SELECT 15172, IFNULL(MAX(`OptionID`) + 1, 0), 9, 'Let\'s do battle!', 0, 22, 1, 0, 0, 0, 0, NULL, 0, 0
FROM `gossip_menu_option` WHERE `MenuID` = 15172;

UPDATE `creature_template` SET `npcflag` = `npcflag` | 1,
    `gossip_menu_id` = IF(`gossip_menu_id` = 0, 990001, `gossip_menu_id`)
WHERE `entry` IN (
    63194, 64330, 65648, 65651, 65655, 65656, 66126, 66135, 66136, 66137,
    66352, 66372, 66412, 66422, 66436, 66442, 66452, 66466, 66478, 66512,
    66515, 66518, 66520, 66522, 66550, 66551, 66552, 66553, 66557, 66635,
    66636, 66638, 66639, 66675, 66730, 66733, 66734, 66738, 66739, 66741,
    66815, 66819, 66822, 66824, 66918, 67370, 68462, 68463, 68464, 68465,
    73626);
