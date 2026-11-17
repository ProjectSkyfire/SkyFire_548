-- Grizzly Hills: Dun-da-Dun-tah! (12082). Harrison Jones stopped at his 18th waypoint because the two action lists
-- that follow it (his fight with the snake and the walk out of the ruins, which gives the quest credit) and the
-- waypoints of that walk were missing.

DELETE FROM `smart_scripts` WHERE `source_type` = 9 AND `entryorguid` IN (2681403, 2681404);
INSERT INTO `smart_scripts` (`entryorguid`, `source_type`, `id`, `link`, `event_type`, `event_phase_mask`, `event_chance`, `event_flags`, `event_param1`, `event_param2`, `event_param3`, `event_param4`, `event_param5`, `action_type`, `action_param1`, `action_param2`, `action_param3`, `action_param4`, `action_param5`, `action_param6`, `target_type`, `target_param1`, `target_param2`, `target_param3`, `target_x`, `target_y`, `target_z`, `target_o`, `comment`) VALUES
(2681403,9,0,0,0,0,100,0,0,0,0,0,0,1,7,0,0,0,0,0,1,0,0,0,0,0,0,0,'Harrison Jones - On Script - Say Line 7'),
(2681403,9,1,0,0,0,100,0,0,0,0,0,0,101,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Harrison Jones - On Script - Set Home Position'),
(2681403,9,2,0,0,0,100,0,7200,7200,0,0,0,1,8,0,0,0,0,0,1,0,0,0,0,0,0,0,'Harrison Jones - On Script - Say Line 8'),
(2681403,9,3,0,0,0,100,0,6000,6000,0,0,0,8,2,0,0,0,0,0,1,0,0,0,0,0,0,0,'Harrison Jones - On Script - Set Reactstate Aggressive'),
(2681404,9,0,0,0,0,100,0,1000,1000,0,0,0,53,1,268140,0,0,0,0,1,0,0,0,0,0,0,0,'Harrison Jones - On Script - Start Waypoint'),
(2681404,9,1,0,0,0,100,0,0,0,0,0,0,22,0,0,0,0,0,0,1,0,0,0,0,0,0,0,'Harrison Jones - On Script - Set Event Phase 0');

DELETE FROM `waypoints` WHERE `entry` = 268140;
INSERT INTO `waypoints` (`entry`, `pointid`, `position_x`, `position_y`, `position_z`, `point_comment`) VALUES
(268140,1,4893.09,-4802.88,32.0295,'Harrison Jones'),
(268140,2,4881.01,-4795.62,32.508,'Harrison Jones'),
(268140,3,4857.75,-4778.79,32.6265,'Harrison Jones'),
(268140,4,4837.62,-4774.61,32.5914,'Harrison Jones'),
(268140,5,4817.82,-4790.49,25.4644,'Harrison Jones'),
(268140,6,4826.31,-4827.56,25.4895,'Harrison Jones'),
(268140,7,4826.34,-4841.32,25.4841,'Harrison Jones'),
(268140,8,4809.19,-4851.31,25.5278,'Harrison Jones'),
(268140,9,4781.12,-4849.69,25.4412,'Harrison Jones'),
(268140,10,4761.5,-4853.98,25.0099,'Harrison Jones'),
(268140,11,4719.89,-4857.75,26.1367,'Harrison Jones'),
(268140,12,4693.85,-4858.84,32.4999,'Harrison Jones'),
(268140,13,4680.42,-4861.25,35.5674,'Harrison Jones'),
(268140,14,4673.97,-4864.33,35.5674,'Harrison Jones'),
(268140,15,4662.38,-4882.54,35.5876,'Harrison Jones'),
(268140,16,4658.19,-4895.92,36.1955,'Harrison Jones'),
(268140,17,4656.81,-4915.52,43.7827,'Harrison Jones'),
(268140,18,4659.83,-4928.78,47.8299,'Harrison Jones'),
(268140,19,4664.12,-4942.44,48.0028,'Harrison Jones'),
(268140,20,4666.97,-4951.51,48.1059,'Harrison Jones'),
(268140,21,4675.69,-4970.23,47.6443,'Harrison Jones'),
(268140,22,4681.64,-4973.98,46.7499,'Harrison Jones'),
(268140,23,4707.98,-4997.29,40.704,'Harrison Jones'),
(268140,24,4712.89,-5002.18,39.1201,'Harrison Jones'),
(268140,25,4739.86,-5031.48,31.1823,'Harrison Jones');
