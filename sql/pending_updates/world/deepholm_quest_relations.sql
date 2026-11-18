-- Deepholm: quest givers and enders of the 5.4.8 data that the base data lacks (Maruut Stonebinder gives "Silvermarsh
-- Rendezvous", Gravel Longslab and Clay Mudaxle give the second quests of their chains, Slate Quicksand ends "One With
-- the Ground", Examiner Rowe gives "The Reliquary" and so on), and the end of the Horde quest "Decryption Made Easy".
-- Based on Legends of Azeroth 5.4.8, except the last row, which is not in any data set: "Decryption Made Easy" (27041)
-- has no quest ender anywhere, so the Horde chain "The Twilight Plot" - "Decryption Made Easy" - "The Wrong Sequence" stopped
-- after the six decrypted plans. Examiner Rowe, who starts it, ends it, as the Alliance twin 27040 is ended by its giver.
DELETE FROM `creature_queststarter` WHERE (`id`, `quest`) IN ((43065, 27007), (43168, 26501), (43169, 26502), (43395, 26580), (44238, 28292), (44799, 27952), (44823, 27953), (47195, 27935), (47571, 28293));
INSERT INTO `creature_queststarter` (`id`, `quest`) VALUES
(43065, 27007), -- Maruut Stonebinder, Silvermarsh Rendezvous
(43168, 26501), -- Gravel Longslab, Sealing the Way
(43169, 26502), -- Clay Mudaxle, Thunder Stones
(43395, 26580), -- Windspeaker Lorvarius, Familiar Intruders
(44238, 28292), -- That's No Pyramid! (Alliance)
(44799, 27952), -- The Explorers (Alliance)
(44823, 27953), -- Examiner Rowe, The Reliquary
(47195, 27935), -- Slate Quicksand, Bring Down the Avalanche
(47571, 28293); -- Belloc Brightblade, That's No Pyramid!

DELETE FROM `creature_questender` WHERE (`id`, `quest`) IN ((47195, 27934), (44823, 27041));
INSERT INTO `creature_questender` (`id`, `quest`) VALUES
(47195, 27934), -- Slate Quicksand, One With the Ground
(44823, 27041); -- Examiner Rowe, Decryption Made Easy (not in any data set)
