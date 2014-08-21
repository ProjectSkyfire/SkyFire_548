-- Update quest_template with SourceItems and without SourceItemCount
UPDATE quest_template SET SourceItemCount=1 WHERE SourceItemId>0 AND SourceItemCount=0;