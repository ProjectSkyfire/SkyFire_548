-- Chief Engineer Copperclaw sells reagents like his Alliance counterpart Boltwrench, and Utamu sells food like Galley
-- Chief Dolinger, but their npc flags lacked the vendor type.
UPDATE `creature_template` SET `npcflag` = 6275 WHERE `entry` = 30825;
UPDATE `creature_template` SET `npcflag` = 640 WHERE `entry` = 30827;
