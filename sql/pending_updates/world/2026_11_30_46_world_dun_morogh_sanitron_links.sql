-- Sanitron 500 (46185): rows 0 to 2 link to the next row, but those rows have their own events (reset, passenger boarded),
-- not the link event 61, so the core rejects every link and logs it on each respawn and reset. Each row already runs on
-- its own event; clear the links.
UPDATE `smart_scripts` SET `link` = 0 WHERE `entryorguid` = 46185 AND `source_type` = 0 AND `id` IN (0, 1, 2) AND `link` <> 0;
