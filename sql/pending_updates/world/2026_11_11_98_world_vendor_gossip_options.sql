-- Vendors whose gossip menu has its own wares option never opened the vendor window: the option was saved with no type
-- and no NPC flag, so it did nothing, and a menu with its own options does not fall back to the default vendor and
-- repair options. Give the wares or repair option of each such vendor menu the vendor type and flag. The options are the
-- ones the 5.4.8 data has as vendor options, plus the plainly worded ones of vendors whose 5.4.8 menu options are not
-- typed (the Krom'gar Quartermaster among them).
UPDATE `gossip_menu_option` SET `OptionType` = 3, `OptionNpcflag` = 128 WHERE (`MenuID`, `OptionID`) IN (
(1964, 0), (1965, 0), (2242, 0), (3501, 0), (3625, 0), (4845, 0), (5181, 0), (5542, 0), (6083, 0), (6085, 0),
(7396, 0), (7448, 0), (7628, 0), (8031, 0), (8035, 0), (8094, 0), (8243, 0), (8386, 0), (8629, 0), (8760, 1),
(9706, 0), (9755, 0), (10123, 0), (10257, 0), (10437, 1), (11009, 0), (11097, 0), (11099, 0), (11143, 0),
(11159, 0), (11223, 0), (11460, 0), (11538, 1), (11731, 0), (11794, 0), (11801, 0), (11934, 1), (11936, 1),
(11996, 0), (12019, 0), (12033, 1), (12034, 2), (12056, 0), (12268, 0), (12449, 0), (12591, 0), (12607, 0),
(12759, 0), (12846, 1));

-- Sathein and Megzeg Nukklebust carry the repair flag, so their pointer shows the repair icon, but they have no vendor
-- window to repair in; the 5.4.8 data has neither flag bit.
UPDATE `creature_template` SET `npcflag` = 80 WHERE `entry` = 16278;
UPDATE `creature_template` SET `npcflag` = 3 WHERE `entry` = 21283;
