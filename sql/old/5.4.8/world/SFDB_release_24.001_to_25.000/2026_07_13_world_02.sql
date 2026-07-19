-- Fix class trainer dual-spec, talent gossip, npcflags, and trainer spell links.
-- Dual-spec info options used OptionType 20 (unlearn specialization) instead of GOSSIP(1).
-- Many class trainers lacked UNIT_NPC_FLAG_TRAINER / gossip menus / npc_trainer template refs.

-- Dual-spec purchase menu
DELETE FROM `gossip_menu_option` WHERE `MenuID` = 10371 AND `OptionID` = 0;
INSERT INTO `gossip_menu_option` (`MenuID`,`OptionID`,`OptionIcon`,`OptionText`,`OptionBroadcastTextID`,`OptionType`,`OptionNpcflag`,`ActionMenuID`,`ActionPoiID`,`BoxCoded`,`BoxMoney`,`BoxText`,`BoxBroadcastTextID`,`VerifiedBuild`) VALUES
(10371, 0, 0, 'Purchase a Dual Talent Specialization.', 33765, 18, 16, 10373, 0, 0, 100000, 'Are you sure you would like to purchase your second talent specialization?', 0, 15595);

-- Talent unlearn confirmation
DELETE FROM `gossip_menu_option` WHERE `MenuID` = 4461 AND `OptionID` = 0;
INSERT INTO `gossip_menu_option` (`MenuID`,`OptionID`,`OptionIcon`,`OptionText`,`OptionBroadcastTextID`,`OptionType`,`OptionNpcflag`,`ActionMenuID`,`ActionPoiID`,`BoxCoded`,`BoxMoney`,`BoxText`,`BoxBroadcastTextID`,`VerifiedBuild`) VALUES
(4461, 0, 11, 'Yes. I do.', 60961, 16, 16, 0, 0, 0, 0, NULL, 0, 15595);

