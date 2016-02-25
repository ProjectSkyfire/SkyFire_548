DELETE FROM `command` WHERE `name` IN ('ticket bug assign',
                                       'ticket bug close',
                                       'ticket bug closedlist',
                                       'ticket bug comment',
                                       'ticket bug delete',
                                       'ticket bug list',
                                       'ticket bug unassign',
                                       'ticket bug view',
                                       'ticket bug reset');

INSERT INTO `command` (`name`, `permission`, `help`) VALUES
('ticket bug assign',     778, 'Usage: .ticket bug assign $ticketid $gmname.\r\nAssigns the specified ticket to the specified Game Master.'),
('ticket bug close',      779, 'Usage: .ticket bug close $ticketid.\r\nCloses the specified ticket. Does not delete permanently.'),
('ticket bug closedlist', 780, 'Usage: Displays a list of closed bug tickets.'),
('ticket bug comment',    781, 'Usage: .ticket bug comment $ticketid $comment.\r\nAllows the adding or modifying of a comment to the specified ticket.'),
('ticket bug delete',     782, 'Usage: .ticket bug delete $ticketid.\r\nDeletes the specified ticket permanently. Ticket must be closed first.'),
('ticket bug list',       783, 'Usage: Displays a list of open bug tickets.'),
('ticket bug unassign',   784, 'Usage: .ticket bug unassign $ticketid.\r\nUnassigns the specified ticket from the current assigned Game Master.'),
('ticket bug view',       785, 'Usage: .ticket bug view $ticketid.\r\nReturns details about specified ticket. Ticket must be open and not deleted.'),
('ticket bug reset',      786, 'Usage: Removes all closed bug tickets and resets the counter, if no pending open tickets exist.');