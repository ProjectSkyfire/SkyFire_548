-- Enable the existing spawn-specific moonwell history events.
-- Their summon spells retain quest and nearby-shade conditions to prevent repeats.
UPDATE `creature_template`
SET `AIName` = 'SmartAI'
WHERE `entry` = 34575 AND `AIName` = '' AND `ScriptName` = '';
