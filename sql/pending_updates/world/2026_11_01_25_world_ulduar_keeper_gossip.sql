-- Ulduar: Thorim's and Mimiron's images in the Observation Ring offered "Lend us your aid" but led to menus that do not
-- exist (10334 and 10332), so the players could not ask them to join the Yogg-Saron fight. Like Freya and Hodir, they
-- lead to the confirmation menu 10333, whose answer the script handles.
UPDATE `gossip_menu_option` SET `ActionMenuID` = 10333 WHERE `MenuID` IN (10336, 10337) AND `OptionID` = 0;
