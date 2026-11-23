-- Billibub Cogspinner (5519): make the shop gossip option open the vendor (issue #1508).
-- Hugo Lentner (52637) shares menu 698 and is also a vendor.
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE `MenuID` = 698 AND `OptionID` = 0;
