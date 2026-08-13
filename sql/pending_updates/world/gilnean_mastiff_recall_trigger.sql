DELETE FROM creature_template WHERE entry=900000;
INSERT INTO creature_template
SELECT 900000, difficulty_entry_1, difficulty_entry_2, difficulty_entry_3, KillCredit1, KillCredit2,
       modelid1, modelid2, modelid3, modelid4, 'Gilnean Mastiff Recall Trigger', '', IconName, gossip_menu_id,
       minlevel, maxlevel, exp, exp_unk, faction_A, faction_H, npcflag, speed_walk, speed_run, scale, npc_rank, mindmg, maxdmg,
       dmgschool, attackpower, dmg_multiplier, baseattacktime, rangeattacktime, unit_class, unit_flags, unit_flags2,
       dynamicflags, family, trainer_type, trainer_class, trainer_race, minrangedmg, maxrangedmg,
       rangedattackpower, type, type_flags, type_flags2, lootid, pickpocketloot, skinloot, resistance1, resistance2,
       resistance3, resistance4, resistance5, resistance6, spell1, spell2, spell3, spell4, spell5, spell6, spell7,
       spell8, PetSpellDataId, VehicleId, mingold, maxgold, AIName, MovementType, InhabitType, HoverHeight, Health_mod,
       Mana_mod, Mana_mod_extra, Armor_mod, RacialLeader, questItem1, questItem2, questItem3, questItem4, questItem5, questItem6,
       movementId, RegenHealth, mechanic_immune_mask, flags_extra, 'npc_gilnean_mastiff_recall_trigger',
       ModLevel, WDBVerified
FROM creature_template WHERE entry=35374;

DELETE FROM creature WHERE id=900000;
INSERT INTO creature (id, map, spawnMask, phaseId, phaseGroup, modelid, equipment_id, position_x, position_y, position_z, orientation, spawntimesecs, spawndist, currentwaypoint, curhealth, curmana, MovementType, npcflag, unit_flags, dynamicflags)
VALUES (900000, 654, 1, 171, 0, 0, 0, -1790.120361, 1438.133423, 20.060425, 0, 300, 0, 0, 1, 0, 0, 0, 0, 0);
