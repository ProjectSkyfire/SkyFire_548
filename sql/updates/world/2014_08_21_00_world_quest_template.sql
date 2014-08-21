-- Update quest_template with SourceItems and without SourceItemCount /* Affected rows: 388 */
UPDATE quest_template SET SourceItemCount=1 WHERE SourceItemId>0 AND SourceItemCount=0;
-- Update quest_template with method 1 for quests where minlevel>90; -- most obsolete quests /* Affected rows: 74 */
UPDATE quest_template SET Method=1 WHERE MinLevel>90;