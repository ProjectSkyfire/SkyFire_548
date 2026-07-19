-- Add the inline gossip option locale table used by the gossip menu option loader.

DROP TABLE IF EXISTS `gossip_menu_option_locale`;
CREATE TABLE `gossip_menu_option_locale` (
  `MenuID` int unsigned NOT NULL DEFAULT '0',
  `OptionID` int unsigned NOT NULL DEFAULT '0',
  `Locale` varchar(4) CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci NOT NULL,
  `OptionText` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci,
  `BoxText` text CHARACTER SET utf8mb4 COLLATE utf8mb4_unicode_ci,
  PRIMARY KEY (`MenuID`,`OptionID`,`Locale`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

UPDATE `command`
SET `name` = 'reload gossip_menu_option_locale',
    `help` = 'Syntax: .reload gossip_menu_option_locale\nReload gossip_menu_option_locale table.'
WHERE `permission` = 661
  AND `name` = 'reload locales_gossip_menu_option';
