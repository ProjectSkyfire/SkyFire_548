-- Quest 26512: yellow SMSG_QUESTUPDATE_ADD_CREDIT text needs objective Description.
UPDATE quest_objective
SET Description = 'Test the Gnomecorder at the Lakeshire Graveyard'
WHERE QuestId = 26512 AND Type = 10 AND (Description IS NULL OR Description = '');
