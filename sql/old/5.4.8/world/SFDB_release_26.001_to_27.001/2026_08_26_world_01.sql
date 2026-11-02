-- Nyoma (4265): vendor gossip option had OptionType/OptionNpcflag 0, so buy/sell never opened
UPDATE `gossip_menu_option` SET
    `OptionBroadcastTextID` = 20225,
    `OptionType` = 3,
    `OptionNpcflag` = 128
WHERE `MenuID` = 11778 AND `OptionID` = 0;