-- Dual Talent Specialization info options: OptionType GOSSIP(1) -> menu 10371
DELETE FROM `gossip_menu_option` WHERE (`MenuID`,`OptionID`) IN (
(63,2),(64,2),(85,2),(141,2),(381,2),(410,2),(411,2),(436,2),(523,2),(655,2),(656,2),(1403,2),(1503,2),(1522,2),(2304,2),(2381,2),(2383,2),(2384,2),(3642,2),(3643,2),(3644,2),(3645,2),(3921,2),(3924,2),(3925,2),(3926,2),(3984,2),(4007,2),(4008,2),(4009,2),(4010,2),(4011,2),(4012,2),(4017,2),(4023,2),(4091,2),(4092,2),(4101,2),(4103,2),(4104,2),(4463,2),(4464,2),(4466,2),(4467,2),(4468,2),(4469,2),(4470,2),(4471,2),(4472,2),(4473,2),
(4474,2),(4475,2),(4481,2),(4482,2),(4484,2),(4485,2),(4486,2),(4502,2),(4503,2),(4504,2),(4505,2),(4506,2),(4507,2),(4508,2),(4509,2),(4511,2),(4512,2),(4513,2),(4515,2),(4516,2),(4517,2),(4518,2),(4519,2),(4520,2),(4521,2),(4522,2),(4523,2),(4524,2),(4525,2),(4526,2),(4527,2),(4528,2),(4529,2),(4530,2),(4531,2),(4532,2),(4533,2),(4534,2),(4535,2),(4537,2),(4538,2),(4539,2),(4540,2),(4541,2),(4542,2),(4543,2),(4544,2),(4545,2),(4546,2),(4547,2),
(4548,2),(4549,2),(4550,2),(4551,2),(4552,2),(4556,2),(4557,2),(4558,2),(4559,2),(4561,2),(4562,2),(4566,2),(4567,2),(4568,2),(4569,2),(4570,2),(4571,2),(4572,2),(4573,2),(4574,2),(4575,2),(4576,2),(4577,2),(4578,2),(4579,2),(4581,2),(4603,2),(4604,2),(4605,2),(4606,2),(4607,2),(4609,2),(4610,2),(4621,2),(4641,2),(4642,2),(4643,2),(4645,2),(4646,2),(4647,2),(4648,2),(4649,2),(4650,2),(4651,2),(4652,2),(4653,2),(4654,2),(4655,2),(4656,2),(4657,2),
(4658,2),(4659,2),(4660,2),(4661,2),(4662,2),(4665,2),(4666,2),(4667,2),(4674,2),(4675,2),(4676,2),(4677,2),(4678,2),(4679,2),(4680,2),(4681,2),(4682,2),(4683,2),(4684,2),(4685,2),(4686,2),(4687,2),(4688,2),(4690,2),(4691,2),(4692,2),(4693,2),(4694,2),(4695,2),(4696,2),(4697,2),(4801,2),(5061,2),(5123,2),(6628,2),(6647,2),(6648,2),(6649,2),(6650,2),(6652,2),(7260,2),(7262,2),(7263,2),(7264,2),(7265,2),(7349,2),(7357,2),(7366,2),(7368,2),(7377,3),
(7437,2),(7438,2),(7467,2),(7522,2),(7566,2),(7567,2),(8110,2),(8111,2),(9580,2),(9691,2),(9692,2),(9693,2),(9792,2),(9793,2),(9794,2),(9990,2),(10027,2),(11767,2),(11793,2),(11817,2),(12052,2),(12529,2),(12606,2),(12746,2),(14199,2),(21221,2));
INSERT INTO `gossip_menu_option` (`MenuID`,`OptionID`,`OptionIcon`,`OptionText`,`OptionBroadcastTextID`,`OptionType`,`OptionNpcflag`,`ActionMenuID`,`ActionPoiID`,`BoxCoded`,`BoxMoney`,`BoxText`,`BoxBroadcastTextID`,`VerifiedBuild`) VALUES
(63,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(64,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(85,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(141,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(381,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(410,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(411,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(436,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(523,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(655,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(656,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(1403,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(1503,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(1522,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(2304,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,15050),
(2381,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(2383,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,14545),
(2384,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,15005),
(3642,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(3643,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(3644,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(3645,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(3921,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(3924,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(3925,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(3926,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(3984,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4007,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4008,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4009,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4010,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4011,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4012,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4017,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4023,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4091,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4092,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4101,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4103,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4104,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4463,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4464,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4466,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4467,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4468,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4469,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4470,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4471,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,15595),
(4472,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4473,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4474,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4475,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4481,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4482,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4484,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4485,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4486,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4502,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4503,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4504,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4505,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,13623),
(4506,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4507,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4508,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4509,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4511,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4512,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4513,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4515,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4516,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4517,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4518,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4519,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4520,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4521,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4522,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4523,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4524,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4525,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4526,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4527,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4528,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4529,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4530,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4531,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4532,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4533,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4534,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4535,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4537,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4538,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4539,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4540,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4541,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4542,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4543,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4544,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4545,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4546,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4547,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4548,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4549,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4550,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4551,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4552,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4556,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,15050),
(4557,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,15050),
(4558,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4559,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4561,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4562,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4566,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4567,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4568,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4569,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4570,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4571,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4572,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4573,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4574,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4575,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4576,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4577,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4578,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4579,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4581,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4603,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4604,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4605,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4606,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4607,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4609,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,14545),
(4610,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4621,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4641,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4642,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4643,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4645,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4646,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4647,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4648,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4649,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4650,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4651,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4652,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4653,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4654,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4655,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4656,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4657,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4658,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4659,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4660,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4661,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4662,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4665,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4666,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4667,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4674,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4675,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4676,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4677,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4678,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4679,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4680,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4681,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4682,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4683,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4684,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4685,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4686,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4687,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4688,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4690,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4691,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4692,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4693,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4694,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4695,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4696,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4697,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(4801,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(5061,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(5123,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(6628,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(6647,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,15354),
(6648,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(6649,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,15211),
(6650,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(6652,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(7260,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,15595),
(7262,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(7263,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(7264,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(7265,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(7349,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(7357,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(7366,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(7368,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(7377,3,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(7437,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(7438,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(7467,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(7522,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(7566,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,14545),
(7567,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(8110,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(8111,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(9580,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(9691,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,14545),
(9692,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,14545),
(9693,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,14545),
(9792,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,14480),
(9793,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(9794,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(9990,2,0,'Learn about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(10027,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,14545),
(11767,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,15595),
(11793,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,15211),
(11817,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(12052,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,15050),
(12529,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(12606,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,15211),
(12746,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,14545),
(14199,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0),
(21221,2,0,'I wish to know about Dual Talent Specialization.',33762,1,1,10371,0,0,0,NULL,0,0);

-- Specialization unlearn options (OptionType 20)
DELETE FROM `gossip_menu_option` WHERE (`MenuID`,`OptionID`) IN (
(63,3),(64,3),(85,5),(141,10),(381,5),(410,5),(411,5),(436,5),(523,3),(655,3),(656,3),(1403,4),(1503,3),(1522,3),(2304,3),(2381,3),(2383,3),(2384,3),(3642,3),(3643,3),(3644,3),(3645,3),(3921,3),(3924,3),(3925,3),(3926,3),(3984,5),(4007,3),(4008,3),(4009,3),(4010,3),(4011,3),(4012,3),(4017,3),(4023,3),(4091,3),(4092,3),(4101,3),(4103,3),(4104,3),(4463,3),(4464,3),(4466,3),(4467,3),(4468,3),(4469,3),(4470,3),(4471,3),(4472,3),(4473,3),
(4474,3),(4475,3),(4481,3),(4482,3),(4484,3),(4485,3),(4486,3),(4502,5),(4503,3),(4504,3),(4505,3),(4506,3),(4507,3),(4508,3),(4509,3),(4511,3),(4512,5),(4513,5),(4515,3),(4516,3),(4517,3),(4518,3),(4519,3),(4520,3),(4521,3),(4522,3),(4523,3),(4524,3),(4525,3),(4526,3),(4527,3),(4528,3),(4529,3),(4530,3),(4531,3),(4532,3),(4533,3),(4534,3),(4535,3),(4537,3),(4538,3),(4539,3),(4540,5),(4541,5),(4542,5),(4543,3),(4544,3),(4545,3),(4546,3),(4547,3),
(4548,3),(4549,3),(4550,3),(4551,3),(4552,3),(4556,3),(4557,3),(4558,3),(4559,3),(4561,5),(4562,5),(4566,3),(4567,3),(4568,3),(4569,3),(4570,3),(4571,3),(4572,3),(4573,3),(4574,3),(4575,5),(4576,5),(4577,5),(4578,3),(4579,3),(4581,3),(4603,3),(4604,3),(4605,3),(4606,3),(4607,3),(4609,3),(4610,3),(4621,3),(4641,3),(4642,3),(4643,3),(4645,3),(4646,3),(4647,3),(4648,3),(4649,3),(4650,3),(4651,3),(4652,3),(4653,3),(4654,3),(4655,3),(4656,3),(4657,3),
(4658,5),(4659,5),(4660,3),(4661,3),(4662,3),(4665,3),(4666,3),(4667,3),(4674,3),(4675,3),(4676,5),(4677,3),(4678,3),(4679,3),(4680,3),(4681,3),(4682,3),(4683,3),(4684,3),(4685,3),(4686,3),(4687,3),(4688,3),(4690,5),(4691,3),(4692,3),(4693,3),(4694,3),(4695,3),(4696,3),(4697,3),(4801,3),(5061,5),(5123,3),(6628,3),(6647,3),(6648,3),(6649,3),(6650,5),(6652,3),(7260,3),(7262,3),(7263,3),(7264,3),(7265,3),(7349,3),(7357,3),(7366,3),(7368,5),(7377,4),
(7437,3),(7438,3),(7467,3),(7522,3),(7566,3),(7567,3),(8110,3),(8111,3),(9580,3),(9691,3),(9692,3),(9693,3),(9792,3),(9793,3),(9794,3),(9990,3),(10027,3),(11767,3),(11793,3),(11817,3),(12052,3),(12529,3),(12606,3),(12746,3),(14199,3),(21221,3));
INSERT INTO `gossip_menu_option` (`MenuID`,`OptionID`,`OptionIcon`,`OptionText`,`OptionBroadcastTextID`,`OptionType`,`OptionNpcflag`,`ActionMenuID`,`ActionPoiID`,`BoxCoded`,`BoxMoney`,`BoxText`,`BoxBroadcastTextID`,`VerifiedBuild`) VALUES
(63,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(64,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(85,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(141,10,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(381,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(410,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(411,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(436,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(523,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(655,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(656,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(1403,4,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(1503,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(1522,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(2304,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(2381,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(2383,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(2384,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(3642,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(3643,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(3644,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(3645,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(3921,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(3924,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(3925,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(3926,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(3984,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4007,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4008,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4009,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4010,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4011,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4012,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4017,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4023,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4091,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4092,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4101,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4103,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4104,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4463,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4464,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4466,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4467,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4468,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4469,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4470,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4471,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4472,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4473,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4474,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4475,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4481,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4482,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4484,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4485,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4486,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4502,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4503,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4504,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4505,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4506,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4507,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4508,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4509,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4511,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4512,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4513,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4515,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4516,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4517,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4518,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4519,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4520,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4521,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4522,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4523,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4524,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4525,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4526,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4527,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4528,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4529,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4530,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4531,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4532,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4533,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4534,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4535,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4537,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4538,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4539,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4540,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4541,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4542,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4543,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4544,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4545,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4546,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4547,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4548,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4549,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4550,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4551,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4552,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4556,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4557,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4558,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4559,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4561,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4562,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4566,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4567,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4568,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4569,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4570,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4571,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4572,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4573,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4574,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4575,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4576,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4577,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4578,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4579,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4581,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4603,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4604,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4605,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4606,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4607,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4609,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4610,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4621,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4641,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4642,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4643,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4645,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4646,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4647,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4648,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4649,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4650,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4651,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4652,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4653,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4654,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4655,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4656,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4657,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4658,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4659,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4660,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4661,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4662,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4665,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4666,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4667,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4674,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4675,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4676,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4677,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4678,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4679,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4680,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4681,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4682,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4683,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4684,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4685,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4686,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4687,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4688,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4690,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4691,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4692,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4693,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4694,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4695,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4696,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4697,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(4801,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(5061,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(5123,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(6628,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(6647,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(6648,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(6649,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(6650,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(6652,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(7260,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(7262,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(7263,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(7264,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(7265,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(7349,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(7357,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(7366,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(7368,5,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(7377,4,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(7437,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(7438,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(7467,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(7522,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(7566,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(7567,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(8110,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(8111,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(9580,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(9691,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(9692,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(9693,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(9792,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(9793,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(9794,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(9990,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(10027,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(11767,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(11793,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(11817,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(12052,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(12529,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(12606,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(12746,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(14199,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0),
(21221,3,0,'I wish to unlearn my specialization',0,20,16,0,0,0,0,NULL,0,0);

-- Talent unlearn options (OptionType 16)
DELETE FROM `gossip_menu_option` WHERE (`MenuID`,`OptionID`) IN (
(0,14),(63,1),(64,1),(85,1),(141,1),(141,9),(381,1),(410,1),(411,1),(436,1),(523,1),(655,1),(656,1),(1403,1),(1503,1),(1522,1),(2304,1),(2381,1),(2383,1),(2384,1),(3642,1),(3643,1),(3644,1),(3645,1),(3921,1),(3923,1),(3924,1),(3925,1),(3926,1),(3984,1),(4007,1),(4008,1),(4009,1),(4010,1),(4011,1),(4012,1),(4017,1),(4023,1),(4091,1),(4092,1),(4101,1),(4103,1),(4104,1),(4105,1),(4463,1),(4464,1),(4466,1),(4467,1),(4468,1),(4469,1),
(4470,1),(4471,1),(4472,1),(4473,1),(4474,1),(4475,1),(4481,1),(4482,1),(4484,1),(4485,1),(4486,1),(4502,1),(4503,1),(4504,1),(4505,1),(4506,1),(4507,1),(4508,1),(4509,1),(4511,1),(4512,1),(4513,1),(4515,1),(4516,1),(4517,1),(4518,1),(4519,1),(4520,1),(4521,1),(4522,1),(4523,1),(4524,1),(4525,1),(4526,1),(4527,1),(4528,1),(4529,1),(4530,1),(4531,1),(4532,1),(4533,1),(4534,1),(4535,1),(4537,1),(4538,1),(4539,1),(4540,1),(4541,1),(4542,1),(4543,1),
(4544,1),(4545,1),(4546,1),(4547,1),(4548,1),(4549,1),(4550,1),(4551,1),(4552,1),(4553,1),(4554,1),(4555,1),(4556,1),(4557,1),(4558,1),(4559,1),(4561,1),(4562,1),(4566,1),(4567,1),(4568,1),(4569,1),(4570,1),(4571,1),(4572,1),(4573,1),(4574,1),(4575,1),(4576,1),(4577,1),(4578,1),(4579,1),(4581,1),(4603,1),(4604,1),(4605,1),(4606,1),(4607,1),(4609,1),(4610,1),(4621,1),(4641,1),(4642,1),(4643,1),(4645,1),(4646,1),(4647,1),(4648,1),(4649,1),(4650,1),
(4651,1),(4652,1),(4653,1),(4654,1),(4655,1),(4656,1),(4657,1),(4658,1),(4659,1),(4660,1),(4661,1),(4662,1),(4665,1),(4666,1),(4667,1),(4674,1),(4675,1),(4676,1),(4677,1),(4678,1),(4679,1),(4680,1),(4681,1),(4682,1),(4683,1),(4684,1),(4685,1),(4686,1),(4687,1),(4688,1),(4690,1),(4691,1),(4692,1),(4693,1),(4694,1),(4695,1),(4696,1),(4697,1),(4801,1),(5061,1),(5123,1),(6628,1),(6647,1),(6648,1),(6649,1),(6650,1),(6652,1),(7260,1),(7262,1),(7263,1),
(7264,1),(7265,1),(7349,1),(7357,1),(7366,1),(7368,1),(7377,2),(7437,1),(7438,1),(7467,1),(7522,1),(7566,1),(7567,1),(8110,1),(8111,1),(9580,1),(9792,1),(9793,1),(9794,1),(9990,1),(10027,1),(11154,1),(11169,1),(11172,1),(11174,1),(11217,1),(11265,1),(11266,1),(11268,1),(11766,1),(11767,1),(11793,1),(11817,1),(11824,1),(11875,1),(11877,1),(11878,1),(11879,1),(11912,1),(11913,1),(11932,1),(12010,1),(12048,1),(12052,1),(12151,1),(12233,1),(12341,1),(12344,1),(12516,1),(12521,1),
(12522,1),(12523,1),(12527,1),(12529,1),(12537,1),(12539,1),(12540,1),(12580,1),(12606,1),(12714,1),(12716,1),(12746,1),(12755,1),(12821,1),(12920,1),(12927,1),(14199,1),(21221,1));
INSERT INTO `gossip_menu_option` (`MenuID`,`OptionID`,`OptionIcon`,`OptionText`,`OptionBroadcastTextID`,`OptionType`,`OptionNpcflag`,`ActionMenuID`,`ActionPoiID`,`BoxCoded`,`BoxMoney`,`BoxText`,`BoxBroadcastTextID`,`VerifiedBuild`) VALUES
(0,14,2,'I wish to unlearn my talents',8271,16,16,0,0,0,0,NULL,0,0),
(63,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15595),
(64,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(85,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(141,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(141,9,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15595),
(381,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(410,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(411,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(436,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(523,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(655,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(656,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(1403,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(1503,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15050),
(1522,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(2304,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15595),
(2381,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15050),
(2383,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,14545),
(2384,1,0,'I wish to unlearn my talents.',62295,16,16,4463,0,0,0,NULL,0,15050),
(3642,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(3643,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(3644,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15595),
(3645,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(3921,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(3923,1,0,'I wish to unlearn my talents.',62295,16,16,0,0,0,0,NULL,0,15595),
(3924,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15595),
(3925,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(3926,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(3984,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4007,1,0,'I wish to unlearn my talents.',8271,16,16,4461,0,0,0,NULL,0,0),
(4008,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4009,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,14333),
(4010,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4011,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4012,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4017,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4023,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4091,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4092,1,0,'I wish to unlearn my talents.',8271,16,16,4461,0,0,0,NULL,0,0),
(4101,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4103,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4104,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15211),
(4105,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15050),
(4463,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4464,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4466,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4467,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4468,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4469,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4470,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4471,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15595),
(4472,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4473,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4474,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4475,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4481,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4482,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4484,1,0,'I wish to unlearn my talents.',62295,16,16,4463,0,0,0,NULL,0,15211),
(4485,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15211),
(4486,1,0,'I wish to unlearn my talents.',62295,16,16,4463,0,0,0,NULL,0,15211),
(4502,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4503,1,0,'I wish to unlearn my talents.',62295,16,16,4463,0,0,0,NULL,0,15050),
(4504,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15050),
(4505,1,0,'I wish to unlearn my talents.',62295,16,16,4463,0,0,0,NULL,0,15050),
(4506,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4507,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4508,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4509,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,14545),
(4511,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,14545),
(4512,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15595),
(4513,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15595),
(4515,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,14007),
(4516,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15050),
(4517,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4518,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4519,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4520,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4521,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15211),
(4522,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4523,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4524,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4525,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4526,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4527,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4528,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4529,1,0,'I wish to unlearn my talents.',62295,16,16,4463,0,0,0,NULL,0,15050),
(4530,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4531,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4532,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4533,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4534,1,0,'I wish to unlearn my talents.',8271,16,16,4461,0,0,0,NULL,0,0),
(4535,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4537,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4538,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4539,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4540,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4541,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4542,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4543,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4544,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15211),
(4545,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4546,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4547,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4548,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4549,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4550,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4551,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4552,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15211),
(4553,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15211),
(4554,1,0,'I wish to unlearn my talents.',8271,16,16,0,0,0,0,NULL,0,15211),
(4555,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15050),
(4556,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15595),
(4557,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15595),
(4558,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4559,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4561,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4562,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4566,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15050),
(4567,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15050),
(4568,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4569,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4570,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4571,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15595),
(4572,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4573,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4574,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4575,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4576,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4577,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4578,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4579,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4581,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4603,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4604,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4605,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4606,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4607,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4609,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,14545),
(4610,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15050),
(4621,1,0,'I wish to unlearn my talents.',8271,16,16,4461,0,0,0,NULL,0,0),
(4641,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4642,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4643,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4645,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4646,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4647,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4648,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4649,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,14333),
(4650,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4651,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4652,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4653,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4654,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4655,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4656,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4657,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4658,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4659,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4660,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4661,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4662,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4665,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4666,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15595),
(4667,1,0,'I wish to unlearn my talents.',62295,16,16,4463,0,0,0,NULL,0,15050),
(4674,1,0,'I wish to unlearn my talents.',8271,16,16,4461,0,0,0,NULL,0,0),
(4675,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4676,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4677,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4678,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4679,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4680,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4681,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15050),
(4682,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15050),
(4683,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4684,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4685,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4686,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4687,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15354),
(4688,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4690,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4691,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4692,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4693,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4694,1,0,'I wish to unlearn my talents.',8271,16,16,4461,0,0,0,NULL,0,0),
(4695,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4696,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4697,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(4801,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(5061,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(5123,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15050),
(6628,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(6647,1,0,'I wish to unlearn my talents.',62295,16,16,0,0,0,0,NULL,0,15354),
(6648,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(6649,1,0,'I wish to unlearn my talents.',62295,16,16,4463,0,0,0,NULL,0,15211),
(6650,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(6652,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15211),
(7260,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15595),
(7262,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(7263,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(7264,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15211),
(7265,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(7349,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(7357,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,14007),
(7366,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(7368,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(7377,2,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(7437,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15050),
(7438,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(7467,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(7522,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(7566,1,0,'I wish to unlearn my talents.',62295,16,16,4463,0,0,0,NULL,0,15354),
(7567,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(8110,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(8111,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,0),
(9580,1,0,'I wish to unlearn my talents.',62295,16,16,0,0,0,0,NULL,0,14545),
(9792,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,14480),
(9793,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,0),
(9794,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,0),
(9990,1,0,'I wish to unlearn my talents.',62295,16,16,4461,0,0,0,NULL,0,15211),
(10027,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,14545),
(11154,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15595),
(11169,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15595),
(11172,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15005),
(11174,1,0,'I wish to unlearn my talents.',8271,16,16,0,0,0,0,NULL,0,15595),
(11217,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,14545),
(11265,1,0,'I wish to unlearn my talents.',8271,16,16,0,0,0,0,NULL,0,15595),
(11266,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15595),
(11268,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15005),
(11766,1,0,'I wish to unlearn my talents.',8271,16,16,2001,0,0,0,NULL,0,15211),
(11767,1,0,'I wish to unlearn my talents.',62295,16,16,4463,0,0,0,NULL,0,15595),
(11793,1,0,'I wish to unlearn my talents.',62295,16,16,0,0,0,0,NULL,0,15211),
(11817,1,0,'I wish to unlearn my talents.',62295,16,16,0,0,0,0,NULL,0,0),
(11824,1,0,'I wish to unlearn my talents.',8271,16,16,0,0,0,0,NULL,0,14333),
(11875,1,0,'I wish to unlearn my talents.',0,16,16,4461,0,0,0,NULL,0,14545),
(11877,1,0,'I wish to unlearn my talents.',0,16,16,4463,0,0,0,NULL,0,15595),
(11878,1,0,'I wish to unlearn my talents.',8271,16,16,0,0,0,0,NULL,0,15050),
(11879,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,14545),
(11912,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,14545),
(11913,1,0,'I wish to unlearn my talents.',0,16,16,4461,0,0,0,NULL,0,14545),
(11932,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15354),
(12010,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15211),
(12048,1,0,'I wish to unlearn my talents.',0,16,16,4461,0,0,0,NULL,0,14545),
(12052,1,0,'I wish to unlearn my talents.',0,16,16,4463,0,0,0,NULL,0,15050),
(12151,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,14545),
(12233,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15595),
(12341,1,0,'I wish to unlearn my talents.',8271,16,16,0,0,0,0,NULL,0,15005),
(12344,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,14545),
(12516,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,0),
(12521,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15050),
(12522,1,0,'I wish to unlearn my talents.',0,16,16,4463,0,0,0,NULL,0,15050),
(12523,1,0,'I wish to unlearn my talents.',8271,16,16,0,0,0,0,NULL,0,14333),
(12527,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,0),
(12529,1,0,'I wish to unlearn my talents.',62295,16,16,0,0,0,0,NULL,0,0),
(12537,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,14545),
(12539,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15595),
(12540,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15595),
(12580,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,14545),
(12606,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15211),
(12714,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,14545),
(12716,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15050),
(12746,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15050),
(12755,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15354),
(12821,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15050),
(12920,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15211),
(12927,1,0,'I wish to unlearn my talents.',0,16,16,0,0,0,0,NULL,0,15595),
(14199,1,0,'I wish to unlearn my talents.',62295,16,16,0,0,0,0,NULL,0,0),
(21221,1,0,'I wish to unlearn my talents.',8271,16,16,4461,0,0,0,NULL,0,0);

-- Class trainer npcflag + gossip_menu_id (explicit final values; UPDATE avoids wiping other template columns)
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4661 WHERE `entry`=328;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4486 WHERE `entry`=331;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4466 WHERE `entry`=376;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4666 WHERE `entry`=377;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=1503 WHERE `entry`=459;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4681 WHERE `entry`=460;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4503 WHERE `entry`=461;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4679 WHERE `entry`=837;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4675 WHERE `entry`=895;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4667 WHERE `entry`=906;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4684 WHERE `entry`=912;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4649 WHERE `entry`=913;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4475 WHERE `entry`=914;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4676 WHERE `entry`=916;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=381 WHERE `entry`=917;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4502 WHERE `entry`=918;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4678 WHERE `entry`=926;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4664 WHERE `entry`=927;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4470 WHERE `entry`=928;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4686 WHERE `entry`=944;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=523 WHERE `entry`=985;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4104 WHERE `entry`=986;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4101 WHERE `entry`=987;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4642 WHERE `entry`=988;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4680 WHERE `entry`=1226;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4463 WHERE `entry`=1228;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4683 WHERE `entry`=1229;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4677 WHERE `entry`=1232;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=410 WHERE `entry`=1234;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4658 WHERE `entry`=1411;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4654 WHERE `entry`=2124;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4656 WHERE `entry`=2127;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=63 WHERE `entry`=2128;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=3645 WHERE `entry`=2129;
UPDATE `creature_template` SET `npcflag`=17, `gossip_menu_id`=4822 WHERE `entry`=2485;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4823 WHERE `entry`=2489;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4528 WHERE `entry`=3030;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4530 WHERE `entry`=3031;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4529 WHERE `entry`=3032;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=3921 WHERE `entry`=3033;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4606 WHERE `entry`=3034;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4607 WHERE `entry`=3036;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4011 WHERE `entry`=3038;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4023 WHERE `entry`=3039;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4524 WHERE `entry`=3040;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4526 WHERE `entry`=3041;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4525 WHERE `entry`=3042;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4527 WHERE `entry`=3043;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4533 WHERE `entry`=3044;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4531 WHERE `entry`=3045;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4532 WHERE `entry`=3046;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4534 WHERE `entry`=3047;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4536 WHERE `entry`=3048;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4535 WHERE `entry`=3049;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4645 WHERE `entry`=3059;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4644 WHERE `entry`=3060;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4647 WHERE `entry`=3061;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4103 WHERE `entry`=3062;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=3926 WHERE `entry`=3064;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4012 WHERE `entry`=3065;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4103 WHERE `entry`=3066;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=523 WHERE `entry`=3153;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4648 WHERE `entry`=3154;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=141 WHERE `entry`=3155;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4643 WHERE `entry`=3156;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4652 WHERE `entry`=3157;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=523 WHERE `entry`=3169;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=141 WHERE `entry`=3170;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4017 WHERE `entry`=3171;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4641 WHERE `entry`=3172;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4104 WHERE `entry`=3173;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4603 WHERE `entry`=3324;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4604 WHERE `entry`=3325;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=2384 WHERE `entry`=3326;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4513 WHERE `entry`=3327;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4512 WHERE `entry`=3328;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4516 WHERE `entry`=3344;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4010 WHERE `entry`=3352;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4509 WHERE `entry`=3353;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4511 WHERE `entry`=3354;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=21221 WHERE `entry`=3401;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4515 WHERE `entry`=3403;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4506 WHERE `entry`=3406;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4506 WHERE `entry`=3407;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=655 WHERE `entry`=3408;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4696 WHERE `entry`=3593;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4690 WHERE `entry`=3594;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4692 WHERE `entry`=3595;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4695 WHERE `entry`=3596;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4688 WHERE `entry`=3597;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4697 WHERE `entry`=3598;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=436 WHERE `entry`=3599;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4691 WHERE `entry`=3600;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4009 WHERE `entry`=3601;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=3923 WHERE `entry`=3602;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=3644 WHERE `entry`=3706;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=3644 WHERE `entry`=3707;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4581 WHERE `entry`=4087;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4578 WHERE `entry`=4089;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4574 WHERE `entry`=4090;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4573 WHERE `entry`=4091;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4572 WHERE `entry`=4092;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4008 WHERE `entry`=4146;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4576 WHERE `entry`=4163;
UPDATE `creature_template` SET `npcflag`=17, `gossip_menu_id`=4821 WHERE `entry`=4165;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4693 WHERE `entry`=4205;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4577 WHERE `entry`=4214;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4575 WHERE `entry`=4215;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=1403 WHERE `entry`=4217;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=3924 WHERE `entry`=4218;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=10373 WHERE `entry`=4219;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4570 WHERE `entry`=5113;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4549 WHERE `entry`=5115;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4550 WHERE `entry`=5116;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4551 WHERE `entry`=5117;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4558 WHERE `entry`=5142;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4560 WHERE `entry`=5143;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4463 WHERE `entry`=5144;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4463 WHERE `entry`=5145;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4556 WHERE `entry`=5148;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=2304 WHERE `entry`=5149;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4482 WHERE `entry`=5479;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4481 WHERE `entry`=5480;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4468 WHERE `entry`=5484;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4467 WHERE `entry`=5489;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4470 WHERE `entry`=5491;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4470 WHERE `entry`=5492;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4505 WHERE `entry`=5495;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4504 WHERE `entry`=5496;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4463 WHERE `entry`=5497;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4484 WHERE `entry`=5498;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4508 WHERE `entry`=5504;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=21228 WHERE `entry`=5505;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4507 WHERE `entry`=5506;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4474 WHERE `entry`=5515;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4473 WHERE `entry`=5516;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4472 WHERE `entry`=5517;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4682 WHERE `entry`=5612;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12670 WHERE `entry`=5750;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=12670 WHERE `entry`=5815;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=64 WHERE `entry`=5880;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4520 WHERE `entry`=5882;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4518 WHERE `entry`=5883;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4651 WHERE `entry`=5884;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4519 WHERE `entry`=5885;
UPDATE `creature_template` SET `npcflag`=17, `gossip_menu_id`=4825 WHERE `entry`=5957;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4826 WHERE `entry`=5958;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4522 WHERE `entry`=5994;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4523 WHERE `entry`=6014;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4521 WHERE `entry`=6018;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12670 WHERE `entry`=6027;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12670 WHERE `entry`=6328;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12670 WHERE `entry`=6373;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12670 WHERE `entry`=6374;
UPDATE `creature_template` SET `npcflag`=19, `gossip_menu_id`=3984 WHERE `entry`=6707;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11933 WHERE `entry`=7311;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4553 WHERE `entry`=7312;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4579 WHERE `entry`=7315;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4662 WHERE `entry`=8140;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4091 WHERE `entry`=8141;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4646 WHERE `entry`=8142;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4092 WHERE `entry`=8308;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11767 WHERE `entry`=8664;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=3925 WHERE `entry`=9465;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4674 WHERE `entry`=10930;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=3643 WHERE `entry`=11397;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4801 WHERE `entry`=11401;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=3642 WHERE `entry`=11406;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4687 WHERE `entry`=12042;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12670 WHERE `entry`=12776;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=5061 WHERE `entry`=13283;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=5123 WHERE `entry`=13417;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11997 WHERE `entry`=13476;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6648 WHERE `entry`=15279;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6647 WHERE `entry`=15280;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6628 WHERE `entry`=15283;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6649 WHERE `entry`=15284;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6650 WHERE `entry`=15285;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6652 WHERE `entry`=15513;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=7566 WHERE `entry`=16266;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=6648 WHERE `entry`=16269;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=6652 WHERE `entry`=16270;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6647 WHERE `entry`=16275;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7438 WHERE `entry`=16276;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=6650 WHERE `entry`=16279;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7262 WHERE `entry`=16499;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7264 WHERE `entry`=16500;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7260 WHERE `entry`=16501;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7349 WHERE `entry`=16502;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7263 WHERE `entry`=16503;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7566 WHERE `entry`=16646;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7437 WHERE `entry`=16647;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7566 WHERE `entry`=16648;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=12670 WHERE `entry`=16649;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6648 WHERE `entry`=16651;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6648 WHERE `entry`=16652;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6648 WHERE `entry`=16653;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4824 WHERE `entry`=16654;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4605 WHERE `entry`=16655;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6649 WHERE `entry`=16658;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6649 WHERE `entry`=16659;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=6649 WHERE `entry`=16660;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6652 WHERE `entry`=16672;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6652 WHERE `entry`=16673;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6652 WHERE `entry`=16674;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=6647 WHERE `entry`=16679;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=6647 WHERE `entry`=16680;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6647 WHERE `entry`=16681;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6650 WHERE `entry`=16684;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6650 WHERE `entry`=16685;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=6650 WHERE `entry`=16686;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7567 WHERE `entry`=16721;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7262 WHERE `entry`=16738;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7264 WHERE `entry`=16749;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4824 WHERE `entry`=16755;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7265 WHERE `entry`=16756;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7260 WHERE `entry`=16761;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7263 WHERE `entry`=16771;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7522 WHERE `entry`=17089;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7366 WHERE `entry`=17105;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7368 WHERE `entry`=17110;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7263 WHERE `entry`=17120;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=7260 WHERE `entry`=17121;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7262 WHERE `entry`=17122;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7377 WHERE `entry`=17204;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7467 WHERE `entry`=17212;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7357 WHERE `entry`=17219;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7263 WHERE `entry`=17480;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7264 WHERE `entry`=17481;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7265 WHERE `entry`=17482;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7260 WHERE `entry`=17483;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=7263 WHERE `entry`=17504;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7262 WHERE `entry`=17505;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7260 WHERE `entry`=17509;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=7265 WHERE `entry`=17510;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7265 WHERE `entry`=17511;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7264 WHERE `entry`=17513;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7264 WHERE `entry`=17514;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7357 WHERE `entry`=17519;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7357 WHERE `entry`=17520;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7517 WHERE `entry`=17844;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=8433 WHERE `entry`=19340;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=8111 WHERE `entry`=20406;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=8110 WHERE `entry`=20407;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=8268 WHERE `entry`=20791;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7357 WHERE `entry`=23127;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6647 WHERE `entry`=23128;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=1522 WHERE `entry`=23534;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12670 WHERE `entry`=23535;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=9581 WHERE `entry`=27703;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=9580 WHERE `entry`=27704;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=9691 WHERE `entry`=28471;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=9692 WHERE `entry`=28472;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=9693 WHERE `entry`=28474;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=9990 WHERE `entry`=28956;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=9990 WHERE `entry`=28958;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=9777 WHERE `entry`=29156;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=9792 WHERE `entry`=29194;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=9793 WHERE `entry`=29195;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=10371 WHERE `entry`=29196;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10027 WHERE `entry`=31084;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10675 WHERE `entry`=34673;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10683 WHERE `entry`=34689;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10685 WHERE `entry`=34692;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10686 WHERE `entry`=34693;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10684 WHERE `entry`=34695;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10681 WHERE `entry`=34696;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10821 WHERE `entry`=34697;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7260 WHERE `entry`=35281;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10549 WHERE `entry`=35758;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=10550 WHERE `entry`=35778;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=10551 WHERE `entry`=35780;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10552 WHERE `entry`=35786;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=10553 WHERE `entry`=35805;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=10554 WHERE `entry`=35806;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=10555 WHERE `entry`=35807;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10694 WHERE `entry`=35839;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10702 WHERE `entry`=35869;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10700 WHERE `entry`=35870;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10699 WHERE `entry`=35871;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10698 WHERE `entry`=35872;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10696 WHERE `entry`=35873;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10697 WHERE `entry`=35874;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=10814 WHERE `entry`=36518;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=10815 WHERE `entry`=36519;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=10816 WHERE `entry`=36520;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10817 WHERE `entry`=36521;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=10818 WHERE `entry`=36523;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=10819 WHERE `entry`=36524;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=10555 WHERE `entry`=36525;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10834 WHERE `entry`=36628;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10835 WHERE `entry`=36629;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=10837 WHERE `entry`=36631;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10838 WHERE `entry`=36632;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10836 WHERE `entry`=36651;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10879 WHERE `entry`=37115;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10878 WHERE `entry`=37121;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11766 WHERE `entry`=37724;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11767 WHERE `entry`=37737;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11045 WHERE `entry`=38122;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10985 WHERE `entry`=38244;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10986 WHERE `entry`=38245;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10987 WHERE `entry`=38246;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10988 WHERE `entry`=38247;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=10837 WHERE `entry`=38465;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12576 WHERE `entry`=38513;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12572 WHERE `entry`=38514;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11049 WHERE `entry`=38515;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11050 WHERE `entry`=38516;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11051 WHERE `entry`=38517;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11052 WHERE `entry`=38518;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10836 WHERE `entry`=38793;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=10837 WHERE `entry`=38794;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10838 WHERE `entry`=38795;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10843 WHERE `entry`=38796;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10840 WHERE `entry`=38797;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10835 WHERE `entry`=38798;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10834 WHERE `entry`=38799;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4651 WHERE `entry`=39206;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11185 WHERE `entry`=39214;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11617 WHERE `entry`=42323;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11620 WHERE `entry`=42331;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11621 WHERE `entry`=42366;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11645 WHERE `entry`=42618;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11767 WHERE `entry`=43001;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11766 WHERE `entry`=43004;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11810 WHERE `entry`=43006;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11817 WHERE `entry`=43011;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11767 WHERE `entry`=43013;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11766 WHERE `entry`=43015;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11817 WHERE `entry`=43277;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11860 WHERE `entry`=43278;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11831 WHERE `entry`=43455;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12529 WHERE `entry`=43464;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11767 WHERE `entry`=43795;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11766 WHERE `entry`=43796;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11766 WHERE `entry`=43870;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=12821 WHERE `entry`=43881;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12821 WHERE `entry`=43883;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12821 WHERE `entry`=43892;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11817 WHERE `entry`=44247;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11817 WHERE `entry`=44249;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11824 WHERE `entry`=44380;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12529 WHERE `entry`=44393;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12529 WHERE `entry`=44394;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11829 WHERE `entry`=44395;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10694 WHERE `entry`=44455;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10696 WHERE `entry`=44459;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10697 WHERE `entry`=44461;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10699 WHERE `entry`=44464;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10698 WHERE `entry`=44465;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10700 WHERE `entry`=44468;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=10702 WHERE `entry`=44469;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11875 WHERE `entry`=44723;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11767 WHERE `entry`=44725;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4463 WHERE `entry`=44726;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11766 WHERE `entry`=44735;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11878 WHERE `entry`=44740;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11879 WHERE `entry`=44743;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=11906 WHERE `entry`=44978;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11912 WHERE `entry`=45019;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11913 WHERE `entry`=45023;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4105 WHERE `entry`=45029;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12050 WHERE `entry`=45095;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4521 WHERE `entry`=45137;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11932 WHERE `entry`=45138;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=12004 WHERE `entry`=45306;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=6649 WHERE `entry`=45337;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4544 WHERE `entry`=45339;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=12010 WHERE `entry`=45347;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12344 WHERE `entry`=45709;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12048 WHERE `entry`=45713;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12049 WHERE `entry`=45714;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12051 WHERE `entry`=45717;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12052 WHERE `entry`=45718;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12053 WHERE `entry`=45720;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12151 WHERE `entry`=46667;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11817 WHERE `entry`=46983;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=141 WHERE `entry`=47233;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=12233 WHERE `entry`=47246;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12233 WHERE `entry`=47247;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12233 WHERE `entry`=47248;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12341 WHERE `entry`=47767;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12341 WHERE `entry`=47788;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=63 WHERE `entry`=48613;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12513 WHERE `entry`=49736;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12516 WHERE `entry`=49741;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12517 WHERE `entry`=49745;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4666 WHERE `entry`=49749;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12521 WHERE `entry`=49760;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12522 WHERE `entry`=49769;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12523 WHERE `entry`=49781;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12524 WHERE `entry`=49782;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=3642 WHERE `entry`=49784;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12525 WHERE `entry`=49786;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12526 WHERE `entry`=49791;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12527 WHERE `entry`=49793;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12528 WHERE `entry`=49806;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12529 WHERE `entry`=49808;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12053 WHERE `entry`=49895;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12049 WHERE `entry`=49896;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12052 WHERE `entry`=49900;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12536 WHERE `entry`=49939;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12537 WHERE `entry`=49940;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12539 WHERE `entry`=49942;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=7264 WHERE `entry`=49959;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=7263 WHERE `entry`=49961;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=7265 WHERE `entry`=49962;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=7357 WHERE `entry`=49964;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=7262 WHERE `entry`=49966;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4463 WHERE `entry`=49968;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11932 WHERE `entry`=49998;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=3644 WHERE `entry`=50001;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11933 WHERE `entry`=50002;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11912 WHERE `entry`=50006;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12050 WHERE `entry`=50015;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=7357 WHERE `entry`=50019;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=7262 WHERE `entry`=50020;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=7264 WHERE `entry`=50021;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4104 WHERE `entry`=50022;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=7260 WHERE `entry`=50023;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=7265 WHERE `entry`=50024;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=7263 WHERE `entry`=50025;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12543 WHERE `entry`=50027;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4602 WHERE `entry`=50028;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12233 WHERE `entry`=50029;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=3644 WHERE `entry`=50031;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12341 WHERE `entry`=50032;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=523 WHERE `entry`=50033;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11877 WHERE `entry`=50034;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11767 WHERE `entry`=50035;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12533 WHERE `entry`=50155;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12522 WHERE `entry`=50156;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12535 WHERE `entry`=50157;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12536 WHERE `entry`=50158;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12516 WHERE `entry`=50160;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12714 WHERE `entry`=50163;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12539 WHERE `entry`=50171;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=7357 WHERE `entry`=50174;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=11817 WHERE `entry`=50497;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12517 WHERE `entry`=50498;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12716 WHERE `entry`=50499;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12513 WHERE `entry`=50500;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12714 WHERE `entry`=50501;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12882 WHERE `entry`=50502;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12539 WHERE `entry`=50505;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12539 WHERE `entry`=50506;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12539 WHERE `entry`=50507;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=12606 WHERE `entry`=50690;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4103 WHERE `entry`=51637;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=4530 WHERE `entry`=51638;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4529 WHERE `entry`=51639;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=4528 WHERE `entry`=51640;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=7357 WHERE `entry`=52292;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12755 WHERE `entry`=52335;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12841 WHERE `entry`=53403;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12845 WHERE `entry`=53404;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12529 WHERE `entry`=53405;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=55001 WHERE `entry`=62321;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=55001 WHERE `entry`=62393;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=55001 WHERE `entry`=62419;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=55001 WHERE `entry`=62425;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=55001 WHERE `entry`=62445;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=55001 WHERE `entry`=62450;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=55001 WHERE `entry`=62462;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=55001 WHERE `entry`=62463;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=55001 WHERE `entry`=62464;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=55001 WHERE `entry`=63238;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=55001 WHERE `entry`=63258;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=55001 WHERE `entry`=63272;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=55001 WHERE `entry`=63285;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=55001 WHERE `entry`=63296;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=55001 WHERE `entry`=63310;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=55001 WHERE `entry`=63327;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=55001 WHERE `entry`=63331;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=55001 WHERE `entry`=63332;
UPDATE `creature_template` SET `npcflag`=51, `gossip_menu_id`=55001 WHERE `entry`=63335;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12529 WHERE `entry`=44393;
UPDATE `creature_template` SET `npcflag`=49, `gossip_menu_id`=12529 WHERE `entry`=44394;

-- Class trainer spell-template links
DELETE FROM `npc_trainer` WHERE (`entry`,`spell`) IN (
(328,-200008),(331,-200008),(354,-200007),(373,-200007),(376,-200005),(377,-200005),(459,-200009),(460,-200009),(461,-200009),(542,-200011),(837,-200005),(895,-200003),(897,-200003),(906,-200009),(912,-200001),(913,-200001),(914,-200001),(916,-200004),(917,-200004),(918,-200004),(926,-200002),(927,-200002),(928,-200002),(944,-200008),(985,-200001),(986,-200007),(987,-200003),(988,-200009),(1226,-200005),(1228,-200008),(1229,-200001),(1232,-200002),(1234,-200004),(1403,-200001),(1404,-200003),(1406,-200007),(1408,-200009),(1409,-200011),(1411,-200004),(1901,-200001),(2124,-200008),(2127,-200009),(2128,-200008),(2129,-200005),(2217,-200011),(2218,-200003),(2219,-200007),(3030,-200007),(3031,-200007),(3032,-200007),
(3033,-200011),(3034,-200011),(3036,-200011),(3038,-200003),(3039,-200003),(3040,-200003),(3041,-200001),(3042,-200001),(3043,-200001),(3044,-200005),(3045,-200005),(3046,-200005),(3047,-200008),(3048,-200008),(3049,-200008),(3059,-200001),(3060,-200011),(3061,-200003),(3062,-200007),(3064,-200011),(3065,-200003),(3066,-200007),(3153,-200001),(3154,-200003),(3155,-200004),(3156,-200009),(3157,-200007),(3169,-200001),(3170,-200004),(3171,-200003),(3172,-200009),(3173,-200007),(3324,-200009),(3325,-200009),(3326,-200009),(3327,-200004),(3328,-200004),(3344,-200007),(3352,-200003),(3353,-200001),(3354,-200001),(3401,-200004),(3403,-200007),(3406,-200003),(3407,-200003),(3408,-200001),(3593,-200001),(3594,-200004),(3595,-200005),(3596,-200003),
(3597,-200011),(3598,-200001),(3599,-200004),(3600,-200005),(3601,-200003),(3602,-200011),(3706,-200005),(3707,-200005),(3963,-200003),(4087,-200001),(4089,-200001),(4090,-200005),(4091,-200005),(4092,-200005),(4146,-200003),(4163,-200004),(4205,-200003),(4214,-200004),(4215,-200004),(4217,-200011),(4218,-200011),(4219,-200011),(4564,-200009),(4565,-200009),(4582,-200004),(4594,-200001),(4607,-200005),(4608,-200005),(4985,-200011),(4986,-200003),(4987,-200008),(4988,-200002),(4989,-200005),(4990,-200004),(4991,-200007),(4992,-200001),(4993,-200009),(5039,-200009),(5113,-200001),(5114,-200001),(5115,-200003),(5116,-200003),(5117,-200003),(5141,-200005),(5142,-200005),(5143,-200005),(5144,-200008),(5145,-200008),(5148,-200002),(5149,-200002),
(5166,-200004),(5171,-200009),(5479,-200001),(5480,-200001),(5484,-200005),(5489,-200005),(5491,-200002),(5492,-200002),(5495,-200009),(5496,-200009),(5497,-200008),(5498,-200008),(5501,-200003),(5504,-200011),(5505,-200011),(5506,-200011),(5515,-200003),(5516,-200003),(5517,-200003),(5612,-200009),(5750,-200003),(5815,-200003),(5880,-200008),(5882,-200008),(5883,-200008),(5884,-200008),(5885,-200008),(5958,-200008),(5959,-200001),(5960,-200004),(5961,-200008),(5962,-200009),(5963,-200011),(5964,-200005),(5965,-200007),(5966,-200001),(5967,-200001),(5968,-200004),(5969,-200008),(5970,-200009),(5971,-200007),(5972,-200011),(5973,-200005),(5994,-200005),(6014,-200005),(6018,-200005),(6027,-200003),(6328,-200003),(6373,-200003),(6374,-200003),
(6707,-200004),(7311,-200008),(7312,-200008),(7315,-200001),(7488,-200008),(8140,-200002),(8141,-200001),(8142,-200011),(8308,-200003),(8664,-200002),(9465,-200011),(10291,-200001),(10930,-200003),(11397,-200005),(11401,-200005),(11406,-200005),(12042,-200011),(12776,-200003),(13283,-200004),(13417,-200007),(13476,-200011),(15279,-200008),(15280,-200002),(15283,-200009),(15284,-200005),(15285,-200004),(15513,-200003),(16266,-200009),(16269,-200008),(16270,-200003),(16275,-200002),(16276,-200005),(16279,-200004),(16499,-200003),(16500,-200008),(16501,-200002),(16502,-200005),(16503,-200001),(16646,-200009),(16647,-200009),(16648,-200009),(16649,-200003),(16651,-200008),(16652,-200008),(16653,-200008),(16655,-200011),(16658,-200005),(16659,-200005),(16660,-200005),(16672,-200003),
(16673,-200003),(16674,-200003),(16679,-200002),(16680,-200002),(16681,-200002),(16684,-200004),(16685,-200004),(16686,-200004),(16721,-200011),(16738,-200003),(16749,-200008),(16756,-200005),(16759,-200004),(16761,-200002),(16770,-200009),(16771,-200001),(17089,-200007),(17105,-200008),(17110,-200003),(17120,-200001),(17121,-200002),(17122,-200003),(17204,-200007),(17212,-200007),(17219,-200007),(17480,-200001),(17481,-200008),(17482,-200005),(17483,-200002),(17504,-200001),(17505,-200003),(17509,-200002),(17510,-200005),(17511,-200005),(17513,-200008),(17514,-200008),(17519,-200007),(17520,-200007),(17844,-200002),(18270,-200007),(19247,-200005),(20406,-200002),(20407,-200007),(23103,-200008),(23127,-200007),(23128,-200002),(23534,-200009),(23535,-200009),(26324,-200011),(26325,-200003),
(26326,-200008),(26327,-200002),(26328,-200005),(26329,-200004),(26330,-200007),(26331,-200009),(26332,-200001),(27704,-200008),(27916,-200006),(28471,-200006),(28472,-200006),(28474,-200006),(28956,-200008),(28958,-200008),(29156,-200008),(29194,-200006),(29195,-200006),(29196,-200006),(31084,-200006),(34600,-200003),(34673,-200003),(34689,-200008),(34692,-200005),(34693,-200004),(34695,-200007),(34696,-200009),(34697,-200001),(35281,-200002),(35758,-200003),(35778,-200009),(35780,-200008),(35786,-200007),(35805,-200005),(35806,-200004),(35807,-200001),(35839,-200001),(35869,-200009),(35870,-200005),(35871,-200004),(35872,-200008),(35873,-200011),(35874,-200003),(36518,-200003),(36519,-200009),(36520,-200008),(36521,-200007),(36523,-200005),(36524,-200004),(36525,-200001),(36628,-200011),
(36629,-200003),(36631,-200008),(36632,-200005),(36651,-200001),(36664,-200003),(36699,-200001),(36700,-200007),(36702,-200002),(36703,-200011),(36704,-200005),(36705,-200003),(36857,-200001),(37115,-200007),(37121,-200008),(37724,-200005),(37737,-200002),(38122,-200003),(38242,-200007),(38244,-200004),(38245,-200005),(38246,-200008),(38247,-200003),(38465,-200008),(38513,-200009),(38514,-200008),(38515,-200007),(38516,-200005),(38517,-200004),(38518,-200001),(38793,-200001),(38794,-200008),(38795,-200005),(38796,-200004),(38797,-200009),(38798,-200003),(38799,-200011),(39206,-200008),(39214,-200003),(42323,-200005),(42331,-200008),(42366,-200004),(42618,-200009),(43001,-200002),(43004,-200005),(43005,-200008),(43006,-200008),(43008,-200003),(43011,-200003),(43012,-200001),(43013,-200002),
(43015,-200005),(43277,-200003),(43278,-200003),(43455,-200009),(43464,-200007),(43795,-200002),(43796,-200005),(43870,-200005),(43881,-200009),(43883,-200009),(43892,-200009),(43909,-200003),(44128,-200001),(44129,-200001),(44247,-200003),(44249,-200003),(44250,-200003),(44380,-200011),(44393,-200007),(44394,-200007),(44395,-200011),(44396,-200011),(44455,-200001),(44459,-200011),(44461,-200003),(44464,-200004),(44465,-200008),(44468,-200005),(44469,-200009),(44723,-200001),(44725,-200002),(44726,-200011),(44735,-200005),(44740,-200007),(44743,-200003),(44978,-200011),(45019,-200001),(45023,-200003),(45029,-200007),(45095,-200004),(45137,-200005),(45138,-200009),(45306,-200005),(45337,-200005),(45339,-200005),(45347,-200005),(45709,-200001),(45713,-200003),(45714,-200008),(45717,-200004),
(45718,-200007),(45720,-200009),(46667,-200001),(46983,-200003),(47233,-200004),(47246,-200008),(47247,-200008),(47248,-200008),(47767,-200003),(47788,-200003),(48612,-200009),(48613,-200008),(48614,-200005),(48615,-200004),(48616,-200001),(48618,-200003),(49713,-200003),(49715,-200005),(49716,-200008),(49718,-200009),(49720,-200001),(49730,-200004),(49736,-200001),(49741,-200002),(49745,-200004),(49749,-200005),(49760,-200008),(49769,-200009),(49781,-200001),(49782,-200004),(49784,-200005),(49786,-200008),(49791,-200009),(49793,-200002),(49806,-200003),(49808,-200007),(49870,-200004),(49894,-200003),(49895,-200009),(49896,-200008),(49900,-200007),(49901,-200005),(49902,-200001),(49909,-200004),(49915,-200011),(49916,-200002),(49923,-200001),(49927,-200003),(49939,-200004),(49940,-200005),
(49942,-200011),(49945,-200009),(49946,-200003),(49949,-200004),(49950,-200005),(49952,-200008),(49954,-200002),(49955,-200001),(49957,-200002),(49958,-200003),(49959,-200008),(49961,-200001),(49962,-200005),(49963,-200009),(49964,-200007),(49966,-200003),(49968,-200008),(49997,-200011),(49998,-200009),(50001,-200005),(50002,-200008),(50004,-200003),(50006,-200001),(50007,-200003),(50010,-200008),(50011,-200007),(50012,-200002),(50013,-200005),(50015,-200004),(50016,-200004),(50017,-200009),(50018,-200001),(50019,-200007),(50020,-200003),(50021,-200008),(50022,-200007),(50023,-200002),(50024,-200005),(50025,-200001),(50027,-200004),(50028,-200009),(50029,-200008),(50031,-200005),(50032,-200003),(50033,-200001),(50034,-200011),(50035,-200002),(50127,-200004),(50128,-200009),(50130,-200007),
(50136,-200011),(50137,-200011),(50142,-200009),(50144,-200003),(50147,-200004),(50148,-200005),(50149,-200001),(50150,-200002),(50151,-200008),(50152,-200011),(50153,-200007),(50155,-200001),(50156,-200009),(50157,-200003),(50158,-200004),(50160,-200002),(50163,-200005),(50166,-200008),(50171,-200011),(50174,-200007),(50492,-200011),(50497,-200003),(50498,-200004),(50499,-200008),(50500,-200001),(50501,-200005),(50502,-200009),(50505,-200011),(50506,-200011),(50507,-200011),(50609,-200003),(50690,-200008),(50714,-200008),(50715,-200008),(50716,-200008),(50717,-200008),(50720,-200008),(50723,-200009),(50729,-200009),(50732,-200009),(51637,-200007),(51638,-200007),(51639,-200007),(51640,-200007),(52290,-200007),(52292,-200007),(52299,-200007),(52317,-200007),(52318,-200011),(52335,-200011),
(53403,-200005),(53404,-200009),(53405,-200007));
INSERT INTO `npc_trainer` (`entry`,`spell`,`spellcost`,`reqskill`,`reqskillvalue`,`reqlevel`) VALUES
(328,-200008,0,0,0,0),(331,-200008,0,0,0,0),(354,-200007,0,0,0,0),(373,-200007,0,0,0,0),(376,-200005,0,0,0,0),(377,-200005,0,0,0,0),(459,-200009,0,0,0,0),(460,-200009,0,0,0,0),(461,-200009,0,0,0,0),(542,-200011,0,0,0,0),(837,-200005,0,0,0,0),(895,-200003,0,0,0,0),(897,-200003,0,0,0,0),(906,-200009,0,0,0,0),(912,-200001,0,0,0,0),(913,-200001,0,0,0,0),(914,-200001,0,0,0,0),(916,-200004,0,0,0,0),(917,-200004,0,0,0,0),(918,-200004,0,0,0,0),(926,-200002,0,0,0,0),(927,-200002,0,0,0,0),(928,-200002,0,0,0,0),(944,-200008,0,0,0,0),(985,-200001,0,0,0,0),(986,-200007,0,0,0,0),(987,-200003,0,0,0,0),(988,-200009,0,0,0,0),(1226,-200005,0,0,0,0),(1228,-200008,0,0,0,0),(1229,-200001,0,0,0,0),(1232,-200002,0,0,0,0),(1234,-200004,0,0,0,0),(1403,-200001,0,0,0,0),(1404,-200003,0,0,0,0),(1406,-200007,0,0,0,0),(1408,-200009,0,0,0,0),(1409,-200011,0,0,0,0),(1411,-200004,0,0,0,0),(1901,-200001,0,0,0,0),
(2124,-200008,0,0,0,0),(2127,-200009,0,0,0,0),(2128,-200008,0,0,0,0),(2129,-200005,0,0,0,0),(2217,-200011,0,0,0,0),(2218,-200003,0,0,0,0),(2219,-200007,0,0,0,0),(3030,-200007,0,0,0,0),(3031,-200007,0,0,0,0),(3032,-200007,0,0,0,0),(3033,-200011,0,0,0,0),(3034,-200011,0,0,0,0),(3036,-200011,0,0,0,0),(3038,-200003,0,0,0,0),(3039,-200003,0,0,0,0),(3040,-200003,0,0,0,0),(3041,-200001,0,0,0,0),(3042,-200001,0,0,0,0),(3043,-200001,0,0,0,0),(3044,-200005,0,0,0,0),(3045,-200005,0,0,0,0),(3046,-200005,0,0,0,0),(3047,-200008,0,0,0,0),(3048,-200008,0,0,0,0),(3049,-200008,0,0,0,0),(3059,-200001,0,0,0,0),(3060,-200011,0,0,0,0),(3061,-200003,0,0,0,0),(3062,-200007,0,0,0,0),(3064,-200011,0,0,0,0),(3065,-200003,0,0,0,0),(3066,-200007,0,0,0,0),(3153,-200001,0,0,0,0),(3154,-200003,0,0,0,0),(3155,-200004,0,0,0,0),(3156,-200009,0,0,0,0),(3157,-200007,0,0,0,0),(3169,-200001,0,0,0,0),(3170,-200004,0,0,0,0),(3171,-200003,0,0,0,0),
(3172,-200009,0,0,0,0),(3173,-200007,0,0,0,0),(3324,-200009,0,0,0,0),(3325,-200009,0,0,0,0),(3326,-200009,0,0,0,0),(3327,-200004,0,0,0,0),(3328,-200004,0,0,0,0),(3344,-200007,0,0,0,0),(3352,-200003,0,0,0,0),(3353,-200001,0,0,0,0),(3354,-200001,0,0,0,0),(3401,-200004,0,0,0,0),(3403,-200007,0,0,0,0),(3406,-200003,0,0,0,0),(3407,-200003,0,0,0,0),(3408,-200001,0,0,0,0),(3593,-200001,0,0,0,0),(3594,-200004,0,0,0,0),(3595,-200005,0,0,0,0),(3596,-200003,0,0,0,0),(3597,-200011,0,0,0,0),(3598,-200001,0,0,0,0),(3599,-200004,0,0,0,0),(3600,-200005,0,0,0,0),(3601,-200003,0,0,0,0),(3602,-200011,0,0,0,0),(3706,-200005,0,0,0,0),(3707,-200005,0,0,0,0),(3963,-200003,0,0,0,0),(4087,-200001,0,0,0,0),(4089,-200001,0,0,0,0),(4090,-200005,0,0,0,0),(4091,-200005,0,0,0,0),(4092,-200005,0,0,0,0),(4146,-200003,0,0,0,0),(4163,-200004,0,0,0,0),(4205,-200003,0,0,0,0),(4214,-200004,0,0,0,0),(4215,-200004,0,0,0,0),(4217,-200011,0,0,0,0),
(4218,-200011,0,0,0,0),(4219,-200011,0,0,0,0),(4564,-200009,0,0,0,0),(4565,-200009,0,0,0,0),(4582,-200004,0,0,0,0),(4594,-200001,0,0,0,0),(4607,-200005,0,0,0,0),(4608,-200005,0,0,0,0),(4985,-200011,0,0,0,0),(4986,-200003,0,0,0,0),(4987,-200008,0,0,0,0),(4988,-200002,0,0,0,0),(4989,-200005,0,0,0,0),(4990,-200004,0,0,0,0),(4991,-200007,0,0,0,0),(4992,-200001,0,0,0,0),(4993,-200009,0,0,0,0),(5039,-200009,0,0,0,0),(5113,-200001,0,0,0,0),(5114,-200001,0,0,0,0),(5115,-200003,0,0,0,0),(5116,-200003,0,0,0,0),(5117,-200003,0,0,0,0),(5141,-200005,0,0,0,0),(5142,-200005,0,0,0,0),(5143,-200005,0,0,0,0),(5144,-200008,0,0,0,0),(5145,-200008,0,0,0,0),(5148,-200002,0,0,0,0),(5149,-200002,0,0,0,0),(5166,-200004,0,0,0,0),(5171,-200009,0,0,0,0),(5479,-200001,0,0,0,0),(5480,-200001,0,0,0,0),(5484,-200005,0,0,0,0),(5489,-200005,0,0,0,0),(5491,-200002,0,0,0,0),(5492,-200002,0,0,0,0),(5495,-200009,0,0,0,0),(5496,-200009,0,0,0,0),
(5497,-200008,0,0,0,0),(5498,-200008,0,0,0,0),(5501,-200003,0,0,0,0),(5504,-200011,0,0,0,0),(5505,-200011,0,0,0,0),(5506,-200011,0,0,0,0),(5515,-200003,0,0,0,0),(5516,-200003,0,0,0,0),(5517,-200003,0,0,0,0),(5612,-200009,0,0,0,0),(5750,-200003,0,0,0,0),(5815,-200003,0,0,0,0),(5880,-200008,0,0,0,0),(5882,-200008,0,0,0,0),(5883,-200008,0,0,0,0),(5884,-200008,0,0,0,0),(5885,-200008,0,0,0,0),(5958,-200008,0,0,0,0),(5959,-200001,0,0,0,0),(5960,-200004,0,0,0,0),(5961,-200008,0,0,0,0),(5962,-200009,0,0,0,0),(5963,-200011,0,0,0,0),(5964,-200005,0,0,0,0),(5965,-200007,0,0,0,0),(5966,-200001,0,0,0,0),(5967,-200001,0,0,0,0),(5968,-200004,0,0,0,0),(5969,-200008,0,0,0,0),(5970,-200009,0,0,0,0),(5971,-200007,0,0,0,0),(5972,-200011,0,0,0,0),(5973,-200005,0,0,0,0),(5994,-200005,0,0,0,0),(6014,-200005,0,0,0,0),(6018,-200005,0,0,0,0),(6027,-200003,0,0,0,0),(6328,-200003,0,0,0,0),(6373,-200003,0,0,0,0),(6374,-200003,0,0,0,0),
(6707,-200004,0,0,0,0),(7311,-200008,0,0,0,0),(7312,-200008,0,0,0,0),(7315,-200001,0,0,0,0),(7488,-200008,0,0,0,0),(8140,-200002,0,0,0,0),(8141,-200001,0,0,0,0),(8142,-200011,0,0,0,0),(8308,-200003,0,0,0,0),(8664,-200002,0,0,0,0),(9465,-200011,0,0,0,0),(10291,-200001,0,0,0,0),(10930,-200003,0,0,0,0),(11397,-200005,0,0,0,0),(11401,-200005,0,0,0,0),(11406,-200005,0,0,0,0),(12042,-200011,0,0,0,0),(12776,-200003,0,0,0,0),(13283,-200004,0,0,0,0),(13417,-200007,0,0,0,0),(13476,-200011,0,0,0,0),(15279,-200008,0,0,0,0),(15280,-200002,0,0,0,0),(15283,-200009,0,0,0,0),(15284,-200005,0,0,0,0),(15285,-200004,0,0,0,0),(15513,-200003,0,0,0,0),(16266,-200009,0,0,0,0),(16269,-200008,0,0,0,0),(16270,-200003,0,0,0,0),(16275,-200002,0,0,0,0),(16276,-200005,0,0,0,0),(16279,-200004,0,0,0,0),(16499,-200003,0,0,0,0),(16500,-200008,0,0,0,0),(16501,-200002,0,0,0,0),(16502,-200005,0,0,0,0),(16503,-200001,0,0,0,0),(16646,-200009,0,0,0,0),(16647,-200009,0,0,0,0),
(16648,-200009,0,0,0,0),(16649,-200003,0,0,0,0),(16651,-200008,0,0,0,0),(16652,-200008,0,0,0,0),(16653,-200008,0,0,0,0),(16655,-200011,0,0,0,0),(16658,-200005,0,0,0,0),(16659,-200005,0,0,0,0),(16660,-200005,0,0,0,0),(16672,-200003,0,0,0,0),(16673,-200003,0,0,0,0),(16674,-200003,0,0,0,0),(16679,-200002,0,0,0,0),(16680,-200002,0,0,0,0),(16681,-200002,0,0,0,0),(16684,-200004,0,0,0,0),(16685,-200004,0,0,0,0),(16686,-200004,0,0,0,0),(16721,-200011,0,0,0,0),(16738,-200003,0,0,0,0),(16749,-200008,0,0,0,0),(16756,-200005,0,0,0,0),(16759,-200004,0,0,0,0),(16761,-200002,0,0,0,0),(16770,-200009,0,0,0,0),(16771,-200001,0,0,0,0),(17089,-200007,0,0,0,0),(17105,-200008,0,0,0,0),(17110,-200003,0,0,0,0),(17120,-200001,0,0,0,0),(17121,-200002,0,0,0,0),(17122,-200003,0,0,0,0),(17204,-200007,0,0,0,0),(17212,-200007,0,0,0,0),(17219,-200007,0,0,0,0),(17480,-200001,0,0,0,0),(17481,-200008,0,0,0,0),(17482,-200005,0,0,0,0),(17483,-200002,0,0,0,0),(17504,-200001,0,0,0,0),
(17505,-200003,0,0,0,0),(17509,-200002,0,0,0,0),(17510,-200005,0,0,0,0),(17511,-200005,0,0,0,0),(17513,-200008,0,0,0,0),(17514,-200008,0,0,0,0),(17519,-200007,0,0,0,0),(17520,-200007,0,0,0,0),(17844,-200002,0,0,0,0),(18270,-200007,0,0,0,0),(19247,-200005,0,0,0,0),(20406,-200002,0,0,0,0),(20407,-200007,0,0,0,0),(23103,-200008,0,0,0,0),(23127,-200007,0,0,0,0),(23128,-200002,0,0,0,0),(23534,-200009,0,0,0,0),(23535,-200009,0,0,0,0),(26324,-200011,0,0,0,0),(26325,-200003,0,0,0,0),(26326,-200008,0,0,0,0),(26327,-200002,0,0,0,0),(26328,-200005,0,0,0,0),(26329,-200004,0,0,0,0),(26330,-200007,0,0,0,0),(26331,-200009,0,0,0,0),(26332,-200001,0,0,0,0),(27704,-200008,0,0,0,0),(27916,-200006,0,0,0,0),(28471,-200006,0,0,0,0),(28472,-200006,0,0,0,0),(28474,-200006,0,0,0,0),(28956,-200008,0,0,0,0),(28958,-200008,0,0,0,0),(29156,-200008,0,0,0,0),(29194,-200006,0,0,0,0),(29195,-200006,0,0,0,0),(29196,-200006,0,0,0,0),(31084,-200006,0,0,0,0),(34600,-200003,0,0,0,0),
(34673,-200003,0,0,0,0),(34689,-200008,0,0,0,0),(34692,-200005,0,0,0,0),(34693,-200004,0,0,0,0),(34695,-200007,0,0,0,0),(34696,-200009,0,0,0,0),(34697,-200001,0,0,0,0),(35281,-200002,0,0,0,0),(35758,-200003,0,0,0,0),(35778,-200009,0,0,0,0),(35780,-200008,0,0,0,0),(35786,-200007,0,0,0,0),(35805,-200005,0,0,0,0),(35806,-200004,0,0,0,0),(35807,-200001,0,0,0,0),(35839,-200001,0,0,0,0),(35869,-200009,0,0,0,0),(35870,-200005,0,0,0,0),(35871,-200004,0,0,0,0),(35872,-200008,0,0,0,0),(35873,-200011,0,0,0,0),(35874,-200003,0,0,0,0),(36518,-200003,0,0,0,0),(36519,-200009,0,0,0,0),(36520,-200008,0,0,0,0),(36521,-200007,0,0,0,0),(36523,-200005,0,0,0,0),(36524,-200004,0,0,0,0),(36525,-200001,0,0,0,0),(36628,-200011,0,0,0,0),(36629,-200003,0,0,0,0),(36631,-200008,0,0,0,0),(36632,-200005,0,0,0,0),(36651,-200001,0,0,0,0),(36664,-200003,0,0,0,0),(36699,-200001,0,0,0,0),(36700,-200007,0,0,0,0),(36702,-200002,0,0,0,0),(36703,-200011,0,0,0,0),(36704,-200005,0,0,0,0),
(36705,-200003,0,0,0,0),(36857,-200001,0,0,0,0),(37115,-200007,0,0,0,0),(37121,-200008,0,0,0,0),(37724,-200005,0,0,0,0),(37737,-200002,0,0,0,0),(38122,-200003,0,0,0,0),(38242,-200007,0,0,0,0),(38244,-200004,0,0,0,0),(38245,-200005,0,0,0,0),(38246,-200008,0,0,0,0),(38247,-200003,0,0,0,0),(38465,-200008,0,0,0,0),(38513,-200009,0,0,0,0),(38514,-200008,0,0,0,0),(38515,-200007,0,0,0,0),(38516,-200005,0,0,0,0),(38517,-200004,0,0,0,0),(38518,-200001,0,0,0,0),(38793,-200001,0,0,0,0),(38794,-200008,0,0,0,0),(38795,-200005,0,0,0,0),(38796,-200004,0,0,0,0),(38797,-200009,0,0,0,0),(38798,-200003,0,0,0,0),(38799,-200011,0,0,0,0),(39206,-200008,0,0,0,0),(39214,-200003,0,0,0,0),(42323,-200005,0,0,0,0),(42331,-200008,0,0,0,0),(42366,-200004,0,0,0,0),(42618,-200009,0,0,0,0),(43001,-200002,0,0,0,0),(43004,-200005,0,0,0,0),(43005,-200008,0,0,0,0),(43006,-200008,0,0,0,0),(43008,-200003,0,0,0,0),(43011,-200003,0,0,0,0),(43012,-200001,0,0,0,0),(43013,-200002,0,0,0,0),
(43015,-200005,0,0,0,0),(43277,-200003,0,0,0,0),(43278,-200003,0,0,0,0),(43455,-200009,0,0,0,0),(43464,-200007,0,0,0,0),(43795,-200002,0,0,0,0),(43796,-200005,0,0,0,0),(43870,-200005,0,0,0,0),(43881,-200009,0,0,0,0),(43883,-200009,0,0,0,0),(43892,-200009,0,0,0,0),(43909,-200003,0,0,0,0),(44128,-200001,0,0,0,0),(44129,-200001,0,0,0,0),(44247,-200003,0,0,0,0),(44249,-200003,0,0,0,0),(44250,-200003,0,0,0,0),(44380,-200011,0,0,0,0),(44393,-200007,0,0,0,0),(44394,-200007,0,0,0,0),(44395,-200011,0,0,0,0),(44396,-200011,0,0,0,0),(44455,-200001,0,0,0,0),(44459,-200011,0,0,0,0),(44461,-200003,0,0,0,0),(44464,-200004,0,0,0,0),(44465,-200008,0,0,0,0),(44468,-200005,0,0,0,0),(44469,-200009,0,0,0,0),(44723,-200001,0,0,0,0),(44725,-200002,0,0,0,0),(44726,-200011,0,0,0,0),(44735,-200005,0,0,0,0),(44740,-200007,0,0,0,0),(44743,-200003,0,0,0,0),(44978,-200011,0,0,0,0),(45019,-200001,0,0,0,0),(45023,-200003,0,0,0,0),(45029,-200007,0,0,0,0),(45095,-200004,0,0,0,0),
(45137,-200005,0,0,0,0),(45138,-200009,0,0,0,0),(45306,-200005,0,0,0,0),(45337,-200005,0,0,0,0),(45339,-200005,0,0,0,0),(45347,-200005,0,0,0,0),(45709,-200001,0,0,0,0),(45713,-200003,0,0,0,0),(45714,-200008,0,0,0,0),(45717,-200004,0,0,0,0),(45718,-200007,0,0,0,0),(45720,-200009,0,0,0,0),(46667,-200001,0,0,0,0),(46983,-200003,0,0,0,0),(47233,-200004,0,0,0,0),(47246,-200008,0,0,0,0),(47247,-200008,0,0,0,0),(47248,-200008,0,0,0,0),(47767,-200003,0,0,0,0),(47788,-200003,0,0,0,0),(48612,-200009,0,0,0,0),(48613,-200008,0,0,0,0),(48614,-200005,0,0,0,0),(48615,-200004,0,0,0,0),(48616,-200001,0,0,0,0),(48618,-200003,0,0,0,0),(49713,-200003,0,0,0,0),(49715,-200005,0,0,0,0),(49716,-200008,0,0,0,0),(49718,-200009,0,0,0,0),(49720,-200001,0,0,0,0),(49730,-200004,0,0,0,0),(49736,-200001,0,0,0,0),(49741,-200002,0,0,0,0),(49745,-200004,0,0,0,0),(49749,-200005,0,0,0,0),(49760,-200008,0,0,0,0),(49769,-200009,0,0,0,0),(49781,-200001,0,0,0,0),(49782,-200004,0,0,0,0),
(49784,-200005,0,0,0,0),(49786,-200008,0,0,0,0),(49791,-200009,0,0,0,0),(49793,-200002,0,0,0,0),(49806,-200003,0,0,0,0),(49808,-200007,0,0,0,0),(49870,-200004,0,0,0,0),(49894,-200003,0,0,0,0),(49895,-200009,0,0,0,0),(49896,-200008,0,0,0,0),(49900,-200007,0,0,0,0),(49901,-200005,0,0,0,0),(49902,-200001,0,0,0,0),(49909,-200004,0,0,0,0),(49915,-200011,0,0,0,0),(49916,-200002,0,0,0,0),(49923,-200001,0,0,0,0),(49927,-200003,0,0,0,0),(49939,-200004,0,0,0,0),(49940,-200005,0,0,0,0),(49942,-200011,0,0,0,0),(49945,-200009,0,0,0,0),(49946,-200003,0,0,0,0),(49949,-200004,0,0,0,0),(49950,-200005,0,0,0,0),(49952,-200008,0,0,0,0),(49954,-200002,0,0,0,0),(49955,-200001,0,0,0,0),(49957,-200002,0,0,0,0),(49958,-200003,0,0,0,0),(49959,-200008,0,0,0,0),(49961,-200001,0,0,0,0),(49962,-200005,0,0,0,0),(49963,-200009,0,0,0,0),(49964,-200007,0,0,0,0),(49966,-200003,0,0,0,0),(49968,-200008,0,0,0,0),(49997,-200011,0,0,0,0),(49998,-200009,0,0,0,0),(50001,-200005,0,0,0,0),
(50002,-200008,0,0,0,0),(50004,-200003,0,0,0,0),(50006,-200001,0,0,0,0),(50007,-200003,0,0,0,0),(50010,-200008,0,0,0,0),(50011,-200007,0,0,0,0),(50012,-200002,0,0,0,0),(50013,-200005,0,0,0,0),(50015,-200004,0,0,0,0),(50016,-200004,0,0,0,0),(50017,-200009,0,0,0,0),(50018,-200001,0,0,0,0),(50019,-200007,0,0,0,0),(50020,-200003,0,0,0,0),(50021,-200008,0,0,0,0),(50022,-200007,0,0,0,0),(50023,-200002,0,0,0,0),(50024,-200005,0,0,0,0),(50025,-200001,0,0,0,0),(50027,-200004,0,0,0,0),(50028,-200009,0,0,0,0),(50029,-200008,0,0,0,0),(50031,-200005,0,0,0,0),(50032,-200003,0,0,0,0),(50033,-200001,0,0,0,0),(50034,-200011,0,0,0,0),(50035,-200002,0,0,0,0),(50127,-200004,0,0,0,0),(50128,-200009,0,0,0,0),(50130,-200007,0,0,0,0),(50136,-200011,0,0,0,0),(50137,-200011,0,0,0,0),(50142,-200009,0,0,0,0),(50144,-200003,0,0,0,0),(50147,-200004,0,0,0,0),(50148,-200005,0,0,0,0),(50149,-200001,0,0,0,0),(50150,-200002,0,0,0,0),(50151,-200008,0,0,0,0),(50152,-200011,0,0,0,0),
(50153,-200007,0,0,0,0),(50155,-200001,0,0,0,0),(50156,-200009,0,0,0,0),(50157,-200003,0,0,0,0),(50158,-200004,0,0,0,0),(50160,-200002,0,0,0,0),(50163,-200005,0,0,0,0),(50166,-200008,0,0,0,0),(50171,-200011,0,0,0,0),(50174,-200007,0,0,0,0),(50492,-200011,0,0,0,0),(50497,-200003,0,0,0,0),(50498,-200004,0,0,0,0),(50499,-200008,0,0,0,0),(50500,-200001,0,0,0,0),(50501,-200005,0,0,0,0),(50502,-200009,0,0,0,0),(50505,-200011,0,0,0,0),(50506,-200011,0,0,0,0),(50507,-200011,0,0,0,0),(50609,-200003,0,0,0,0),(50690,-200008,0,0,0,0),(50714,-200008,0,0,0,0),(50715,-200008,0,0,0,0),(50716,-200008,0,0,0,0),(50717,-200008,0,0,0,0),(50720,-200008,0,0,0,0),(50723,-200009,0,0,0,0),(50729,-200009,0,0,0,0),(50732,-200009,0,0,0,0),(51637,-200007,0,0,0,0),(51638,-200007,0,0,0,0),(51639,-200007,0,0,0,0),(51640,-200007,0,0,0,0),(52290,-200007,0,0,0,0),(52292,-200007,0,0,0,0),(52299,-200007,0,0,0,0),(52317,-200007,0,0,0,0),(52318,-200011,0,0,0,0),(52335,-200011,0,0,0,0),
(53403,-200005,0,0,0,0),(53404,-200009,0,0,0,0),(53405,-200007,0,0,0,0);
