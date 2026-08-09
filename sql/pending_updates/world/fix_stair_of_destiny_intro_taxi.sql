-- Use direct taxi activation for the Stair of Destiny intro flights.
-- The previous SmartAI rows cast spells that are not present in the 5.4.8 spell data.

UPDATE `smart_scripts`
SET `action_type` = 52,
    `action_param1` = 565,
    `action_param2` = 0,
    `comment` = 'Vlagga Freyfeather - On Gossip Select - Activate Taxi to Thrallmar'
WHERE `entryorguid` = 18930
  AND `source_type` = 0
  AND `id` = 0
  AND `event_type` = 62
  AND `event_param1` = 7938
  AND `event_param2` = 1;

UPDATE `smart_scripts`
SET `action_type` = 52,
    `action_param1` = 564,
    `action_param2` = 0,
    `comment` = 'Amish Wildhammer - On Gossip Select - Activate Taxi to Honor Hold'
WHERE `entryorguid` = 18931
  AND `source_type` = 0
  AND `id` = 0
  AND `event_type` = 62
  AND `event_param1` = 7939
  AND `event_param2` = 1;
