-- Correct melee damage on combat-eligible NPCs that carried placeholder
-- mindmg/maxdmg values (0/0, 1/1, 2/2, or an inverted range).
-- Damage comes from the creature_classlevelstats MoP curve, which already
-- holds the weapon damage average, and attack power comes from the same
-- curve. Health and mana modifiers keep their authored values.
-- Re-runnable: each row is set to its final value regardless of current state.

-- 2092 templates.

UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=6; -- Kobold Vermin
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=8, `dmg_multiplier`=1.5, `Health_mod`=1.5, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=62; -- Gug Fatcandle
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=69; -- Diseased Timber Wolf
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=106; -- Kodo Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=284; -- Brown Horse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=299; -- Young Wolf
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=305; -- White Stallion
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=306; -- Palomino
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=307; -- Pinto
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=308; -- Black Stallion
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=395; -- Markus
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=704; -- Ragged Timber Wolf
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=705; -- Ragged Young Wolf
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=794; -- Matt
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=795; -- Mark
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=796; -- Joshua
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=797; -- Bo
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=798; -- Solomon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=799; -- Kevin
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=800; -- Kyle
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=801; -- Eric
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=802; -- Jay
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=804; -- Dana
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=805; -- Cameron
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=806; -- John
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=807; -- Lisa
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=810; -- Aaron
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=811; -- Jose
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=1366; -- Adam
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=1367; -- Billy
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=1368; -- Justin
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=1370; -- Brandon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=1371; -- Roman
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=1402; -- Topper McNabb
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=1405; -- Morris Lawry
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=1445; -- Jesse Halloran
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=1446; -- Regina Halloran
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=1447; -- Gimlok Rumdnul
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=1501; -- Mindless Zombie
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=1502; -- Wretched Ghoul
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=1508; -- Young Scavenger
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=1512; -- Duskbat
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=1984; -- Young Thistle Boar
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=2032; -- Mangy Nightsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=2955; -- Plainstrider
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=3504; -- Gil
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=4269; -- Chestnut Mare
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=4779; -- Brown Ram
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=4781; -- Snufflenose Gopher
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=5403; -- Riding White Stallion
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=5404; -- Black Stallion
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=5405; -- Pinto
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=5406; -- Palomino
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=5607; -- Roger
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=5608; -- Jamin
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=5629; -- Theramore Commando
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=5774; -- Riding Wolf
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=5917; -- Clara Charles
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=6728; -- Narnie
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=7013; -- Blackrock Guard
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=7690; -- Striped Nightsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=8881; -- Riding Ram
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=8882; -- Riding Tiger
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=8883; -- Riding Horse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=8884; -- Skeletal Mount
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=8885; -- Riding Raptor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=10116; -- Slave
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=10117; -- Tortured Slave
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=11122; -- Restless Soul
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=11136; -- Freed Soul
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=11156; -- Green Skeletal Warhorse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=11260; -- Northshire Peasant
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=11996; -- Ashley Bridenbecker
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12150; -- Riding Kodo (Purple)
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12341; -- Blue Skeletal Horse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12342; -- Brown Skeletal Horse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12343; -- Red Skeletal Horse
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12346; -- Emerald Riding Raptor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12349; -- Turquoise Riding Raptor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12350; -- Violet Riding Raptor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12358; -- Riding Striped Frostsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12359; -- Riding Spotted Frostsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12360; -- Riding Striped Nightsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12363; -- Blue Mechanostrider
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12365; -- Red Mechanostrider
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12366; -- Unpainted Mechanostrider X
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12367; -- Green Mechanostrider
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12372; -- Brown Ram
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12373; -- Gray Ram
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12374; -- White Riding Ram Mount
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12375; -- Chestnut Mare
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=12376; -- Brown Horse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14546; -- Swift Brown Ram
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14547; -- Swift White Ram
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14548; -- Swift Gray Ram
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14551; -- Swift Yellow Mechanostrider
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14552; -- Swift White Mechanostrider
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14553; -- Swift Green Mechanostrider
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14555; -- Swift Mistsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14556; -- Swift Frostsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14558; -- Purple Skeletal Warhorse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14559; -- Swift Palomino
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14560; -- Swift White Steed
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14561; -- Swift Brown Steed
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14602; -- Swift Stormsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14850; -- Gruk
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14857; -- Erk
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14869; -- Pygmy Cockatrice
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14872; -- Trok
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14873; -- Okla
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14874; -- Karu
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=14908; -- Mogg
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=15366; -- Springpaw Cub
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=16520; -- Vale Moth
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=17048; -- Ironforge Firebreather
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=17049; -- Darnassus Firebreather
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=17077; -- Ancestral Spirit Wolf
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=17230; -- Twinkle
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=17250; -- Jia
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=17251; -- Blackbeak
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=17989; -- Archimindy
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=18150; -- Gurgle
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=18151; -- Gargle
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=18360; -- Golden Gryphon
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=18362; -- Swift Purple Gryphon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=18649; -- Innkeeper Monica
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=18655; -- Jay Lemieux
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=18656; -- Julie Honeywell
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=18739; -- Giraffe
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=18762; -- Tinkerbell
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=19163; -- Refugee Kid
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=19164; -- Refugee Child
UPDATE `creature_template` SET `mindmg`=143, `maxdmg`=245, `attackpower`=276, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=19579; -- Cohlien Frostweaver with Hat
UPDATE `creature_template` SET `mindmg`=143, `maxdmg`=245, `attackpower`=276, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=19580; -- Luminrath with Cape
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=19659; -- Great Elite Elekk
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=20098; -- Jane
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=20244; -- Nova
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=20432; -- Beggar
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=20434; -- Horse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=20716; -- Rek'tor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=20894; -- Miihi
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=20895; -- Miiji
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=21317; -- Aimi
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=21376; -- Crusty Bob
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=22267; -- Magic Sucker Device Buttress
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=22314; -- Captive Child
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=22333; -- Orb Collecting Totem
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=22921; -- Ethereum Prisoner (Stasis Chamber Alpha)
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=22974; -- Invis Sparrowhawk Origin
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=22985; -- Bradford Bridenbecker
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=23487; -- Wild Wolpertinger
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=24136; -- Feather Vortex
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=24464; -- Scourging Crystal
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=24515; -- Spotted Hippogryph Hatchling
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=24653; -- Flying Machine
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=24767; -- Molly
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=25265; -- Demonic Vapor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=25267; -- Demonic Vapor (Trail)
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=25305; -- Dancing Flames
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=1, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=25652; -- Nerub'ar Scarab
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=25773; -- Fizzcrank Survivor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=26096; -- Gnome Soul
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=26365; -- Taunka Orphan
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=26902; -- Essence of Warlord Jin'arrak
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=27435; -- Wintergarde Mine Bomb
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=27944; -- Johan
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=28526; -- Released Offspring of Har'koa
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=28821; -- Mine Car
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=29139; -- Naznik Sureshave
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=29141; -- Pella Brassbrush
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=29151; -- Cricket
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=29630; -- Fanged Pit Viper
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=1, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=29939; -- Storm Cloud
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=30835; -- Image of a Shadow Cultist
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=30890; -- Twilight Whelp
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=30891; -- Blessed Banner of the Crusade
UPDATE `creature_template` SET `mindmg`=212, `maxdmg`=409, `attackpower`=528, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=30900; -- Argent Mason
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=31214; -- Sartharion Twilight Whelp
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=31254; -- Lordaeron Footsoldier
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=31276; -- Ghoulish Minion
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=31690; -- Infra-Green Flight Master
UPDATE `creature_template` SET `mindmg`=254, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=31693; -- Stormforged Saboteur
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=31797; -- Ancient Sentinel
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=32414; -- Lordaeron Captain
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=22, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=32855; -- Shatterspear Priestess
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=32858; -- Shatterspear Champion
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=32859; -- Horde Enforcer
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=32861; -- Shatterspear Laborer
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=32863; -- Shatterspear Overseer
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=32888; -- Twilight Fanatic
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=32890; -- Enraged Tidal Spirit
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=46, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=32899; -- High Cultist Azerynn
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=32928; -- Vile Spray
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=46, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=32970; -- Rit'ko
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=32974; -- Laird
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=32975; -- Decomposing Thistle Bear
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=32988; -- Cloudtamer Wildmane
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=32989; -- Windmaster Tzu-Tzu
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=32990; -- Skylord Braax
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=32996; -- Sharax the Defiler
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=32997; -- Fleetfoot
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=32999; -- Unbound Fire Elemental
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33001; -- Thundris Windweaver
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33024; -- Uncorrupted Thistle Bear
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33033; -- Sentinel Elissa Starbreeze
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33035; -- Taldan
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=33043; -- Maddened Blackwood
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=33044; -- Corrupted Blackwood
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33053; -- Grimclaw
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=52, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=33057; -- Twilight Zealot
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33071; -- Shatterspear Raider
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33079; -- Darkscale Myrmidon
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=58, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=33084; -- Blackwood Furbolg
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33106; -- Auberdine Refugee
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33127; -- Moonstalker
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=33179; -- Cursed Highborne
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33180; -- Writhing Highborne
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33181; -- Anaya Dawnrunner
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33183; -- Bathran's Corpse
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=33193; -- Ashenvale Skirmisher
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=33195; -- Ashenvale Bowman
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33206; -- Darkscale Scout
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=18, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=33207; -- Lady Janira
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=30, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=33208; -- Orendil's Apprentice
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=46, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=33262; -- Greymist Tidehunter
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33277; -- Greymist Warrior
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33278; -- Maestra's Post Sentinel
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33311; -- Darkshore Stag
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33334; -- Horde Invader
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=33345; -- Luk'gar
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=33347; -- Delgren the Purifier
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33348; -- Feero Ironhand
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=34, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=33356; -- Orux Thrice-Damned
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33375; -- Injured Sentinel
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=33389; -- Thistlefur Wise One
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33390; -- Thistlefur Warrior
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33451; -- Ashenvale Nightsaber
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33512; -- Palanaar's Elekk
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33728; -- Draaka
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33766; -- Splintertree Peon
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33825; -- Splintertree Overseer
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=33978; -- Hungry Thistle Bear
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=33981; -- Greef
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34046; -- Forsaken Looter
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=34056; -- Vigilant Protector
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34103; -- Keynira Owlwing
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34177; -- Ashenvale Scout
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34208; -- Protector Endolar
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34232; -- Envoy Sheelah
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34238; -- Black Skeletal Horse
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34248; -- Shatterspear Mystic
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34285; -- Trapped Wolf
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34294; -- Protector Dorinar
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34302; -- Consumed Thistle Bear
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=52, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34304; -- Wildkin Spirit
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34309; -- Shatterspear Raider
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34315; -- Marauding Poacher
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34318; -- Whitetail Stag
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34326; -- Doomspeaker Trevellion
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34339; -- Greymist Refugee
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34345; -- Oil Canister
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34350; -- Dangerfish
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34351; -- Fiendishfish
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34366; -- Warsong Vanguard
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34396; -- Whitetail Doe
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34405; -- Twilight Worker
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34406; -- Foreman Balsoth
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34413; -- Faceless One
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34414; -- Darkscale Siren
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34415; -- Darkscale Priestess
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34419; -- Astranaar Skirmisher
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34420; -- Astranaar Officer
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34427; -- Thalya the Voidcaller
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=78, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=5, `Mana_mod_extra`=1 WHERE `entry`=34499; -- Oso Bramblescar
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34503; -- Razormane Pillager
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34511; -- Forsaken Invader
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34522; -- Corrupted Servant
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34523; -- Captured Razormane
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34524; -- Corrupted Minion
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34525; -- Corrupted Gnarlpine
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=34530; -- Ancient Teldrassil Protector
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34545; -- Razormane Frenzy
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34563; -- Injured Grol'dom Grunt
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34576; -- Crossroads Caravan Kodo
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34592; -- Silverwind Conqueror
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34617; -- Captured Furbolg
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34618; -- Ota Wen
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34640; -- Barrens Vulture
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34647; -- Barrens Boar
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34699; -- Bomb-Throwing Monkey
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34706; -- Theramore Sharpshooter
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=46, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34707; -- Theramore Deck Hand
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34727; -- Sleepy Joe
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34729; -- Charlie
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34733; -- Rear Admiral Hartley
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34750; -- Cap'n Garvey
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34756; -- Moriana Dawnlight
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34757; -- Doranel Amberleaf
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34763; -- Daggertooth Shark
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34804; -- Chef Toofus
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34809; -- Stonetalon Grunt
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34829; -- King Reaperclaw
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34848; -- Honor's Stand Sharpshooter
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34851; -- Panicked Citizen
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34855; -- Honor's Stand Peasant
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34864; -- Gilneas City Guard
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34884; -- Rampaging Worgen
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34886; -- Kalimdor Eagle
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34890; -- Izzy
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=34896; -- Seldarria
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34911; -- Fissure-born Fire Elemental
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34931; -- Highborne Charlatan
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34932; -- Highborne Sorcerer
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=90, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34933; -- Lady Benel'derath
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=90, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34937; -- Councilman Stark I'ylar
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34939; -- Guardsman Belothiel
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34940; -- Illyanna Stonewood
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34941; -- Allendril Fallcreek
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=90, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=34946; -- Merchant Kendra
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34954; -- Gobber
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34968; -- Stonetalon Sentinel
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34969; -- Corrupted Sentinel
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=34982; -- Emerald Spirit
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35063; -- Kezan Citizen
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=12, `attackpower`=9, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=35075; -- Kezan Citizen
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35095; -- Talrendis Scout
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35096; -- Weakened Mosshoof Stag
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35111; -- Runaway Shredder
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=35123; -- Vincent Hersham
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=35124; -- Tobias Mistmantle
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35151; -- Stonetalon Warder
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=35152; -- Battlescar Ancient
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=55, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=35154; -- Night Elf Mage
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35158; -- Krom'gar Berserker
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=55, `attackpower`=94, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=35159; -- Krom'gar Witch Doctor
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35160; -- Krom'gar Logger
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35161; -- Krom'gar Warrior
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35168; -- Striped Dawnsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35169; -- Ochre Skeletal Warhorse
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35174; -- Krom'gar Peon
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=35233; -- Gilnean Survivor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35239; -- Troll Slave
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35304; -- Brute Enforcer
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35312; -- Talrendis Saboteur
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35333; -- Goblin Technician
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35334; -- Gnome Engineer
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35457; -- Bloodfang Bloodletter
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=35463; -- Bloodfang Lurker
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35466; -- Restless Spirit
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35505; -- Bloodfang Ripper
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=35554; -- Shaken Survivor
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35623; -- Villa Mook
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=35753; -- Krennan Aranas
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=35759; -- Balboa
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35810; -- Frightened Miner
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35812; -- Smart Mining Monkey
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35829; -- Jr. Bombardier Hackel
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=35838; -- Pygmy Witchdoctor
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=35880; -- Lady Silisthra
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=35881; -- Lady Vesthra
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35882; -- Orc Survivor
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=34, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35891; -- SI:7 Infiltrator
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35892; -- Silisthra's Power Stone
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35894; -- Orc Scout
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35896; -- Poison Spitter
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35897; -- Freezya
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35904; -- Jungle Cat
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=35912; -- Injured Citizen
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35916; -- Bloodfang Ripper
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=35968; -- Vesthra's Power Stone
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=46, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=36012; -- Blackmaw Shaman
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36013; -- Blackmaw Warrior
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36015; -- Blackmaw Pathfinder
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36063; -- Orc Scout
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36092; -- SI:7 Assassin
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36103; -- SI:7 Operative
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36104; -- Orc Battlesworn
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36129; -- Gyrochoppa Pilot
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36131; -- Vile Splash
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36147; -- Static-Charged Hippogryph
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36149; -- Alliance Sailor
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36179; -- Goblin Survivor
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36211; -- Duskhaven Watchman
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=36231; -- Horrid Abomination
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36236; -- Forsaken Footsoldier
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36287; -- Cynthia
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36288; -- Ashley
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36289; -- James
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36293; -- Ogre Minion
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=36312; -- Dark Ranger Thyala
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36344; -- Frightened Miner
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=36370; -- Lux
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=36371; -- Morta the Wretch
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=36376; -- Fib Gyrojolt
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36384; -- Zapper Gnome
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36385; -- Netgun Gnome
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36396; -- Forsaken Sailor
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36397; -- Captain Anson
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36399; -- Captain Morris
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36405; -- Attack Mastiff
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36421; -- Orc Survivor
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36422; -- Orc Scout
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36423; -- Orc Battlesworn
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36424; -- Scout Brax
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36426; -- Goblin Survivor
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=36453; -- Duskhaven Villager
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36460; -- Forsaken Survivor
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36468; -- Candy Cane
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36469; -- Chip Endale
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36472; -- Door Buzzer
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36488; -- Forsaken Castaway
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36501; -- Grimy Greasefingers
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36509; -- Experimental Raptor
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36512; -- Fox
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36578; -- Bastia
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=56, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=36592; -- Apprentice Investigator
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=56, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=36593; -- Apprentice Illuminator
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=52, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=36599; -- Arcane Construct
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36602; -- Duskhaven Watchman
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36611; -- Talrendis Biologist
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=26, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=36614; -- Talrendis Ambassador
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36636; -- Twilight Dragon Hunter
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=27, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=36637; -- Twilight Desecrator
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36639; -- Sable Drakonid
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36653; -- Duskhaven Watchman
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36660; -- Talrendis Marksman
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36671; -- Forsaken Scout
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36681; -- Ravenous Lurker
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=36687; -- Mariel Dawnsong
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36690; -- Duskhaven Watchman
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36696; -- Armored Battleboar
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=36697; -- Bristleback Thorncaller
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36713; -- Gilnean Mastiff
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36719; -- Pterrordax Scavenger
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36722; -- Bilgewater Laborer
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36732; -- Ancient Island Turtle
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36740; -- Teraptor Hatchling
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36742; -- Princess Tess Greymane
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36748; -- Captain Grektar
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36756; -- Dead Soldier
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36809; -- Forsaken Invader
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36813; -- Vilebrood Skitterer
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36816; -- Talrendis Defender
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36825; -- Grimtotem Straggler
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36827; -- Grimtotem Vagabond
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36828; -- Grimtotem Defiler
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=3, `Mana_mod_extra`=1 WHERE `entry`=36845; -- Agitated Earth Spirit
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=36849; -- Talrendis Lorekeeper
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36882; -- Swamp Crocolisk
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=36890; -- Talrendis Glaive Thrower
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36900; -- Grounded Wind Rider
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36914; -- Talrendis Sentinel
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36917; -- Surface to Other Surface Transport
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36922; -- Wounded Soldier
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36925; -- Bilgewater Soldier
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36936; -- Mountainfoot Grunt
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36943; -- Bristleback Invader
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=36962; -- Injured Villager
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=36987; -- Sergeant Dynamo
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37054; -- Goblin Supermodel
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37055; -- Goblin Supermodel
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37056; -- Goblin Supermodel
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37057; -- Goblin Supermodel
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=37067; -- Crash Survivor
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37080; -- Wounded Coldridge Mountaineer
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37084; -- Terrortooth Scytheclaw
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37085; -- Towering Plainstrider
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37086; -- Hecklefang Scavenger
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37090; -- Deviate Terrortooth
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=37108; -- Soothsayer Shi'kala
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=52, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=37140; -- Airborne Priest
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37145; -- Military Gyrocopter
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37160; -- Tower Warden
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37161; -- Honor's Stand Footman
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37167; -- Stonetalon Prisoner
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37170; -- Hunter Hill Scout
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=37173; -- Soothsayer Rikkari
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=37174; -- Soothsayer Mirim'koa
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37177; -- Coldridge Defender
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37206; -- Plains Prowler
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37207; -- Plains Pridemane
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37208; -- Thunderhead
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37489; -- Feral Watcher
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37492; -- Blackwald Fox
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=37499; -- Duskhaven Survivor
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37507; -- Frostmane Blade
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37511; -- Bristleback Bladewarden
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37517; -- Taurajo Orphan
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37555; -- Landquaker Kodo
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37556; -- Landquaker Bull
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=73, `attackpower`=106, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=37557; -- Thunderhawk Cloudscraper
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37559; -- Savannah Boar
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37560; -- Razormane Pathfinder
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37585; -- Northwatch Recon
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37659; -- Horde Field Defender
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37660; -- Razormane Warfrenzy
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=110, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=37661; -- Razormane Seer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37685; -- Valnov the Mad
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37686; -- Executor Cornell
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37692; -- Forsaken Infantry
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37694; -- Enslaved Villager
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37701; -- Forsaken Slavedriver
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37706; -- Desolation Grunt
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37716; -- Tempest's Reach Watchman
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37733; -- Lord Walden
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37740; -- Yellowfin Shark
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37741; -- Bilgewater Seal
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37743; -- Taurajo Looter
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=16, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=37757; -- Howling Banshee
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37784; -- Gilnean Militia
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37785; -- Wild Horse
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37786; -- Brown Stag
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37802; -- Brothogg the Slavemaster
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37805; -- Forsaken Soldier
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37806; -- Captain Asther
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37817; -- Zang'do
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37836; -- Larhasha
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37875; -- Lord Godfrey
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=37884; -- Disturbed Spirit
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37885; -- Restless Ancestor
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37916; -- Orc Raider
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37922; -- Desolation Raider
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37923; -- Triumph Vanguard
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37924; -- Barrens Operative
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37925; -- Sapper Specialist
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37926; -- Triumph Captain
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37938; -- Wolfmaw Outrider
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=52, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=37940; -- Mangletooth
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37956; -- Bloodtalon Raptor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37960; -- Bloodtalon Hatchling
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=37961; -- Corrupted Bloodtalon
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38022; -- Veteran Dark Ranger
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38141; -- Docile Island Boar
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38187; -- Spiny Raptor
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38192; -- Forsaken Infantry
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38210; -- Forsaken Crossbowman
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38221; -- Gilnean Militia
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38268; -- Novice Darkspear Warrior
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38272; -- Novice Darkspear Rogue
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38278; -- Novice Darkspear Priest
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=3, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38279; -- Novice Darkspear Mage
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=3, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38280; -- Novice Darkspear Druid
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38281; -- Novice Darkspear Shaman
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=3, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38282; -- Novice Darkspear Hunter
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38301; -- Spitescale Siren
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38324; -- Darkspear Tribesman
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38326; -- Darkspear Shaman
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38327; -- Northwatch Siege Engineer
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38348; -- Worgen Warrior
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38359; -- Vashj'elan Warrior
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=15, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38360; -- Vashj'elan Siren
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38364; -- Forsaken Plaguesmith
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38392; -- Val'kyr Protector
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38409; -- Goblin Survivor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38412; -- Naga Hatchling
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38423; -- Zuni
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38425; -- Freed Emberstone Villager
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38468; -- Gilnean Mastiff
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=18, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38473; -- Soultethered Banshee
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38571; -- Steamwheedle Survivor
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=15, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38574; -- Oomlot Shaman
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38575; -- Oomlot Tribesman
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38613; -- Worgen Warrior
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38616; -- Forsaken Infantry
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38617; -- Forsaken General
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38618; -- Forsaken Sergeant
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38624; -- Northwatch Defender
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38627; -- Northwatch Dockworker
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38643; -- Goblin Captive
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38646; -- Covert Ops Hardsuit
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38648; -- Bilgewater Battlebruiser
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38658; -- Rageroar Grunt
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38661; -- Rageroar Sea Dog
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38664; -- Blackpool Crewman
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38753; -- Goblin Zombie
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38762; -- Ogre Ambusher
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38781; -- Gilnean Survivor
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38805; -- Wounded Defender
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38811; -- Oostan Headhunter
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38812; -- Goblin Captive
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38832; -- Baron
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38844; -- Gilnean Mastiff
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38845; -- Child of Volcanoth
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38850; -- Volcanoth Champion
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38851; -- Volcanoth Priest
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38881; -- Fox
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=100, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38884; -- Deviate Thornweaver
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38909; -- Sandfury Zombie
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38914; -- Sandstone Golem
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38916; -- Sandstone Earthen
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38933; -- Briny Sea Cucumber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38937; -- Vile Fin Tadpole
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=98, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=38940; -- Deviate Crone
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=104, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=4, `Mana_mod_extra`=1 WHERE `entry`=38941; -- Three-Tooth
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=2, `Mana_mod_extra`=1 WHERE `entry`=38952; -- Nordrassil Druid
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38988; -- Notera
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38990; -- Kortin
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38997; -- Reliquary Digger
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=38998; -- Explorer's League Digger
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39005; -- Northwatch Rifleman
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39006; -- Siege Tank Engineer
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39008; -- Hira'jin
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39014; -- Pavlova
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39020; -- Crazed Bilgewater Survivor
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39022; -- Tidal Strider
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39044; -- Orc Battlesworn
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39049; -- Plagued Bruin
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39064; -- Orc Survivor
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39067; -- Scout Brax
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39068; -- Orc Scout
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39069; -- Alliance Paratrooper
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39086; -- Slain Scarlet Zealot
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=39087; -- Slain Scarlet Friar
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=39095; -- Slain Guard
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39124; -- Mizzy's Apprentice
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39129; -- Peasant Laborer
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39139; -- Triumph Rifleman
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39147; -- Bastia
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39153; -- Excavation Raider
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1.5, `Health_mod`=1.5 WHERE `entry`=39185; -- Slaverjaw
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1.5, `Health_mod`=1.5 WHERE `entry`=39186; -- Hellgazer
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39193; -- Brute Overseer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=39194; -- Blastshadow the Brutemaster
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=42, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39195; -- Delicia Whipsnaps
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39215; -- Captured Northwatch Scout
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39224; -- Lo'Shall
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39260; -- Northwatch Infantryman
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=39261; -- Northwatch Ranger
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39267; -- Northwatch Marine
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39268; -- Northwatch Sharpshooter
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=39272; -- Foaming Sea Elemental
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39279; -- Frazzlecraz Foreman
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39280; -- Frazzlecraz Miner
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39281; -- Fenton Frazzlecraz
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39337; -- Wayward Plainstrider
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39354; -- Steamwheedle Shark
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=39366; -- Sun-Touched Servant
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=39373; -- Sun-Touched Speaker
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39376; -- KTC Oil Bot
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=39381; -- Crimsonborne Guardian
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39399; -- Farmer Lok'lub
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39400; -- Farmer Krella
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=10, `Mana_mod_extra`=1 WHERE `entry`=39405; -- Crimsonborne Seer
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39408; -- Durotar Farmhand
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=39411; -- Earthen Ring Shaman
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=39414; -- Ascended Windwalker
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39418; -- Green Sand Crab
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39419; -- Oceanic Broadhead
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39422; -- Coilshell Sifter
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39452; -- Dreadmaw Toothgnasher
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39456; -- Captured Goblin
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39464; -- Drowned Thunder Lizard
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39589; -- Brute Bodyguard
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39591; -- Orc Battlesworn
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39592; -- Ultimate Footbomb Uniform
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=39603; -- Swollen Voidwalker
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=39626; -- Crimsonborne Warlord
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39629; -- Abyssal Seahorse
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39638; -- Azsh'ir Patroller
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39655; -- Orc Survivor
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39663; -- Drowning Soldier
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39664; -- Glimmerdeep Tidehunter
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39668; -- Mack Fearsen
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39729; -- Nespirah Survivor
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39745; -- Spiralung
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39796; -- Anemone Frenzy
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=39854; -- Azureborne Guardian
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=10, `Mana_mod_extra`=1 WHERE `entry`=39855; -- Azureborne Seer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=10, `Mana_mod_extra`=1 WHERE `entry`=39870; -- Twilight Firecatcher
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=39874; -- Derth Firesinger
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39886; -- Drowned Soldier
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=10, `Mana_mod_extra`=1 WHERE `entry`=39890; -- Twilight Earthshaper
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=39909; -- Azureborne Warlord
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39911; -- Dead Zin'jatar Raider
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39913; -- Frenzied Orca
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39918; -- Clacksnap Pincer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=10, `Mana_mod_extra`=1 WHERE `entry`=39954; -- Twilight Shadow Weaver
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=39956; -- Twilight Enforcer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=10, `Mana_mod_extra`=1 WHERE `entry`=39962; -- Twilight Stormbreaker
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=39964; -- Akasha
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=40033; -- Flux Animator
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40057; -- Mekkatorque's  Mechanostrider
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=40061; -- Galak Stormer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40064; -- Jungle Panther
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40067; -- Iceeya
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40068; -- Venom Spitter
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40071; -- Julia Scrabblewrench
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=10, `Mana_mod_extra`=1 WHERE `entry`=40167; -- Twilight Beguiler
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40200; -- Spiketooth Eel
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40219; -- Ravenous Thresher
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40222; -- Scout Bat
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40256; -- Troll Citizen
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40257; -- Troll Citizen
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=5, `Mana_mod_extra`=1 WHERE `entry`=40268; -- Twilight War-Mage
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=10, `Mana_mod_extra`=1 WHERE `entry`=40270; -- Twilight Thundercaller
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=40272; -- Ascended Rockbreaker
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=40273; -- Ascended Waterlasher
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40275; -- Zin'jatar Ravager
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40276; -- Sabreclaw Skitterer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=10, `Mana_mod_extra`=1 WHERE `entry`=40290; -- Crimsonborne Seer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=10, `Mana_mod_extra`=1 WHERE `entry`=40291; -- Azureborne Seer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=40306; -- Twilight Armsmaster
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40388; -- Darkspear Ancestor
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40416; -- Darkspear Scout
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=40448; -- Twilight Enforcer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=40450; -- Dustbone Horror
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40466; -- Blackfin
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40480; -- Empowered Water Elemental
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40482; -- Earthen Ring Wavebinder
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40483; -- Gnome Bar Patron
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40494; -- Goblin Bar Patron
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40508; -- Butcherbot
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40512; -- Brute Squad
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40527; -- Duneshore Crab
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40528; -- Steamwheedle Bughunter
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40529; -- Gnome Bar Patron
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40530; -- Goblin Bar Patron
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40560; -- Covert Ops Agent
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40582; -- Chelsea Rustflutter
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40583; -- Southsea Blockader
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40588; -- Steamwheedle Refugee
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40593; -- Southsea Taskmaster
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40604; -- Steamwheedle Rescue Balloon
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40632; -- Southsea Musketeer
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40635; -- Southsea Strongarm
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40636; -- Southsea Swabbie
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40649; -- Sea Snake
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40657; -- Basking Cobra
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40662; -- Steamwheedle Victim
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40677; -- Gilblin Scavenger
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40685; -- Splitclaw Skitterer
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40747; -- Selia Sunglow
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40764; -- Desert Crawler
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40776; -- Speedbarge Crewmember
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40777; -- Speedbarge Crewmember
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40778; -- Speedbarge Crewmember
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40779; -- Speedbarge Crewmember
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40798; -- Spineshell Pincer
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40799; -- Boog the "Gear Whisperer"
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40800; -- Minx
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40802; -- Softshell Sea Turtle
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=40808; -- Dustbone Horror
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40810; -- Gilblin Scavenger
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40811; -- Gilblin Scavenger
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40869; -- Holdfast Cannon
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40889; -- Sandskin Pincer
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40890; -- Dranosh'ar Archer
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40891; -- Dranosh'ar Laborer
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40892; -- Dranosh'ar Overseer
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40893; -- Dranosh'ar Fisherman
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=40909; -- Large Daddy
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40911; -- Redgill Scavenger
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40940; -- Young Wendigo
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40941; -- Wendigo
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40948; -- Dranosh'ar Farmer
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=40949; -- Kraka
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=40958; -- Tony Two-Tusk
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41002; -- Slickskin Eel
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41016; -- Gilblin Hoarder
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41017; -- Gilblin Collector
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41018; -- King Gurboggle
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41019; -- Rope Ladder
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41037; -- Salty Dog
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41038; -- Undead Officer
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41042; -- Giant Sea Grub
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41047; -- Injured Gnome Engineer
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41070; -- Captured Huntress
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41072; -- Dragonmaw Grunt
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=41073; -- Twilight Armsmaster
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41080; -- Dragonmaw Scout
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41121; -- Frostmane Seer
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41122; -- Frostmane Snowstrider
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41127; -- Dreadmaul Captive
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41132; -- Flood Elemental
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41136; -- Dreadmaul Ambusher
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41147; -- Slavering Ooze
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41162; -- Terrified Nethergarde Miner
UPDATE `creature_template` SET `mindmg`=115, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41171; -- Reassuring Nethergarde Foreman
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41175; -- Frostmane Scout
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41181; -- Kharanos Mountaineer
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41182; -- Kharanos Rifleman
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41183; -- Gilblingle
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41237; -- Kharanos Mountaineer
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41238; -- Wetlands Surveyor
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41251; -- Frostmane Builder
UPDATE `creature_template` SET `mindmg`=115, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41254; -- Enthralled Cultist
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41258; -- Frostmane Warrior
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41260; -- Brisket
UPDATE `creature_template` SET `mindmg`=122, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41269; -- Alliance Portal-Sentry
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41272; -- Dark Iron Smouldermage
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41293; -- Harbor Shredfin
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41326; -- Slickback Remora
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41333; -- Lieutenant Timothy Clark
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41335; -- Covert Operative
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41342; -- Mottled Screecher
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41343; -- Mottled Raptor
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41348; -- Gnomebot Helper
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41349; -- Harvestbot Assistant
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41359; -- Bloodwash Crate Spawner
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41361; -- Northwatch Infantry
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=41371; -- Void Seeker
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41384; -- Rockpool Murloc
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41385; -- Rockpool Tadpole
UPDATE `creature_template` SET `mindmg`=115, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41386; -- Bloodwash Barbarian
UPDATE `creature_template` SET `mindmg`=115, `maxdmg`=205, `attackpower`=224, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41387; -- Bloodwash Enchantress
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41389; -- Paleolithic Elemental
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41393; -- Technician
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41394; -- Dark Iron Holdout
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41400; -- Highland Razormaw
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41401; -- Highland Scytheclaw
UPDATE `creature_template` SET `mindmg`=115, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41404; -- Bloodwash Gambler
UPDATE `creature_template` SET `mindmg`=115, `maxdmg`=205, `attackpower`=106, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41405; -- Bloodwash Idolater
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41408; -- Goblin Stripminer
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41417; -- The Bomb
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41422; -- Bloodwash Zealot
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41423; -- Bloodwash Acolyte
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41424; -- Mouldering Mirebeast
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41425; -- Bluegill Murloc
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=34, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41426; -- Bluegill Oracle
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41427; -- Cursed Marine
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41428; -- Cursed Sailor
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41431; -- Grimtotem Invader
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41449; -- Rampant Fire Elemental
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41453; -- Smouldering Ooze
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41466; -- Idra'kess Sentinel
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41467; -- Idra'kess Enchantress
UPDATE `creature_template` SET `mindmg`=122, `maxdmg`=213, `attackpower`=234, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41470; -- Doomguard Destroyer
UPDATE `creature_template` SET `mindmg`=122, `maxdmg`=213, `attackpower`=234, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=3, `Mana_mod_extra`=1 WHERE `entry`=41471; -- Dreadlord Defiler
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41475; -- Oil Pump
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=304, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41477; -- Zin'jatar Pearlbinder
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41478; -- Snow Tracker Wolf
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41494; -- Enslaved Alliance Pearl Miner
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41495; -- Enslaved Horde Pearl Miner
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41496; -- Marsh Lasher
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41517; -- Idra'kess Harpooner
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41520; -- Deepseeker Crab
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41526; -- Enslaved Horde Mage
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41527; -- Enslaved Alliance Mage
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41528; -- Helpless Young Druid
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41539; -- Stolen Ram
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41544; -- Frostmane Raider
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41548; -- Imprisoned Soldier
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41561; -- Wary Mastiff
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41566; -- Muckskin Scrounger
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41569; -- Kvaldir Fleshcarver
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41573; -- Nespirah Fluid
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41586; -- Sira'kess Guardian
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=326, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41588; -- Sira'kess Sea Witch
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=326, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41589; -- Sira'kess Tide Priestess
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=304, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41592; -- Deepfin Seer
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41593; -- Deepfin Scrounger
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=304, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41606; -- Kvaldir Seahorror
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41607; -- Idra'kess Warlord
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41608; -- Idra'kess Prophet
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41609; -- Silversand Burrower
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41611; -- Amberstill Mountaineer
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41613; -- Greenwarden Sentinel
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41616; -- Vasuuvata
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41617; -- Jeffrey Gregarius
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41619; -- Grove Druid
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41643; -- Bloodcrazed Thresher
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41644; -- Faceless Defiler
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41646; -- Crushing Eel
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41647; -- Deep Remora
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41650; -- Nether Fiend
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41652; -- Twilight Champion
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=326, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41657; -- Twilight Candidate
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41660; -- Pincer X2 Crewmember
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41661; -- Verne Crewmember
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41671; -- Trapped Miner
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41680; -- Scout Utvoch
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41681; -- Horde Infantry
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=41731; -- Overseer Idra'kess
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41746; -- Gilblin Plunderer
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41762; -- Helm's Bed Surger
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41763; -- Frozen Mountaineer
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41846; -- Mathel Rustwrench
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41865; -- Darnassian Scout
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41897; -- Dun Morogh Mountaineer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41898; -- Dun Morogh Rifleman
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=3, `Mana_mod_extra`=1 WHERE `entry`=41902; -- Dark Iron Pyromancer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41909; -- Airfield Guard
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41922; -- Pyreshell Scuttler
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41925; -- Coldlight Hunter
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=304, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41926; -- Coldlight Oracle
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=41927; -- Devious Great-Eel
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=41935; -- Northwatch Encroacher
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41955; -- Imprisoned Warrior
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41979; -- Naz'jar Honor Guard
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41988; -- The Bomb
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41993; -- Raging Earth Elemental
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41997; -- Famished Great Shark
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=41998; -- Famished Great Shark
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42003; -- Dark Iron Golem
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42010; -- General Grimaxe
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42012; -- Dark Iron Invader
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42014; -- Ironforge Bomber
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42029; -- Rope Ladder
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42041; -- Dragonmaw Whelpstealer
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42042; -- Ebon Whelp
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42043; -- Ebon Slavehunter
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42051; -- Faceless Defiler
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42090; -- Arathi Deathstalker
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42092; -- Repaired Bomber
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42101; -- Helpless Young Druid
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42104; -- Wolf
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42107; -- Dragonmaw Grunt
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42108; -- Seabrush Terrapin
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42110; -- Bluegill Puddlejumper
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42111; -- Bluegill Forager
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42112; -- Scourgut Remora
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42113; -- Spinescale Hammerhead
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42114; -- Alliance Sea-Scout
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42115; -- Hellscream Seadog
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42146; -- Ambassador Slaghammer
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42147; -- Dark Iron Guard
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42154; -- Twilight Convert
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42155; -- Twilight Rifleman
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42170; -- Snow Leopard
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42174; -- Rixa Quickwrench
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42185; -- Living Contamination
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42210; -- Compelled Elemental
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42216; -- Stormwind Army Registrar
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42225; -- Alliance Prisoner
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42226; -- Crushcog Battle Suit
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42228; -- Okril'lon Scout
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42235; -- Darktail Bonepicker
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42244; -- Drowned Gilnean Merchant
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42248; -- Drowned Gilnean Sailor
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42249; -- Drowned Gilnean Settler
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42259; -- Dead Thief
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42280; -- Twilight Devotee
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42281; -- Twilight Devotee
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42286; -- Young Snow Leopard
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42290; -- Winter Wolf
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42291; -- Crushcog Sentry-Bot
UPDATE `creature_template` SET `mindmg`=122, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42301; -- Horde Portal-Sentry
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42311; -- Crime Scene Alarm-o-Bot
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42329; -- Gnomeregan Trainee
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=209, `attackpower`=242, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42336; -- Tainted Black Bear
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=209, `attackpower`=242, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42337; -- Tainted Nightstalker
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=209, `attackpower`=242, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42338; -- Tainted Screecher
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42339; -- Canal Crab
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42341; -- Barbfin Skimmer
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42342; -- Energized Harvest Reaper
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42359; -- Okril'lon Infantry
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42367; -- Rockpool Gladiator
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42381; -- Overloaded Harvest Golem
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42385; -- Orphan
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42386; -- Homeless Stormwind Citizen
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42390; -- Small-time Hustler
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42391; -- West Plains Drifter
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=304, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42453; -- Azsh'ir Abyss Priestess
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42475; -- Fungal Behemoth
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42490; -- Elgin Clickspring
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42504; -- Mature Swine
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42518; -- Wildhammer Warrior
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42524; -- Jaspertip Borer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42525; -- Jaspertip Ravager
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42527; -- Irestone Rumbler
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42548; -- Muddy Crawfish
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42552; -- Physician's Assistant
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=47, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42554; -- Wildhammer Shaman
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=47, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42555; -- Revantusk Hexxer
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42606; -- Rockslice Flayer
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42607; -- Rockslice Ripper
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42610; -- Thornar Thunderclash
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42616; -- Spanks
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=3, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42619; -- Novice Darkspear Warlock
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42620; -- Lashtail Egg
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42644; -- Smoot
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42646; -- AWOL Grunt
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42647; -- Axebite Warrior
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42650; -- Goblin Siegeworker
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42653; -- Jango Spothide
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=42656; -- Mercenary
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42671; -- Chief Engineer
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42677; -- Moonbrook Thug
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42780; -- Irestone Rumbler
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42804; -- Bilgewater Socialite
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42823; -- Twilight Priestess
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42824; -- Twilight Pyremaw
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42857; -- Sacrificed Vilebranch
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42859; -- Wild Mature Swine
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42879; -- Spawn of Shadra
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42880; -- Crushclaw
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42901; -- Vilebranch Broodguard
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42904; -- Rana Bubblefizz
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=42905; -- Grennor Strongbrew
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42912; -- Snaptrap
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42913; -- Shadraspawn Venomspitter
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42916; -- Twilight Soulreaper
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42917; -- Twilight Duskwarden
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25, `Health_mod`=4.5 WHERE `entry`=42918; -- Zoltrik Drakebane
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42924; -- Twilight Laborer
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=42970; -- Gilneas Refugee
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43002; -- Bastia
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43003; -- Venture Co. Sawblade
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43026; -- Deepstone Elemental
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43032; -- Slain Cannoneer
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43048; -- Captain Skullshatter
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43083; -- Redridge Fox
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43103; -- Bluetip Thresher
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43115; -- Dormant Stonebound Elemental
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=43123; -- Living Blood
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43134; -- Stone Trogg Ambusher
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43138; -- Stonehearth Defender
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43158; -- Mercurial Ooze
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=43170; -- Earthen Geomancer
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43174; -- Stone Trogg Digger
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43178; -- Libby Parker
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43181; -- Shalehide Basilisk
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43183; -- Freshwater Eel
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43196; -- Huge Boulder
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43203; -- Gmurgl
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43204; -- Orgus
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43205; -- Captain Bartholomew Softbeard
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=326, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=43218; -- Twilight Bloodshaper
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43222; -- Redridge Citizen
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43228; -- Stone Trogg Berserker
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=43229; -- Injured Earthen
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=43230; -- Crushcog Technician
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=43232; -- Earthen Champion
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=43233; -- Stonehearth Geomaster
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43247; -- Redridge Citizen
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=43248; -- Big Earl
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=43249; -- Dumpy
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43254; -- Energized Geode
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43258; -- Lodestone Elemental
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43291; -- Wilson
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43292; -- Adele
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43294; -- Gonzalez
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43297; -- Gnome Traveler
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43301; -- Devon Rackled
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43304; -- Heron Tuns
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43325; -- Rockjaw Fungus-Flinger
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43331; -- Golden Stonefish
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=56, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=3, `Mana_mod_extra`=1 WHERE `entry`=43350; -- Utroka the Keymistress
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=43354; -- Kelaan
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43355; -- Lowan Featherbreeze
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43360; -- Refugee
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=56, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=3, `Mana_mod_extra`=1 WHERE `entry`=43363; -- Ritualist Tarak
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43364; -- Bloodsail Hooch-Hound
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43367; -- Verlok Grubthumper
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=43368; -- Verlok Shroomtender
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43374; -- Pulsing Geode
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43417; -- Skymane Bonobo
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43451; -- Olivia Jayne
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43480; -- Temperamental Rumbler
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43513; -- Verlok Pillartumbler
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43532; -- Muckdweller
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=58, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=5, `Mana_mod_extra`=1 WHERE `entry`=43535; -- Blackrock Warden
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43536; -- Southsea Mako
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43545; -- Opalescent Guardian
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43571; -- Kidnapped Redridge Citizen
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43572; -- Kidnapped Redridge Citizen
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43596; -- Venture Co. Oil Worker
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=43598; -- Stone Trogg Beast Tamer
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43599; -- Thornguard
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43605; -- Bloodsail Oarsman
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=43616; -- Stone Trogg Earthrager
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43621; -- Targol
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43636; -- Bloodsail Corsair
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43657; -- Silverwind Vanquisher
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43659; -- Long John Copper
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43660; -- Enormous Shawn Stooker
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43661; -- Wailing Mary Smitts
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43747; -- Hippogryph
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=42, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=43749; -- Tobias Mistmantle
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43799; -- Lurking Worgen
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43891; -- Vilebranch Zealot
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43923; -- Forlorn Spirit
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43960; -- Stone Trogg Reinforcement
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43966; -- Twilight Dragonspawn
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43967; -- Twilight Scalesister
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43971; -- Stonescale Drake
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=43981; -- Jadecrest Basilisk
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=43992; -- Twilight Dragonstalker
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44020; -- Barn Owl
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44028; -- Silent Shade
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44029; -- Clattering Coldwraith
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44039; -- Twilight's Hammer Prisoner
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44046; -- Earthen Ring Shaman
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44049; -- Giant Mushroom
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44057; -- Riverboat
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44076; -- Defaced Earthrager
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44086; -- Panicked Citizen
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44089; -- Blackbelly Forager
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44098; -- Writhing Terror
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44110; -- Ocean Shredfin
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44142; -- Rockpool Tadpole
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=44153; -- Stonefather Oremantle
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44163; -- Stonesplinter Worker
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44166; -- Grazing Zhevra
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44167; -- Sunscale Raptor
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44172; -- Moldy Dark Iron Trapper Corpse
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44184; -- Loch Buzzard
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44188; -- Bobcat
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44189; -- Twilight Pyremaw
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44209; -- Spotted Swellfish
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44210; -- Sand Ray
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44211; -- Giant Driftray
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44220; -- Jade Rager
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44221; -- Scalesworn Cultist
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44239; -- Janric Moller
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44240; -- Argent Hippogryph
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44241; -- Lana Dubing
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44243; -- Darnella Winford
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44256; -- Greypaw
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44262; -- Mosshide Representative
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44263; -- Bluegill Representative
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44264; -- Tunnel Rat Lackey
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44284; -- Northridge Spider
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=110, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44315; -- Desiccated Magus
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44316; -- Desiccated Spearman
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=110, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44326; -- Andorhal Deathguard
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=110, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44328; -- Andorhal Defender
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44361; -- Earthmender Narvra
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44362; -- Earthmender Govrum
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44388; -- Gilnean Militia
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44390; -- Spiny Rock Crab
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44392; -- Muzzle
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44425; -- Crimson Shale Spider
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44432; -- Alliance Fox Hunter
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44433; -- Alliance Laborer
UPDATE `creature_template` SET `mindmg`=47, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44435; -- Argent Lumberjack
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44436; -- Hearthglen Shieldbearer
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44437; -- Hearthglen Recruit
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44439; -- Hearthglen Drudge
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=89, `attackpower`=120, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44443; -- Cult-Sworn Warlock
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=89, `attackpower`=120, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44444; -- Cult-Sworn Pugilist
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44447; -- Andorhal Deathguard
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44463; -- Baron
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44470; -- Injured Citizen
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44473; -- Shaggy Black Bear
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44474; -- Whitetail Fox
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44475; -- Brownfeather Hawk
UPDATE `creature_template` SET `mindmg`=47, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44478; -- Redpine Gnoll
UPDATE `creature_template` SET `mindmg`=47, `maxdmg`=85, `attackpower`=116, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44479; -- Redpine Shaman
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44481; -- Diseased Hawk
UPDATE `creature_template` SET `mindmg`=47, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44482; -- Hulking Plaguebear
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44546; -- Duneclaw Burrower
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44547; -- Bloodfang Scavenger
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44549; -- Bloodfang Scavenger
UPDATE `creature_template` SET `mindmg`=47, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44551; -- Rabid Fox
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44562; -- Opengut Behemoth
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44568; -- Dune Rattler
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44569; -- Sand Lasher
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44576; -- Proto-Strider
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44578; -- Naga Hatchling
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44580; -- Naga Hatchling
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44592; -- Fallen Human
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44593; -- Fallen Human
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44594; -- Sunburst Adder
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44595; -- Sand Slitherer
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44598; -- Desert Bloom
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44599; -- Duneclaw Broodlord
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44611; -- Wastewander Survivalist
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44612; -- Wastewander Tracker
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=85, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44613; -- Wastewander Darkcaster
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44616; -- Haywire Battle-Chicken
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=326, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44619; -- Twilight Binder
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44626; -- Lakebed Snapper
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44627; -- Mudbelly Boar
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44638; -- Hill Grizzly
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44647; -- Mariahn the Soulcleanser
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44649; -- Twilight Centurion
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44667; -- Seer Kormo
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44668; -- Seer Galekk
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44669; -- Earthmender Narvra
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44670; -- Earthmender Govrum
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44678; -- Mariahn the Soulcleanser
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44680; -- Twilight Defiler
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44681; -- Twilight Heretic
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=188, `dmg_multiplier`=1.5, `Health_mod`=1.5, `Mana_mod`=2, `Mana_mod_extra`=1 WHERE `entry`=44714; -- Fronkle the Disturbed
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44729; -- Twilight Encroacher
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44732; -- Forsaken Trooper
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44733; -- Forsaken Trooper
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44734; -- Horde Engineer
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44745; -- Sunwalker Atohmo's Kodo
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44758; -- Mo'grosh Darkmauler
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=58, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44760; -- Mo'grosh Earthbender
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44762; -- Drenched Southsea Musketeer
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44763; -- Drowned Southsea Strongarm
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44768; -- Enslaved Miner
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44769; -- Twilight Desecrator
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44773; -- Anastasia
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=2, `Mana_mod_extra`=1 WHERE `entry`=44786; -- Apothecary Initiate
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44791; -- Forsaken Trooper
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44792; -- Forsaken Trooper
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44793; -- Worgen Renegade
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44836; -- Hearthglen Mustang
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44847; -- Twilight Armsman
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44849; -- Twilight Crusher
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=343, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44855; -- Twilight Cryptomancer
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=44879; -- Ogre Bodyguard
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44885; -- Bound Air Elemental
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44886; -- Bound Water Elemental
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44887; -- Bound Fire Elemental
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44894; -- Armoire
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=7, `Mana_mod_extra`=1 WHERE `entry`=44898; -- Pygmy Firebreather
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44902; -- Captive Plaguebear
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44913; -- Orc Sea Dog
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=44922; -- Oathsworn Axemaster
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=44924; -- Oathsworn Myrmidon
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=44926; -- Oathsworn Wanderer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=44932; -- Oathsworn Pathfinder
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44936; -- Murkstone Trogg
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=7, `Mana_mod_extra`=1 WHERE `entry`=44976; -- Neferset Plaguebringer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=7, `Mana_mod_extra`=1 WHERE `entry`=44977; -- Neferset Torturer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=7, `Mana_mod_extra`=1 WHERE `entry`=44980; -- Neferset Theurgist
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=44981; -- Oathsworn Skinner
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=7, `Mana_mod_extra`=1 WHERE `entry`=44982; -- Neferset Darkcaster
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44986; -- Corporal Nobsy
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=44988; -- Greater Quicksilver Ooze
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=44998; -- Earthen Ring Shaman
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45031; -- Nethergarde Footman
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=7, `Mana_mod_extra`=2 WHERE `entry`=45062; -- Oathsworn Scorpid Keeper
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45064; -- Catapult Driver
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45084; -- Elemental Overseer
UPDATE `creature_template` SET `mindmg`=122, `maxdmg`=209, `attackpower`=242, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45087; -- Felscale Crawler
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45096; -- Tamed Tol'vir Prowler
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=45097; -- Oathsworn Tamer
UPDATE `creature_template` SET `mindmg`=122, `maxdmg`=209, `attackpower`=242, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45118; -- Darkwood Treant
UPDATE `creature_template` SET `mindmg`=124, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45119; -- Corrupted Darkwood Treant
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=209, `attackpower`=242, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45125; -- Felspore Bog Lord
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45153; -- Redpine Looter
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=89, `attackpower`=120, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=45154; -- Redpine Necromancer
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45166; -- Withdrawn Soul
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=48, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45196; -- Orc Sea Dog
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45197; -- Veteran Forsaken Trooper
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=45198; -- Forsaken Catapult
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45205; -- Sultan Oogah
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=80, `attackpower`=112, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=45208; -- Provincial Settler
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45254; -- Hillsbrad Worgen
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45255; -- Hillsbrad Worgen
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45272; -- Innkeeper Abeqwa
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=45274; -- Magistrix Elosai
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45302; -- Sunchaser Gazelle
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45335; -- Highlands Fox
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45336; -- Burning Invader
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45367; -- Gilnean Hound
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1.5, `Health_mod`=1.5 WHERE `entry`=45369; -- Morick Darkbrew
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=107, `attackpower`=67, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=45381; -- Grimtotem Geomancer
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=107, `attackpower`=142, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=45383; -- Grimtotem Stomper
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45385; -- Grimtotem Reaver
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=45387; -- Isha Gloomaxe
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1.5, `Health_mod`=1.5 WHERE `entry`=45398; -- Grizlak
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1.5, `Health_mod`=1.5 WHERE `entry`=45399; -- Optimo
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=28, `dmg_multiplier`=1.5, `Health_mod`=1.5, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=45404; -- Geoshaper Maren
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=112, `attackpower`=162, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=45438; -- Arnak Grimtotem
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=45443; -- Death's Step Putrifier
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45453; -- Blighthound
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45475; -- Plague Disseminator
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45517; -- Ravaged Dire Wolf
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45613; -- Half-Eaten Coalpelt Bear
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45614; -- Anguished Spirit
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=45619; -- Morbidius
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45664; -- Landlocked Grouper
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=107, `attackpower`=142, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=45691; -- Skullmage
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45692; -- Noxious Assassin
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45695; -- Slain Scourge Trooper
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=56, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=45711; -- Elemental Servitor
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45715; -- Orsis Survivor
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=26, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=45728; -- Arcane Remnant
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1.5, `Health_mod`=1.5 WHERE `entry`=45739; -- The Unknown Soldier
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45750; -- Inconspicuous Bear
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=58, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=45753; -- Ambermill Portal Guardian
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45755; -- Scion of Al'Akir
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45784; -- Crablegs
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45807; -- Sawtooth Crocolisk
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45809; -- Swampshore Makrura
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1.5, `Health_mod`=1.5 WHERE `entry`=45811; -- Marina DeSirrus
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45825; -- Swampstrider
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45830; -- Off-Duty Siegeworker
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45852; -- Bogpaddle Beach Bum
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45859; -- Venomblood Scorpid
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=172, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=45897; -- Plaguewood Reanimator
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45905; -- Wastewander Outrider
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45950; -- Sorrowmurk Snapjaw
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45967; -- Marshfin Murkdweller
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=45988; -- Twilight Bloodsmith
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46014; -- Deceased Bogpaddle Privateer
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46025; -- S.A.F.E. Officer
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=46058; -- Abomination
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=172, `attackpower`=206, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=46068; -- Corrupted Guardian
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46080; -- Kazit
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=112, `attackpower`=152, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=46138; -- Argent Centurion
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46146; -- Stagalbog Serpent
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=180, `attackpower`=224, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46164; -- Marshtide Footman
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=180, `attackpower`=224, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46166; -- Stonard Warrior
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46187; -- Marshtide Carouser
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46188; -- Bogpaddle Entertainer
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46189; -- Stonard Carouser
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46230; -- S.A.F.E. Technician
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46245; -- Lil' Crazy Jerry
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=46248; -- Riverpaw Basher
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=46249; -- Riverpaw Slayer
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=46250; -- Riverpaw Poacher
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=46251; -- Riverpaw Looter
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=78, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=3, `Mana_mod_extra`=1 WHERE `entry`=46252; -- Riverpaw Shaman
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=78, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=2, `Mana_mod_extra`=1 WHERE `entry`=46260; -- Searing Destroyer
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=78, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=2, `Mana_mod_extra`=1 WHERE `entry`=46261; -- Enraged Fire Elemental
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=78, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=2, `Mana_mod_extra`=1 WHERE `entry`=46263; -- Slag Fury
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46267; -- Rescued Survivor
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46302; -- Oil-Slicked Earth Spirit
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46365; -- Enthralled Gilblin
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46366; -- Enthralled Siren
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=95, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=46367; -- Enthralled Murloc
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46368; -- Enthralled Makrura
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46369; -- Enthralled Crustacean
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=46375; -- Rowdy Troublemaker
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=46379; -- Vicious Thug
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=46381; -- Shifty Thief
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46393; -- Billy Goat
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46401; -- Mar'at Villager
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46414; -- Marshtide Knight
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46415; -- Stabled Warhorse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46447; -- Injured Gnome
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46474; -- Shimmerspine Harvester
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46478; -- Highvale Sentry
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=2, `Mana_mod_extra`=1 WHERE `entry`=46483; -- Forsaken Apothecary
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46486; -- Stonard Peon
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46487; -- Marshtide Peasant
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46503; -- Lil' Crazy Daisy
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46508; -- Darkwood Lurker
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46559; -- Horde Engineer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=46573; -- Vile Fin Oracle
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=42, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=46574; -- Vile Fin Tidehunter
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46575; -- Darktusk Boar
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46590; -- Crazed Digger
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46605; -- Shipwrecked Sailor
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46606; -- Spiny Tidecrawler
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46617; -- Cursed Engineer
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46645; -- Cursed Stallion
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=81, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=46693; -- Dustbelcher Instructor
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=81, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=46694; -- Dustbelcher Initiate
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=81, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=46695; -- Dustbelcher Trainee
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=180, `attackpower`=224, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46749; -- Stonard Wardrummer
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46772; -- Reliquary Excavator
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46773; -- Explorers' League Excavator
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46823; -- Restless Infantry
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46825; -- Restless Soldier
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=180, `attackpower`=99, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=46841; -- Marshtide Cleric
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46884; -- Neferset Smuggler
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46888; -- Unstable Bomb Bot
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=180, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=46914; -- Raging Whelp
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=180, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=46918; -- Darkflight Shadowspeaker
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46928; -- Dustbelcher Butcher
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46929; -- Dustbelcher Merchant
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=101, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=46950; -- Misty Grell
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=46953; -- Purespring Elemental
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46979; -- Schnottz Air Trooper
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1.5, `Health_mod`=1.5 WHERE `entry`=46981; -- Nightlash
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46982; -- Sly
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=46989; -- Crazed Soldier
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1.5, `Health_mod`=1.5 WHERE `entry`=46992; -- Berard the Moon-Crazed
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46993; -- Schnottz Air Officer
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=46997; -- Shifting Mireglob
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1.5, `Health_mod`=1.5 WHERE `entry`=47003; -- Bolgaff
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1.5, `Health_mod`=1.5 WHERE `entry`=47008; -- Fenwick Thatros
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=52, `dmg_multiplier`=1.5, `Health_mod`=1.5, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47009; -- Aquarius the Unbound
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1.5, `Health_mod`=1.5 WHERE `entry`=47010; -- Indigos
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1.5, `Health_mod`=2 WHERE `entry`=47012; -- Effritus
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47014; -- Cursed Surveyor
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1.5, `Health_mod`=2 WHERE `entry`=47015; -- Lost Son of Arugal
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=30, `dmg_multiplier`=1.5, `Health_mod`=1.5, `Mana_mod`=2, `Mana_mod_extra`=1 WHERE `entry`=47023; -- Thule Ravenclaw
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=47030; -- Veteran Forsaken Trooper
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=47031; -- Forsaken Blightspreader
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47053; -- Duskfang
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47071; -- Deep Spider
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=180, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47073; -- New Kargath Grunt
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47131; -- Frantic Geist
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47132; -- Dark Creeper
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=47134; -- Corpse Eater
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=47135; -- Fetid Ghoul
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=47136; -- Unstable Ravager
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=3, `Mana_mod_extra`=1 WHERE `entry`=47140; -- Sorcerous Skeleton
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=3, `Mana_mod_extra`=1 WHERE `entry`=47141; -- Dread Scryer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47181; -- Overlook Spirit
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47182; -- Overlook Spectre
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47183; -- Ghastly Worker
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=343, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47184; -- Stone Trogg Fungalmancer
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47185; -- Refined Gentleman
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47193; -- Schnottz's Bodyguard
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47207; -- Dead Trooper
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47213; -- Dead Trooper
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47216; -- Dead Trooper
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47219; -- Dead Trooper
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47220; -- Furious Specter
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=343, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47227; -- Eternal Protector
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=3, `Mana_mod_extra`=1 WHERE `entry`=47231; -- Shadowy Attendant
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=47232; -- Ghostly Cook
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47283; -- Sand Serpent
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47289; -- Argent Crusade Engineer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47291; -- Schnottz's Landing Laborer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=47294; -- High Warlord Cromush
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47306; -- Neferset Sentinel
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47324; -- Baradin Guard
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47325; -- Baradin Guard
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47335; -- Baradin Grunt
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47336; -- Baradin Grunt
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1.5, `Health_mod`=1.5 WHERE `entry`=47386; -- Ainamiss the Hive Queen
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1.5, `Health_mod`=1.5 WHERE `entry`=47387; -- Harakiss the Infestor
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47388; -- Silithid Ravager
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47390; -- Silithid Defender
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47395; -- Injured Miner
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47415; -- Howler
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47445; -- Skeletal Steed
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47446; -- Dead Employee
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47472; -- Privileged Socialite
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=116, `attackpower`=166, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47479; -- Twilight Trapper
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47481; -- Highperch Prideling
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47485; -- Highperch Wind Rider
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47487; -- Twilight Subduer
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47514; -- Pretentious Businessman
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47528; -- Aerie Ace
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47583; -- Bulwark Defender
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=81, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47585; -- Twilight Magus
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47621; -- Bulwark Prisoner
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47627; -- Rustberg Bandit
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47643; -- Tanzar
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47657; -- Suspicious Villager
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47659; -- Apprehensive Worker
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=47663; -- Scout Obrok
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47666; -- Rustberg Horse
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47676; -- Baradin Fox
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47680; -- Rustberg Gull
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=47688; -- Officer Connelly
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47698; -- Menacing Emissary
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47699; -- Ramkahen Field Worker
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47716; -- Sun Priest Iset
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47720; -- Camel
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47722; -- Neferset Armorer
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47725; -- Tol'vir Merchant
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47727; -- Neferset Blade Twister
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47729; -- Neferset Savage
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47738; -- Neferset Snake Charmer
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47741; -- Neferset Venom Keeper
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=2, `Mana_mod_extra`=1 WHERE `entry`=47753; -- Dark Pharaoh Tekahn
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=5, `Mana_mod_extra`=1 WHERE `entry`=47760; -- Neferset Cursebringer
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=360, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47762; -- Neferset Ritualist
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47782; -- Blackrock Whelper
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=94, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47786; -- Sludge Guard
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47801; -- Temple Scarab
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47803; -- Sand Scorpid
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=81, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47804; -- Twilight Shaper
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47808; -- Black Wolf
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47809; -- Brown Wolf
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47815; -- Grimfang
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47816; -- Shackled Earth
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47817; -- Scuttler
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47818; -- Tusker
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47865; -- Forsaken Trooper
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47870; -- Twinkles
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=47871; -- Sludge Guard
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47872; -- Human Seedling
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47978; -- Captain Kronkh
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47981; -- Captain Margun
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=47982; -- Schnottz Sea Trooper
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=48017; -- Infected Sludge Guard
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48037; -- Chiseled Golem
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48068; -- Fuselight Engineer
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48113; -- Juliette
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48114; -- Raphael
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=149, `attackpower`=194, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=48118; -- Smolderthorn Shaman
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48119; -- Smolderthorn Assassin
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=149, `attackpower`=194, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=48120; -- Firegut Reaver
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=48121; -- Firegut Flamespeaker
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48124; -- Rabid Mastiff
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48128; -- Stranded Sparkleshell
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48130; -- Scorpid Cliffcrawler
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48131; -- Needlespine Cobra
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48132; -- Needlespine Shimmerback
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48136; -- Blight Slime
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=112, `attackpower`=162, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48137; -- Monstrous Macaw
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=112, `attackpower`=162, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48138; -- Brilliant Clayscraper
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=48139; -- Temple Swiftstalker
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=7, `Mana_mod_extra`=1 WHERE `entry`=48140; -- Temple Runecaster
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=48141; -- Temple Shadowlancer
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48153; -- Goblin Flamethrower
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=112, `attackpower`=162, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48155; -- Saltspray Gull
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=48159; -- Wyrtle Spreelthonket
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25, `Health_mod`=4, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=48168; -- Salhet's Lion
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48177; -- Silithid Ravager
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48178; -- Silithid Ravager
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48179; -- Silithid Wasp
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48180; -- Wild Horse
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48182; -- Shimmershell Snail
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48184; -- Hill Stag
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48187; -- Hill Fawn
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48188; -- Siltwash Terrapin
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48191; -- Remora Scrounger
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=112, `attackpower`=162, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48192; -- Barbed Gasgill
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48204; -- Schnottz Excavator
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48205; -- Schnottz Overseer
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48248; -- Sorrow Venomspitter
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48249; -- Sorrow Screecher
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48252; -- Baradin Recruit
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48267; -- Neferset Denizen
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48272; -- Bloodfang Forsaken Hunter
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48308; -- Farson Hold Prisoner
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48309; -- Farson Hold Prisoner
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48325; -- Farson Horse
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48357; -- Hellscream's Reach Recruit
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=48374; -- Myzerian
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=164, `attackpower`=95, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=48413; -- Blackrock Necromancer
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48482; -- Stormpike Mountaineer
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48484; -- Bloodfang Sentry
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=48512; -- Ramkahen Legionnaire
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48514; -- Ramkahen Marksman
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48519; -- Micro Mechachicken
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48520; -- Crystalwing Stone Drake
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48526; -- Bilgewater Buccaneer
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48550; -- Neferset Champion
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=360, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=48625; -- Neferset Scryer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48626; -- Neferset Raider
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48627; -- Neferset Looter
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48629; -- Schnottz Infantryman
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48632; -- Golden King
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48664; -- Wretched Spirit
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=48665; -- Wretched Spectre
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48670; -- Betrayed Ranger
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48694; -- Weathered Nomad
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48697; -- Ramkahen Archer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48701; -- Nahom Sun Priest
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48705; -- Gertrude Nozzlerocket
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48708; -- The Uncrashable
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48741; -- SI:7 Operative
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48742; -- SI:7 Assassin
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48763; -- Forlorn Highborne
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48875; -- Agee Tyler
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48912; -- Stormpike Trainee
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48918; -- Winterhorn Stag
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=48950; -- The Bogpaddle Bullet
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49217; -- Wintervine Lasher
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49242; -- Ramkahen Ranger
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=49263; -- Zephyrus
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49269; -- Dun Garok Spirit
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49281; -- Neferset Scout
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49346; -- Coldlurk Creeper
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49347; -- Coldlurk Burrower
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49398; -- Beauxbeaux
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49422; -- Rotbrain Berserker
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=49423; -- Rotbrain Magus
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49455; -- Dehydrated Prisoner
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49478; -- Dentaria Silverglade
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49639; -- Kartanus
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49640; -- Ranadae
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49691; -- Fertilize-o-tron 2000
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49692; -- Sunflower
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49815; -- Deep Spider
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49816; -- Deep Spider
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1.5, `Health_mod`=1.5 WHERE `entry`=49822; -- Jadefang
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49869; -- Stormwind Infantry
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49875; -- Ace
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49878; -- Coach Crosscheck
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49880; -- Foreman Dampwick
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49881; -- Geargrinder Gizmo
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49883; -- Izzy
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49886; -- Missa Spekkies
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49907; -- Dylan Aguilar
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49910; -- Vermillion Vanguard
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49912; -- Jafix
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49921; -- Kramer
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49922; -- Kerwin the Destroyer
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49924; -- Joe Mag
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49925; -- Johnny Shinster
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49926; -- Max Boomson
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49932; -- Boss Mack
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49933; -- Nik the Mountain
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49935; -- Ujumqin
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49936; -- Zak Callabang
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49937; -- Aleesha Killem
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49944; -- Jeigh Southie
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49953; -- Dr. Wizzle Doomsday
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=49960; -- Hobbes
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=50036; -- Ramkahen Scout
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=50037; -- Ramkahen Scout
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=50161; -- Dar Rummond
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=50164; -- Tulgar Flamefist
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50258; -- Frostmaul Tumbler
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50271; -- Melinda Hammond
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50273; -- Cynthia
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50274; -- Ashley
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50275; -- James
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50291; -- Arcane Tesseract
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50301; -- Goblin Admirer
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50302; -- Imported Mottled Boar
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50310; -- Goblin Captive
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50311; -- Goblin Captive
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=50312; -- Mana-Compelled Shade
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=180, `attackpower`=210, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=50316; -- K'areshi Trader
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50318; -- Xorothian Satyr
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50374; -- Risen Dead
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50412; -- Kaitlin
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50413; -- Mr. Woofles
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50414; -- Risen Recruit
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50422; -- Zakk Sinon
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=50432; -- Deva Marie
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=98, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=50435; -- Judy Gregory
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50471; -- Afflicted Gilnean
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50508; -- Carrie Eileen Steen
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50509; -- Jenn Stravaganza
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50510; -- Rachel DeSimone
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50511; -- Demolitions Analyst I
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50512; -- Demolitions Analyst II
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50513; -- Jamie Harriott
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50516; -- Gilnean Refugee
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50517; -- Gilnean Refugee
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50518; -- Gilnean Refugee
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50519; -- Gilnean Refugee
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50520; -- Gilnean Refugee
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50521; -- Gilnean Refugee
UPDATE `creature_template` SET `mindmg`=225, `maxdmg`=385, `attackpower`=478, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50525; -- Chris Miller
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50526; -- Ian Jolly
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50527; -- Joanna Jolly
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50528; -- Samantha Cleland
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50571; -- Tidebreaker Deckhand
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50572; -- Intrepid Deckhand
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50573; -- Galerek's Remorse Deckhand
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50575; -- Iron Reaper Deckhand
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50579; -- Alliance Cannon
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50580; -- Horde Cannon
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=50615; -- Tormented Officer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=50939; -- Domesticated Kodo
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51081; -- Nathan Stockton
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51193; -- Wild Camel
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51195; -- Kor'kron Annihilator
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51217; -- Cavorting Pygmy
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=51230; -- Earthmender
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51396; -- Stonecore Teleporter
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51397; -- Stonecore Teleporter
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51411; -- Neill Penny
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51440; -- Adam Pressler
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51631; -- Witherbark Fisher
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=90, `dmg_multiplier`=1.5, `Health_mod`=2, `Mana_mod`=2, `Mana_mod_extra`=1 WHERE `entry`=51663; -- Pogeyan
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51671; -- Rabid Hyena
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51672; -- Sweeping Winds
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51673; -- Venomscale Spitter
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51674; -- Sand-Husk Scarab
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51681; -- Frostsaber Cub
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51753; -- Schnottz Overseer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51760; -- Carrion Bird
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51961; -- Tirisfal Stag
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=51963; -- Fey-Drunk Darter
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51965; -- Tirisfal Fawn
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51970; -- Anton Buckey
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51971; -- Jermaine Buckey
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51972; -- Black Bear
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51973; -- Young Black Bear
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51976; -- Dun Morogh Sledder
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51977; -- Dun Morogh Sledder
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51978; -- Dun Morogh Sledder
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51979; -- Dun Morogh Sledder
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51982; -- Ollie
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51983; -- Deadwind Widow
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=51987; -- Deadwind Widow
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52000; -- Astranaar Sentinel
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=10, `Mana_mod_extra`=1 WHERE `entry`=52076; -- Gurubashi Cauldron-Mixer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=10, `Mana_mod_extra`=1 WHERE `entry`=52081; -- Gurubashi Cauldron-Mixer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=10, `Mana_mod_extra`=1 WHERE `entry`=52082; -- Gurubashi Cauldron-Mixer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=10, `Mana_mod_extra`=1 WHERE `entry`=52088; -- Gurubashi Cauldron-Mixer
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1.5, `Health_mod`=1.5 WHERE `entry`=52146; -- Chitter
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52161; -- Foulweald Pathfinder
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52171; -- Muddy Tracks
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52196; -- Burning Blade Bruiser
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52227; -- Balgor Whipshank
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52323; -- Chosen of Hethiss
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=52325; -- Gurubashi Blood Drinker
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=52327; -- Gurubashi Shadow Hunter
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52338; -- Burning Blade Attacker
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=7, `Mana_mod_extra`=1 WHERE `entry`=52339; -- Lesser Priest of Bethekk
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=52345; -- Pride of Bethekk
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52356; -- Venture Co. Drudger
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52357; -- Venture Co. Mercenary
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52373; -- Florawing Needler
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52375; -- Florawing Needler
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52376; -- Florawing Needler
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52377; -- Florawing Needler
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52379; -- Venomtip Needler
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52386; -- Burning Blade Windrider
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=52392; -- Gurubashi Master Chef
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=52413; -- Zulian Gnasher
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=52417; -- Shredtooth Frenzy
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52434; -- Gurubashi Villager
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52435; -- Gurubashi Villager
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52436; -- Gurubashi Refugee
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52437; -- Gurubashi Refugee
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52446; -- Ancient Dwarven Artifact
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52448; -- Gef
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52449; -- Ancient Dwarven Artifact
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52450; -- Ancient Elven Artifact
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52451; -- Ancient Elven Artifact
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52452; -- Ancient Troll Artifact
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52453; -- Ancient Troll Artifact
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52454; -- Ancient Fossil
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52455; -- Ancient Fossil
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52456; -- Jesi
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52457; -- Kodo Calfling
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=10, `Mana_mod_extra`=1 WHERE `entry`=52598; -- Gurubashi Soul-Eater
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=52672; -- Flameward Hippogryph
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=52730; -- Gurubashi Spirit
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=53044; -- Blackwater Ruffian
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=53479; -- Cinderweb Matriarch
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=53516; -- Essence of Doubt
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=53517; -- Squirming Slime Mold
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=53522; -- Baby Octopus
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=53526; -- Brightwater Snail
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=53544; -- Squirky
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=53561; -- Stonebull Crayfish
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=53626; -- Stormwind Orphan
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=53642; -- Cinderweb Spinner
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=53646; -- Tides of Longing
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=53718; -- Cinderweb Matriarch
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=53732; -- Unbound Smoldering Elemental
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=0.7, `Mana_mod_extra`=1 WHERE `entry`=53739; -- Element of Patience
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4.5 WHERE `entry`=53894; -- Corestone of Patience
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25, `Health_mod`=5, `Mana_mod`=10, `Mana_mod_extra`=1 WHERE `entry`=54019; -- Captive Druid of the Talon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54026; -- Dotty
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54113; -- Spooks
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54214; -- Scott Keenan
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54215; -- Lana Ashwin
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54216; -- Warren Fulton
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54217; -- Daphne "The Rose" Bloom
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54218; -- Daniel Kinsey
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54219; -- Sean Copeland
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54244; -- Undercity Citizen
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54299; -- Magma Orb
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54308; -- Kirsty Logan
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54510; -- Baby
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25, `Health_mod`=4 WHERE `entry`=54511; -- Time-Twisted Geist
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=7, `Mana_mod_extra`=1 WHERE `entry`=54517; -- Time-Twisted Shadowtalon
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54545; -- Loose Stones
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54557; -- Amberglass Crab
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54595; -- Maggy
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54596; -- Raan
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54618; -- Nimm Codejack
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54623; -- Shipwrecked Footman
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54688; -- Time-Twisted Nightsaber
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54699; -- Time-Twisted Nightsaber
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54700; -- Time-Twisted Nightsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54780; -- Orchard Fire
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54855; -- Min Dimwind
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54856; -- Master Li Fei
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=54983; -- Treant
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55019; -- Tushui Monk
UPDATE `creature_template` SET `mindmg`=135, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55088; -- Fire Eater
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55089; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55093; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55117; -- Gargantua
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55118; -- Jumbo
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=409, `attackpower`=528, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55121; -- Frenzyheart Visitor
UPDATE `creature_template` SET `mindmg`=157, `maxdmg`=409, `attackpower`=528, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55124; -- Oracle Visitor
UPDATE `creature_template` SET `mindmg`=157, `maxdmg`=268, `attackpower`=308, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55125; -- Elder Daftusk
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55155; -- Fine Leather Journal
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55167; -- Alliance Banner
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55174; -- Naval Map
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55175; -- Alliance Cannon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55176; -- Alliance Orders
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55220; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55221; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55222; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55223; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=180, `attackpower`=224, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55225; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55226; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=248, `attackpower`=298, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55229; -- Fire Eater
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=238, `attackpower`=282, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55230; -- Fire Eater
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55231; -- Fire Eater
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55247; -- Poot
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55248; -- Wallace
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55249; -- Bodie
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55250; -- Duquan
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55251; -- Namond
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55252; -- Michael
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55253; -- Randy
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55254; -- Ellie Goodup
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55261; -- Darkmoon Parade Creature
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55268; -- Aja
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55269; -- Jerrica
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55270; -- Kimber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55271; -- Shana
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55272; -- Mountain Horse
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55273; -- Swift Mountain Horse
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55341; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=209, `attackpower`=242, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55342; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55347; -- Alliance Citizen
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55348; -- Horde Citizen
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55397; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55398; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=133, `maxdmg`=231, `attackpower`=272, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55399; -- Fire Eater
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55428; -- Jumbo Jr.
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55429; -- Jumbina
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55440; -- Violet
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55443; -- Goldwing
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55444; -- King Leo
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55446; -- Queen Savannah
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55448; -- M'ku
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55449; -- Binti
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55457; -- Gleok
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55539; -- Wugou
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55558; -- Shu
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55598; -- Krystal
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55602; -- Urefu
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55603; -- Shingo
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55604; -- Mabaka
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55608; -- Crush
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55609; -- Lizzy
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55714; -- Darkmoon Monkey
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55719; -- Thomas Miller
UPDATE `creature_template` SET `mindmg`=200, `maxdmg`=341, `attackpower`=175, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=55722; -- Breanni
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55726; -- Belbi Quikswitch
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55727; -- Topper McNabb
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=55732; -- Champion Faesrol
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=55733; -- Champion Isimode
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55744; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55761; -- Phillipe
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55819; -- Sully "The Pickle" McLeary
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55823; -- Mishka
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55825; -- Amber Kearnen
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55830; -- Nimm Codejack
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55832; -- Nodd Codejack
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=55999; -- Injured Sailor
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56035; -- Kolin
UPDATE `creature_template` SET `mindmg`=200, `maxdmg`=341, `attackpower`=175, `dmg_multiplier`=1, `Health_mod`=1, `Mana_mod`=1, `Mana_mod_extra`=1 WHERE `entry`=56043; -- Martha Weller
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56159; -- Master Shang Xi
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56160; -- Darkmoon Wolf
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56180; -- Speckled Trout
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56195; -- Escaped Horde Engineer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56236; -- Injured Sailor
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56241; -- Dai-Lo Farmer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56362; -- Skyseeker Sailor
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56393; -- Excited Onlooker
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56394; -- Mesmerized Child
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56419; -- Jojo Ironbrow
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56479; -- Legacy of Liu Lang
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56526; -- Tawnyhide Fawn
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56562; -- Anduin Wrynn
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=851, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56592; -- Pearlfin Aqualyte
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56622; -- Brazier
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56661; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56663; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=56686; -- Master Shang Xi
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=56863; -- Sleepy Hozen Brawler
UPDATE `creature_template` SET `mindmg`=3830, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=56870; -- Master Snowdrift
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=57132; -- Wu-Song Villager
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=57207; -- Nourished Yak
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=57258; -- Field Yak
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=57317; -- Skyseeker Sailor
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=57414; -- Temple Guard
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=57419; -- Morning Breeze Villager
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=57420; -- Ten Bitterrage
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=57624; -- Yu Sugargale
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=57709; -- Nourished Yak
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=57712; -- Delivery Cart Tender
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=57720; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=57721; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=57743; -- Nourished Yak
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=57752; -- Quiet Lam
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=57753; -- Ironfist Zhou
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=57853; -- Big Bertha
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=58219; -- Saltback Yearling
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=58342; -- Christopher Lesson
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=58786; -- Swirling Pool
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=58884; -- Sha-Haunted Tigress
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=58885; -- Sha-Haunted Crane
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=58944; -- Kun-Lai Refugee
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59019; -- Binan Villager
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59081; -- Diminutive Cocoon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59154; -- Yak Statuette
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59225; -- Ball of Fire
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59499; -- Nourished Yak
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59551; -- Bopper
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59577; -- Farmstead Slave
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59647; -- Field Hand
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59649; -- Tushui Monk
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59690; -- Freed Farmhand
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59766; -- Hollow Bloom
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59781; -- Thunderpaw Initiate
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59936; -- Armored Carp
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59960; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59962; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59963; -- Jojo Ironbrow
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59986; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59988; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=59989; -- Jojo Ironbrow
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60176; -- Huojin Monk
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60183; -- Trainee Nim
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60244; -- Trainee Guang
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60245; -- Brewer Zhen
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60248; -- Chia-hui Autumnleaf
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60253; -- Brewer Lin
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60259; -- Kong
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60260; -- Jie
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60276; -- Hozen Clinger
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60554; -- Escaped Horde Crewman
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60669; -- Long-Haired Yak
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60713; -- Osul Charger
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60722; -- Deepscale Tormentor
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60729; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60739; -- Steppebeast
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60834; -- Horde Druid
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=60851; -- Delora Lionheart
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=60852; -- Korga Strongmane
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60854; -- Escaped Horde Crewman
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60873; -- Skyseeker Sailor
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60874; -- Escaped Horde Crewman
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60878; -- Alliance Priest
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=60888; -- Korga Strongmane
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=60889; -- Delora Lionheart
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60890; -- Skyseeker Sailor
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60891; -- Escaped Horde Crewman
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60892; -- Jojo Ironbrow
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60893; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60895; -- Alliance Priest
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60896; -- Horde Druid
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60897; -- Injured Sailor
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60898; -- Injured Horde Crewman
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60900; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60917; -- Huo
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60918; -- Shu
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60919; -- Wugou
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60920; -- Dafeng
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=60958; -- Pinning Arrow
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=60997; -- The Wrestler
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=61004; -- Healiss
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=61077; -- Wayward Lamb
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=61260; -- Coach Rok Rok
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=61350; -- Graceful Swan
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=61426; -- Shado-Pan Spike Trap
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=61994; -- Crow
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=62005; -- Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=62210; -- Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=62382; -- Abandoned Kite
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=62539; -- Ball and Chain
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=62601; -- Motherseed
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=62855; -- Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=62856; -- Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=62857; -- Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=62858; -- Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=62860; -- Beast
UPDATE `creature_template` SET `mindmg`=3830, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=62865; -- Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=63239; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=63241; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=63242; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=63556; -- Mogu Statue
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=63603; -- Ban's Balloon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=63740; -- Amber Tap
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=63917; -- Rai
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64087; -- Marri
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64118; -- Erni Tanboshi
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64119; -- Dirki Tanboshi
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64148; -- Fitz Togglescrew
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64158; -- Arenji
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64168; -- Vienh Stormbrew
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64169; -- Ranna
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64382; -- Thunderpaw Initiate
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64481; -- Priest Whitebrow
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64505; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64506; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64514; -- Grand Expedition Yak
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64516; -- Cousin Slowhands
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64524; -- Oil Keg
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64551; -- Duskwing Crow
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64593; -- Korga Strongmane
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64619; -- Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64620; -- Beast
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64745; -- Lowland Porcupine
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64846; -- Springtender Ashani
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64849; -- Kunchong Hatchling
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=64858; -- Airman Hicks
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=64859; -- Sky Captain Callanan
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=64860; -- Captain Kerwin
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=64861; -- Captain Day
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=64862; -- Airman Ellis
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=64863; -- Sky Mage Harlan
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=64864; -- Airman Clifton
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=64865; -- Airman Rogers
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25, `Health_mod`=5 WHERE `entry`=64866; -- Corporal Rasmussen
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64875; -- Lorewalker Amai
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64876; -- Lorewalker Ruolin
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64877; -- Tingting
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64878; -- Young Student
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64879; -- Yin
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64880; -- Nan
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64881; -- Hao
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64885; -- Lorewalker Zan
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=64925; -- Tivilix Bangalter
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65008; -- Huojin Monk
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65035; -- Summer Lily
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65037; -- Treeshaper Shu
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65039; -- Eng Dirtplow
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65040; -- Apothecary Jung
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65042; -- Shun the Serene
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65044; -- Gan Darkcask
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65045; -- Gao Longwagon
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65048; -- Tushui Trainee
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65051; -- Tushui Monk
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65055; -- Hanshi One-Eye
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65056; -- Longbeard the Liar
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65057; -- Fan Slowplow
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65059; -- Jing Stoutgut
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65062; -- Shao-Li Ironbelly
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65064; -- Ki-Ro the Contemplative
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65066; -- Jojo Ironbrow
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65067; -- Steepmaster Tira
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65070; -- Gokan Sharphoe
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65073; -- Naira Watergarden
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65075; -- Hyacinth
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65077; -- Groundskeeper Amalia
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65080; -- Chi Master Lim
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65081; -- Master Cranewing
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65083; -- Foolish Chao
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65084; -- Crimson Butterfly
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65085; -- Jin
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65086; -- Da Na
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65088; -- Groundskeeper Shen
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65089; -- Old Yang
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65093; -- Seeress Weng Wu
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65102; -- Dafeng
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65104; -- Wugou
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65105; -- Shu
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65107; -- Huo
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65146; -- Zach Chow
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65248; -- Kaydee Threesong
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65266; -- David Harrington
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65267; -- Mistfall Brew Keg
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65384; -- Grant
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65392; -- Nibbler
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65467; -- Excited Onlooker
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65468; -- Tushui Monk
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65472; -- Wu-Song Villager
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65473; -- Dai-Lo Farmer
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65535; -- Liu Ze
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65545; -- Huojin Monk
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65550; -- Tushui Monk
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65560; -- Vision of Dafeng
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65574; -- Brad Rhodes
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65576; -- Philip Luke
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65579; -- Eric Thibeau
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65596; -- Lil' Canny
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65599; -- H.A.R.V.E.Y.
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65742; -- Escaped Horde Crewman
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65744; -- Jun-Jun Edgewater
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65745; -- Duyi Edgewater
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65762; -- Shao-Tien Cannon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65870; -- Mei Blackbrow
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65871; -- Lo Blackbrow
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65889; -- Su-Shi
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=65976; -- Bartender Tomro
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66064; -- Bartender Tomro
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66207; -- Master Hsu
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66253; -- Master Kistane
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66254; -- Master Woo
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66255; -- Master Yoon
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66257; -- Master Tsang
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66258; -- Master Cheng
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66297; -- Skyfire Gyrocopter
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66339; -- Horde War Wagon
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66366; -- Captive Pandaren Cub
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66398; -- Garrosh'ar Grunt
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66407; -- Rell Nightwind
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66437; -- Arcanist Xu
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66449; -- Ang the Wise
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66480; -- Skyfire Engineer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66509; -- Amber Kearnen
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66510; -- Sully "The Pickle" McLeary
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66602; -- Petulant Pumpkin
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66654; -- Thunder Hold Supplies
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66671; -- Piddle
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66672; -- Puddle
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66673; -- Poople
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66689; -- Mei Chele
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66690; -- Lamplighter Mu
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66729; -- Krasarang Glider
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66750; -- Nurse Shark
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66943; -- Serpent Statue
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66946; -- Balance Pole
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=66986; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=67009; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=67015; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=67021; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=67066; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=68951; -- Little Jiwon
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=70167; -- Maximo
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=70168; -- Noah
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=70949; -- Jae-Sun Di Fo
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=70950; -- Hilda
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=70951; -- Tom Wat
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=71988; -- Spectral Campfire
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=72006; -- Allie
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1, `Health_mod`=1 WHERE `entry`=73329; -- Cave-In
