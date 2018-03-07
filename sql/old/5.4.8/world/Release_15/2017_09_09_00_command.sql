DELETE FROM `command` WHERE `permission` IN (806, 807);
INSERT INTO `command` (`name`, `permission`, `help`) VALUES
('account boost add', 806, 'Syntax: .account boost add $account \n\nSet Boost for account.'),
('account boost delete', 807, 'Syntax: .account boost delete $account \r\n\r\nRemove Boost for account.');
