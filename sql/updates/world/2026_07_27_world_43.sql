-- Fix invalid vendor and gossip condition targets reported by world database validation.

DELETE FROM `npc_vendor`
WHERE `entry` = 72007
  AND `item` = 101538
  AND `type` = 1;

INSERT IGNORE INTO `gossip_menu` (`MenuID`, `TextID`, `VerifiedBuild`) VALUES
    (7579, 9221, 0),
    (7983, 9039, 0);

INSERT IGNORE INTO `gossip_menu_option`
    (`MenuID`, `OptionID`, `OptionIcon`, `OptionText`, `OptionBroadcastTextID`, `OptionType`, `OptionNpcflag`, `ActionMenuID`, `ActionPoiID`, `BoxCoded`, `BoxMoney`, `BoxText`, `BoxBroadcastTextID`, `VerifiedBuild`)
VALUES
    (8455, 4, 0, 'Send me back to Raven''s Wood.', 0, 1, 1, 0, 0, 0, 0, NULL, 0, 0),
    (8718, 1, 0, 'Yes, I''d love a ride to Blackwind Landing.', 21550, 1, 1, 0, 0, 0, 0, NULL, 0, 0),
    (8719, 1, 0, 'Absolutely!  Send me to the Skyguard Outpost.', 21553, 1, 1, 0, 0, 0, 0, NULL, 0, 0);

UPDATE `conditions`
SET `SourceEntry` = 1
WHERE `SourceTypeOrReferenceId` = 15
  AND `SourceGroup` = 8934
  AND `SourceEntry` = 0
  AND `ConditionValue1` = 11122;
