-- Fix Gorbold Steelhand vendor gossip so the browse option opens the vendor window.
-- Menu 12726 had OptionType/OptionNpcflag 0 from sniff decode; core filters options when OptionNpcflag & npcflags is zero.

UPDATE `gossip_menu_option`
SET `OptionType` = 3, `OptionNpcflag` = 128
WHERE `MenuID` = 12726 AND `OptionID` = 0;
