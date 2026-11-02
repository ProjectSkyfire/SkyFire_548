-- Correct melee damage on combat-eligible NPCs that carried placeholder
-- mindmg/maxdmg values (0/0, 1/1, 2/2, or an inverted range).
-- Damage comes from the creature_classlevelstats MoP curve, which already
-- holds the weapon damage average, and attack power comes from the same
-- curve. Health and mana modifiers keep their authored values.
--
-- Vehicles, world bosses, helpers, noncombat types, non-attackable creatures,
-- ambiguous scripted objects, and unreferenced templates remain excluded.
-- Re-runnable: each row is set to its final value regardless of current state.
-- 5443 templates.

UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=6; -- Kobold Vermin
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=8, `dmg_multiplier`=1.5 WHERE `entry`=62; -- Gug Fatcandle
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=69; -- Diseased Timber Wolf
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=106; -- Kodo Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=247; -- Billy Maclure
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=251; -- Maybell Maclure
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=252; -- Tommy Joe Stonefield
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=284; -- Brown Horse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=299; -- Young Wolf
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=305; -- White Stallion
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=306; -- Palomino
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=307; -- Pinto
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=308; -- Black Stallion
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=395; -- Markus
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=664; -- Benjamin Carevin
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=704; -- Ragged Timber Wolf
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=705; -- Ragged Young Wolf
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=794; -- Matt
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=795; -- Mark
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=796; -- Joshua
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=797; -- Bo
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=798; -- Solomon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=799; -- Kevin
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=800; -- Kyle
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=801; -- Eric
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=802; -- Jay
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=804; -- Dana
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=805; -- Cameron
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=806; -- John
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=807; -- Lisa
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=810; -- Aaron
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=811; -- Jose
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=1366; -- Adam
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=1367; -- Billy
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=1368; -- Justin
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=1370; -- Brandon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=1371; -- Roman
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=1402; -- Topper McNabb
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=1405; -- Morris Lawry
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=1419; -- Fizzles
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=1429; -- Thurman Schneider
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=1445; -- Jesse Halloran
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=1446; -- Regina Halloran
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=1447; -- Gimlok Rumdnul
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=1501; -- Mindless Zombie
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=1502; -- Wretched Ghoul
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=1508; -- Young Scavenger
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=1512; -- Duskbat
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=1984; -- Young Thistle Boar
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=2032; -- Mangy Nightsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=2955; -- Plainstrider
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=3098; -- Mottled Boar
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=3287; -- Hana'zua
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=3504; -- Gil
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=3520; -- Ol' Emma
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=3892; -- Relara Whitemoon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=4269; -- Chestnut Mare
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=4779; -- Brown Ram
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=4781; -- Snufflenose Gopher
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=4901; -- Sara Pierce
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=4982; -- Thomas
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=5403; -- Riding White Stallion
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=5404; -- Black Stallion
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=5405; -- Pinto
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=5406; -- Palomino
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=5524; -- Caravan Watcher
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=5525; -- Caravan Packhorse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=5607; -- Roger
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=5608; -- Jamin
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=5629; -- Theramore Commando
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=5774; -- Riding Wolf
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=5917; -- Clara Charles
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=6728; -- Narnie
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=2.25 WHERE `entry`=7013; -- Blackrock Guard
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=7690; -- Striped Nightsaber
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=8416; -- Felix Whindlebolt
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=112, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=8537; -- Interloper
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=8881; -- Riding Ram
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=8882; -- Riding Tiger
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=8883; -- Riding Horse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=8884; -- Skeletal Mount
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=8885; -- Riding Raptor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=8962; -- Nida
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=8965; -- Shawn
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=9296; -- Milly Osworth
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=10116; -- Slave
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=10117; -- Tortured Slave
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=10404; -- Pustulating Horror
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=10926; -- Pamela Redpath
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=11122; -- Restless Soul
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=11136; -- Freed Soul
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=11156; -- Green Skeletal Warhorse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=11260; -- Northshire Peasant
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=11996; -- Ashley Bridenbecker
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=12036; -- Grella Stonefist
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=12040; -- Brannik Ironbelly
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12150; -- Riding Kodo (Purple)
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12341; -- Blue Skeletal Horse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12342; -- Brown Skeletal Horse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12343; -- Red Skeletal Horse
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12346; -- Emerald Riding Raptor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12349; -- Turquoise Riding Raptor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12350; -- Violet Riding Raptor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12358; -- Riding Striped Frostsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12359; -- Riding Spotted Frostsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12360; -- Riding Striped Nightsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12363; -- Blue Mechanostrider
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12365; -- Red Mechanostrider
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12366; -- Unpainted Mechanostrider X
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12367; -- Green Mechanostrider
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12372; -- Brown Ram
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12373; -- Gray Ram
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12374; -- White Riding Ram Mount
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12375; -- Chestnut Mare
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=12376; -- Brown Horse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14546; -- Swift Brown Ram
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14547; -- Swift White Ram
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14548; -- Swift Gray Ram
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14551; -- Swift Yellow Mechanostrider
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14552; -- Swift White Mechanostrider
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14553; -- Swift Green Mechanostrider
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14555; -- Swift Mistsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14556; -- Swift Frostsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14558; -- Purple Skeletal Warhorse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14559; -- Swift Palomino
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14560; -- Swift White Steed
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14561; -- Swift Brown Steed
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14602; -- Swift Stormsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14850; -- Gruk
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14857; -- Erk
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14869; -- Pygmy Cockatrice
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14872; -- Trok
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14873; -- Okla
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14874; -- Karu
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14884; -- Parasitic Serpent
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=14908; -- Mogg
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=1, `dmg_multiplier`=1 WHERE `entry`=15274; -- Mana Wyrm
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=15366; -- Springpaw Cub
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=16516; -- Volatile Mutation
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=16520; -- Vale Moth
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=17048; -- Ironforge Firebreather
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=17049; -- Darnassus Firebreather
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=17077; -- Ancestral Spirit Wolf
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=17230; -- Twinkle
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=17250; -- Jia
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=17251; -- Blackbeak
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=17989; -- Archimindy
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=18150; -- Gurgle
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=18151; -- Gargle
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=18360; -- Golden Gryphon
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=18362; -- Swift Purple Gryphon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=18649; -- Innkeeper Monica
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=18653; -- Seth
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=18655; -- Jay Lemieux
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=18656; -- Julie Honeywell
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=18739; -- Giraffe
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=18762; -- Tinkerbell
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=18927; -- Human Commoner
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=18928; -- Astromancer Solarian Spotlight
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=19163; -- Refugee Kid
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=19164; -- Refugee Child
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=19169; -- Blood Elf Commoner
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=19171; -- Draenei Commoner
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=19176; -- Tauren Commoner
UPDATE `creature_template` SET `mindmg`=143, `maxdmg`=245, `attackpower`=276, `dmg_multiplier`=1 WHERE `entry`=19579; -- Cohlien Frostweaver with Hat
UPDATE `creature_template` SET `mindmg`=143, `maxdmg`=245, `attackpower`=276, `dmg_multiplier`=1 WHERE `entry`=19580; -- Luminrath with Cape
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=19659; -- Great Elite Elekk
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=20098; -- Jane
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=20102; -- Goblin Commoner
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=20206; -- Fei Fei
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=20244; -- Nova
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=20432; -- Beggar
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=20434; -- Horse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=20716; -- Rek'tor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=20894; -- Miihi
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=20895; -- Miiji
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=21002; -- Nether Vapor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=21317; -- Aimi
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=21376; -- Crusty Bob
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=22267; -- Magic Sucker Device Buttress
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=22279; -- Nadja
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=22280; -- Soren
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=22314; -- Captive Child
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=22333; -- Orb Collecting Totem
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=22365; -- Ethan
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=1, `dmg_multiplier`=1 WHERE `entry`=22823; -- Hch'uu
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=1, `dmg_multiplier`=1 WHERE `entry`=22836; -- Jheel
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=22921; -- Ethereum Prisoner (Stasis Chamber Alpha)
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=22974; -- Invis Sparrowhawk Origin
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=22985; -- Bradford Bridenbecker
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=23487; -- Wild Wolpertinger
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=24136; -- Feather Vortex
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=24396; -- Forest Frog
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=24464; -- Scourging Crystal
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=24515; -- Spotted Hippogryph Hatchling
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=24653; -- Flying Machine
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=24767; -- Molly
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=25201; -- Winterfin Tadpole
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=25214; -- Shadow Image
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=25265; -- Demonic Vapor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=25267; -- Demonic Vapor (Trail)
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=25305; -- Dancing Flames
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=25603; -- Felfire Portal
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=1, `dmg_multiplier`=1 WHERE `entry`=25652; -- Nerub'ar Scarab
UPDATE `creature_template` SET `mindmg`=212, `maxdmg`=362, `attackpower`=432, `dmg_multiplier`=1 WHERE `entry`=25697; -- Luma Skymother
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=341, `attackpower`=370, `dmg_multiplier`=1 WHERE `entry`=25754; -- Earthen Ring Flamecaller
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=25773; -- Fizzcrank Survivor
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=25931; -- Eversong Woods Flame Keeper
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=25933; -- Ghostlands Flame Keeper
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=25969; -- Jenny
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=26096; -- Gnome Soul
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=26365; -- Taunka Orphan
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=26401; -- Summer Scorchling
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=26500; -- Image of Drakuru
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=26520; -- Festival Scorchling
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=26846; -- Kareg
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=26902; -- Essence of Warlord Jin'arrak
UPDATE `creature_template` SET `mindmg`=177, `maxdmg`=302, `attackpower`=320, `dmg_multiplier`=1 WHERE `entry`=27430; -- Destructive Ward
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=27435; -- Wintergarde Mine Bomb
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=27944; -- Johan
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=28037; -- The Spirit of Gnomeregan
UPDATE `creature_template` SET `mindmg`=167, `maxdmg`=284, `attackpower`=314, `dmg_multiplier`=1 WHERE `entry`=28512; -- Quartermaster Ozorg
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=28526; -- Released Offspring of Har'koa
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=28621; -- Grayson Ironwing
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=28821; -- Mine Car
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=28845; -- Scarlet Ghoul
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=29139; -- Naznik Sureshave
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=29141; -- Pella Brassbrush
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=29142; -- Jelinek Sharpshear
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=29143; -- Bebri Coifcurl
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=29151; -- Cricket
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=29346; -- Apothecary Karlov
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=29630; -- Fanged Pit Viper
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=1, `dmg_multiplier`=1 WHERE `entry`=29939; -- Storm Cloud
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=30219; -- Ethereal Frostworg
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=30835; -- Image of a Shadow Cultist
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=30836; -- Image of Vardmadra
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=30882; -- Twilight Egg
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=2.25 WHERE `entry`=30890; -- Twilight Whelp
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=30891; -- Blessed Banner of the Crusade
UPDATE `creature_template` SET `mindmg`=212, `maxdmg`=409, `attackpower`=528, `dmg_multiplier`=1 WHERE `entry`=30900; -- Argent Mason
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=30945; -- Vardmadra
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=30987; -- Hideous Plaguebringer
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=31111; -- Night Elf Mohawk
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=31214; -- Sartharion Twilight Whelp
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=31254; -- Lordaeron Footsoldier
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=31276; -- Ghoulish Minion
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=31426; -- Doras
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=31433; -- Innkeeper Gryshka
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=31690; -- Infra-Green Flight Master
UPDATE `creature_template` SET `mindmg`=254, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=31693; -- Stormforged Saboteur
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=31797; -- Ancient Sentinel
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=32370; -- Refurbished Demolisher
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=32414; -- Lordaeron Captain
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=32423; -- Matthias Lehner
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=32497; -- Matthias Lehner
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=32663; -- Budd
UPDATE `creature_template` SET `mindmg`=200, `maxdmg`=341, `attackpower`=175, `dmg_multiplier`=1 WHERE `entry`=32798; -- Spring Gatherer
UPDATE `creature_template` SET `mindmg`=200, `maxdmg`=341, `attackpower`=175, `dmg_multiplier`=1 WHERE `entry`=32799; -- Spring Collector
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=32836; -- Noblegarden Vendor
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=32837; -- Noblegarden Merchant
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=22, `dmg_multiplier`=1 WHERE `entry`=32855; -- Shatterspear Priestess
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=32858; -- Shatterspear Champion
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=32859; -- Horde Enforcer
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=46, `dmg_multiplier`=1 WHERE `entry`=32860; -- Shatterspear Shaman
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=32861; -- Shatterspear Laborer
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=2.25 WHERE `entry`=32862; -- Jor'kil the Soulripper
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=32863; -- Shatterspear Overseer
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=1 WHERE `entry`=32868; -- Lorenth Thundercall
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=1 WHERE `entry`=32869; -- Sheya Stormweaver
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=32888; -- Twilight Fanatic
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=32890; -- Enraged Tidal Spirit
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=46, `dmg_multiplier`=1 WHERE `entry`=32899; -- High Cultist Azerynn
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=32912; -- Sentinel Lendra
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=32928; -- Vile Spray
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=32932; -- Moon Priestess Tharill
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=32935; -- Corrupted Tide Crawler
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=32963; -- Lieutenant Morra Starbreeze
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=32964; -- Sentinel Aynasha
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=32965; -- Sentinel Tysha Moonblade
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=32966; -- Balthule Shadowstrike
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=58, `dmg_multiplier`=1 WHERE `entry`=32967; -- Elder Brolg
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=58, `dmg_multiplier`=1 WHERE `entry`=32968; -- Gren Tornfur
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=32969; -- Lor'danel Sentinel
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=46, `dmg_multiplier`=1 WHERE `entry`=32970; -- Rit'ko
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=32971; -- Ranger Glynda Nal'Shea
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=32972; -- Serendia Oakwhisper
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=32973; -- Dentaria Silverglade
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=32974; -- Laird
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=32975; -- Decomposing Thistle Bear
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=32977; -- Wizbang Cranktoggle
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=32978; -- Tharnariun Treetender
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=32979; -- Gorbold Steelhand
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=2.25 WHERE `entry`=32987; -- Corvine Moonrise
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=1 WHERE `entry`=32988; -- Cloudtamer Wildmane
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=1 WHERE `entry`=32989; -- Windmaster Tzu-Tzu
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=1 WHERE `entry`=32990; -- Skylord Braax
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=32996; -- Sharax the Defiler
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=32997; -- Fleetfoot
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=32999; -- Unbound Fire Elemental
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=33001; -- Thundris Windweaver
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=33009; -- Corrupted Thistle Bear
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=33020; -- Zenn Foulhoof
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=33021; -- Vile Grell
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=33022; -- Vile Corruptor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=33023; -- Uncorrupted Duskrat
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=33024; -- Uncorrupted Thistle Bear
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=33033; -- Sentinel Elissa Starbreeze
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=33035; -- Taldan
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=2.25 WHERE `entry`=33037; -- Caylais Moonfeather
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=1 WHERE `entry`=33039; -- Enraged Hippogryph
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=52, `dmg_multiplier`=2.25 WHERE `entry`=33041; -- Aetherion
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=33043; -- Maddened Blackwood
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=33044; -- Corrupted Blackwood
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=33048; -- Keeper Karithus
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=33053; -- Grimclaw
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=33055; -- Alanndarian Nightsong
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=33057; -- Twilight Zealot
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=33058; -- Enthralled Earth Elemental
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=33071; -- Shatterspear Raider
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=33072; -- Onu
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=33079; -- Darkscale Myrmidon
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=33083; -- Enraged Earth Elemental
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=58, `dmg_multiplier`=1 WHERE `entry`=33084; -- Blackwood Furbolg
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=33106; -- Auberdine Refugee
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=33107; -- Priestess Alinya
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=33112; -- Selenn
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=33115; -- Lor'danel Sentinel
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=33117; -- Elder Brownpaw
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=58, `dmg_multiplier`=1 WHERE `entry`=33119; -- Aroom
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=33126; -- Seraphine
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=33127; -- Moonstalker
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=33175; -- Johnathan Staats
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=33176; -- Kerlonian Evershade
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=33177; -- Arya Autumnlight
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=2.25 WHERE `entry`=33178; -- Huntress Sandrya Moonfall
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=33179; -- Cursed Highborne
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=33180; -- Writhing Highborne
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=33181; -- Anaya Dawnrunner
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=33182; -- Bathran
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=33183; -- Bathran's Corpse
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=33187; -- Sentinel Shyela
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=33193; -- Ashenvale Skirmisher
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=33195; -- Ashenvale Bowman
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=33201; -- Mor'shan Defender
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=33204; -- Evenar Stillwhisper
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=33206; -- Darkscale Scout
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=18, `dmg_multiplier`=1 WHERE `entry`=33207; -- Lady Janira
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=30, `dmg_multiplier`=1 WHERE `entry`=33208; -- Orendil's Apprentice
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=33209; -- Visera Softloam
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=33231; -- Elisa Steelhand
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=33232; -- Archaeologist Hollee
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=58, `dmg_multiplier`=1 WHERE `entry`=33250; -- Foriel Broadleaf
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=33253; -- Delanea
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=46, `dmg_multiplier`=1 WHERE `entry`=33262; -- Greymist Tidehunter
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=33263; -- Dinah Halfmoon
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=33265; -- Varas
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=33266; -- Wounded Mor'shan Defender
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=35, `dmg_multiplier`=2.25 WHERE `entry`=33276; -- Moon Priestess Maestra
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=33277; -- Greymist Warrior
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=33278; -- Maestra's Post Sentinel
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=33281; -- Mor'shan Watchman
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=33284; -- Truun
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=33294; -- Gorat
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=33295; -- Fallen Horde Assassin
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=33311; -- Darkshore Stag
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=33334; -- Horde Invader
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=1 WHERE `entry`=33345; -- Luk'gar
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=33347; -- Delgren the Purifier
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=33348; -- Feero Ironhand
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=34, `dmg_multiplier`=1 WHERE `entry`=33356; -- Orux Thrice-Damned
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=33359; -- Nightsaber Rider
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=33375; -- Injured Sentinel
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=33381; -- Tomarrek
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=33389; -- Thistlefur Wise One
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=33390; -- Thistlefur Warrior
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=33421; -- Gorka
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=33440; -- Demoralized Peon
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=2.25 WHERE `entry`=33443; -- Keeper Heartwise
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=1 WHERE `entry`=33444; -- Harbinger Aphotic
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=33445; -- Sentinel Avana
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=33451; -- Ashenvale Nightsaber
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=33454; -- Sentinel Luara
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=33456; -- Gorick Guzzledraught
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=2.25 WHERE `entry`=33497; -- Modan Monster
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=33512; -- Palanaar's Elekk
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=2.25 WHERE `entry`=33688; -- Raging Ancient
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=33727; -- Anchorite Buurq
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=33728; -- Draaka
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=1 WHERE `entry`=33760; -- Durak
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=33766; -- Splintertree Peon
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=33777; -- Gaivan Shadewalker
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=33825; -- Splintertree Overseer
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=2.25 WHERE `entry`=33864; -- Vile Terror
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=33903; -- Thistle Bear Cub
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=33905; -- Corrupted Thistle Bear Matriarch
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=33978; -- Hungry Thistle Bear
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=33980; -- Apothecary Furrows
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=33981; -- Greef
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=34033; -- Teegan Holloway
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=34041; -- Mathas Wildwood
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=34046; -- Forsaken Looter
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=34056; -- Vigilant Protector
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=34103; -- Keynira Owlwing
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=34122; -- Commander Grimfang
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=34164; -- Mechagnome Battletank
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=34177; -- Ashenvale Scout
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=34195; -- Kulg Gorespatter
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=1 WHERE `entry`=34204; -- Protector Arminon
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=34205; -- Gary
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=1 WHERE `entry`=34208; -- Protector Endolar
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=34232; -- Envoy Sheelah
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=37, `dmg_multiplier`=1 WHERE `entry`=34233; -- Guardian Menerin
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=34238; -- Black Skeletal Horse
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=74, `dmg_multiplier`=2.25 WHERE `entry`=34239; -- Hephaestus Pilgrim
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=34241; -- Warsong Guardian
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=34242; -- Guardian Gurtar
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=34248; -- Shatterspear Mystic
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=34251; -- Vear Darksnout
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=34258; -- Halga Bloodeye
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=34259; -- Caravan Kodo
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=34261; -- Caravan Peon
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=94, `dmg_multiplier`=2.25 WHERE `entry`=34283; -- Sabina Pilgrim
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=34284; -- Dorak
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=34285; -- Trapped Wolf
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=74, `dmg_multiplier`=2.25 WHERE `entry`=34289; -- The Vortex
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=74, `dmg_multiplier`=2.25 WHERE `entry`=34290; -- Core
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=74, `dmg_multiplier`=2.25 WHERE `entry`=34292; -- Arctanus
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=1 WHERE `entry`=34294; -- Protector Dorinar
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=2.25 WHERE `entry`=34295; -- Lord Magmathar
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=34299; -- Earth Elemental Remains
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=58, `dmg_multiplier`=1 WHERE `entry`=34301; -- Kathrena Winterwisp
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=34302; -- Consumed Thistle Bear
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=34303; -- Dagrun Ragehammer
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=34304; -- Wildkin Spirit
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=34309; -- Shatterspear Raider
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=74, `dmg_multiplier`=2.25 WHERE `entry`=34314; -- Lava Rager
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=34315; -- Marauding Poacher
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=2.25 WHERE `entry`=34316; -- Twilight Portal
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=34318; -- Whitetail Stag
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=34326; -- Doomspeaker Trevellion
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=34335; -- Avrus Illwhisper
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=34339; -- Greymist Refugee
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=34340; -- Archaeologist Groff
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=2.25 WHERE `entry`=34341; -- Saurboz
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=34342; -- Jr. Archaeologist Ferd
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=34343; -- Prospector Remtravel
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=34345; -- Oil Canister
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=34350; -- Dangerfish
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=34351; -- Fiendishfish
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=47, `attackpower`=84, `dmg_multiplier`=2.25 WHERE `entry`=34354; -- Huntress Jalin
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=34359; -- Captain Goggath
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=34366; -- Warsong Vanguard
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=34374; -- Solais
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=1 WHERE `entry`=34377; -- Halannia
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=34378; -- Myre Moonglide
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=34385; -- Horoo the Flamekeeper
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=34392; -- Orseus
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=34395; -- Tweedle
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=34396; -- Whitetail Doe
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=34402; -- Balren of the Claw
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=34403; -- Felros
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=34404; -- Larien
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=34405; -- Twilight Worker
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=34406; -- Foreman Balsoth
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=34413; -- Faceless One
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=34414; -- Darkscale Siren
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=34415; -- Darkscale Priestess
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=34417; -- Young Grizzled Thistle Bear
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=34419; -- Astranaar Skirmisher
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=34420; -- Astranaar Officer
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=34423; -- Warlord Wrathspine
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=34426; -- Laughing Sister
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=34427; -- Thalya the Voidcaller
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=34429; -- Thraka
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=34498; -- Darkscale Assassin
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=78, `dmg_multiplier`=2.25 WHERE `entry`=34499; -- Oso Bramblescar
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=34500; -- Shady Nook Guardian
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=34503; -- Razormane Pillager
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=34504; -- Grol'dom Grunt
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=34510; -- Broyk
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=34511; -- Forsaken Invader
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=46, `dmg_multiplier`=1 WHERE `entry`=34513; -- Togrik
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=34517; -- Ally of Nature
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=2.25 WHERE `entry`=34518; -- Thagg
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=34522; -- Corrupted Servant
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=34523; -- Captured Razormane
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=34524; -- Corrupted Minion
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=34525; -- Corrupted Gnarlpine
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=34530; -- Ancient Teldrassil Protector
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=34544; -- Tortusk
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=34545; -- Razormane Frenzy
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=34547; -- Grol'dom Kodo
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=34559; -- Captain Tarkan
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=34560; -- Una Wolfclaw
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=34563; -- Injured Grol'dom Grunt
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=34569; -- Flooz
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=34571; -- Gwen Armstead
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=34576; -- Crossroads Caravan Kodo
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=34578; -- Rocco Whipshank
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=34590; -- Goblin Technician
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=34591; -- Chief Bombgineer Sploder
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=34592; -- Silverwind Conqueror
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=34596; -- Foreman Jinx
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=2.25 WHERE `entry`=34608; -- Big Baobob
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=34613; -- Ta'jari
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=34617; -- Captured Furbolg
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=34618; -- Ota Wen
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=34624; -- Elestren
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=34626; -- Jerrik Highmountain
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=34634; -- Gorgal Angerscar
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=34638; -- Shoe
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=34640; -- Barrens Vulture
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=34647; -- Barrens Boar
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=34651; -- Sashya
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=34656; -- Hargash
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=34668; -- Sassy Hardwrench
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=34673; -- Bamm Megabomb
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=34674; -- Brak Blusterpipe
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=34689; -- Fizz Lighter
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=34692; -- Sister Goldskimmer
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=34, `dmg_multiplier`=1 WHERE `entry`=34693; -- Slinky Sharpshiv
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=34695; -- Maxx Avalanche
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=34696; -- Evol Fingers
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=34697; -- Warrior-Matic NX-01
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=34698; -- Nozzlepot
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=34699; -- Bomb-Throwing Monkey
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=1 WHERE `entry`=34706; -- Theramore Sharpshooter
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=46, `dmg_multiplier`=1 WHERE `entry`=34707; -- Theramore Deck Hand
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=34727; -- Sleepy Joe
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=34729; -- Charlie
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=34733; -- Rear Admiral Hartley
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=34747; -- Glomp
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=34749; -- Tony Two-Tusk
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=34750; -- Cap'n Garvey
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=34752; -- Lieutenant Pyre
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=34753; -- Lieutenant Buckland
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=34754; -- Chief Engineer Foote
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=34756; -- Moriana Dawnlight
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=34757; -- Doranel Amberleaf
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=34759; -- Carousing Peon
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=34761; -- Citarre Mapleheart
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=34763; -- Daggertooth Shark
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=34804; -- Chef Toofus
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=34805; -- Stonetotem Rifleman
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=34807; -- Malaka'jin Witch Doctor
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=34809; -- Stonetalon Grunt
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=34829; -- King Reaperclaw
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=34830; -- Defiant Troll
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=34835; -- Bruno Flameretardant
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=34841; -- Telar Highstrider
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=1 WHERE `entry`=34848; -- Honor's Stand Sharpshooter
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=34850; -- Prince Liam Greymane
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=34851; -- Panicked Citizen
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=34855; -- Honor's Stand Peasant
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=34863; -- Lieutenant Walden
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=34864; -- Gilneas City Guard
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=34865; -- Tunneling Worm
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=34872; -- Foreman Dampwick
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=34874; -- Megs Dreadshredder
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=34876; -- Frankie Gearslipper
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=34877; -- Jack the Hammer
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=34878; -- Sudsy Magee
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=34884; -- Rampaging Worgen
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=34886; -- Kalimdor Eagle
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=34890; -- Izzy
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=34894; -- Stonetalon Ram
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=2.25 WHERE `entry`=34896; -- Seldarria
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=34897; -- Black Drake
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=55, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=34898; -- Black Dragon Whelp
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=34911; -- Fissure-born Fire Elemental
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=34913; -- Prince Liam Greymane
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=34916; -- Gilneas City Guard
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=94, `dmg_multiplier`=1 WHERE `entry`=34923; -- Harbinger Aph'lass
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=34927; -- Gort Goreflight
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=34931; -- Highborne Charlatan
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=34932; -- Highborne Sorcerer
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=34933; -- Lady Benel'derath
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=34934; -- Lord Ro'minate
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=34936; -- Gwen Armstead
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=34937; -- Councilman Stark I'ylar
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=34938; -- Highborne Citizen
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=34939; -- Guardsman Belothiel
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=34940; -- Illyanna Stonewood
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=34941; -- Allendril Fallcreek
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=34943; -- Wind Tamer Shoshok
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=34945; -- Highborne Lass
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=34946; -- Merchant Kendra
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=34954; -- Gobber
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=34968; -- Stonetalon Sentinel
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=34969; -- Corrupted Sentinel
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=34982; -- Emerald Spirit
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=35053; -- Candy Cane
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=35054; -- Chip Endale
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=35063; -- Kezan Citizen
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=12, `attackpower`=9, `dmg_multiplier`=1 WHERE `entry`=35075; -- Kezan Citizen
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=2.25 WHERE `entry`=35077; -- Lord Darius Crowley
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=35081; -- Sean Dempsey
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=35086; -- Labor Captain Grabbit
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=35095; -- Talrendis Scout
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=35096; -- Weakened Mosshoof Stag
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=35111; -- Runaway Shredder
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=35112; -- King Genn Greymane
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=35115; -- Lord Godfrey
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=35118; -- Bloodfang Worgen
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=35120; -- FBoK Bank Teller
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=35123; -- Vincent Hersham
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1 WHERE `entry`=35124; -- Tobias Mistmantle
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=35126; -- Gappy Silvertooth
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=35128; -- Szabo
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=35130; -- Missa Spekkies
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=35136; -- Kaluna Songflight
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=35137; -- Allana Swiftglide
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=35138; -- Ceyora
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=35139; -- Zillane
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=35140; -- Kormal the Swift
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=35141; -- Orna Skywatcher
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=35142; -- Ergll
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=2.25 WHERE `entry`=35150; -- Darnassian Glaive Thrower
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=35151; -- Stonetalon Warder
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=2.25 WHERE `entry`=35152; -- Battlescar Ancient
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=35153; -- Draenei Paladin
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=55, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=35154; -- Night Elf Mage
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=35158; -- Krom'gar Berserker
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=55, `attackpower`=94, `dmg_multiplier`=1 WHERE `entry`=35159; -- Krom'gar Witch Doctor
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=35160; -- Krom'gar Logger
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=35161; -- Krom'gar Warrior
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=2.25 WHERE `entry`=35163; -- Krom'gar Demolisher
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=35168; -- Striped Dawnsaber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=35169; -- Ochre Skeletal Warhorse
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=35174; -- Krom'gar Peon
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=289, `dmg_multiplier`=1 WHERE `entry`=35187; -- Image of Archmage Xylem
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=2.25 WHERE `entry`=35198; -- Talrendis Ancient
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=35204; -- Goblin Harvester
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=35222; -- Trade Prince Gallywix
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=35232; -- Gilnean Royal Guard
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=35233; -- Gilnean Survivor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=35239; -- Troll Slave
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=35245; -- Greystone Basilisk
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=35257; -- Mountainfoot Miner
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=35296; -- Explosives Stockpile
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=35304; -- Brute Enforcer
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=35312; -- Talrendis Saboteur
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=35333; -- Goblin Technician
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=35334; -- Gnome Engineer
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=35364; -- Slahtz
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=35365; -- Behsten
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=35369; -- Josiah Avery
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=35378; -- Lorna Crowley
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=35456; -- Worgen Runt
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=35457; -- Bloodfang Bloodletter
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1 WHERE `entry`=35463; -- Bloodfang Lurker
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=35466; -- Restless Spirit
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=35484; -- Survey Equipment
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=35504; -- Gilneas City Guard
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=35505; -- Bloodfang Ripper
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=35509; -- Gilneas City Guard
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=35550; -- King Genn Greymane
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=35551; -- Prince Liam Greymane
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=35552; -- Lord Darius Crowley
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=35554; -- Shaken Survivor
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=35566; -- Lord Darius Crowley
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=35618; -- Tobias Mistmantle
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=35623; -- Villa Mook
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=35648; -- Will Robotronic
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=35650; -- Sassy Hardwrench
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=35657; -- Torg Twocrush
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=35753; -- Krennan Aranas
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=18, `dmg_multiplier`=1 WHERE `entry`=35754; -- Quarla Whistlebreak
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=22, `dmg_multiplier`=1 WHERE `entry`=35755; -- Tharkul Ironskull
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=35756; -- Teresa Spireleaf
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=35758; -- Bamm Megabomb
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=2.25 WHERE `entry`=35759; -- Balboa
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=35769; -- Foreman Dampwick
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=35778; -- Evol Fingers
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=35780; -- Fizz Lighter
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=35786; -- Maxx Avalanche
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=35805; -- Sister Goldskimmer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=42, `dmg_multiplier`=1 WHERE `entry`=35806; -- Slinky Sharpshiv
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=35807; -- Warrior-Matic NX-01
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=35810; -- Frightened Miner
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=35812; -- Smart Mining Monkey
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=35817; -- Bombardier Captain Smooks
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=35829; -- Jr. Bombardier Hackel
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=35831; -- Spitelash Stormfury
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=35832; -- Spitelash Seacaller
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=35833; -- Spitelash Priestess
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=35837; -- Dead Orc Scout
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=35838; -- Pygmy Witchdoctor
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=35839; -- Sergeant Cleese
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=35840; -- Gwen Armstead
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=35867; -- Kalec
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=35869; -- Vitus Darkwalker
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=35870; -- Sister Almyra
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=35871; -- Loren the Fence
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=9, `dmg_multiplier`=2.25 WHERE `entry`=35872; -- Myriam Spellwaker
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=35873; -- Celestine of the Harvest
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=35874; -- Huntsman Blake
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=35875; -- Aggra
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=2.25 WHERE `entry`=35880; -- Lady Silisthra
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=2.25 WHERE `entry`=35881; -- Lady Vesthra
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=35882; -- Orc Survivor
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=34, `dmg_multiplier`=1 WHERE `entry`=35891; -- SI:7 Infiltrator
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=35892; -- Silisthra's Power Stone
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=35893; -- Kilag Gorefang
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=35894; -- Orc Scout
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=35896; -- Poison Spitter
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=35897; -- Freezya
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=35904; -- Jungle Cat
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=35906; -- Lord Godfrey
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=35911; -- King Genn Greymane
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=35912; -- Injured Citizen
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=35915; -- Gilneas City Guard
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=35916; -- Bloodfang Ripper
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=35917; -- Kilag Gorefang
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=35968; -- Vesthra's Power Stone
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=46, `dmg_multiplier`=1 WHERE `entry`=36012; -- Blackmaw Shaman
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36013; -- Blackmaw Warrior
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36015; -- Blackmaw Pathfinder
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36057; -- Northgate Rebel
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=36061; -- Research Intern
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=36063; -- Orc Scout
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=36077; -- Assistant Greely
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=36092; -- SI:7 Assassin
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36103; -- SI:7 Operative
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=36104; -- Orc Battlesworn
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=36112; -- Scout Brax
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=36115; -- Aggra
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=36117; -- Kilag Gorefang
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=36127; -- Gyrochoppa
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36129; -- Gyrochoppa Pilot
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=36131; -- Vile Splash
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=36132; -- Krennan Aranas
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=28, `dmg_multiplier`=2.25 WHERE `entry`=36140; -- Prince Liam Greymane
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36146; -- Twistex Happytongs
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=36147; -- Static-Charged Hippogryph
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36149; -- Alliance Sailor
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36153; -- Alliance Captain
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36170; -- Lord Godfrey
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36179; -- Goblin Survivor
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36180; -- Ace
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36184; -- Izzy
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36186; -- Gobber
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=28, `dmg_multiplier`=2.25 WHERE `entry`=36190; -- Prince Liam Greymane
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=36205; -- Slain Watchman
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=36210; -- Sorata Firespinner
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36211; -- Duskhaven Watchman
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=2.25 WHERE `entry`=36231; -- Horrid Abomination
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36236; -- Forsaken Footsoldier
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=36287; -- Cynthia
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=36288; -- Ashley
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=36289; -- James
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36290; -- Lord Godfrey
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36291; -- Melinda Hammond
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=36293; -- Ogre Minion
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=2.25 WHERE `entry`=36294; -- Koroth the Hillbreaker
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=36304; -- Mistwing Cliffdweller
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=2.25 WHERE `entry`=36312; -- Dark Ranger Thyala
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36344; -- Frightened Miner
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=36370; -- Lux
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=36371; -- Morta the Wretch
UPDATE `creature_template` SET `mindmg`=254, `maxdmg`=434, `attackpower`=260, `dmg_multiplier`=1 WHERE `entry`=36372; -- Joanna
UPDATE `creature_template` SET `mindmg`=131, `maxdmg`=223, `attackpower`=262, `dmg_multiplier`=1 WHERE `entry`=36373; -- Coral Moongale
UPDATE `creature_template` SET `mindmg`=167, `maxdmg`=284, `attackpower`=314, `dmg_multiplier`=1 WHERE `entry`=36374; -- Nyrill
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=27, `dmg_multiplier`=1 WHERE `entry`=36375; -- Tex Vortacoil
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=36376; -- Fib Gyrojolt
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=2.25 WHERE `entry`=36379; -- Hobart Grapplehammer
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=36384; -- Zapper Gnome
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=36385; -- Netgun Gnome
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36396; -- Forsaken Sailor
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36397; -- Captain Anson
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36399; -- Captain Morris
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=36403; -- Trade Prince Gallywix
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36404; -- Candy Cane
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36405; -- Attack Mastiff
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36406; -- Chip Endale
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=36417; -- Kilag Gorefang
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=36418; -- Aggra
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36421; -- Orc Survivor
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36422; -- Orc Scout
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=36423; -- Orc Battlesworn
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=36424; -- Scout Brax
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36425; -- Sassy Hardwrench
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36426; -- Goblin Survivor
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36427; -- Brett "Coins" McQuid
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36428; -- Ace
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36429; -- Izzy
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36430; -- Sally "Salvager" Sandscrew
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36432; -- Chawg
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36440; -- Drowning Watchman
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=28, `dmg_multiplier`=2.25 WHERE `entry`=36451; -- Prince Liam Greymane
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=36452; -- Gwen Armstead
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36453; -- Duskhaven Villager
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36454; -- Tim Hayward
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36455; -- Walt Hayward
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36456; -- Sebastian Hayward
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36457; -- Lorna Crowley
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36458; -- Grandma Wahl
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36460; -- Forsaken Survivor
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36461; -- Lucius the Cruel
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36463; -- Gobber
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36464; -- Chawg
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36465; -- Chawg
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36466; -- Brett "Coins" McQuid
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36467; -- Sally "Salvager" Sandscrew
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36468; -- Candy Cane
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36469; -- Chip Endale
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36470; -- Foreman Dampwick
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36471; -- Foreman Dampwick
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=36472; -- Door Buzzer
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36488; -- Forsaken Castaway
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36491; -- Trent Hayward
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36492; -- Ron Hayward
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36496; -- Grimy Greasefingers
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36501; -- Grimy Greasefingers
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36509; -- Experimental Raptor
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36512; -- Fox
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=36518; -- Bamm Megabomb
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36519; -- Evol Fingers
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36520; -- Fizz Lighter
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=36521; -- Maxx Avalanche
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36523; -- Sister Goldskimmer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=42, `dmg_multiplier`=1 WHERE `entry`=36524; -- Slinky Sharpshiv
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=36525; -- Warrior-Matic NX-01
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=2.25 WHERE `entry`=36528; -- Koroth the Hillbreaker
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=36578; -- Bastia
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=36592; -- Apprentice Investigator
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=36593; -- Apprentice Illuminator
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=36594; -- Lorekeeper Amberwind
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=36596; -- Andorel Sunsworn
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=36599; -- Arcane Construct
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36600; -- Geargrinder Gizmo
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36602; -- Duskhaven Watchman
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36606; -- Queen Mia Greymane
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=36611; -- Talrendis Biologist
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=26, `dmg_multiplier`=1 WHERE `entry`=36614; -- Talrendis Ambassador
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36615; -- Doc Zapnozzle
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=2.25 WHERE `entry`=36616; -- Admiral Nightwind
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36628; -- Celestine of the Harvest
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36629; -- Huntsman Blake
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=36630; -- Loren the Fence
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=9, `dmg_multiplier`=2.25 WHERE `entry`=36631; -- Myriam Spellwaker
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36632; -- Sister Almyra
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=36636; -- Twilight Dragon Hunter
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=27, `dmg_multiplier`=1 WHERE `entry`=36637; -- Twilight Desecrator
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=36638; -- Twilight Lord Katrana
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=36639; -- Sable Drakonid
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=36644; -- Ahmo Thunderhorn
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=2.25 WHERE `entry`=36648; -- Baine Bloodhoof
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36651; -- Sergeant Cleese
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36652; -- Vitus Darkwalker
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36653; -- Duskhaven Watchman
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=36660; -- Talrendis Marksman
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36671; -- Forsaken Scout
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=36674; -- Nambria
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=36680; -- Captain Grunwald
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=36681; -- Ravenous Lurker
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=2.25 WHERE `entry`=36682; -- The Hammer
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=36687; -- Mariel Dawnsong
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=2.25 WHERE `entry`=36688; -- Northwatch Bodyguard
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36690; -- Duskhaven Watchman
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36693; -- Unconscious Watchman
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=36694; -- Adana Thunderhorn
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=9, `dmg_multiplier`=1 WHERE `entry`=36695; -- Samantha Buckley
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=36696; -- Armored Battleboar
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=36697; -- Bristleback Thorncaller
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=28, `dmg_multiplier`=2.25 WHERE `entry`=36698; -- Captive Worgen
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=36708; -- Bristleback Gun Thief
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36712; -- Chief Squealer Thornmantle
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36713; -- Gilnean Mastiff
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36717; -- Gerard Walthorn
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36719; -- Pterrordax Scavenger
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=36722; -- Bilgewater Laborer
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=36728; -- Kroum
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=36730; -- Chawg
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36732; -- Ancient Island Turtle
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=2.25 WHERE `entry`=36735; -- Teraptor Matriarch
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36740; -- Teraptor Hatchling
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36742; -- Princess Tess Greymane
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=36743; -- King Genn Greymane
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36744; -- Glix Grindlock
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36746; -- Xiz "The Eye" Salvoblast
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36748; -- Captain Grektar
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36749; -- Commander Molotov
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=36756; -- Dead Soldier
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=9, `dmg_multiplier`=1 WHERE `entry`=36779; -- Marie Allen
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36795; -- Ruckus
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=28, `dmg_multiplier`=2.25 WHERE `entry`=36797; -- Captive Worgen
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=28, `dmg_multiplier`=2.25 WHERE `entry`=36798; -- Captive Worgen
UPDATE `creature_template` SET `mindmg`=239, `maxdmg`=409, `attackpower`=528, `dmg_multiplier`=1 WHERE `entry`=36799; -- Recruiter Burns
UPDATE `creature_template` SET `mindmg`=47, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=36803; -- Dyami Windsoar
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=36809; -- Forsaken Invader
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=36813; -- Vilebrood Skitterer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=36814; -- Talran of the Wild
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=36815; -- Valormok Grunt
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=36816; -- Talrendis Defender
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=36818; -- Captain Grembul
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=36819; -- Crewman Bannon
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=36820; -- Billyclub Billy
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=36821; -- Belindah
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36825; -- Grimtotem Straggler
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36827; -- Grimtotem Vagabond
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=36828; -- Grimtotem Defiler
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=36845; -- Agitated Earth Spirit
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=36849; -- Talrendis Lorekeeper
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=2.25 WHERE `entry`=36868; -- Enslaved Son of Arkkoroc
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=36882; -- Swamp Crocolisk
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=36884; -- Commander Jarrodenus
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=2.25 WHERE `entry`=36890; -- Talrendis Glaive Thrower
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36894; -- Tora Halotrix
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36895; -- Sergeant Zelks
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=36900; -- Grounded Wind Rider
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=36902; -- Wind Rider Gorsch
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=36903; -- Jr. Bombardier Hackel
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=36914; -- Talrendis Sentinel
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=36915; -- Erunak Stonespeaker
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=36917; -- Surface to Other Surface Transport
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=36918; -- Lorekeeper's Summoning Stone
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=36919; -- Andorel Sunsworn
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36920; -- Lieutenant Drex
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=36922; -- Wounded Soldier
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=36925; -- Bilgewater Soldier
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=36936; -- Mountainfoot Grunt
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=36943; -- Bristleback Invader
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=36956; -- Captain Desoto
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=36958; -- Hulking Labgoblin
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=36959; -- Uncle Bedlam
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=36962; -- Injured Villager
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36973; -- Patch
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36974; -- Newt
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36975; -- Volt
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36976; -- Ticker
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=36980; -- Ice Tomb
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36987; -- Sergeant Dynamo
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36988; -- Private Permudo
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=36989; -- Spitelash Invader
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=36999; -- Teemo
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=2.25 WHERE `entry`=37002; -- Cliff Crasher
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=37005; -- Friz Groundspin
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=37009; -- Bleenik Fizzlefuse
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=37010; -- Artillery Technician
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=37015; -- Friz's Gyrocopter
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=37024; -- Una Wildmane
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=37045; -- Rygna
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=37054; -- Goblin Supermodel
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=37055; -- Goblin Supermodel
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=37056; -- Goblin Supermodel
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=37057; -- Goblin Supermodel
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=37061; -- Jellix Fuselighter
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=37064; -- Feno Blastnoggin
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=28, `dmg_multiplier`=2.25 WHERE `entry`=37065; -- Prince Liam Greymane
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=37067; -- Crash Survivor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=37070; -- Rockjaw Invader
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=37073; -- Rockjaw Goon
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=37080; -- Wounded Coldridge Mountaineer
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=1 WHERE `entry`=37081; -- Joren Ironstock
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=37083; -- Terrortooth Runner
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=37084; -- Terrortooth Scytheclaw
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=37085; -- Towering Plainstrider
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=37086; -- Hecklefang Scavenger
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=37087; -- Jona Ironstock
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=37088; -- Elder Zhevra
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=37090; -- Deviate Terrortooth
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=37091; -- Deviate Plainstrider
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=37092; -- Outgrowth
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=37093; -- Lashvine
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=37102; -- Gwen Armstead
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=37105; -- Rockjaw Scavenger
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=37106; -- Coach Crosscheck
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=37108; -- Soothsayer Shi'kala
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=37112; -- Wayward Fire Elemental
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=37115; -- Teo Hammerstorm
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=37121; -- Teegli Merrowith
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=37136; -- Oltarg
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=37138; -- Onatay
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=37140; -- Airborne Priest
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=2.25 WHERE `entry`=37142; -- Gurlorn
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=37145; -- Military Gyrocopter
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=1 WHERE `entry`=37152; -- Image of Archmage Xylem
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=37154; -- Kilrok Gorehammer
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=37157; -- Captain Peake
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=37160; -- Tower Warden
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=37161; -- Honor's Stand Footman
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=37165; -- Henry Zykes
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=37167; -- Stonetalon Prisoner
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=37170; -- Hunter Hill Scout
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1 WHERE `entry`=37173; -- Soothsayer Rikkari
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1 WHERE `entry`=37174; -- Soothsayer Mirim'koa
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=37177; -- Coldridge Defender
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=37180; -- Honor's Stand Guard
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=37195; -- Lord Darius Crowley
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=37197; -- Feral Worgen
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=37204; -- Hunter Hill Brave
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=37206; -- Plains Prowler
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=37207; -- Plains Pridemane
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=37208; -- Thunderhead
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=37216; -- Honor's Stand Officer
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=37218; -- Coldridge Citizen
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=37220; -- Una'fe Watcher
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=37487; -- Taurajo Refugee
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=37489; -- Feral Watcher
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=37492; -- Blackwald Fox
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=37499; -- Duskhaven Survivor
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=37507; -- Frostmane Blade
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=37511; -- Bristleback Bladewarden
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=37515; -- Lane Tallgrass
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=37516; -- Tawane
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=37517; -- Taurajo Orphan
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=37553; -- Disturbed Earth Elemental
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=37555; -- Landquaker Kodo
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=37556; -- Landquaker Bull
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=73, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=37557; -- Thunderhawk Cloudscraper
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=37559; -- Savannah Boar
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=37560; -- Razormane Pathfinder
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=110, `dmg_multiplier`=1 WHERE `entry`=37570; -- Naralex
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=37585; -- Northwatch Recon
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=37659; -- Horde Field Defender
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=37660; -- Razormane Warfrenzy
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=110, `dmg_multiplier`=1 WHERE `entry`=37661; -- Razormane Seer
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=2.25 WHERE `entry`=37679; -- Warlord Bloodhilt
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=37685; -- Valnov the Mad
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=37686; -- Executor Cornell
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=37692; -- Forsaken Infantry
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=37694; -- Enslaved Villager
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=37701; -- Forsaken Slavedriver
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=37706; -- Desolation Grunt
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=37716; -- Tempest's Reach Watchman
UPDATE `creature_template` SET `mindmg`=47, `maxdmg`=80, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=37717; -- Winnoa Pineforest
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=37718; -- Mountain Mastiff
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=37724; -- Seer Ravenfeather
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=37733; -- Lord Walden
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=37735; -- Baron Ashbury
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=37737; -- Sunwalker Helaku
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=37740; -- Yellowfin Shark
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=37741; -- Bilgewater Seal
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=37743; -- Taurajo Looter
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=16, `dmg_multiplier`=1 WHERE `entry`=37757; -- Howling Banshee
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=37761; -- Sally "Salvager" Sandscrew
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=37762; -- Brett "Coins" McQuid
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=37783; -- Lorna Crowley
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=37784; -- Gilnean Militia
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=37785; -- Wild Horse
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=37786; -- Brown Stag
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=37802; -- Brothogg the Slavemaster
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=37803; -- Marcus
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=37805; -- Forsaken Soldier
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=37806; -- Captain Asther
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=2.25 WHERE `entry`=37807; -- Forsaken Catapult
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=2.25 WHERE `entry`=37808; -- Koroth the Hillbreaker
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=2.25 WHERE `entry`=37811; -- Warlord Gar'dul
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=37812; -- Mahka
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=37815; -- Lorna Crowley
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=37817; -- Zang'do
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=37822; -- Belysra Starbreeze
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=37834; -- Tauna Skychaser
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=37835; -- Dorn Redearth
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=37836; -- Larhasha
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=2.25 WHERE `entry`=37847; -- Mankrik
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=37870; -- Lyros Swiftwind
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=37873; -- Vassandra Stormclaw
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=37874; -- Krennan Aranas
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=37875; -- Lord Godfrey
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=37876; -- King Genn Greymane
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=37884; -- Disturbed Spirit
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=37885; -- Restless Ancestor
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=37908; -- Calder Gray
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=37909; -- Tomusa
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=37910; -- Crawgol
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=2.25 WHERE `entry`=37914; -- Towering Ancient
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=37916; -- Orc Raider
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=2.25 WHERE `entry`=37921; -- Orcish War Machine
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=37922; -- Desolation Raider
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=37923; -- Triumph Vanguard
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=37924; -- Barrens Operative
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=37925; -- Sapper Specialist
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=37926; -- Triumph Captain
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=37938; -- Wolfmaw Outrider
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=37940; -- Mangletooth
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=37951; -- Jin'thala
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=37956; -- Bloodtalon Raptor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=37960; -- Bloodtalon Hatchling
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=37961; -- Corrupted Bloodtalon
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=37969; -- Kijara
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=37987; -- Tegashi
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=37989; -- Swiftclaw
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=37991; -- Ikfirus the Vile
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=38005; -- Moraya
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=38015; -- Karthog
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38022; -- Veteran Dark Ranger
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=38029; -- Tobias Mistmantle
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=38033; -- Awkward Gangly Orc
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=38037; -- Nortet
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=38046; -- Wildmane Cat
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=38070; -- Desolation Guard
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=38120; -- Hobart Grapplehammer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=38122; -- Bamm Megabomb
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=38124; -- Assistant Greely
UPDATE `creature_template` SET `mindmg`=47, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=38140; -- Weezil Slipshadow
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=38141; -- Docile Island Boar
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=38142; -- Captive Spitescale Scout
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=38143; -- Magda Whitewall
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=38144; -- Krennan Aranas
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=38149; -- Lord Darius Crowley
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=38150; -- Glaive Thrower
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=38183; -- Haggis Boatmurder
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=38187; -- Spiny Raptor
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=38192; -- Forsaken Infantry
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=38210; -- Forsaken Crossbowman
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=2.25 WHERE `entry`=38217; -- Darkspear Watcher
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=2.25 WHERE `entry`=38218; -- Prince Liam Greymane
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=38221; -- Gilnean Militia
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=38224; -- Mechachicken
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=38225; -- Vol'jin
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=38242; -- Nekali
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=38243; -- Zen'tabra
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=38244; -- Legati
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38245; -- Tunari
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38246; -- Soratha
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=38247; -- Ortezza
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=38268; -- Novice Darkspear Warrior
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=38272; -- Novice Darkspear Rogue
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=38278; -- Novice Darkspear Priest
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=3, `dmg_multiplier`=1 WHERE `entry`=38279; -- Novice Darkspear Mage
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=3, `dmg_multiplier`=1 WHERE `entry`=38280; -- Novice Darkspear Druid
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=38281; -- Novice Darkspear Shaman
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=3, `dmg_multiplier`=1 WHERE `entry`=38282; -- Novice Darkspear Hunter
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=38290; -- Barrens Brave
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1 WHERE `entry`=38300; -- Spitescale Wavethrasher
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1 WHERE `entry`=38301; -- Spitescale Siren
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=38306; -- Zar'jira
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=38314; -- Muyoh
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=2.25 WHERE `entry`=38323; -- General Hawthorne
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38324; -- Darkspear Tribesman
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=38326; -- Darkspear Shaman
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=38327; -- Northwatch Siege Engineer
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38345; -- Captured Brave
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=38348; -- Worgen Warrior
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=38359; -- Vashj'elan Warrior
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=15, `dmg_multiplier`=1 WHERE `entry`=38360; -- Vashj'elan Siren
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=38364; -- Forsaken Plaguesmith
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=38378; -- Commander Walpole
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=2.25 WHERE `entry`=38379; -- Commander Singleton
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=38380; -- Janice Mattingly
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38381; -- Brett "Coins" McQuid
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=38382; -- Lieutenant Worley
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=38383; -- Nibb Spindlegear
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=38384; -- Kona Thunderwalk
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=38385; -- Walpole's Bodyguard
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38387; -- Sassy Hardwrench
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=38392; -- Val'kyr Protector
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38409; -- Goblin Survivor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=38412; -- Naga Hatchling
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=2.25 WHERE `entry`=38415; -- Lord Darius Crowley
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=38420; -- Vile Abomination
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=38423; -- Zuni
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=38425; -- Freed Emberstone Villager
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=38426; -- Lorna Crowley
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38432; -- Megs Dreadshredder
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=38437; -- Vanira
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=38440; -- Tortunga
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=38441; -- Ace
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=2.25 WHERE `entry`=38442; -- Morakki
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=38448; -- Faceless of the Deep
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=2.25 WHERE `entry`=38464; -- Dark Ranger Elite
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=2.25 WHERE `entry`=38465; -- Myriam Spellwaker
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=2.25 WHERE `entry`=38466; -- Sister Almyra
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=2.25 WHERE `entry`=38467; -- Huntsman Blake
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=38468; -- Gilnean Mastiff
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=2.25 WHERE `entry`=38470; -- King Genn Greymane
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=18, `dmg_multiplier`=2.25 WHERE `entry`=38473; -- Soultethered Banshee
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=2.25 WHERE `entry`=38474; -- Prince Liam Greymane
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=38491; -- Kor'kron Lieutenant
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=38502; -- Nablya
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=38510; -- Grimy Greasefingers
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38511; -- Sally "Salvager" Sandscrew
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38513; -- Evol Fingers
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38514; -- Fizz Lighter
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=38515; -- Maxx Avalanche
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38516; -- Sister Goldskimmer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=42, `dmg_multiplier`=1 WHERE `entry`=38517; -- Slinky Sharpshiv
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=38518; -- Warrior-Matic NX-01
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=38532; -- Marin Noggenfogger
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=38534; -- Megs Dreadshredder
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=38535; -- Kelsey Steelspark
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=38539; -- King Genn Greymane
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=38553; -- Krennan Aranas
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=38571; -- Steamwheedle Survivor
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=15, `dmg_multiplier`=1 WHERE `entry`=38574; -- Oomlot Shaman
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=38575; -- Oomlot Tribesman
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=38578; -- Flinn
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=38611; -- Lorna Crowley
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=38613; -- Worgen Warrior
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=2.25 WHERE `entry`=38614; -- Prince Liam Greymane
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=38616; -- Forsaken Infantry
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=38617; -- Forsaken General
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=38618; -- Forsaken Sergeant
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=2.25 WHERE `entry`=38619; -- Admiral Aubrey
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=38620; -- Thomas Paxton
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=38621; -- Jeffrey Long
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=38622; -- Captain Fisher
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=38623; -- Dockmaster Lewis
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=38624; -- Northwatch Defender
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=38627; -- Northwatch Dockworker
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=38636; -- Northwatch Guard
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=38643; -- Goblin Captive
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=38646; -- Covert Ops Hardsuit
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=38647; -- Izzy
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=38648; -- Bilgewater Battlebruiser
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=2.25 WHERE `entry`=38650; -- Rental Shredder
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=38658; -- Rageroar Grunt
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=38659; -- Rageroar Lieutenant
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=38661; -- Rageroar Sea Dog
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=38663; -- Karga Rageroar
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=38664; -- Blackpool Crewman
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=38696; -- Yngwie
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=38697; -- Nathan Blaine
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=38706; -- Zeke Bootscuff
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=38714; -- Carmen Ibanozzle
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38738; -- Coach Crosscheck
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=38746; -- Gobber
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=38749; -- Captain Dreadbeard
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=38753; -- Goblin Zombie
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=38762; -- Ogre Ambusher
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=38764; -- Lord Hewell
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=38780; -- Night Elf Sentinel
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=38781; -- Gilnean Survivor
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=9, `dmg_multiplier`=1 WHERE `entry`=38783; -- Marie Allen
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=9, `dmg_multiplier`=1 WHERE `entry`=38791; -- Willa Arnes
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=9, `dmg_multiplier`=1 WHERE `entry`=38792; -- Willa Arnes
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38793; -- Sergeant Cleese
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=9, `dmg_multiplier`=2.25 WHERE `entry`=38794; -- Myriam Spellwaker
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=38795; -- Sister Almyra
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=42, `dmg_multiplier`=1 WHERE `entry`=38796; -- Loren the Fence
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38797; -- Vitus Darkwalker
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=38798; -- Huntsman Blake
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=38799; -- Celestine of the Harvest
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=38800; -- Killick
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=38801; -- Norbin
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=38804; -- Tolliver Houndstooth
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=38805; -- Wounded Defender
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=38808; -- Gaahl
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=38809; -- Malmo
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=38810; -- Teloch
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=38811; -- Oostan Headhunter
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=38812; -- Goblin Captive
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=55, `attackpower`=94, `dmg_multiplier`=1 WHERE `entry`=38826; -- Hexmaster Bastoon
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=38832; -- Baron
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=38844; -- Gilnean Mastiff
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=38845; -- Child of Volcanoth
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1 WHERE `entry`=38847; -- Grohk
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=38849; -- Megs Dreadshredder
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=38850; -- Volcanoth Champion
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=38851; -- Volcanoth Priest
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=9, `dmg_multiplier`=1 WHERE `entry`=38853; -- Marie Allen
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=38855; -- Volcanoth
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1 WHERE `entry`=38856; -- Dunemaul "Emissary"
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=38871; -- Corporal Teegan
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=38873; -- Goucho
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=38875; -- Ol' Durty Pete
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=38876; -- Hannah Bridgewater
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1 WHERE `entry`=38880; -- Sandscraper
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=38881; -- Fox
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=38884; -- Deviate Thornweaver
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38898; -- Deathguard Hansel
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38899; -- Deathguard Balteus
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=38909; -- Sandfury Zombie
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=38910; -- Lilian Voss
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=38911; -- Xavier the Huntsman
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1 WHERE `entry`=38914; -- Sandstone Golem
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1 WHERE `entry`=38916; -- Sandstone Earthen
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=91, `dmg_multiplier`=1 WHERE `entry`=38922; -- Examiner Andoren Dawnrise
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=38925; -- Sedrick Calston
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=81, `dmg_multiplier`=1 WHERE `entry`=38927; -- Mazoga
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38928; -- Sassy Hardwrench
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=38933; -- Briny Sea Cucumber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=38937; -- Vile Fin Tadpole
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=98, `dmg_multiplier`=1 WHERE `entry`=38940; -- Deviate Crone
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=104, `dmg_multiplier`=2.25 WHERE `entry`=38941; -- Three-Tooth
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=38952; -- Nordrassil Druid
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=2.25 WHERE `entry`=38966; -- Vol'jin
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=42, `dmg_multiplier`=1 WHERE `entry`=38977; -- Apothecary Jerrod
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=38978; -- Apprentice Crispin
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=38986; -- Ambassador Gaines
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38988; -- Notera
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38989; -- Jornun
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=38990; -- Kortin
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1 WHERE `entry`=38997; -- Reliquary Digger
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1 WHERE `entry`=38998; -- Explorer's League Digger
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=38999; -- Lilian Voss
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=39003; -- Sam Trawley
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=39004; -- Pygmy Surf Crawler
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=39005; -- Northwatch Rifleman
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=39006; -- Siege Tank Engineer
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=39007; -- Tora'jin
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=39008; -- Hira'jin
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=39009; -- Hawthorne's Steed
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=39014; -- Pavlova
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1 WHERE `entry`=39020; -- Crazed Bilgewater Survivor
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=39022; -- Tidal Strider
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=39024; -- Karl
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=39025; -- Shaina
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=39027; -- Vanira
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=39031; -- Torenda
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=39032; -- Gora'tin
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=39033; -- Sortura
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=39034; -- Dr. Dealwell
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=39044; -- Orc Battlesworn
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=39049; -- Plagued Bruin
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1 WHERE `entry`=39061; -- Meatface
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=39062; -- Darkspear Jailor
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=39063; -- Chawg
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=39064; -- Orc Survivor
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=39065; -- Aggra
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=39066; -- Kilag Gorefang
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=39067; -- Scout Brax
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=39068; -- Orc Scout
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=39069; -- Alliance Paratrooper
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=39081; -- Hazzali Cocoon
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=39083; -- Commander Roberts
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=39084; -- Mizzy Pistonhammer
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=39085; -- Logan Talonstrike
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=39086; -- Slain Scarlet Zealot
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=39087; -- Slain Scarlet Friar
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=39094; -- Barton Trask
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=39095; -- Slain Guard
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=39096; -- Painmaster Thundrak
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=46, `dmg_multiplier`=1 WHERE `entry`=39100; -- Dedlow Wormwood
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=194, `dmg_multiplier`=1 WHERE `entry`=39116; -- Apolos
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=39117; -- Shadow Priestess Malia
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=2.25 WHERE `entry`=39118; -- General Twinbraid
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=39124; -- Mizzy's Apprentice
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=39129; -- Peasant Laborer
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=39136; -- Triumph Sentry
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=93, `attackpower`=132, `dmg_multiplier`=1 WHERE `entry`=39138; -- Theramore Highway Guard
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=39139; -- Triumph Rifleman
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=39140; -- Aronus
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=42, `dmg_multiplier`=1 WHERE `entry`=39141; -- Commander Arrington
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=42, `dmg_multiplier`=1 WHERE `entry`=39142; -- Darkblade Cyn
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=42, `dmg_multiplier`=1 WHERE `entry`=39143; -- Alexi Silenthowl
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=116, `attackpower`=166, `dmg_multiplier`=1 WHERE `entry`=39144; -- Alto Stonespire
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=39147; -- Bastia
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=39153; -- Excavation Raider
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=39154; -- Hurlston Stonesthrow
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=39155; -- Marley Twinbraid
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=39156; -- Twinbraid's Bodyguard
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=39157; -- Lost Bloodtalon Hatchling
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=39174; -- Feegly the Exiled
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=39178; -- Driz Plunkbow
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1.5 WHERE `entry`=39185; -- Slaverjaw
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1.5 WHERE `entry`=39186; -- Hellgazer
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=39190; -- Wicked Spirit
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=39191; -- Hilda Runesworn
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=39193; -- Brute Overseer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=39194; -- Blastshadow the Brutemaster
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=42, `dmg_multiplier`=1 WHERE `entry`=39195; -- Delicia Whipsnaps
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=39196; -- Deathguard Swallon
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=39206; -- Acrypha
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=39210; -- John Johnson
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=39211; -- Steve Stevenson
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=39212; -- Bill Williamson
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=39214; -- Karranisha
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=39215; -- Captured Northwatch Scout
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=2.25 WHERE `entry`=39217; -- Terenas Menethil
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=39224; -- Lo'Shall
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=39226; -- Farseer Gadra
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=39245; -- Northwatch Lug
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=39249; -- Northwatch Lug
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=39251; -- Northwatch Supply Crate
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=39254; -- Stardust Sentinel
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=94, `dmg_multiplier`=2.25 WHERE `entry`=39256; -- Kalen Trueshot
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=39257; -- Stardust Archer
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=39260; -- Northwatch Infantryman
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=39261; -- Northwatch Ranger
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=39267; -- Northwatch Marine
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=39268; -- Northwatch Sharpshooter
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=39269; -- Lieutenant Palliter
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=39270; -- Injured Razor Hill Grunt
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=39272; -- Foaming Sea Elemental
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=39279; -- Frazzlecraz Foreman
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=39280; -- Frazzlecraz Miner
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=39281; -- Fenton Frazzlecraz
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=39282; -- Frazzlecraz Floozy
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39313; -- Zin'jatar Raider
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=39317; -- Northwatch Scout
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=39322; -- Horton Gimbleheart
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=39323; -- Thonk
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=39324; -- Zen'Taji
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=39325; -- Grandmatron Tekla
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=39326; -- Raggaran
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=39330; -- Crador
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=39337; -- Wayward Plainstrider
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=39340; -- Unega
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=39341; -- Mine Cart
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=39351; -- Ghislania
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=39352; -- Gaur Icehorn
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=39353; -- Griswold Hanniston
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=39354; -- Steamwheedle Shark
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=39363; -- Chip Endale
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=39366; -- Sun-Touched Servant
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=39372; -- Garrosh Hellscream
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=39373; -- Sun-Touched Speaker
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=39376; -- KTC Oil Bot
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=39379; -- Gor the Enforcer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=39380; -- Shin Stonepillar
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=39381; -- Crimsonborne Guardian
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=39385; -- Screamslash
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=39390; -- Twilight Drake
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=39392; -- Faceless Corruptor
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=39399; -- Farmer Lok'lub
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=39400; -- Farmer Krella
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=39405; -- Crimsonborne Seer
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=39408; -- Durotar Farmhand
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=1 WHERE `entry`=39411; -- Earthen Ring Shaman
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=39414; -- Ascended Windwalker
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39418; -- Green Sand Crab
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39419; -- Oceanic Broadhead
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39422; -- Coilshell Sifter
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=39423; -- Gail Nozzywig
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=39425; -- Temple Guardian Anhuur
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=39426; -- Candy Cane
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=39428; -- Earthrager Ptah
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=39444; -- Pit Viper
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=39450; -- Trogg Dweller
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=39452; -- Dreadmaw Toothgnasher
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=39456; -- Captured Goblin
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=39464; -- Drowned Thunder Lizard
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=39582; -- Trade Prince Gallywix
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=39587; -- Isiset
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=39589; -- Brute Bodyguard
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=39590; -- Spiketooth
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=39591; -- Orc Battlesworn
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=39592; -- Ultimate Footbomb Uniform
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=39594; -- Thrall
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=39595; -- Furious Earthguard
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=39596; -- Teeming Waterguard
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=39603; -- Swollen Voidwalker
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=39604; -- Vek'nag
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=2.25 WHERE `entry`=39605; -- Garrosh Hellscream
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=39609; -- Kor'kron Loyalist
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=39610; -- Battleworg
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=39615; -- Trade Prince Gallywix
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=39625; -- General Umbriss
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=39626; -- Crimsonborne Warlord
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39629; -- Abyssal Seahorse
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39638; -- Azsh'ir Patroller
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=39655; -- Orc Survivor
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=39660; -- Spirit Healer
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39663; -- Drowning Soldier
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39664; -- Glimmerdeep Tidehunter
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39667; -- Adarrah
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39668; -- Mack Fearsen
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39669; -- Captain Samir
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=39697; -- Nato Raintree
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=39718; -- Doktor Professor Ironpants
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=39729; -- Nespirah Survivor
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=39732; -- Setesh
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39745; -- Spiralung
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=39747; -- Saviana Ragefire
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39796; -- Anemone Frenzy
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=39800; -- Flame Warden
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=39801; -- Earth Warden
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=39802; -- Water Warden
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=39803; -- Air Warden
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=39804; -- Stone Trogg Pillager
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=2.25 WHERE `entry`=39814; -- Onyx Flamecaller
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=39854; -- Azureborne Guardian
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=39855; -- Azureborne Seer
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=289, `dmg_multiplier`=1 WHERE `entry`=39865; -- Emissary Windsong
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=39870; -- Twilight Firecatcher
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=39873; -- Twilight Wyrmcaller
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=39874; -- Derth Firesinger
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=39875; -- Earthmender Duarn
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=39876; -- Felora Firewreath
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=2.25 WHERE `entry`=39877; -- Toshe Chaosrender
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=39878; -- Caretaker Movra
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=39882; -- The Great Sambino
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39883; -- Adarrah
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39886; -- Drowned Soldier
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=39890; -- Twilight Earthshaper
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=39895; -- Kanati Greycloud
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=39899; -- Baltharus the Warborn
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=39908; -- Brann Bronzebeard
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=39909; -- Azureborne Warlord
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39911; -- Dead Zin'jatar Raider
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39913; -- Frenzied Orca
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39918; -- Clacksnap Pincer
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=39947; -- Grimtotem Pillager
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39948; -- Brinescale Serpent
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=39954; -- Twilight Shadow Weaver
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=39956; -- Twilight Enforcer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=39962; -- Twilight Stormbreaker
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=93, `attackpower`=132, `dmg_multiplier`=1 WHERE `entry`=39963; -- Motega Firemane
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=39964; -- Akasha
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=39984; -- Malignant Trogg
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=2.25 WHERE `entry`=40027; -- Fizzle Brassbolts
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=2.25 WHERE `entry`=40028; -- Pozzik
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=40033; -- Flux Animator
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=40057; -- Mekkatorque's  Mechanostrider
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=40061; -- Galak Stormer
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=40062; -- Galak Mauler
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=40063; -- Galak Wrangler
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=40064; -- Jungle Panther
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=40067; -- Iceeya
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=40068; -- Venom Spitter
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=40071; -- Julia Scrabblewrench
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=40082; -- Skycaller Vrakthris
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=40092; -- Southsea Sailor
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=40106; -- Star Shard
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=40109; -- Prospector Gunstan
UPDATE `creature_template` SET `mindmg`=200, `maxdmg`=341, `attackpower`=175, `dmg_multiplier`=1 WHERE `entry`=40160; -- Frozo the Renowned
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=40166; -- Enslaved Gronn Brute
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=40167; -- Twilight Beguiler
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=40184; -- Vanira
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40200; -- Spiketooth Eel
UPDATE `creature_template` SET `mindmg`=239, `maxdmg`=409, `attackpower`=500, `dmg_multiplier`=1 WHERE `entry`=40204; -- Handler Marnlek
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=40209; -- Grex Brainboiler
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=40216; -- Blazzek the Biter
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40219; -- Ravenous Thresher
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=40221; -- Toshe Chaosrender
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40222; -- Scout Bat
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=40227; -- Felice
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=40230; -- Modified Earth Elemental
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=40247; -- Sambino's Modified Earth Elemental Totem
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=40251; -- Stone Trogg Brute
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=40252; -- Stone Trogg Rock Flinger
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=40256; -- Troll Citizen
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=40257; -- Troll Citizen
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=40268; -- Twilight War-Mage
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=40270; -- Twilight Thundercaller
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=40272; -- Ascended Rockbreaker
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=40273; -- Ascended Waterlasher
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40275; -- Zin'jatar Ravager
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40276; -- Sabreclaw Skitterer
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40280; -- Swarming Serpent
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40282; -- Kliklak
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=40290; -- Crimsonborne Seer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=40291; -- Azureborne Seer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=40306; -- Twilight Armsmaster
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=40319; -- Drahga Shadowburner
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=2.25 WHERE `entry`=40329; -- Zen'tabra
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=2.25 WHERE `entry`=40344; -- Nyse
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=2.25 WHERE `entry`=40345; -- Thyssiana
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=40352; -- Witch Doctor Hez'tok
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=40358; -- Dyslix Silvergrub
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40388; -- Darkspear Ancestor
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40416; -- Darkspear Scout
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=289, `dmg_multiplier`=1 WHERE `entry`=40429; -- Sanctum Guardian Xerestrasza
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=93, `attackpower`=132, `dmg_multiplier`=1 WHERE `entry`=40431; -- Savage Parrot
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=40432; -- Southsea Parrot Handler
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=40448; -- Twilight Enforcer
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=40449; -- Southsea Treasure Hunter
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=40450; -- Dustbone Horror
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=40458; -- Jeweled Scarab
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40466; -- Blackfin
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=40475; -- Jinky Twizzlefixxit
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=40480; -- Empowered Water Elemental
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40482; -- Earthen Ring Wavebinder
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=40483; -- Gnome Bar Patron
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=40484; -- Erudax
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=40485; -- Twilight Egg
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=40486; -- Alexstrasza's Egg
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=40487; -- Razzeric
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=40494; -- Goblin Bar Patron
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=40508; -- Butcherbot
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=40510; -- Fathom-Lord Zin'jatar
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=1 WHERE `entry`=40512; -- Brute Squad
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=40527; -- Duneshore Crab
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=40528; -- Steamwheedle Bughunter
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=40529; -- Gnome Bar Patron
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=40530; -- Goblin Bar Patron
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=40535; -- Ruby Defender
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=40537; -- Ruby Guardian
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=40552; -- Leora
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=40553; -- Fidelio
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=40554; -- Rizzle Brassbolts
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=40556; -- Zamek
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=40558; -- Gazrix
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=40560; -- Covert Ops Agent
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=40572; -- Haughty Modiste
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=40580; -- Gus Rustflutter
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=40581; -- Gargantapid
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=40582; -- Chelsea Rustflutter
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=40583; -- Southsea Blockader
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=40588; -- Steamwheedle Refugee
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=40589; -- Dirge Quikcleave
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=40593; -- Southsea Taskmaster
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=40604; -- Steamwheedle Rescue Balloon
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=40632; -- Southsea Musketeer
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=40635; -- Southsea Strongarm
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=40636; -- Southsea Swabbie
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=40639; -- Engineer Hexascrub
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=40641; -- Divemaster Birmingham
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=40646; -- Glimmerdeep Diver
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=40648; -- Zakkaru
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40649; -- Sea Snake
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=40656; -- Duneclaw Matriarch
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=40657; -- Basking Cobra
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40661; -- Drowned Crewman
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=40662; -- Steamwheedle Victim
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=112, `attackpower`=162, `dmg_multiplier`=1 WHERE `entry`=40666; -- Duneclaw Broodling
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40677; -- Gilblin Scavenger
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40685; -- Splitclaw Skitterer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=40715; -- Lifewarden Nymph
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=40716; -- Seedling Pod
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=40717; -- Duneclaw Lasher
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=40726; -- Mazzer Stripscrew
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=40727; -- Griznak
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=40728; -- Whale Shark
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1 WHERE `entry`=40747; -- Selia Sunglow
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=40764; -- Desert Crawler
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=40768; -- Tilly Topspin
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=40769; -- Zazzix Boomride
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=40776; -- Speedbarge Crewmember
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=40777; -- Speedbarge Crewmember
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=40778; -- Speedbarge Crewmember
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=40779; -- Speedbarge Crewmember
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40798; -- Spineshell Pincer
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=40799; -- Boog the "Gear Whisperer"
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=40800; -- Minx
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40802; -- Softshell Sea Turtle
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=40808; -- Dustbone Horror
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=40809; -- Tak
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40810; -- Gilblin Scavenger
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40811; -- Gilblin Scavenger
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=40815; -- Fedli Caskcheer
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=40826; -- Brod Anvilbeard
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=40827; -- Thurda
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=40832; -- Daisy
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=40851; -- Swift Seahorse
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=40852; -- Swift Seahorse
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40855; -- Slitherfin Eel
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=40866; -- Swift Seahorse
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=40867; -- Swift Seahorse
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=40869; -- Holdfast Cannon
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=40871; -- Swift Seahorse
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=40873; -- Swift Seahorse
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40877; -- Azsh'ir Monitor
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=40879; -- Boog the "Gear Whisperer"
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=40881; -- Minx
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=40885; -- Wizzle Brassbolts
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=40888; -- Synge
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40889; -- Sandskin Pincer
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=40890; -- Dranosh'ar Archer
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=40891; -- Dranosh'ar Laborer
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=40892; -- Dranosh'ar Overseer
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=40893; -- Dranosh'ar Fisherman
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=40894; -- Gnombus the X-Terminator
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=40895; -- Professor Xakxak Gyromate
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=2.25 WHERE `entry`=40896; -- Lord Fallowmere
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=2.25 WHERE `entry`=40897; -- Northwatch Captain Kosak
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=40898; -- Alithia Fallowmere
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=40899; -- Arcanist Valdurian
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=40900; -- Neophyte Starcrest
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=40901; -- Blastgineer Fuzzwhistle
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=40902; -- Chief Blastgineer Bombgutz
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=40903; -- Spy-Mistress Anara
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=40905; -- Windshear Overseer
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=40906; -- Lumbering Oaf
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=40907; -- Clarissa
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=40908; -- Alice
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=2.25 WHERE `entry`=40909; -- Large Daddy
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40911; -- Redgill Scavenger
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=40915; -- Anderov Ryon
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=40918; -- Fiasco Sizzlegrin
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=40926; -- Kilag Gorefang
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=40940; -- Young Wendigo
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=40941; -- Wendigo
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=40948; -- Dranosh'ar Farmer
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=40949; -- Kraka
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=40950; -- Captain Tharran
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=40951; -- Quartermaster Glynna
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=40957; -- Warsong Stockpile
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=2.25 WHERE `entry`=40958; -- Tony Two-Tusk
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=2.25 WHERE `entry`=40959; -- Den Whomper
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=2.25 WHERE `entry`=40973; -- Sentinel Heliana
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=40983; -- Mack Fearsen
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=40987; -- Gnash
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=40991; -- Mountaineer Dunstan
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=40994; -- Mountaineer Lewin
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=41002; -- Slickskin Eel
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=41015; -- Northgate Rebel
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=41016; -- Gilblin Hoarder
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=41017; -- Gilblin Collector
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=41018; -- King Gurboggle
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=41019; -- Rope Ladder
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=2.25 WHERE `entry`=41023; -- Overlord Krom'gar
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=41037; -- Salty Dog
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=41038; -- Undead Officer
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=41039; -- Windshear Hold Guardian
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=41042; -- Giant Sea Grub
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=41047; -- Injured Gnome Engineer
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=41053; -- Ryan Mills
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=41054; -- Ol' Irongoat
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=41056; -- Mountaineer Valgrum
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=41060; -- Ajamon Ghostcaller
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=41062; -- Krom'gar Infantry
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=41070; -- Captured Huntress
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=41071; -- Huntress Illiona
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=41072; -- Dragonmaw Grunt
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=41073; -- Twilight Armsmaster
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41074; -- Mountaineer Grugelm
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41075; -- Mountaineer Rharen
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=41076; -- Evil Dolly
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=41080; -- Dragonmaw Scout
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=41081; -- Seadog Fajardo
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=41082; -- Lilly Landlubber
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=1 WHERE `entry`=41086; -- Forba Slabchisel
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=41095; -- Twilight Drake
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=41097; -- Saltstone Basilisk
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=70, `attackpower`=104, `dmg_multiplier`=1 WHERE `entry`=41099; -- Saltstone Crystalhide
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=41100; -- Scorpid Reaver
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=41101; -- Scorpid Terror
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=41103; -- Sparkleshell Snapper
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=41104; -- Sparkleshell Tortoise
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=41109; -- Saltstone Gazer
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=41111; -- Salt Flats Vulture
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=41113; -- Sparkleshell Borer
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=41121; -- Frostmane Seer
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=41122; -- Frostmane Snowstrider
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=2.25 WHERE `entry`=41124; -- Okrilla
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=41125; -- Master Aitokk
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1 WHERE `entry`=41127; -- Dreadmaul Captive
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=41128; -- Dunlor Marblebeard
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=41129; -- Surveyor Thurdan
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=67, `dmg_multiplier`=1 WHERE `entry`=41131; -- Mad Magus Tirth
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=41132; -- Flood Elemental
UPDATE `creature_template` SET `mindmg`=124, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1 WHERE `entry`=41133; -- Rofilian Dane
UPDATE `creature_template` SET `mindmg`=129, `maxdmg`=220, `attackpower`=258, `dmg_multiplier`=1 WHERE `entry`=41134; -- Tak'arili
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=41135; -- "Plucky" Johnson
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=41136; -- Dreadmaul Ambusher
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=41137; -- Displaced Threshadon
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=1 WHERE `entry`=41138; -- Okril'lon Defender
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=41140; -- Burok
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=41142; -- Handler Marnlek
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=41145; -- Silty Oozeling
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=41146; -- Frostmane Scavenger
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=41147; -- Slavering Ooze
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=41151; -- Drungeld Glowerglare
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=1 WHERE `entry`=41158; -- Nethergarde Defender
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=41159; -- Watcher Wazzik
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=41162; -- Terrified Nethergarde Miner
UPDATE `creature_template` SET `mindmg`=115, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1 WHERE `entry`=41163; -- Illycor
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=41164; -- Jarroc Torn-Wing
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=205, `attackpower`=224, `dmg_multiplier`=1 WHERE `entry`=41165; -- Shahandana
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=41166; -- Gomegaz
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=41167; -- Torrention
UPDATE `creature_template` SET `mindmg`=115, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1 WHERE `entry`=41171; -- Reassuring Nethergarde Foreman
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=41175; -- Frostmane Scout
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=41181; -- Kharanos Mountaineer
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=41182; -- Kharanos Rifleman
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41183; -- Gilblingle
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=41184; -- Riznek
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=41185; -- Queen Silith
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=41190; -- Crazzle Sprysprocket
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=41196; -- Kravel Koalbeard
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=41199; -- Trackmaster Zherin
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=41204; -- Khan Ablinh
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=41205; -- Khan Blizh
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=41214; -- Slick Dropdip
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=41215; -- Raina Sunglide
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41228; -- Naz'jar Serpent Guard
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=2.25 WHERE `entry`=41229; -- Force Commander Valen
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=41230; -- Krom'gar Rifleman
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=2.25 WHERE `entry`=41233; -- Scout Commander Barus
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=41237; -- Kharanos Mountaineer
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=41238; -- Wetlands Surveyor
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=41240; -- Fiora Moonsoar
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=41246; -- Flok
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=41251; -- Frostmane Builder
UPDATE `creature_template` SET `mindmg`=115, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=41253; -- Oath-Chained Infernal
UPDATE `creature_template` SET `mindmg`=115, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=41254; -- Enthralled Cultist
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=41258; -- Frostmane Warrior
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=41260; -- Brisket
UPDATE `creature_template` SET `mindmg`=122, `maxdmg`=209, `attackpower`=242, `dmg_multiplier`=1 WHERE `entry`=41265; -- Salt-Flop
UPDATE `creature_template` SET `mindmg`=122, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1 WHERE `entry`=41269; -- Alliance Portal-Sentry
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=41272; -- Dark Iron Smouldermage
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=41273; -- Horghast Flarecrazed
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=2.25 WHERE `entry`=41274; -- Chief Explorer Jansun
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=41275; -- Explorer Tabby Triloc
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=41276; -- Steeltoe McGee
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=41277; -- Lieutenant Paulson
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=41278; -- "Cookie" McWeaksauce
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=41282; -- Corporal Wocard
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=41284; -- Battok the Berserker
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=41286; -- Lyanath
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=41291; -- Krom'gar Assassin
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=41293; -- Harbor Shredfin
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=41295; -- Ocean Crawler
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41297; -- Karl Boran
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=41298; -- Slamp Wobblecog
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=41307; -- First Mate Snellig
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=41311; -- Master Assassin Kel'istra
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=41321; -- Elgin Baelor
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=41322; -- Halana
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=41323; -- Damon Baelor
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=41325; -- Caleb Baelor
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41326; -- Slickback Remora
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=41332; -- Eeryven Grayer
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=41333; -- Lieutenant Timothy Clark
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=41335; -- Covert Operative
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=41342; -- Mottled Screecher
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=41343; -- Mottled Raptor
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=41348; -- Gnomebot Helper
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=41349; -- Harvestbot Assistant
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=2.25 WHERE `entry`=41350; -- Ton Windbow
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=2.25 WHERE `entry`=41351; -- Grimtotem Bodyguard
UPDATE `creature_template` SET `mindmg`=122, `maxdmg`=209, `attackpower`=242, `dmg_multiplier`=1 WHERE `entry`=41354; -- Neptool
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=41359; -- Bloodwash Crate Spawner
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=41360; -- Krom'gar Marauder
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=41361; -- Northwatch Infantry
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=41363; -- Delber Cranktoggle
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=41364; -- Void Lord
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=94, `dmg_multiplier`=1 WHERE `entry`=41370; -- General Vol'tar
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=41371; -- Void Seeker
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=41374; -- Void Wurm
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1 WHERE `entry`=41384; -- Rockpool Murloc
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=41385; -- Rockpool Tadpole
UPDATE `creature_template` SET `mindmg`=115, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=41386; -- Bloodwash Barbarian
UPDATE `creature_template` SET `mindmg`=115, `maxdmg`=205, `attackpower`=224, `dmg_multiplier`=1 WHERE `entry`=41387; -- Bloodwash Enchantress
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=41388; -- Living Fossil
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=1 WHERE `entry`=41389; -- Paleolithic Elemental
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=41390; -- Mosshide Gnoll
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=1 WHERE `entry`=41391; -- Mosshide Mystic
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=41392; -- Quentin
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=41393; -- Technician
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=41394; -- Dark Iron Holdout
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=41400; -- Highland Razormaw
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=41401; -- Highland Scytheclaw
UPDATE `creature_template` SET `mindmg`=115, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=41404; -- Bloodwash Gambler
UPDATE `creature_template` SET `mindmg`=115, `maxdmg`=205, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=41405; -- Bloodwash Idolater
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=41407; -- Krom'gar Blood Guard
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=41408; -- Goblin Stripminer
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=41409; -- Dark Iron Trapper
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=41410; -- Captured Mosshide
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41411; -- Prospector Whelgar
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41412; -- Ormer Ironbraid
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41413; -- Merrin Rockweaver
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41415; -- Shilah Slabchisel
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=41416; -- Yorla Darksnare
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=41417; -- The Bomb
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=41419; -- Wetlands Crocolisk
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=41420; -- Horrorjaw
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=41421; -- Rau Cliffrunner
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=41422; -- Bloodwash Zealot
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=41423; -- Bloodwash Acolyte
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41424; -- Mouldering Mirebeast
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=1 WHERE `entry`=41425; -- Bluegill Murloc
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=34, `dmg_multiplier`=1 WHERE `entry`=41426; -- Bluegill Oracle
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=41427; -- Cursed Marine
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=41428; -- Cursed Sailor
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=41429; -- Captain Halyndor
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=41431; -- Grimtotem Invader
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=41432; -- Mirkfallon Post Scout
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41433; -- James Halloran
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=41435; -- Fradd Swiftgear
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=41439; -- Freewind Warrior
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=2.25 WHERE `entry`=41441; -- Scout Mistress Yvonia
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=67, `dmg_multiplier`=1 WHERE `entry`=41444; -- Brienna Starglow
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=41446; -- Thalia Amberhide
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=41447; -- Bor'zehn
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=41448; -- Nag'zehn
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41449; -- Rampant Fire Elemental
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=112, `attackpower`=162, `dmg_multiplier`=1 WHERE `entry`=41452; -- Starn
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41453; -- Smouldering Ooze
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=112, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=41458; -- Cliffwatcher Longhorn
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=41461; -- Rusty Anvil
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41466; -- Idra'kess Sentinel
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=1 WHERE `entry`=41467; -- Idra'kess Enchantress
UPDATE `creature_template` SET `mindmg`=122, `maxdmg`=213, `attackpower`=234, `dmg_multiplier`=1 WHERE `entry`=41470; -- Doomguard Destroyer
UPDATE `creature_template` SET `mindmg`=122, `maxdmg`=213, `attackpower`=234, `dmg_multiplier`=1 WHERE `entry`=41471; -- Dreadlord Defiler
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=41475; -- Oil Pump
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=41477; -- Zin'jatar Pearlbinder
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=41478; -- Snow Tracker Wolf
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=41481; -- Zin'jatar Guardian
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=2.25 WHERE `entry`=41482; -- Master Thal'darah
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=41485; -- Thal'darah Guardian
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=55, `attackpower`=94, `dmg_multiplier`=1 WHERE `entry`=41486; -- Elder Sareth'na
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=94, `dmg_multiplier`=1 WHERE `entry`=41487; -- Hierophant Malyk
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=41488; -- Sentinel Mistress Geleneth
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=98, `dmg_multiplier`=1 WHERE `entry`=41489; -- Mirin
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=41490; -- Donald Dealright
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=47, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=41491; -- Valos Shadowrest
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=41493; -- Poppy
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41494; -- Enslaved Alliance Pearl Miner
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41495; -- Enslaved Horde Pearl Miner
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=41496; -- Marsh Lasher
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=2.25 WHERE `entry`=41503; -- Rethiel the Greenwarden
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=41508; -- Salvaged Goods
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=1 WHERE `entry`=41517; -- Idra'kess Harpooner
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41520; -- Deepseeker Crab
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41526; -- Enslaved Horde Mage
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41527; -- Enslaved Alliance Mage
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=41528; -- Helpless Young Druid
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41530; -- Warden Azjakir
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=41539; -- Stolen Ram
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=41544; -- Frostmane Raider
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41548; -- Imprisoned Soldier
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41549; -- Zin'jatar Overseer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=41561; -- Wary Mastiff
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41566; -- Muckskin Scrounger
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=41569; -- Kvaldir Fleshcarver
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41573; -- Nespirah Fluid
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=41578; -- Sergeant Flinthammer
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=41586; -- Sira'kess Guardian
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=326, `dmg_multiplier`=1 WHERE `entry`=41588; -- Sira'kess Sea Witch
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=326, `dmg_multiplier`=1 WHERE `entry`=41589; -- Sira'kess Tide Priestess
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=41590; -- Fathom-Caller Azrajar
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=41592; -- Deepfin Seer
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41593; -- Deepfin Scrounger
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=1 WHERE `entry`=41601; -- Merciless One
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=41606; -- Kvaldir Seahorror
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41607; -- Idra'kess Warlord
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=1 WHERE `entry`=41608; -- Idra'kess Prophet
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=41609; -- Silversand Burrower
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=41611; -- Amberstill Mountaineer
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41612; -- Huntress Iczelia
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=41613; -- Greenwarden Sentinel
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41615; -- Ferilon Leafborn
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41616; -- Vasuuvata
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41617; -- Jeffrey Gregarius
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=41618; -- Erunak Stonespeaker
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41619; -- Grove Druid
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=41621; -- Commander Thorak
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41622; -- Victorina
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41623; -- Aluril
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=41627; -- Farwatcher Guardian
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=41629; -- Drowned Warrior
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=93, `attackpower`=132, `dmg_multiplier`=1 WHERE `entry`=41635; -- Iolo
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=41637; -- Vera
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=41638; -- Houndmaster Jonathan
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=41642; -- Wil'hai
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=41643; -- Bloodcrazed Thresher
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=41644; -- Faceless Defiler
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=41646; -- Crushing Eel
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=41647; -- Deep Remora
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=41648; -- Ick'thys the Unfathomable
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=41650; -- Nether Fiend
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=41652; -- Twilight Champion
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=41654; -- Ur'Goz
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=41655; -- Sku'Bu
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=41656; -- Neph'Lahim
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=326, `dmg_multiplier`=1 WHERE `entry`=41657; -- Twilight Candidate
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=326, `dmg_multiplier`=2.25 WHERE `entry`=41658; -- Ascendant of the Deeps
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=2.25 WHERE `entry`=41659; -- Hallazeal the Ascended
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=41660; -- Pincer X2 Crewmember
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=41661; -- Verne Crewmember
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=2.25 WHERE `entry`=41662; -- Captain Glovaal
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=2.25 WHERE `entry`=41663; -- Captain "Jewels" Verne
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=41664; -- Salsbury the "Help"
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=41665; -- Jorlan Trueblade
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=41666; -- Engineer Hexascrub
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=41667; -- Lieutenant "Foxy" Topper
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=41668; -- Blood Guard Toldrek
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=41669; -- Fiasco Sizzlegrin
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=41670; -- Sergeant Gertrude
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=41671; -- Trapped Miner
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=41673; -- Krom'gar Enforcer
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=41674; -- Taluka the Hunter
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41675; -- Vernal the Fixer
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=41676; -- Ya'mon
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=41677; -- Krom'gar Grunt
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=41678; -- Blood Guard Aldo Rockrain
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=41679; -- Sergeant Dontrag
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=41680; -- Scout Utvoch
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=41681; -- Horde Infantry
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=41731; -- Overseer Idra'kess
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41743; -- Krom'gar Caravan Hauler
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=41746; -- Gilblin Plunderer
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=41762; -- Helm's Bed Surger
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=41763; -- Frozen Mountaineer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=41786; -- Sergeant Bahrum
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=289, `dmg_multiplier`=1 WHERE `entry`=41795; -- Idra'kess Mistress
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=41804; -- Khurgorn Singefeather
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=41846; -- Mathel Rustwrench
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=41848; -- Mathel's Flying Machine
UPDATE `creature_template` SET `mindmg`=200, `maxdmg`=341, `attackpower`=392, `dmg_multiplier`=1 WHERE `entry`=41853; -- Commander Stonebreaker
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41854; -- Sentinel Ambusher
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=47, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=41864; -- Huntress Davinia
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=41865; -- Darnassian Scout
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=41870; -- Krom'gar Enforcer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=41871; -- Earthwatcher Komo
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=41874; -- Earthwatcher Shaman
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=41878; -- Earthwatcher Faldor
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=41890; -- Barshuk Heavyhammer
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=41891; -- Mirkin
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41892; -- Felonius Stark
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=41893; -- Gelbin
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=41897; -- Dun Morogh Mountaineer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=41898; -- Dun Morogh Rifleman
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=41902; -- Dark Iron Pyromancer
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41903; -- Tender Aru
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=1 WHERE `entry`=41908; -- Orako
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=41909; -- Airfield Guard
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=41916; -- Underlight Nibbler
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41922; -- Pyreshell Scuttler
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41925; -- Coldlight Hunter
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=41926; -- Coldlight Oracle
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=41927; -- Devious Great-Eel
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=41935; -- Northwatch Encroacher
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41937; -- Marshal Paltrow
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41944; -- Galen Trollbane
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41949; -- Dark Ranger Alina
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=41955; -- Imprisoned Warrior
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41966; -- Deathstalker Maudria
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=41979; -- Naz'jar Honor Guard
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=41983; -- Apprentice Kryten
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=41988; -- The Bomb
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=41989; -- Frightened Peon
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=41990; -- Scout Utvoch
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=41991; -- Sergeant Dontrag
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=41992; -- Blastgineer Igore
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=41993; -- Raging Earth Elemental
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=41997; -- Famished Great Shark
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=41998; -- Famished Great Shark
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=42003; -- Dark Iron Golem
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=42010; -- General Grimaxe
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=42012; -- Dark Iron Invader
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=42014; -- Ironforge Bomber
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=1 WHERE `entry`=42023; -- Subjugator Devo
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=42028; -- Krom'gar Quartermaster
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=42029; -- Rope Ladder
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=42032; -- Sludgewerks Enforcer
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=2.25 WHERE `entry`=42033; -- Jibbly Rakit
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=110, `dmg_multiplier`=2.25 WHERE `entry`=42039; -- High Chieftain Cliffwalker
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=42040; -- Cliffwalker Brave
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=42041; -- Dragonmaw Whelpstealer
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=42042; -- Ebon Whelp
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=42043; -- Ebon Slavehunter
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=2.25 WHERE `entry`=42044; -- Pyrricion
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=96, `dmg_multiplier`=2.25 WHERE `entry`=42047; -- Masha Cliffwalker
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=2.25 WHERE `entry`=42050; -- General Grebo
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=42051; -- Faceless Defiler
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=42088; -- Goutgut
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=42090; -- Arathi Deathstalker
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=42091; -- Orthus Cliffwalker
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=42092; -- Repaired Bomber
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=42101; -- Helpless Young Druid
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=42104; -- Wolf
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=42107; -- Dragonmaw Grunt
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=42108; -- Seabrush Terrapin
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=42110; -- Bluegill Puddlejumper
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=42111; -- Bluegill Forager
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=42112; -- Scourgut Remora
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=42113; -- Spinescale Hammerhead
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=42114; -- Alliance Sea-Scout
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=42115; -- Hellscream Seadog
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=2.25 WHERE `entry`=42129; -- Moira Thaurissan
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=42130; -- Gor'mul
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=2.25 WHERE `entry`=42131; -- Falstad Wildhammer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=42146; -- Ambassador Slaghammer
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=42147; -- Dark Iron Guard
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=42154; -- Twilight Convert
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=42155; -- Twilight Rifleman
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=42160; -- Thargas Anvilmar
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=42170; -- Snow Leopard
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=42171; -- Snow Leopard Cub
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=42174; -- Rixa Quickwrench
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=42175; -- Rixa's Flying Machine
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=42181; -- Velskar
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=42184; -- Toxic Sludge
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=42185; -- Living Contamination
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=42197; -- L'ghorek
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=1 WHERE `entry`=42210; -- Compelled Elemental
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=42215; -- Kolorath
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=42216; -- Stormwind Army Registrar
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=42218; -- Stormwind Royal Guard
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=42221; -- Rockjaw Bonepicker
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=42222; -- Rockjaw Marauder
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1 WHERE `entry`=42223; -- Irradiated Technician
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1 WHERE `entry`=42224; -- Repaired Mechano-Tank
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=42225; -- Alliance Prisoner
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1 WHERE `entry`=42226; -- Crushcog Battle Suit
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1 WHERE `entry`=42228; -- Okril'lon Scout
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=42, `dmg_multiplier`=1 WHERE `entry`=42230; -- Grawmug
UPDATE `creature_template` SET `mindmg`=115, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1 WHERE `entry`=42231; -- Captain Metlek
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1 WHERE `entry`=42235; -- Darktail Bonepicker
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1 WHERE `entry`=42244; -- Drowned Gilnean Merchant
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1 WHERE `entry`=42248; -- Drowned Gilnean Sailor
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1 WHERE `entry`=42249; -- Drowned Gilnean Settler
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=42256; -- Marshal McCree
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=42259; -- Dead Thief
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=42262; -- Leyan Steelson
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=42264; -- Buttonwillow McKittrick
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=42280; -- Twilight Devotee
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=42281; -- Twilight Devotee
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=2.25 WHERE `entry`=42285; -- Faceless Overseer
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=42286; -- Young Snow Leopard
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=42288; -- Robby Flay
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=42290; -- Winter Wolf
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=42291; -- Crushcog Sentry-Bot
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1 WHERE `entry`=42296; -- Shadowsworn Spellblade
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=220, `dmg_multiplier`=1 WHERE `entry`=42297; -- Shadowsworn Occultist
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=91, `dmg_multiplier`=1 WHERE `entry`=42298; -- Kasim Sharim
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=42299; -- Watcher Grimeo
UPDATE `creature_template` SET `mindmg`=122, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1 WHERE `entry`=42301; -- Horde Portal-Sentry
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=2.25 WHERE `entry`=42308; -- Lieutenant Horatio Laine
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=42309; -- Stormwind Investigator
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=42311; -- Crime Scene Alarm-o-Bot
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=2.25 WHERE `entry`=42316; -- Gnomeregan Infantry
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=2.25 WHERE `entry`=42317; -- High Tinker Mekkatorque
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=42319; -- Gnomeregan Infantry
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=2.25 WHERE `entry`=42323; -- "Doc" Cogspin
UPDATE `creature_template` SET `mindmg`=200, `maxdmg`=341, `attackpower`=392, `dmg_multiplier`=1 WHERE `entry`=42324; -- Drill Sergeant Steamcrank
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=42329; -- Gnomeregan Trainee
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=42331; -- Bipsi Frostflinger
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=42332; -- Manus Swiftneedle
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=42334; -- Marl Wormthorn
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=42335; -- Elly Sharpedge
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=209, `attackpower`=242, `dmg_multiplier`=1 WHERE `entry`=42336; -- Tainted Black Bear
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=209, `attackpower`=242, `dmg_multiplier`=1 WHERE `entry`=42337; -- Tainted Nightstalker
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=209, `attackpower`=242, `dmg_multiplier`=1 WHERE `entry`=42338; -- Tainted Screecher
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=42339; -- Canal Crab
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=42341; -- Barbfin Skimmer
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=42342; -- Energized Harvest Reaper
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=42344; -- Rohan Sunveil
UPDATE `creature_template` SET `mindmg`=124, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1 WHERE `entry`=42346; -- Clarya Sunveil
UPDATE `creature_template` SET `mindmg`=133, `maxdmg`=227, `attackpower`=268, `dmg_multiplier`=1 WHERE `entry`=42349; -- Mayor Charlton Connisport
UPDATE `creature_template` SET `mindmg`=133, `maxdmg`=227, `attackpower`=268, `dmg_multiplier`=1 WHERE `entry`=42350; -- Garrod Pubhammer
UPDATE `creature_template` SET `mindmg`=133, `maxdmg`=227, `attackpower`=268, `dmg_multiplier`=1 WHERE `entry`=42352; -- Cenarion Observer Shayana
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=42353; -- Jarvi Shadowstep
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=42357; -- Hulking Goretusk
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1 WHERE `entry`=42359; -- Okril'lon Infantry
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=2.25 WHERE `entry`=42361; -- Ascended Zealot
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=42366; -- Kelsey Steelspark
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1 WHERE `entry`=42367; -- Rockpool Gladiator
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=42381; -- Overloaded Harvest Golem
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=42383; -- Transient
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=42384; -- Homeless Stormwind Citizen
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=42385; -- Orphan
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=42386; -- Homeless Stormwind Citizen
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=42387; -- Thug
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=42390; -- Small-time Hustler
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=42391; -- West Plains Drifter
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=42396; -- Nevin Twistwrench
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=42405; -- Two-Shoed Lou
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=42406; -- Hoboair
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=42407; -- Sentinel Hill Guard
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=42418; -- Voldrin's Hold Deckhand
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=42421; -- Stormwind Fisherman
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=42424; -- Stygian Bounty Deckhand
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=2.25 WHERE `entry`=42425; -- Captain Alpert
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=42426; -- Tina Skyden
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=42428; -- Devout Follower
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=42453; -- Azsh'ir Abyss Priestess
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=42464; -- Grognard
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=42465; -- Therazane
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=42467; -- Diamant the Patient
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=42468; -- Felsen the Enduring
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=42469; -- Kor the Immovable
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=42470; -- Ma'haat the Indomitable
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=42471; -- Boden the Imposing
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=42472; -- Gorsik the Tumultuous
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=42473; -- Grimy Greasefingers
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42475; -- Fungal Behemoth
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=42479; -- Servant of Therazane
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=42488; -- Chief Engineer Yoon
UPDATE `creature_template` SET `mindmg`=200, `maxdmg`=341, `attackpower`=392, `dmg_multiplier`=1 WHERE `entry`=42489; -- Captain Tread Sparknozzle
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=42490; -- Elgin Clickspring
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=42491; -- Hinkles Fastblast
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=42497; -- Mama Celeste
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=42498; -- Jimb "Candles" McHannigan
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=2.25 WHERE `entry`=42501; -- Wounded Infantry
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=42504; -- Mature Swine
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=2.25 WHERE `entry`=42505; -- Image of Razlo Crushcog
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=42506; -- Marogg
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=42518; -- Wildhammer Warrior
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42521; -- Jaspertip Swarmer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=42522; -- Stone Drake
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42523; -- Stone Bat
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42524; -- Jaspertip Borer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42525; -- Jaspertip Ravager
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42527; -- Irestone Rumbler
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=42548; -- Muddy Crawfish
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=14, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=42552; -- Physician's Assistant
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=42553; -- Engineer Grindspark
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=47, `dmg_multiplier`=1 WHERE `entry`=42554; -- Wildhammer Shaman
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=47, `dmg_multiplier`=1 WHERE `entry`=42555; -- Revantusk Hexxer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=42573; -- Earthcaller Yevaa
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=42574; -- Initiate Goldmine
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=42575; -- Hope Saldean
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42606; -- Rockslice Flayer
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42607; -- Rockslice Ripper
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=42608; -- Battered Red Drake
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=42609; -- Gan'dranda
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=42610; -- Thornar Thunderclash
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=42611; -- Tock Sprysprocket
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=42612; -- Malcom Fendelson
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=42613; -- Elder Torntusk
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=42614; -- Terrath the Steady
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=42616; -- Spanks
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=42618; -- Voldreka
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=3, `dmg_multiplier`=1 WHERE `entry`=42619; -- Novice Darkspear Warlock
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=42620; -- Lashtail Egg
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=42622; -- Eliza Darkgrin
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=42624; -- Kotonga
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=42626; -- Vallo Shinesteel
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1 WHERE `entry`=42630; -- Corporal Fizzwhistle
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=2.25 WHERE `entry`=42635; -- Ripsnarl
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1 WHERE `entry`=42637; -- Sauranok the Mystic
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=42638; -- Herezegor Flametusk
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=42640; -- Captain Krazz
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=42641; -- Warlord Krogg
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=42642; -- Primal Torntusk
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42643; -- Fleep
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42644; -- Smoot
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=42645; -- Captured Demolitionist
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42646; -- AWOL Grunt
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42647; -- Axebite Warrior
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=42650; -- Goblin Siegeworker
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=42651; -- Thoralius the Wise
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=42653; -- Jango Spothide
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=2.25 WHERE `entry`=42656; -- Mercenary
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=42669; -- Chasm Slime
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=42671; -- Chief Engineer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42672; -- Foreman Glibbs
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42673; -- Hellscream Demolisher
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=42676; -- Chief Engineer Petibas
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=42677; -- Moonbrook Thug
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=42689; -- Skittering Spiderling
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=42691; -- Stonecore Rift Conjurer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=42692; -- Stonecore Bruiser
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=42695; -- Stonecore Sentry
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=42696; -- Stonecore Warbringer
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=42708; -- Jessup McCree
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=42709; -- Gravy
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=42724; -- Umboda Three-Heads
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=42726; -- Earthwatcher Shaman
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=42730; -- Earthcaller Torunscar
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=42731; -- Earthmender Norsala
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=42732; -- Windspeaker Lorvarius
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=42773; -- Boss Bruggor
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=42775; -- Bilgewater Dockworker
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42777; -- Bilgewater Foreman
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=42779; -- Exhausted Earthguard Sentinel
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42780; -- Irestone Rumbler
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=42781; -- Servant of Therazane
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=42782; -- Colin O'Rourke
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=42789; -- Stonecore Magmalord
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=42804; -- Bilgewater Socialite
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=42808; -- Stonecore Flayer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=42810; -- Crystalspawn Giant
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=42811; -- Surkhan
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=42812; -- Priestess Hu'rala
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=42813; -- Kil'karil
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=42814; -- Skeezy Whillzap
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1 WHERE `entry`=42823; -- Twilight Priestess
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1 WHERE `entry`=42824; -- Twilight Pyremaw
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=42836; -- Bilgewater Mechanic
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=42845; -- Rock Borer
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=42852; -- Mountaineer Stonegrind
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=9, `dmg_multiplier`=1 WHERE `entry`=42853; -- Karen Murray
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=42857; -- Sacrificed Vilebranch
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=55, `attackpower`=94, `dmg_multiplier`=1 WHERE `entry`=42858; -- Braddok
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=42859; -- Wild Mature Swine
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42873; -- Anissa Matherly
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42875; -- Miriam Brassbomb
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42876; -- Frell "Fixit" Fastbolt
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42878; -- Salena Madeux
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=42879; -- Spawn of Shadra
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42880; -- Crushclaw
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=42896; -- Apothecary Surlis
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=42898; -- Darkcleric Marnal
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=42899; -- Quartz Stonetender
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=42900; -- Quartz Rockling
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=42901; -- Vilebranch Broodguard
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=42904; -- Rana Bubblefizz
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=42905; -- Grennor Strongbrew
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=42907; -- Cocooned Victim
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42908; -- Zun'ja
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42909; -- Ginna Gearcrank
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42910; -- Roxa
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42911; -- Larok
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42912; -- Snaptrap
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=42913; -- Shadraspawn Venomspitter
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=42914; -- High Priestess Lorthuna
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42916; -- Twilight Soulreaper
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42917; -- Twilight Duskwarden
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=42918; -- Zoltrik Drakebane
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=42919; -- Qiaga the Keeper
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42921; -- Dragul Giantbutcher
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42923; -- Boldrich Stonerender
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42924; -- Twilight Laborer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=42925; -- Ravenous Tunneler
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=2.25 WHERE `entry`=42928; -- Muradin Bronzebeard
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=42933; -- Ciara Deepstone
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=42936; -- Sentinel Cordressa Briarbow
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=42937; -- Blackrock Invader
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=42938; -- Kurtok the Slayer
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=2.25 WHERE `entry`=42946; -- Kor'kron Deathbringer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=42953; -- Benjamin Sykes
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42963; -- Barracks Officer Milson
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42966; -- Chase Whithers
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42967; -- Quartermaster Rallings
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=42968; -- Krennan Aranas
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=42970; -- Gilneas Refugee
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=42972; -- Winzy Truesprocket
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=42974; -- Darkbreak Guard
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=42983; -- Bartlett the Brave
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43000; -- Goss the Swift
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=43001; -- Sunwalker Reha
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=43002; -- Bastia
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=43003; -- Venture Co. Sawblade
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=91, `dmg_multiplier`=1 WHERE `entry`=43004; -- Seer Kaya
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=30, `dmg_multiplier`=1 WHERE `entry`=43005; -- Irriende
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=43006; -- Rhyanda
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=43010; -- Delios Silverblade
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=43011; -- Lyndsay Hollister
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=43013; -- Sunwalker Iopi
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=43015; -- Seer Alsoomse
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=43017; -- Fahlestad
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=43019; -- Teldorae
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=43021; -- Adrius
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43026; -- Deepstone Elemental
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=43032; -- Slain Cannoneer
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43034; -- Colin Field
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43036; -- Flint Oremantle
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=2.25 WHERE `entry`=43041; -- Ol' Gummers
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43042; -- Robert Rhodes
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43043; -- Colin Swifthammer
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43045; -- James Stillair
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=43048; -- Captain Skullshatter
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=43049; -- Nesingwary Hired Gun
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43052; -- Raskha
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43053; -- Hizzle
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=43062; -- Bort
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=43063; -- Cromula
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=2.25 WHERE `entry`=43065; -- Maruut Stonebinder
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43071; -- Crag Rockcrusher
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43072; -- Nora Baldwin
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43080; -- Guard Flippit
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43081; -- Guard Bateman
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=43083; -- Redridge Fox
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43086; -- Skeezie
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43087; -- Paola Baldwin
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43088; -- Yedrin
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=2.25 WHERE `entry`=43092; -- Gnomeregan Recruit
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=2.25 WHERE `entry`=43094; -- Canyon Ettin
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43095; -- Sassy Hardwrench
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43096; -- Flem Gizzix
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43097; -- Mixmaster Jasper
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43098; -- Chabal
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=43099; -- Stone Guard Towhide
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43101; -- Son of Kor
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=43103; -- Bluetip Thresher
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43104; -- Rhoda Bowers
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=43106; -- Redridge Fox Kit
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43107; -- Graham McAllister
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43108; -- Kerr Ironsight
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43109; -- Dron Blastbrew
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43114; -- Salena
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=43115; -- Dormant Stonebound Elemental
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43121; -- Preda
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=1 WHERE `entry`=43123; -- Living Blood
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43124; -- Anette Williams
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43134; -- Stone Trogg Ambusher
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43138; -- Stonehearth Defender
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43139; -- Zan'zata
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43140; -- Chef Boiardi
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43141; -- Innkeeper Nerius
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43149; -- Eriyala
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43150; -- Impaled Blackrock Orc
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43151; -- Shurrak
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43152; -- Shady Quickshank
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43154; -- Kurg
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43155; -- Taley
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=43156; -- Fraggar Thundermantle
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=43157; -- Doran Steelwing
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43158; -- Mercurial Ooze
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=43160; -- Earthbreaker Dolomite
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43168; -- Gravel Longslab
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=2.25 WHERE `entry`=43169; -- Clay Mudaxle
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=43170; -- Earthen Geomancer
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=43173; -- Redridge Garrison Watchman
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43174; -- Stone Trogg Digger
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=43178; -- Libby Parker
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=43181; -- Shalehide Basilisk
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=43182; -- Petrified Stone Bat
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=43183; -- Freshwater Eel
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=43185; -- Blackrock Overseer
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=43196; -- Huge Boulder
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43200; -- Gilda Cloudcaller
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=43201; -- Cavern Grunt
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=43203; -- Gmurgl
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=43204; -- Orgus
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1 WHERE `entry`=43205; -- Captain Bartholomew Softbeard
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43216; -- Swift Seahorse
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=43217; -- Bilgewater Rocket-jockey
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=326, `dmg_multiplier`=1 WHERE `entry`=43218; -- Twilight Bloodshaper
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=43219; -- Hardwrench Guard
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43220; -- Briglar
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43221; -- Colonel Troteman
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=43222; -- Redridge Citizen
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=43223; -- Zanzil Mindslave
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43224; -- Wounded Mountaineer
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43228; -- Stone Trogg Berserker
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=43229; -- Injured Earthen
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1 WHERE `entry`=43230; -- Crushcog Technician
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=43232; -- Earthen Champion
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=43233; -- Stonehearth Geomaster
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43239; -- Razgar
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=43247; -- Redridge Citizen
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=43248; -- Big Earl
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=43249; -- Dumpy
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43254; -- Energized Geode
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43258; -- Lodestone Elemental
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=2.25 WHERE `entry`=43270; -- Messner
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=2.25 WHERE `entry`=43272; -- Jorgensen
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=2.25 WHERE `entry`=43274; -- Krakauer
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=2.25 WHERE `entry`=43275; -- Danforth
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=43277; -- Don Omar
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=43278; -- Ashley Blank
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43287; -- Swift Seahorse
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43290; -- Francis Greene
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=43291; -- Wilson
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=43292; -- Adele
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=43294; -- Gonzalez
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=43297; -- Gnome Traveler
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=2.25 WHERE `entry`=43298; -- Wildhammer Lookout
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43299; -- Deathstalker Lookout
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=43301; -- Devon Rackled
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=43304; -- Heron Tuns
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=670, `dmg_multiplier`=2.25 WHERE `entry`=43319; -- Earthmender Deepvein
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=43325; -- Rockjaw Fungus-Flinger
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=43327; -- Murdunk
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=43328; -- Blitz Blastospazz
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=43329; -- Homurk
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=43330; -- Blitz's Gyrocopter
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=43331; -- Golden Stonefish
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=43339; -- Gorgonite
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=2.25 WHERE `entry`=43340; -- Blackrock Battle Worg
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=52, `dmg_multiplier`=2.25 WHERE `entry`=43341; -- Blackrock Worg Captain
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43344; -- Berracite
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=56, `dmg_multiplier`=2.25 WHERE `entry`=43350; -- Utroka the Keymistress
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=43352; -- Buin "Phloppy" Freehand
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=43354; -- Kelaan
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=43355; -- Lowan Featherbreeze
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43356; -- Porecite the Silent
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43358; -- Feldspar the Eternal
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=43360; -- Refugee
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=56, `dmg_multiplier`=2.25 WHERE `entry`=43363; -- Ritualist Tarak
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=43364; -- Bloodsail Hooch-Hound
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=43367; -- Verlok Grubthumper
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1 WHERE `entry`=43368; -- Verlok Shroomtender
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=2.25 WHERE `entry`=43369; -- Overlord Barbarius
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43371; -- Arlen Marsters
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43372; -- Fungalmancer Glop
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=43373; -- Raging Crystal-walker
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=43374; -- Pulsing Geode
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=43376; -- Freewheelin' Juntz Fitztittle
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=43377; -- Ephram "Midriff" Moonfall
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=43384; -- Monann Coldforge
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=43388; -- Doomshroom
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43391; -- Millhouse Manastorm
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=43395; -- Windspeaker Lorvarius
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=2.25 WHERE `entry`=43397; -- Seer Kormo
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1 WHERE `entry`=43405; -- Mariahn the Soulcleanser
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=43417; -- Skymane Bonobo
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=43418; -- Sprinkle Noggenfogger
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=43419; -- Rembar Bellanne
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43420; -- Innkeeper Kyteran
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=43424; -- Ayriala
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=43425; -- Syleath Fairglade
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=43428; -- Faeyrin Willowmoon
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=43429; -- Taryel Firestrike
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=43430; -- Stonecore Berserker
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43431; -- Periale
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=43436; -- Ceriale Duskwhisper
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43438; -- Corborus
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=43439; -- Nyrisse
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=43451; -- Olivia Jayne
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43453; -- Tobias Mistmantle
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=43454; -- "Good-Boy" Bruce
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=9, `dmg_multiplier`=1 WHERE `entry`=43455; -- Saripal Smolderbrew
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1 WHERE `entry`=43456; -- Troggzor the Earthinator
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=43464; -- Halbin Frosthammer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=43480; -- Temperamental Rumbler
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=43485; -- Ticky the Alchemist
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=43503; -- Earthmender Norsala
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1 WHERE `entry`=43504; -- Yancey Grillsen
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=43505; -- Bossy
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=43513; -- Verlok Pillartumbler
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=43514; -- Earthmender Norsala
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=43532; -- Muckdweller
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=58, `dmg_multiplier`=2.25 WHERE `entry`=43533; -- Blackrock Drake Rider
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=58, `dmg_multiplier`=2.25 WHERE `entry`=43535; -- Blackrock Warden
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=70, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=43536; -- Southsea Mako
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=43537; -- Stonecore Earthshaper
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=43545; -- Opalescent Guardian
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43556; -- "Dead-Eye" Drederick McGumm
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=9, `dmg_multiplier`=1 WHERE `entry`=43558; -- Marie Allen
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43560; -- Smilin' Timmy Sticks
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43568; -- Dro Shadowfree
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43570; -- Brock Rockbeard
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=43571; -- Kidnapped Redridge Citizen
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=43572; -- Kidnapped Redridge Citizen
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43573; -- Kellen Kuhn
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43586; -- Opal Stonethrower
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=43596; -- Venture Co. Oil Worker
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=43598; -- Stone Trogg Beast Tamer
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=43599; -- Thornguard
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43605; -- Bloodsail Oarsman
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43606; -- Innkeeper Duras
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43612; -- High Prophet Barim
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43614; -- Lockmaw
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=43615; -- Toral
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1 WHERE `entry`=43616; -- Stone Trogg Earthrager
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43617; -- Lursa
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43619; -- Beh'tor
UPDATE `creature_template` SET `mindmg`=32, `maxdmg`=55, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=43621; -- Targol
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43624; -- Innkeeper Linkasa
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=43625; -- Shenara
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43630; -- Drek
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43633; -- Innkeeper Chin'toka
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43634; -- Vorcha
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43636; -- Bloodsail Corsair
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43637; -- Marrok
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43641; -- Aeosera
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43645; -- Ornag
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43646; -- Kitanga
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43653; -- Hellscream Grunt
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43657; -- Silverwind Vanquisher
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43659; -- Long John Copper
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43660; -- Enormous Shawn Stooker
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43661; -- Wailing Mary Smitts
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=43662; -- Unbound Earth Rager
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=43692; -- Kristin Cazarez
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=43693; -- Darlene Stokx
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=43694; -- Katie Stokx
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43697; -- John Shelby
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43699; -- Innkeeper Keirnan
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43701; -- Brolan Galebeard
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43702; -- Dominic Galebeard
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=43704; -- Dire Wolf
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=43705; -- Geezle Spinspark
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=43709; -- Feezle Spinspark
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=43710; -- Meezle Spinspark
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=43715; -- Hiri'watha Deathguard
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=43723; -- Jamie Crester
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=43724; -- Lee Crester
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=43725; -- Curtis Crester
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=2.25 WHERE `entry`=43727; -- Lorna Crowley
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=43730; -- Oliver Harris
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=37, `dmg_multiplier`=1 WHERE `entry`=43731; -- Sister Elsington
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=43732; -- Corpseweed
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=43738; -- Apprentice Fess
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43739; -- Bitsy
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=2.25 WHERE `entry`=43742; -- Vengeful Protector
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=43747; -- Hippogryph
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43748; -- Janice Winters
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=42, `dmg_multiplier`=1 WHERE `entry`=43749; -- Tobias Mistmantle
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43750; -- Marvin Winters
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=37, `dmg_multiplier`=2.25 WHERE `entry`=43761; -- Morbent Fel
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=43766; -- Roslyn Paxton
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=43768; -- Tannec Stonebeak
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=43769; -- Bralla Cloudwing
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43770; -- Tathan Thunderstone
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43771; -- Mixi
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=43772; -- Stek Orespazz
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43773; -- Stella Boomboom
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43774; -- Feena Simplesap
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43776; -- Zizo Seasizzle
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43792; -- Therazane
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=43795; -- Aponi Brightmane
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=43796; -- Tahu Sagewind
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=43799; -- Lurking Worgen
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43804; -- Gorsik the Tumultuous
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43805; -- Felsen the Enduring
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43806; -- Terrath the Steady
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43807; -- Diamant the Patient
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=43809; -- Earthcaller Torunscar
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43810; -- Gorsik the Tumultuous
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=43814; -- Lurking Worgen
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=2.25 WHERE `entry`=43818; -- Maruut Stonebinder
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=43819; -- Kirsten Westmill
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=43820; -- Scott Westmill
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=43822; -- Thomas Westmill
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=43823; -- Leslie Wainwright
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=43824; -- Ann Wainwright
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=43825; -- Warren Wainwright
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=43835; -- Earthcaller Torunscar
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=43836; -- Windspeaker Lorvarius
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=43837; -- High Priestess Lorthuna
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=43840; -- Fineas G. Bankworthy
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=43841; -- Auctioneer Lauffer
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=43842; -- Auctioneer Fitzgerald
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=43849; -- Garley Lightrider
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=43870; -- Seer Beryl
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=43871; -- War Construct
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=43881; -- Delano Morisett
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=43883; -- Jensen Thomasson
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=43891; -- Vilebranch Zealot
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=43892; -- Morairania Horton
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43897; -- Pyrium Lodestone
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43898; -- Flint Oremantle
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=43910; -- Chief Anders
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=43911; -- Chief Gaulus
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=43912; -- Chief Esquivel
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=47, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=43913; -- Chief Miranda
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=43923; -- Forlorn Spirit
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=43945; -- Innkeeper Kritzle
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=43946; -- Innkeeper Kerntis
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=43948; -- Private Jackson
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=43949; -- Martang
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=43951; -- Durnok
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=43953; -- Niriap
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43954; -- Fungal Terror
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=43955; -- Naman
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=43956; -- Lokarbo
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=43957; -- Frazzik
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=43960; -- Stone Trogg Reinforcement
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=81, `dmg_multiplier`=1 WHERE `entry`=43964; -- Grux Sparklesnap
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43966; -- Twilight Dragonspawn
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43967; -- Twilight Scalesister
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43971; -- Stonescale Drake
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=91, `dmg_multiplier`=1 WHERE `entry`=43972; -- Cordelia Everseek
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43979; -- Gravin Steelbeard
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=91, `dmg_multiplier`=1 WHERE `entry`=43980; -- Chase Everseek
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=43981; -- Jadecrest Basilisk
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=43982; -- Vernon Soursprye
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=43984; -- Deactivated War Construct
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=43987; -- Skippy Hophappy
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=43988; -- Carthok
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=43991; -- Zaldaan
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=43992; -- Twilight Dragonstalker
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43993; -- Innkeeper Larisal
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=43994; -- Salustred
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=43997; -- Breg FullBeard
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=43998; -- Darvish Quickhammer
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=44001; -- Cedric Bronzeflint
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=44004; -- Gilnean Envoy
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44005; -- Teep Topup
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=44006; -- Innkeeper Daughny
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44007; -- Shep Goldtend
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=44010; -- Stormcaller Mylra
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=44013; -- Dragonmaw Envoy
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=44016; -- Coalpelt Bear
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=44017; -- Priestess Thaalia
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=44018; -- Wulfred Harrys
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=44019; -- Livingston Marshal
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=44020; -- Barn Owl
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=44021; -- Ghaliri
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44022; -- Frank Carlson
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=44025; -- Therazane
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=44027; -- Ardan Softmoon
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=44028; -- Silent Shade
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=44029; -- Clattering Coldwraith
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=44030; -- Draelan
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44034; -- Franklin Brinklestein
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=44035; -- Fungal Monstrosity
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=44039; -- Twilight's Hammer Prisoner
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=44040; -- Kramlod Farsight
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44043; -- Kinnel
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1 WHERE `entry`=44046; -- Earthen Ring Shaman
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=44047; -- Audrid Grenich
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=44049; -- Giant Mushroom
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=44057; -- Riverboat
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44058; -- Horton Hornblower
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=44076; -- Defaced Earthrager
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=44077; -- Stone Drake
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=44078; -- Earthbound Rumbler
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=44079; -- Agate Mancrusher
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=44080; -- Boden the Imposing
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=44081; -- Diamant the Patient
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=44082; -- Bronwyn Hewstrike
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=100, `dmg_multiplier`=1 WHERE `entry`=44083; -- Linzi Hewstrike
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=2.25 WHERE `entry`=44084; -- Maywiki
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=44086; -- Panicked Citizen
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=44089; -- Blackbelly Forager
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=44098; -- Writhing Terror
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=104, `dmg_multiplier`=1 WHERE `entry`=44099; -- Dask "The Flask" Gobfizzle
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44100; -- Goris
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=2.25 WHERE `entry`=44106; -- Hambone
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=44110; -- Ocean Shredfin
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=44112; -- Airwyn Bantamflax
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44113; -- Ironjaw Behemoth
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=44114; -- Wilkinson
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=44115; -- Ella
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=44116; -- Harbor Crawler
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44123; -- Emily Jackson
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=44124; -- Opal Stonethrower
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=44125; -- Chris Moller
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=44126; -- War Guardian
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=44131; -- Servant of Therazane
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=44142; -- Rockpool Tadpole
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=44143; -- Slate Quicksand
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=44148; -- Stonescale Matriarch
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=44149; -- Partially-Digested Mosshide Gnoll
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=72, `dmg_multiplier`=1 WHERE `entry`=44150; -- Slowly Decaying Mosshide Mystic
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1 WHERE `entry`=44151; -- Bouldergut
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=44152; -- Flint Oremantle
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=44153; -- Stonefather Oremantle
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=44158; -- Orgrimmar Skyway Peon
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=44160; -- Suspicious Peon
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=44161; -- Mosshide Scout
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=44162; -- Mosshide Basher
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=44163; -- Stonesplinter Worker
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=44164; -- Sunscale Ravager
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=44165; -- Sunscale Consort
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=44166; -- Grazing Zhevra
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=44167; -- Sunscale Raptor
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=44168; -- Southsea Recruit
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=1 WHERE `entry`=44170; -- Hezrul's Bloodguard
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=44172; -- Moldy Dark Iron Trapper Corpse
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=44176; -- Bluegill Wanderer
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44177; -- Innkeeper Bernice
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44178; -- Sweet Gary Guns
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44179; -- Harry No-Hooks
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44181; -- Daniel Roberts
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44182; -- Gurlgrl
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44183; -- Durango
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=44184; -- Loch Buzzard
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44186; -- Morango
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44187; -- Pechanga
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=44188; -- Bobcat
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=44189; -- Twilight Pyremaw
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44190; -- Innkeeper Draxle
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44191; -- Finzy Watchwoozle
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44192; -- Bobber Spazzspark
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44193; -- Marpi Greenwrench
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44194; -- Gruzz Thinxlotz
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44195; -- Smity Hammerhead
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44196; -- Pop Sodaslam
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=44197; -- "Commander" Nazrim
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=44198; -- Foreman Sharpsneer
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=44200; -- Scout Dorli
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=44204; -- Stonefather Oremantle
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=44209; -- Spotted Swellfish
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=44210; -- Sand Ray
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=44211; -- Giant Driftray
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=44218; -- Emerald Colossus
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44219; -- Logistics Officer Renaldo
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=44220; -- Jade Rager
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=44221; -- Scalesworn Cultist
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1 WHERE `entry`=44222; -- Seer Galekk
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1.5 WHERE `entry`=44226; -- Sarltooth
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=44230; -- Richard Trueflight
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=44231; -- Devon Manning
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=44232; -- Janice Myers
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=44233; -- William Kielar Jr.
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44235; -- Thaegra Tillstone
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44236; -- Myrla Stoneround
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=44237; -- Maegan Tillman
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=44238; -- Harrison Jones
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44239; -- Janric Moller
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=44240; -- Argent Hippogryph
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44241; -- Lana Dubing
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44243; -- Darnella Winford
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=101, `dmg_multiplier`=1 WHERE `entry`=44245; -- Faldren Tillsdale
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=101, `dmg_multiplier`=1 WHERE `entry`=44246; -- Magatha Silverton
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=44247; -- Wulf Hansreim
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=44249; -- Sarisse Jume
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=44251; -- Alma Deering
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44252; -- Karin
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=44256; -- Greypaw
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=44259; -- Gorged Gyreworm
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=44262; -- Mosshide Representative
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=44263; -- Bluegill Representative
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=44264; -- Tunnel Rat Lackey
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44267; -- Logistics Officer Salista
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=44268; -- Keep Watcher Kerry
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=44270; -- Innkeeper Hurnahet
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=44276; -- Innkeeper Lhakadd
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44277; -- Gary Henton
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44279; -- Donnach
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44280; -- Serena Arclight
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44283; -- Camran
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=44284; -- Northridge Spider
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=44285; -- Ramja Skyspinner
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=44286; -- Murhane
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=44287; -- Terndak
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=2.25 WHERE `entry`=44289; -- Abyssion
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=44292; -- Bluegill Mudskipper
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44294; -- Quartermaster Lawson
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44296; -- Turrana
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=44297; -- Kurinika Spiritseeker
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44299; -- Beathan Firebrew
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=44300; -- Quartermaster Dernhak
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44301; -- Roger Sternbach
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44302; -- Quartermaster Winfred
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44303; -- Quartermaster Rutherford
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44304; -- Quartermaster Higgins
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=44305; -- Trank
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44307; -- Cranston Fizzlespit
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=2.25 WHERE `entry`=44309; -- Innkeeper Grak
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=1 WHERE `entry`=44310; -- Kroff
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=2.25 WHERE `entry`=44311; -- Overseer Struk
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=2.25 WHERE `entry`=44312; -- Chef Braf
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=1 WHERE `entry`=44313; -- Neka
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=110, `dmg_multiplier`=1 WHERE `entry`=44315; -- Desiccated Magus
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44316; -- Desiccated Spearman
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=2.25 WHERE `entry`=44317; -- The Ravenian
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=2.25 WHERE `entry`=44318; -- Rattlegore
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=89, `dmg_multiplier`=1 WHERE `entry`=44321; -- Sarah Lightbrew
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=89, `dmg_multiplier`=1 WHERE `entry`=44322; -- Tonya Lightbrew
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=110, `dmg_multiplier`=2.25 WHERE `entry`=44323; -- Darkmaster Gandling
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1 WHERE `entry`=44324; -- Keri Thunderbrew
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1 WHERE `entry`=44325; -- Mama Morton
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=110, `dmg_multiplier`=1 WHERE `entry`=44326; -- Andorhal Deathguard
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=110, `dmg_multiplier`=1 WHERE `entry`=44328; -- Andorhal Defender
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1 WHERE `entry`=44330; -- Gina Gellar
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1 WHERE `entry`=44333; -- Marcy Lewis
UPDATE `creature_template` SET `mindmg`=133, `maxdmg`=227, `attackpower`=268, `dmg_multiplier`=1 WHERE `entry`=44334; -- Donna Berrymore
UPDATE `creature_template` SET `mindmg`=133, `maxdmg`=227, `attackpower`=268, `dmg_multiplier`=1 WHERE `entry`=44335; -- Willard C. Bennington
UPDATE `creature_template` SET `mindmg`=133, `maxdmg`=227, `attackpower`=268, `dmg_multiplier`=1 WHERE `entry`=44336; -- Stephan Kebbel
UPDATE `creature_template` SET `mindmg`=133, `maxdmg`=227, `attackpower`=268, `dmg_multiplier`=1 WHERE `entry`=44337; -- Maurice Essman
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=44339; -- Salaran
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=44340; -- Janella
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=44341; -- Aeilara
UPDATE `creature_template` SET `mindmg`=122, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1 WHERE `entry`=44343; -- Quartermaster Dekrok
UPDATE `creature_template` SET `mindmg`=122, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1 WHERE `entry`=44344; -- Quartermaster Apone
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=44345; -- Cannary Caskshot
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44346; -- Brandon Merriweather
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44347; -- Werner Eastbrook
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44348; -- Carey Willis
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44349; -- Munada
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44350; -- Tharm Wildfire
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44351; -- Stormcaller Jalara
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44354; -- Grantor
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=44357; -- Exhausted Fire Elemental
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=44358; -- Exhausted Air Elemental
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=2.25 WHERE `entry`=44361; -- Earthmender Narvra
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=2.25 WHERE `entry`=44362; -- Earthmender Govrum
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=44363; -- Greater Air Elemental
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=44364; -- Greater Fire Elemental
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44371; -- Yuldris Smolderfury
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=44372; -- Avalanchion
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=44374; -- Sherm
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=1 WHERE `entry`=44380; -- Jabul
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=44383; -- Regina Salister
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=44388; -- Gilnean Militia
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=44390; -- Spiny Rock Crab
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=44392; -- Muzzle
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=194, `dmg_multiplier`=1 WHERE `entry`=44393; -- Bolner Hammerbeak
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=44394; -- Dalga Hammerbeak
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=44398; -- Snart Razzlegrin
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=44399; -- Nah'te
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=44402; -- Auld Stonespire
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44405; -- Wounded Militia
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=44407; -- Mixi Sweetride
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=44408; -- Selara
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=44409; -- Nancy Skybrew
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=44410; -- Jake Badlands
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=2.25 WHERE `entry`=44415; -- Spirit of Agamaggan
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1 WHERE `entry`=44417; -- Rations Officer Flexgear
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=44425; -- Crimson Shale Spider
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=44427; -- Crowley's Horse
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=44429; -- Crowley's Horse
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=44432; -- Alliance Fox Hunter
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=44433; -- Alliance Laborer
UPDATE `creature_template` SET `mindmg`=47, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=44435; -- Argent Lumberjack
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=44436; -- Hearthglen Shieldbearer
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=44437; -- Hearthglen Recruit
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=2.25 WHERE `entry`=44438; -- Hearthglen Commander
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=44439; -- Hearthglen Drudge
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=44440; -- Hearthglen Honor Guard
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=2.25 WHERE `entry`=44441; -- Highlord Tirion Fordring
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=93, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=44442; -- Instructor Malicia
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=89, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=44443; -- Cult-Sworn Warlock
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=89, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=44444; -- Cult-Sworn Pugilist
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=93, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=44445; -- Krastinovian Disciple
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=44447; -- Andorhal Deathguard
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=2.25 WHERE `entry`=44451; -- Lady Sylvanas Windrunner
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44452; -- Koltira Deathweaver
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=44453; -- Thassarian
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=44454; -- Field Agent Kaartish
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=44455; -- Sergeant Cleese
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=44456; -- Adrine Towhide
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=44457; -- Selyria Groenveld
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=44459; -- Celestine of the Harvest
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=44460; -- Gwen Armstead
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=44461; -- Huntsman Blake
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=44462; -- Jearl Donald
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=44463; -- Baron
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=44464; -- Loren the Fence
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=9, `dmg_multiplier`=2.25 WHERE `entry`=44465; -- Myriam Spellwaker
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=44466; -- Lindsay Ravensun
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=44467; -- Lang Loosegrip
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=44468; -- Sister Almyra
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=44469; -- Vitus Darkwalker
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=44470; -- Injured Citizen
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=44471; -- Lurid
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=44472; -- Kelly Dumah
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=44473; -- Shaggy Black Bear
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=44474; -- Whitetail Fox
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=44475; -- Brownfeather Hawk
UPDATE `creature_template` SET `mindmg`=47, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=44478; -- Redpine Gnoll
UPDATE `creature_template` SET `mindmg`=47, `maxdmg`=85, `attackpower`=116, `dmg_multiplier`=1 WHERE `entry`=44479; -- Redpine Shaman
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=44481; -- Diseased Hawk
UPDATE `creature_template` SET `mindmg`=47, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=44482; -- Hulking Plaguebear
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=112, `attackpower`=152, `dmg_multiplier`=2.25 WHERE `entry`=44483; -- Kirtonos the Herald
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=93, `attackpower`=132, `dmg_multiplier`=1 WHERE `entry`=44484; -- Flesh-Cobbled Brute
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=93, `attackpower`=132, `dmg_multiplier`=1 WHERE `entry`=44485; -- Flesh-Cobbled Ripper
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=93, `attackpower`=132, `dmg_multiplier`=1 WHERE `entry`=44486; -- Unholy Corpuscle
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=44546; -- Duneclaw Burrower
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=44547; -- Bloodfang Scavenger
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=44549; -- Bloodfang Scavenger
UPDATE `creature_template` SET `mindmg`=47, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=44551; -- Rabid Fox
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44562; -- Opengut Behemoth
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44564; -- Wounded Trainee
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=44568; -- Dune Rattler
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=44569; -- Sand Lasher
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=44576; -- Proto-Strider
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=44578; -- Naga Hatchling
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=44579; -- Naga Hatchling
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=44580; -- Naga Hatchling
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44582; -- Theresa Denman
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44583; -- Terrance Denman
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=44592; -- Fallen Human
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=44593; -- Fallen Human
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1 WHERE `entry`=44594; -- Sunburst Adder
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1 WHERE `entry`=44595; -- Sand Slitherer
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1 WHERE `entry`=44598; -- Desert Bloom
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1 WHERE `entry`=44599; -- Duneclaw Broodlord
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=48, `dmg_multiplier`=2.25 WHERE `entry`=44608; -- Agatha
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=44611; -- Wastewander Survivalist
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=44612; -- Wastewander Tracker
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=85, `dmg_multiplier`=1 WHERE `entry`=44613; -- Wastewander Darkcaster
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44615; -- Grand Executor Mortuus
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=44616; -- Haywire Battle-Chicken
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44617; -- Wounded Sentinel
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=44618; -- Rusted Skystrider
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=326, `dmg_multiplier`=1 WHERE `entry`=44619; -- Twilight Binder
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=44620; -- Marsh Hornet
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=44626; -- Lakebed Snapper
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=44627; -- Mudbelly Boar
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=44628; -- Golden Eagle
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44631; -- Tharm Wildfire
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=44632; -- Dark Ranger
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44633; -- Stormcaller Jalara
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44634; -- Tawn Winterbluff
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=44635; -- Hill Fox
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=44638; -- Hill Grizzly
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44642; -- Stormcaller Mylra
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44644; -- Hargoth Dimblaze
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=44646; -- Earthcaller Yevaa
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1 WHERE `entry`=44647; -- Mariahn the Soulcleanser
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=44649; -- Twilight Centurion
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=44666; -- Earthmender Norsala
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=2.25 WHERE `entry`=44667; -- Seer Kormo
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1 WHERE `entry`=44668; -- Seer Galekk
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=2.25 WHERE `entry`=44669; -- Earthmender Narvra
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=2.25 WHERE `entry`=44670; -- Earthmender Govrum
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44671; -- Tharm Wildfire
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44672; -- Stormcaller Jalara
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44673; -- Stormcaller Mylra
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44674; -- Tawn Winterbluff
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44675; -- Hargoth Dimblaze
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=44676; -- Earthcaller Yevaa
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=44677; -- Windspeaker Lorvarius
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1 WHERE `entry`=44678; -- Mariahn the Soulcleanser
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=44679; -- Gorsik the Tumultuous
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=44680; -- Twilight Defiler
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1 WHERE `entry`=44681; -- Twilight Heretic
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=44683; -- Desecrated Earthrager
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=44684; -- Greater Fire Elemental
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=44685; -- Greater Air Elemental
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=44686; -- Greater Earth Elemental
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=188, `dmg_multiplier`=1.5 WHERE `entry`=44714; -- Fronkle the Disturbed
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=44723; -- Nahu Ragehoof
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=26, `dmg_multiplier`=1 WHERE `entry`=44724; -- Twilight Landshaper
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=44725; -- Sunwalker Atohmo
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=44726; -- Shalla Whiteleaf
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=44729; -- Twilight Encroacher
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=44732; -- Forsaken Trooper
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=44733; -- Forsaken Trooper
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=44734; -- Horde Engineer
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=44735; -- Seer Liwatha
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=2.25 WHERE `entry`=44737; -- Subdued Forest Ettin
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=44740; -- Sahi Cloudsinger
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=44743; -- Nohi Plainswalker
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=44745; -- Sunwalker Atohmo's Kodo
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=44749; -- Supply Sergeant Graves
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=44758; -- Mo'grosh Darkmauler
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=58, `dmg_multiplier`=1 WHERE `entry`=44760; -- Mo'grosh Earthbender
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=44762; -- Drenched Southsea Musketeer
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=44763; -- Drowned Southsea Strongarm
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=44768; -- Enslaved Miner
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=44769; -- Twilight Desecrator
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=44770; -- Tatepi
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=44771; -- Gor'kresh
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=44773; -- Anastasia
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=94, `dmg_multiplier`=1 WHERE `entry`=44774; -- Genevieve
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=2.25 WHERE `entry`=44776; -- Sharpbeak
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44779; -- Owato
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44780; -- Isashi
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44781; -- Opuno Ironhorn
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44782; -- Rento
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44783; -- Hiwahi Three-Feathers
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44785; -- Miwana
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=44786; -- Apothecary Initiate
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=44787; -- Auctioneer Sowata
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44788; -- Lonto
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=44791; -- Forsaken Trooper
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=44792; -- Forsaken Trooper
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=44793; -- Worgen Renegade
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=44795; -- Wounded Deathguard
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=44799; -- Explorer Mowi
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=44802; -- Prospector Brewer
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=44806; -- Fargo Flintlocke
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=44807; -- Farmer Wollerton
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=343, `dmg_multiplier`=2.25 WHERE `entry`=44818; -- Reliquary Jes'ca Darksun
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=44819; -- Siamat
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=343, `dmg_multiplier`=2.25 WHERE `entry`=44823; -- Examiner Rowe
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=44830; -- Ma'haat the Indomitable
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=44835; -- Haethen Kaul
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=44836; -- Hearthglen Mustang
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=44847; -- Twilight Armsman
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=44849; -- Twilight Crusher
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44852; -- Rilgiz
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44853; -- Branzlit
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44854; -- Kixa
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=343, `dmg_multiplier`=1 WHERE `entry`=44855; -- Twilight Cryptomancer
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44856; -- Perixa
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=44859; -- Safety Warden Pipsy
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=132, `dmg_multiplier`=1 WHERE `entry`=44863; -- Zen'Kiki
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=44865; -- Auctioneer Fazdran
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=44866; -- Auctioneer Drezmit
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=44867; -- Auctioneer Ralinza
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=44868; -- Auctioneer Xifa
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=44869; -- Marvon Rivetseeker
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=44870; -- Ando Blastenheimer
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=44871; -- Grunt Grimful
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=44872; -- Grunt Wabang
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=44874; -- Grunt Thathung
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=44875; -- Millhouse Manastorm
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=44876; -- Grunt Koma
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=44877; -- Grunt Soran
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=44878; -- Grunt Karus
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=44879; -- Ogre Bodyguard
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1 WHERE `entry`=44885; -- Bound Air Elemental
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1 WHERE `entry`=44886; -- Bound Water Elemental
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1 WHERE `entry`=44887; -- Bound Fire Elemental
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=44894; -- Armoire
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=44898; -- Pygmy Firebreather
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=44902; -- Captive Plaguebear
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=132, `dmg_multiplier`=1 WHERE `entry`=44905; -- Del Gahrron
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=44906; -- Skitterweb Matriarch
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=44911; -- Dreadguard
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=44912; -- Apothecary Wormcrud
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=44913; -- Orc Sea Dog
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=2.25 WHERE `entry`=44916; -- Admiral Hatchet
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=2.25 WHERE `entry`=44917; -- Warlord Torok
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=44918; -- Drakma
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=44919; -- Maztha
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=44922; -- Oathsworn Axemaster
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=44924; -- Oathsworn Myrmidon
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=44926; -- Oathsworn Wanderer
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=44927; -- Rohaku Stonehoof
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=44932; -- Oathsworn Pathfinder
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=44936; -- Murkstone Trogg
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=44945; -- Pyrite Stonetender
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=44947; -- Pyrite Rockling
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=44954; -- Hillsbrad Refugee
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=2.25 WHERE `entry`=44956; -- Stormcaller Mylra
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=44966; -- Hillsbrad Refugee
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=44967; -- Maziel
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=44970; -- Shale Drilldeep
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=44972; -- Rocky Cliffedge
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=44973; -- Ruberick
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=44975; -- Old Umbehto
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44976; -- Neferset Plaguebringer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44977; -- Neferset Torturer
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=44978; -- Sesebi
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44980; -- Neferset Theurgist
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=44981; -- Oathsworn Skinner
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=44982; -- Neferset Darkcaster
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=44983; -- Stormwind Traveling Merchant
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=2.25 WHERE `entry`=44984; -- Commander Vines
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=44985; -- Sergeant Krolan
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=44986; -- Corporal Nobsy
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=44988; -- Greater Quicksilver Ooze
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1 WHERE `entry`=44998; -- Earthen Ring Shaman
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=45001; -- Enslaved Bandit
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=45007; -- Enslaved Bandit
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45008; -- Batamsi
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=45012; -- Durnt Brightfalcon
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=45013; -- Damion Steel
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=45015; -- Darkspear Headhunter
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=45019; -- Berserker Zanga
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=45023; -- Huntress Kuzari
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=45025; -- Mimbubu
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=45029; -- Witch Doctor Umbu
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=197, `attackpower`=234, `dmg_multiplier`=1 WHERE `entry`=45031; -- Nethergarde Footman
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=45033; -- Ma'haat the Indomitable
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=45034; -- Yuldris Smolderfury
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=45035; -- Yuldris Smolderfury
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=45036; -- Hargoth Dimblaze
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=45037; -- Tawn Winterbluff
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=45038; -- Stormcaller Jalara
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=45039; -- Tharm Wildfire
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=45043; -- Peak Grindstone
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=45062; -- Oathsworn Scorpid Keeper
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=45064; -- Catapult Driver
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=45065; -- Tol'vir Merchant
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=45066; -- Alliance Spirit Guide
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=45067; -- Horde Spirit Guide
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=45081; -- Makavu
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=45082; -- Auctioneer Ziji
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=45084; -- Elemental Overseer
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45086; -- Sijambi
UPDATE `creature_template` SET `mindmg`=122, `maxdmg`=209, `attackpower`=242, `dmg_multiplier`=1 WHERE `entry`=45087; -- Felscale Crawler
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45093; -- Huju
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45094; -- Jin'diza
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=45096; -- Tamed Tol'vir Prowler
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=45097; -- Oathsworn Tamer
UPDATE `creature_template` SET `mindmg`=122, `maxdmg`=209, `attackpower`=242, `dmg_multiplier`=1 WHERE `entry`=45118; -- Darkwood Treant
UPDATE `creature_template` SET `mindmg`=124, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1 WHERE `entry`=45119; -- Corrupted Darkwood Treant
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=45122; -- Oathsworn Captain
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=209, `attackpower`=242, `dmg_multiplier`=1 WHERE `entry`=45125; -- Felspore Bog Lord
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=45129; -- Lurid
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=45137; -- Shadow-Walker Zuru
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=45138; -- Unjari Feltongue
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=45139; -- Zirazi the Star-Gazer
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=93, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=45147; -- Daria L'Rayne
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=93, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=45148; -- Morris Vant
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=93, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=45149; -- Outfitter Mendelev
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=93, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=45150; -- Bree Ironstock
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=93, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=45151; -- High Cleric Alphus
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=89, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=45152; -- Magus Bisp
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=45153; -- Redpine Looter
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=89, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=45154; -- Redpine Necromancer
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=89, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=45155; -- Moldfang
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=89, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=45156; -- Shadril
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=176, `dmg_multiplier`=1 WHERE `entry`=45157; -- Lieutenant Myner
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=45162; -- Hearthglen Trainee
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=172, `dmg_multiplier`=1 WHERE `entry`=45165; -- Thurman Grant
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=45166; -- Withdrawn Soul
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=48, `dmg_multiplier`=1 WHERE `entry`=45196; -- Orc Sea Dog
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=45197; -- Veteran Forsaken Trooper
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=2.25 WHERE `entry`=45198; -- Forsaken Catapult
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45199; -- Wounded Brave
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=45202; -- Mangy Hyena
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=45205; -- Sultan Oogah
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=80, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=45208; -- Provincial Settler
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=45211; -- Fiznak
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=2.25 WHERE `entry`=45212; -- Gory
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=45219; -- Caretaker Smithers
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=2.25 WHERE `entry`=45226; -- Naraat the Earthspeaker
UPDATE `creature_template` SET `mindmg`=47, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=45227; -- Darkmist Broodqueen
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=45230; -- Orgrimmar Brave
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=2.25 WHERE `entry`=45244; -- Farseer Krogar
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=2.25 WHERE `entry`=45253; -- Genn Greymane
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=45254; -- Hillsbrad Worgen
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=45255; -- Hillsbrad Worgen
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=45271; -- Montarr
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=45272; -- Innkeeper Abeqwa
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=45274; -- Magistrix Elosai
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=45286; -- KTC Train-a-Tron Deluxe
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=45289; -- Magdala Copperpick
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=45290; -- Varx Hagglemore
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=45293; -- Rixi "The Driller" Bombdigger
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=45294; -- Dugsley Deepdelver
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=45297; -- Beast-Handler Rustclamp
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=45298; -- Mule Driver Ironshod
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=45300; -- Caretaker Nuunwa
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=45302; -- Sunchaser Gazelle
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45305; -- Reliquary Pack Kodo
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=45306; -- Chief Surgeon Gashweld
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=45335; -- Highlands Fox
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=45336; -- Burning Invader
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=45337; -- Tyelis
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=45339; -- Dark Cleric Cecille
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=45347; -- Brother Silverhallow
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1 WHERE `entry`=45361; -- Hegrid Blazewing
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1 WHERE `entry`=45363; -- Earthmender Doros
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=45367; -- Gilnean Hound
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1.5 WHERE `entry`=45369; -- Morick Darkbrew
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=45377; -- Augh
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1.5 WHERE `entry`=45380; -- Ashtail
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=107, `attackpower`=67, `dmg_multiplier`=1 WHERE `entry`=45381; -- Grimtotem Geomancer
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=107, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=45383; -- Grimtotem Stomper
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=45385; -- Grimtotem Reaver
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=2.25 WHERE `entry`=45387; -- Isha Gloomaxe
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1.5 WHERE `entry`=45398; -- Grizlak
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1.5 WHERE `entry`=45399; -- Optimo
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1.5 WHERE `entry`=45402; -- Nix
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=28, `dmg_multiplier`=1.5 WHERE `entry`=45404; -- Geoshaper Maren
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=45407; -- Ibdil the Mender
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=45408; -- D'lom the Collector
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=142, `dmg_multiplier`=2.25 WHERE `entry`=45410; -- Elder Stormhoof
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=172, `dmg_multiplier`=1 WHERE `entry`=45417; -- Fiona
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=2.25 WHERE `entry`=45418; -- Grundig Darkcloud
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=172, `dmg_multiplier`=1 WHERE `entry`=45428; -- Gidwin Goldbraids
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=176, `dmg_multiplier`=1 WHERE `entry`=45429; -- Tarenar Sunstrike
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=112, `attackpower`=162, `dmg_multiplier`=2.25 WHERE `entry`=45438; -- Arnak Grimtotem
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=112, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=45442; -- Lakota Windsong
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=45443; -- Death's Step Putrifier
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=45444; -- Death's Step Miscreation
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=45450; -- The Lone Hunter
UPDATE `creature_template` SET `mindmg`=129, `maxdmg`=220, `attackpower`=258, `dmg_multiplier`=1 WHERE `entry`=45451; -- Argus Highbeacon
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=45453; -- Blighthound
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=45462; -- Toshe Chaosrender
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=45467; -- Trogg Dweller
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=45475; -- Plague Disseminator
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=2.25 WHERE `entry`=45482; -- Argent Officer Irizarry
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=45496; -- Commander Hickley
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=45497; -- "Salty" Gorgar
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=45498; -- "Salty" Rocka
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=45500; -- Urk Gagbaz
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=2.25 WHERE `entry`=45514; -- Wavespeaker Valoren
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=45517; -- Ravaged Dire Wolf
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=45540; -- Krenk Choplimb
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=45545; -- "Jack" Pisarek Slamfix
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45546; -- Vizna Bangwrench
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=45548; -- Kark Helmbreaker
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45549; -- Zido Helmbreaker
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=45550; -- Zarbo Porkpatty
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45551; -- Karizi Porkpatty
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45552; -- Pezik Lockfast
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45553; -- Denk Hordewell
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45558; -- Lizna Goldweaver
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=45559; -- Nivi Weavewell
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45563; -- Tinza Silvermug
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45565; -- Sanzi
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45566; -- Tanzi
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45567; -- Miragohn Mixmaster
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=112, `attackpower`=162, `dmg_multiplier`=1 WHERE `entry`=45575; -- Frederick Calston
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=45582; -- Black Widow
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=45613; -- Half-Eaten Coalpelt Bear
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=45614; -- Anguished Spirit
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=45619; -- Morbidius
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=45659; -- Auctioneer Fenk
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=45661; -- Zerit
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=45662; -- Nuzo
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=45664; -- Landlocked Grouper
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=2.25 WHERE `entry`=45681; -- Garginox
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=107, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=45691; -- Skullmage
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=45692; -- Noxious Assassin
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=45695; -- Slain Scourge Trooper
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=45709; -- Bruiser Janx
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=45711; -- Elemental Servitor
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=45713; -- Dankin Farsnipe
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=45714; -- Conjurer Mixli
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=45715; -- Orsis Survivor
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=45717; -- Vish the Sneak
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=45718; -- Environmental Engineer Linza
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=45720; -- Kazrali the Witch
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=26, `dmg_multiplier`=1 WHERE `entry`=45728; -- Arcane Remnant
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=45734; -- Relios the Relic Keeper
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=172, `dmg_multiplier`=1 WHERE `entry`=45735; -- Gamella Cracklefizz
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=172, `dmg_multiplier`=1 WHERE `entry`=45736; -- Deacon Andaal
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1.5 WHERE `entry`=45739; -- The Unknown Soldier
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=112, `attackpower`=162, `dmg_multiplier`=1 WHERE `entry`=45743; -- Nerubian Sycophant
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=112, `attackpower`=162, `dmg_multiplier`=1 WHERE `entry`=45744; -- Ix'lar the Underlord
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=45750; -- Inconspicuous Bear
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=1, `dmg_multiplier`=1 WHERE `entry`=45752; -- Ambermill Dimensional Portal
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=58, `dmg_multiplier`=1 WHERE `entry`=45753; -- Ambermill Portal Guardian
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=45755; -- Scion of Al'Akir
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=45772; -- General Ammantep
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=45780; -- Nipnuk
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=45784; -- Crablegs
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=2.25 WHERE `entry`=45786; -- Trade Baron Silversnap
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=45789; -- Bezzil
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=2.25 WHERE `entry`=45791; -- Bogpaddle Worker
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=45799; -- Prince Nadun
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=45807; -- Sawtooth Crocolisk
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=45809; -- Swampshore Makrura
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1.5 WHERE `entry`=45811; -- Marina DeSirrus
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=45814; -- Orgrimmar Bruiser
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1 WHERE `entry`=45816; -- Corpseburner Tim
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=45822; -- Off-Duty Bruiser
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=45825; -- Swampstrider
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1 WHERE `entry`=45826; -- Kirkian Dawnshield
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=45827; -- Off-Duty Grunt
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=45828; -- Argent Apothecary Judkins
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=45830; -- Off-Duty Siegeworker
UPDATE `creature_template` SET `mindmg`=133, `maxdmg`=227, `attackpower`=268, `dmg_multiplier`=1 WHERE `entry`=45831; -- Crusader Kevin Frost
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1 WHERE `entry`=45851; -- Overstuffed Golem
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=45852; -- Bogpaddle Beach Bum
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=45859; -- Venomblood Scorpid
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=172, `dmg_multiplier`=1 WHERE `entry`=45867; -- Omasum Blighthoof
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=172, `dmg_multiplier`=1 WHERE `entry`=45868; -- Karthis Darkrune
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=45887; -- Bogpaddle Privateer
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=45896; -- Marsh Crocolisk
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=172, `dmg_multiplier`=1 WHERE `entry`=45897; -- Plaguewood Reanimator
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=45905; -- Wastewander Outrider
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=45907; -- Feralas Sentinel
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=45948; -- Crazy Larry
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=45949; -- Indentured Protector
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=45950; -- Sorrowmurk Snapjaw
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=45966; -- Nevin Twistwrench
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1 WHERE `entry`=45967; -- Marshfin Murkdweller
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=45988; -- Twilight Bloodsmith
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46003; -- Prophet Hadassi
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=46004; -- Rhonda Molver
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=46006; -- Ginny Goodwin
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=2.25 WHERE `entry`=46010; -- Brono Goodgroove
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=46011; -- Marge Heffman
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=46014; -- Deceased Bogpaddle Privateer
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1 WHERE `entry`=46022; -- Beezil Linkspanner
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=46025; -- S.A.F.E. Officer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46041; -- Colossus of the Sun
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46042; -- Colossus of the Moon
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=46053; -- Forsaken Trooper
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=46054; -- Forsaken Trooper
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=2.25 WHERE `entry`=46058; -- Abomination
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=172, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=46068; -- Corrupted Guardian
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=46071; -- Lord Itharius
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=46078; -- Boss Mida
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=46080; -- Kazit
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=46082; -- Engineer Niff
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=46122; -- Gorfax Angerfang
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46126; -- Turquoise Scarab
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46127; -- Sapphire Scarab
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46128; -- Emerald Scarab
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46136; -- Vizier Tanotep
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=112, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=46138; -- Argent Centurion
UPDATE `creature_template` SET `mindmg`=200, `maxdmg`=341, `attackpower`=392, `dmg_multiplier`=1 WHERE `entry`=46140; -- Silvermoon Delegation Guardian
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=46142; -- Forsaken Delegation Deathguard
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=46146; -- Stagalbog Serpent
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=180, `attackpower`=224, `dmg_multiplier`=1 WHERE `entry`=46164; -- Marshtide Footman
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=180, `attackpower`=224, `dmg_multiplier`=1 WHERE `entry`=46166; -- Stonard Warrior
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=46167; -- Stephen Browman
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=46172; -- Baba Bogbrew
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=46180; -- Ithurian Whitespire
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=46181; -- Enchanter Farendin
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=46182; -- Pierre Fishflay
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=46184; -- Skinner Selma
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=46187; -- Marshtide Carouser
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=46188; -- Bogpaddle Entertainer
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=46189; -- Stonard Carouser
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=46190; -- Bogpaddle Bruiser
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=46230; -- S.A.F.E. Technician
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=46245; -- Lil' Crazy Jerry
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=2.25 WHERE `entry`=46248; -- Riverpaw Basher
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=2.25 WHERE `entry`=46249; -- Riverpaw Slayer
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=2.25 WHERE `entry`=46250; -- Riverpaw Poacher
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=2.25 WHERE `entry`=46251; -- Riverpaw Looter
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=78, `dmg_multiplier`=2.25 WHERE `entry`=46252; -- Riverpaw Shaman
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=46254; -- Hogger
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=46255; -- Technician Braggle
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=78, `dmg_multiplier`=2.25 WHERE `entry`=46260; -- Searing Destroyer
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=78, `dmg_multiplier`=2.25 WHERE `entry`=46261; -- Enraged Fire Elemental
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=46262; -- Rumbling Earth
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=78, `dmg_multiplier`=2.25 WHERE `entry`=46263; -- Slag Fury
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=2.25 WHERE `entry`=46264; -- Lord Overheat
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=46267; -- Rescued Survivor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=46268; -- Survivor
UPDATE `creature_template` SET `mindmg`=47, `maxdmg`=80, `attackpower`=120, `dmg_multiplier`=1 WHERE `entry`=46269; -- Mother Matterly
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=46271; -- Provisioner Elda
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1 WHERE `entry`=46274; -- Gaffer Coilspring
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=18, `dmg_multiplier`=1 WHERE `entry`=46293; -- Torben Zapblast
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=46302; -- Oil-Slicked Earth Spirit
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=46358; -- Lutah
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=46359; -- Punra
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=46363; -- Crazed Leper Gnome
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=46365; -- Enthralled Gilblin
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=46366; -- Enthralled Siren
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=95, `dmg_multiplier`=1 WHERE `entry`=46367; -- Enthralled Murloc
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=46368; -- Enthralled Makrura
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=46369; -- Enthralled Crustacean
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1 WHERE `entry`=46371; -- Hr'nglth the Lost
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=2.25 WHERE `entry`=46375; -- Rowdy Troublemaker
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=2.25 WHERE `entry`=46379; -- Vicious Thug
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=2.25 WHERE `entry`=46381; -- Shifty Thief
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=46382; -- Petty Criminal
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=46383; -- Randolph Moloch
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=46393; -- Billy Goat
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46401; -- Mar'at Villager
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=46405; -- Stockade Guard
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=46406; -- Stockade Rifleman
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=46407; -- Injured Stockade Guard
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=46408; -- Injured Stockade Rifleman
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=46409; -- Warden Thelwater
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=46410; -- Nurse Lillian
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1 WHERE `entry`=46414; -- Marshtide Knight
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1 WHERE `entry`=46415; -- Stabled Warhorse
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=46416; -- Twilight Skyterror
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=2.25 WHERE `entry`=46417; -- Rifle Commander Coe
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=46418; -- Lirastrasza
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=180, `attackpower`=99, `dmg_multiplier`=1 WHERE `entry`=46424; -- Priestess Udum'bra
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=46447; -- Injured Gnome
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=46458; -- Budd
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=46474; -- Shimmerspine Harvester
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=49, `dmg_multiplier`=2.25 WHERE `entry`=46475; -- Anchorite Traska
UPDATE `creature_template` SET `mindmg`=40, `maxdmg`=70, `attackpower`=49, `dmg_multiplier`=1 WHERE `entry`=46476; -- Tracker Yoro
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=46478; -- Highvale Sentry
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=46482; -- Mortimer Moloch
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=46483; -- Forsaken Apothecary
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1 WHERE `entry`=46486; -- Stonard Peon
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1 WHERE `entry`=46487; -- Marshtide Peasant
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=46503; -- Lil' Crazy Daisy
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46507; -- Darkwood Broodmother
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46508; -- Darkwood Lurker
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=46552; -- Steven Stutzka
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=46555; -- Gunra
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=46556; -- Jamus'Vaz
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=46559; -- Horde Engineer
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=2.25 WHERE `entry`=46560; -- Subdued Forest Ettin
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46569; -- Forgotten Ghoul
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46570; -- Putrid Worg
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46571; -- First Lieutenant Connor
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=46572; -- Goram
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=46573; -- Vile Fin Oracle
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=42, `dmg_multiplier`=1 WHERE `entry`=46574; -- Vile Fin Tidehunter
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=46575; -- Darktusk Boar
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46580; -- Elementium Spinner
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=46581; -- Violated Corpse
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46582; -- Hungry Ghoul
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46586; -- Wandering Soul
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=2.25 WHERE `entry`=46587; -- Twizzleflux the Insane
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46590; -- Crazed Digger
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46592; -- Mekgineer Mixeltweed
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46597; -- Skeletal Beastmaster
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=46602; -- Shay Pressler
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46603; -- Nomarch Teneth
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46605; -- Shipwrecked Sailor
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46606; -- Spiny Tidecrawler
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=46608; -- Tank
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46617; -- Cursed Engineer
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=46618; -- Fibi
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=46619; -- Binzella
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=46620; -- Vink
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=46621; -- Pank
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=180, `attackpower`=224, `dmg_multiplier`=1 WHERE `entry`=46623; -- Gomora the Bloodletter
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46630; -- Accursed Longshoreman
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=46637; -- Auctioneer Drezbit
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=46638; -- Auctioneer Vizput
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=46639; -- Auctioneer Zilbeena
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=46640; -- Auctioneer Kuvi
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46641; -- Ghastly Dockhand
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=46642; -- Innkeeper Nufa
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=46643; -- Accursed Shipbuilder
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=46645; -- Cursed Stallion
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46648; -- Foreman Wellson
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=46650; -- Eddie Flofizzle
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=46652; -- Dolph Blastus
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=46653; -- Garyanne Fleezlebop
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46654; -- Rhea
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=180, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=46656; -- Jammal'an the Prophet
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=46659; -- Gavran Goodspade
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=46660; -- Aidan Summerwind
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=46664; -- Dr. Hieronymus Blam
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=46667; -- Blademaster Ronakada
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46671; -- Twilight Rune of Earth
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=46675; -- Lugrah
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=2.25 WHERE `entry`=46676; -- Joanna Blueheart
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=46692; -- ROCK ON HOLOGRAM
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=81, `dmg_multiplier`=1 WHERE `entry`=46693; -- Dustbelcher Instructor
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=81, `dmg_multiplier`=1 WHERE `entry`=46694; -- Dustbelcher Initiate
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=81, `dmg_multiplier`=1 WHERE `entry`=46695; -- Dustbelcher Trainee
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1 WHERE `entry`=46702; -- Hull Forgehammer
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=46708; -- Suja
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=46709; -- Arugi
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=46714; -- Marshtide Sentry
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=46716; -- Nerog
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=46717; -- The Hammer of Twilight
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=46718; -- Moraka
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=46741; -- Muraga
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=46742; -- Brunda
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=180, `attackpower`=224, `dmg_multiplier`=1 WHERE `entry`=46749; -- Stonard Wardrummer
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=46753; -- Al'Akir
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=2.25 WHERE `entry`=46757; -- High Examiner Tae'thelan Bloodwatcher
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=2.25 WHERE `entry`=46758; -- Aoren Sunglow
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=46759; -- Lidia Sunglow
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=2.25 WHERE `entry`=46760; -- Lead Prospector Durdin
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=46761; -- Olga Runesworn
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=46762; -- Kand Sandseeker
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=46768; -- The Warden
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=46769; -- The Sentinel
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=46772; -- Reliquary Excavator
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=46773; -- Explorers' League Excavator
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46823; -- Restless Infantry
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46825; -- Restless Soldier
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=180, `attackpower`=99, `dmg_multiplier`=1 WHERE `entry`=46841; -- Marshtide Cleric
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=46852; -- Eric "The Swift"
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=46853; -- Baelog
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=46854; -- Olaf
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=2.25 WHERE `entry`=46859; -- Kalaran the Annihilator
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=180, `dmg_multiplier`=2.25 WHERE `entry`=46860; -- General Jirakka
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=2.25 WHERE `entry`=46861; -- Nyxondra
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46871; -- Scalemother Hevna
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46872; -- Prince Nadun
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46877; -- Tanzar
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46878; -- Mack
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46879; -- Harkor
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46880; -- Samir
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46884; -- Neferset Smuggler
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46888; -- Unstable Bomb Bot
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=180, `dmg_multiplier`=1 WHERE `entry`=46914; -- Raging Whelp
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=180, `dmg_multiplier`=1 WHERE `entry`=46915; -- Darkflight Soldier
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=180, `dmg_multiplier`=1 WHERE `entry`=46916; -- Nyxondra's Broodling
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=180, `dmg_multiplier`=1 WHERE `entry`=46917; -- Darkflight Flameblade
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=180, `dmg_multiplier`=1 WHERE `entry`=46918; -- Darkflight Shadowspeaker
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=46928; -- Dustbelcher Butcher
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=46929; -- Dustbelcher Merchant
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1 WHERE `entry`=46930; -- Victoria Dolen
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=2.25 WHERE `entry`=46938; -- Moldarr
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=101, `dmg_multiplier`=1 WHERE `entry`=46950; -- Misty Grell
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=46953; -- Purespring Elemental
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=46960; -- Ghastly Scavenger
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=2.25 WHERE `entry`=46963; -- Lord Walden
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=2.25 WHERE `entry`=46964; -- Lord Godfrey
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=46966; -- Carson Crazybread
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=46972; -- Terrance Storm
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46979; -- Schnottz Air Trooper
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1.5 WHERE `entry`=46981; -- Nightlash
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=46982; -- Sly
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=42, `dmg_multiplier`=1 WHERE `entry`=46983; -- Benjamin Foxworthy
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=46989; -- Crazed Soldier
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1.5 WHERE `entry`=46992; -- Berard the Moon-Crazed
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=46993; -- Schnottz Air Officer
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=46994; -- Trenton Ramalot
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=46995; -- Steven Ramalot
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=46996; -- Jade Ramalot
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=46997; -- Shifting Mireglob
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1.5 WHERE `entry`=47003; -- Bolgaff
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1.5 WHERE `entry`=47008; -- Fenwick Thatros
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=52, `dmg_multiplier`=1.5 WHERE `entry`=47009; -- Aquarius the Unbound
UPDATE `creature_template` SET `mindmg`=18, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1.5 WHERE `entry`=47010; -- Indigos
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1.5 WHERE `entry`=47012; -- Effritus
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47014; -- Cursed Surveyor
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1.5 WHERE `entry`=47015; -- Lost Son of Arugal
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=30, `dmg_multiplier`=1.5 WHERE `entry`=47023; -- Thule Ravenclaw
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=2.25 WHERE `entry`=47030; -- Veteran Forsaken Trooper
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=2.25 WHERE `entry`=47031; -- Forsaken Blightspreader
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=47041; -- Neeka Bloodscar
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=47042; -- Porsha Brassbearing
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=47043; -- Lixi Greasenozzle
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=47044; -- Lamba Ginwhistle
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=47053; -- Duskfang
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=47057; -- Wounded Darkspear Watcher
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1 WHERE `entry`=47059; -- Dronk Drophammer
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=47060; -- Argent Sentry
UPDATE `creature_template` SET `mindmg`=54, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=47064; -- Mossflayer Abducter
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47071; -- Deep Spider
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47072; -- Amthea
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=132, `attackpower`=180, `dmg_multiplier`=1 WHERE `entry`=47073; -- New Kargath Grunt
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=47091; -- Wounded Guard
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47093; -- Darkbreak Guard
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47104; -- Anthony Waler
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47105; -- Marc Daiton
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47106; -- Christine Gade
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47110; -- Cavern Grunt
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=47120; -- Argaloth
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=47130; -- Crazed Guard
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47131; -- Frantic Geist
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47132; -- Dark Creeper
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=47134; -- Corpse Eater
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=47135; -- Fetid Ghoul
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=47136; -- Unstable Ravager
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=47137; -- Mindless Horror
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=47138; -- Pustulant Monstrosity
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47139; -- Gerald Fairbanks
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=47140; -- Sorcerous Skeleton
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=47141; -- Dread Scryer
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47142; -- Rahmalaha
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=47143; -- Spitebone Skeleton
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47144; -- Patricia Marshall
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=47145; -- Spitebone Guardian
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=47146; -- Spitebone Flayer
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47148; -- Donovan Minewell
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47149; -- Kathryn Humphries
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47153; -- Cabe Minewell
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47164; -- James Litman
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47165; -- Casey Tipple
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47166; -- Dorris Manchester
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47167; -- Brett Nelson
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=1 WHERE `entry`=47181; -- Overlook Spirit
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=47182; -- Overlook Spectre
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47183; -- Ghastly Worker
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=343, `dmg_multiplier`=1 WHERE `entry`=47184; -- Stone Trogg Fungalmancer
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=47185; -- Refined Gentleman
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47190; -- Shaggy Desert Coyote
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=47193; -- Schnottz's Bodyguard
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=47195; -- Slate Quicksand
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=47197; -- Flint Oremantle
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47201; -- Desert Fox
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47202; -- Diseased Vulture
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=47203; -- Creeper Egg
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=47205; -- Domesticated Mine Creeper
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47207; -- Dead Trooper
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47213; -- Dead Trooper
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=47214; -- Dreadguard
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47216; -- Dead Trooper
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47219; -- Dead Trooper
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47220; -- Furious Specter
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=343, `dmg_multiplier`=1 WHERE `entry`=47227; -- Eternal Protector
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=47231; -- Shadowy Attendant
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=47232; -- Ghostly Cook
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=47233; -- Gordul
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=47240; -- Commander Marcus Johnson
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=47246; -- Ureda
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=91, `dmg_multiplier`=1 WHERE `entry`=47247; -- Marud
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=47248; -- Gija
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=47250; -- Carvo Blastbolt
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=81, `dmg_multiplier`=1 WHERE `entry`=47253; -- Rundok
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=81, `dmg_multiplier`=1 WHERE `entry`=47254; -- Gizput
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47283; -- Sand Serpent
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=47285; -- Siamat
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47286; -- Vincent Randolph
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47287; -- Captain P. Harris
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47288; -- Gutcheck
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47289; -- Argent Crusade Engineer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47291; -- Schnottz's Landing Laborer
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=2.25 WHERE `entry`=47293; -- Deathstalker Commander Belmont
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=47294; -- High Warlord Cromush
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=47304; -- Commander Largo
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47306; -- Neferset Sentinel
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47318; -- Mack
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=1 WHERE `entry`=47320; -- Kaellin Tarvane
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=1 WHERE `entry`=47321; -- Zugra Flamefist
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47324; -- Baradin Guard
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47325; -- Baradin Guard
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47328; -- Quartermaster Brazie
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=47334; -- Cap'n Geech
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47335; -- Baradin Grunt
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47336; -- Baradin Grunt
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=47337; -- Shecky Shrimpshoot
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=47338; -- Frankie Goodtimes
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=47340; -- Bullets Bigblast
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=47343; -- Zipper Sizzlesnap
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=47345; -- Spike Coilee
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=47347; -- Mitch Yostpaddle
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=180, `attackpower`=224, `dmg_multiplier`=1 WHERE `entry`=47363; -- Rebecca Blackman
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=2.25 WHERE `entry`=47367; -- Verad
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=2.25 WHERE `entry`=47368; -- Joran
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=47370; -- Harborage Guardian
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47373; -- Warden's Sentry
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=47382; -- Adam Hossack
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=47383; -- Brivelthwerp
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=47384; -- Lien Farner
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1.5 WHERE `entry`=47386; -- Ainamiss the Hive Queen
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1.5 WHERE `entry`=47387; -- Harakiss the Infestor
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=47388; -- Silithid Ravager
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=47390; -- Silithid Defender
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=47395; -- Injured Miner
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=47396; -- Wembil Taskwidget
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=47400; -- Nedric Sallow
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=47405; -- The Chef
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=47415; -- Howler
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=47418; -- Runda
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=47419; -- Lalum Darkmane
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=47420; -- Iranis Shadebloom
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47428; -- Hellscream's Sentry
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=47431; -- Valn
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=47432; -- Apothecary Underhill
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=47433; -- Captured Bloodfang Worgen
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=47445; -- Skeletal Steed
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=47446; -- Dead Employee
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=47447; -- Keep Lord Farson
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=47455; -- Siege Tank Gun
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47457; -- Twilight's Hammer Horse
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=47461; -- Prolific Writer
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=47471; -- Pao'ka Swiftmountain
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=47472; -- Privileged Socialite
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=116, `attackpower`=166, `dmg_multiplier`=1 WHERE `entry`=47479; -- Twilight Trapper
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=47481; -- Highperch Prideling
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=47484; -- Yetimus the Yeti Lord
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1 WHERE `entry`=47485; -- Highperch Wind Rider
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=2.25 WHERE `entry`=47486; -- Heartrazor
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1 WHERE `entry`=47487; -- Twilight Subduer
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=47514; -- Pretentious Businessman
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1 WHERE `entry`=47528; -- Aerie Ace
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=47530; -- Kizter Kahboom
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=47532; -- Darbo Stableflux
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=47545; -- Brita Bramblebrush
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=47547; -- Sid Shillcopper
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=2.25 WHERE `entry`=47551; -- Spider-Handler Sarus
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=2.25 WHERE `entry`=47554; -- Captain Keyton
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=2.25 WHERE `entry`=47565; -- Baby
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47567; -- Kavem the Callous
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=47569; -- Hammon the Jaded
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=828, `dmg_multiplier`=1 WHERE `entry`=47571; -- Belloc Brightblade
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=47572; -- Otoh Greyhide
UPDATE `creature_template` SET `mindmg`=135, `maxdmg`=231, `attackpower`=272, `dmg_multiplier`=1 WHERE `entry`=47575; -- Boduro the Seeker
UPDATE `creature_template` SET `mindmg`=200, `maxdmg`=341, `attackpower`=175, `dmg_multiplier`=1 WHERE `entry`=47579; -- Dariness the Learned
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=112, `attackpower`=152, `dmg_multiplier`=1 WHERE `entry`=47580; -- Lakota Windsong
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=47582; -- Erdunor Whitespire
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47583; -- Bulwark Defender
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=47584; -- Aladrel Whitespire
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=81, `dmg_multiplier`=1 WHERE `entry`=47585; -- Twilight Magus
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=47587; -- Archmage Pratt
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=47589; -- Enchantress Herim
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=2.25 WHERE `entry`=47602; -- Codemaster Deethuk
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=81, `dmg_multiplier`=2.25 WHERE `entry`=47619; -- Elementalist Starion
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=2.25 WHERE `entry`=47620; -- Commander Fastfuse
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47621; -- Bulwark Prisoner
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47625; -- Samir
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47627; -- Rustberg Bandit
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=47642; -- Rustberg Fisherman
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47643; -- Tanzar
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=47644; -- Darla Harris
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=47655; -- Pamela Stutzka
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47657; -- Suspicious Villager
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47659; -- Apprehensive Worker
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=47661; -- Darren Longfellow
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=47663; -- Scout Obrok
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=47665; -- Phillip Harding
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=47666; -- Rustberg Horse
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=47668; -- Marsha Duchamp
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=47676; -- Baradin Fox
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=47680; -- Rustberg Gull
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=47681; -- Dumass
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=47688; -- Officer Connelly
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=47698; -- Menacing Emissary
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47699; -- Ramkahen Field Worker
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=47700; -- Menacing Emissary
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47709; -- Sun Acolyte
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=47712; -- Harland Waldek
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47715; -- Sun Priest Asaris
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=47716; -- Sun Priest Iset
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=47717; -- Benjamin Henderson
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=2.25 WHERE `entry`=47718; -- Magatha Grimtotem
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=47719; -- Alexis Torry
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=47720; -- Camel
UPDATE `creature_template` SET `mindmg`=51, `maxdmg`=89, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=47721; -- Foreman Milos
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47722; -- Neferset Armorer
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=47725; -- Tol'vir Merchant
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47726; -- Tiger
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=47727; -- Neferset Blade Twister
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=47729; -- Neferset Savage
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=47730; -- High Priest Sekhemet
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1 WHERE `entry`=47738; -- Neferset Snake Charmer
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=47741; -- Neferset Venom Keeper
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=47742; -- Scorpion-Lord Namkhare
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=47753; -- Dark Pharaoh Tekahn
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=47755; -- Warlord Ihsenn
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47756; -- Chad Fillmore
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47757; -- George Randolph
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47758; -- Gurt
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=47759; -- Muckgill
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1 WHERE `entry`=47760; -- Neferset Cursebringer
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=47761; -- Hank Ford
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=360, `dmg_multiplier`=1 WHERE `entry`=47762; -- Neferset Ritualist
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=172, `dmg_multiplier`=1 WHERE `entry`=47763; -- Bound Fury
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=47764; -- Murog
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=172, `dmg_multiplier`=1 WHERE `entry`=47766; -- Bound Vortex
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=47767; -- Korla
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=47771; -- Drukma
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=47772; -- Siege Tank Gun
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=172, `dmg_multiplier`=1 WHERE `entry`=47777; -- Animus
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=47779; -- Mouton Flamestar
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=2.25 WHERE `entry`=47781; -- Warden Stillwater
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=47782; -- Blackrock Whelper
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=94, `dmg_multiplier`=1 WHERE `entry`=47786; -- Sludge Guard
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=47788; -- Guldor
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=47789; -- Clerk Horrace Whitesteed
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=2.25 WHERE `entry`=47790; -- Magistrate Burnside
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=47791; -- Citizen Wilkes
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=47792; -- Blacksmith Verringtan
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=2.25 WHERE `entry`=47793; -- The Ray-Getz-Kalaba Monstrosity
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=47801; -- Temple Scarab
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=47803; -- Sand Scorpid
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=81, `dmg_multiplier`=1 WHERE `entry`=47804; -- Twilight Shaper
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=194, `dmg_multiplier`=1 WHERE `entry`=47805; -- Ner'gosh the Shadow
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=47808; -- Black Wolf
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=47809; -- Brown Wolf
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=47814; -- Fettered Red Whelpling
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=47815; -- Grimfang
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=47816; -- Shackled Earth
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=47817; -- Scuttler
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=47818; -- Tusker
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=47820; -- Fettered Green Whelpling
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=47821; -- Fettered Blue Whelpling
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=47822; -- Fettered Bronze Whelpling
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=47854; -- Nicolette Garner
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=47856; -- Adaleen Garner
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=47857; -- Roman Garner
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=47858; -- Albert Garner
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=47859; -- Risen Hillsbrad Farmer
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=47860; -- Wanda Henderson
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=47861; -- Risen Hillsbrad Peasant
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=2.25 WHERE `entry`=47862; -- Magistrate Henry Maleb
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=47863; -- Joseph Henderson
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=47864; -- Donald Emery
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=47865; -- Forsaken Trooper
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=47866; -- Lois Henderson
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=47870; -- Twinkles
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=47871; -- Sludge Guard
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=47872; -- Human Seedling
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=47875; -- William Henderson
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=2.25 WHERE `entry`=47899; -- Flesh-Shaper Arnauld
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=326, `dmg_multiplier`=2.25 WHERE `entry`=47969; -- Bound Torrent
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=47978; -- Captain Kronkh
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=47981; -- Captain Margun
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=47982; -- Schnottz Sea Trooper
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=48001; -- Thelaron Direneedle
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=48012; -- Sergeant Mehat
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=38, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=48017; -- Infected Sludge Guard
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=48033; -- Stebben Oreknuckle
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=48037; -- Chiseled Golem
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=48043; -- Pygmy Thief
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=48054; -- Sally Gearwell
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=48055; -- Deedee Dropbolt
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=48056; -- Fritz Roughtorque
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=48057; -- Artie Steelbolt
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=48058; -- Buzz Sawspinner
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=48060; -- "Chef" Overheat
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=48064; -- Milly Brewtap
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=48067; -- Gina Gotgoods
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=48068; -- Fuselight Engineer
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=48075; -- Fuselight Bruiser
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=2.25 WHERE `entry`=48085; -- Quartermaster Kaoshin
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=48090; -- Vernon Farcavern
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=48093; -- Ivan Zypher
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1 WHERE `entry`=48095; -- Katrina Lyons
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1 WHERE `entry`=48096; -- "Snake" Sanders
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1 WHERE `entry`=48098; -- Tarquinn
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=2.25 WHERE `entry`=48099; -- Worgmistress Othana
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=2.25 WHERE `entry`=48100; -- Voodooist Timan
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=48111; -- Blackrock War Kodo
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=48113; -- Juliette
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=48114; -- Raphael
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=149, `attackpower`=194, `dmg_multiplier`=1 WHERE `entry`=48118; -- Smolderthorn Shaman
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=48119; -- Smolderthorn Assassin
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=149, `attackpower`=194, `dmg_multiplier`=1 WHERE `entry`=48120; -- Firegut Reaver
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=1 WHERE `entry`=48121; -- Firegut Flamespeaker
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=48122; -- Ramah Goldenray
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=48123; -- Senara Swiftsilver
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=48124; -- Rabid Mastiff
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=116, `attackpower`=166, `dmg_multiplier`=1 WHERE `entry`=48125; -- Advisor Sarophas
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1 WHERE `entry`=48128; -- Stranded Sparkleshell
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=116, `attackpower`=166, `dmg_multiplier`=2.25 WHERE `entry`=48129; -- Helcular
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=48130; -- Scorpid Cliffcrawler
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=48131; -- Needlespine Cobra
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=48132; -- Needlespine Shimmerback
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=2.25 WHERE `entry`=48133; -- General Thorg'izog
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=48136; -- Blight Slime
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=112, `attackpower`=162, `dmg_multiplier`=1 WHERE `entry`=48137; -- Monstrous Macaw
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=112, `attackpower`=162, `dmg_multiplier`=1 WHERE `entry`=48138; -- Brilliant Clayscraper
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=48139; -- Temple Swiftstalker
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=48140; -- Temple Runecaster
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=48141; -- Temple Shadowlancer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=48143; -- Temple Fireshaper
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=63, `attackpower`=106, `dmg_multiplier`=1 WHERE `entry`=48147; -- Sparkleshell Tortoise
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=48148; -- Sparkleshell Snapper
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=48153; -- Goblin Flamethrower
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=112, `attackpower`=162, `dmg_multiplier`=1 WHERE `entry`=48155; -- Saltspray Gull
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=48156; -- Magma Lord Kolob
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=48158; -- Blightguard
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=1 WHERE `entry`=48159; -- Wyrtle Spreelthonket
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=48168; -- Salhet's Lion
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=48177; -- Silithid Ravager
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=48178; -- Silithid Ravager
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=48179; -- Silithid Wasp
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=48180; -- Wild Horse
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=48182; -- Shimmershell Snail
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=48184; -- Hill Stag
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=48185; -- Whale Shark
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=48187; -- Hill Fawn
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1 WHERE `entry`=48188; -- Siltwash Terrapin
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=102, `attackpower`=142, `dmg_multiplier`=1 WHERE `entry`=48191; -- Remora Scrounger
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=112, `attackpower`=162, `dmg_multiplier`=1 WHERE `entry`=48192; -- Barbed Gasgill
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=48201; -- Blackrock Sergeant
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=48203; -- Sullah
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=48204; -- Schnottz Excavator
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=48205; -- Schnottz Overseer
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=48208; -- The Rattle of Bones
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=48237; -- Salhet
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=48248; -- Sorrow Venomspitter
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1 WHERE `entry`=48249; -- Sorrow Screecher
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=48250; -- Lieutenant Farnsworth
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=48251; -- Kevin Geissler
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=48252; -- Baradin Recruit
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=48253; -- Baradin Guard
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=48254; -- Sergeant Gray
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=48255; -- Camp Coordinator Brack
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=48267; -- Neferset Denizen
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=48268; -- Battle Creeper
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=2.25 WHERE `entry`=48269; -- Dark Ranger
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=48272; -- Bloodfang Forsaken Hunter
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=48273; -- Evax Oilspark
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=48274; -- Jock Lindsey
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=1 WHERE `entry`=48287; -- Whelptamer Akumi
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=1 WHERE `entry`=48288; -- Terromath the Seared
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=1 WHERE `entry`=48289; -- Minyoth
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=1 WHERE `entry`=48291; -- Tugnar Goremaw
UPDATE `creature_template` SET `mindmg`=131, `maxdmg`=223, `attackpower`=262, `dmg_multiplier`=2.25 WHERE `entry`=48292; -- Acride
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=48305; -- Injured Employee
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=2.25 WHERE `entry`=48306; -- John J. Keeshan
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=48307; -- Colonel Troteman
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=48308; -- Farson Hold Prisoner
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=48309; -- Farson Hold Prisoner
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=1 WHERE `entry`=48312; -- High Warlock Xi'lun
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=1 WHERE `entry`=48314; -- Dragon-Lord Neeralak
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=1 WHERE `entry`=48316; -- General Thorg'izog
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=48318; -- Hans Oreflight
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=48319; -- Angry Blight Slime
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=48321; -- Grimly Singefeather
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=2.25 WHERE `entry`=48322; -- Chet the Slime-Breeder
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=48325; -- Farson Horse
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=48355; -- Hellscream Guard
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=48356; -- Karosh
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=48357; -- Hellscream's Reach Recruit
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=48358; -- Commander Larmash
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=48360; -- 3rd Officer Kronkar
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=48361; -- Private Sarlosk
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=48363; -- Captain Prug
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=48374; -- Myzerian
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=164, `attackpower`=95, `dmg_multiplier`=1 WHERE `entry`=48413; -- Blackrock Necromancer
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=48482; -- Stormpike Mountaineer
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=48483; -- Stormpike Ram Rider
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=48484; -- Bloodfang Sentry
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=48494; -- Hobart Grapplehammer
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=48496; -- Assistant Greely
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=48501; -- Sun Prophet Tumet
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=48510; -- Kall Worthaton
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1 WHERE `entry`=48512; -- Ramkahen Legionnaire
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=48513; -- Revi Ramrod
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=48514; -- Ramkahen Marksman
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=48519; -- Micro Mechachicken
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=48520; -- Crystalwing Stone Drake
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=48526; -- Bilgewater Buccaneer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=48531; -- Pogg
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=48534; -- Schnottz Elite Trooper Corpse
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=48545; -- High Warlord Cromush
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=2.25 WHERE `entry`=48546; -- Captain Winky
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=2.25 WHERE `entry`=48548; -- Immortal Colossus
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=48550; -- Neferset Champion
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=48564; -- King Phaoris
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=48612; -- Maressa Milner
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=48613; -- Larah Firesong
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=48614; -- Dark Cleric Claressa
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=48615; -- Shernon the Footpad
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=48616; -- Karla Fain
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=1 WHERE `entry`=48618; -- Darna Woad
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=48619; -- Therisa Sallow
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=48622; -- Gormir Stoneshaper
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=360, `dmg_multiplier`=1 WHERE `entry`=48625; -- Neferset Scryer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=48626; -- Neferset Raider
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=48627; -- Neferset Looter
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=48628; -- Ferocious Yeti
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=48629; -- Schnottz Infantryman
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=48631; -- Schnottz Elite Infantryman
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=48632; -- Golden King
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=48664; -- Wretched Spirit
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=156, `attackpower`=198, `dmg_multiplier`=1 WHERE `entry`=48665; -- Wretched Spectre
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=48668; -- Schnottz Elite Trooper
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=48670; -- Betrayed Ranger
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=48678; -- Shade of the Spiritspeaker
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=48694; -- Weathered Nomad
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=48697; -- Ramkahen Archer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=48701; -- Nahom Sun Priest
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=48704; -- Gek Nozzlerocket
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=48705; -- Gertrude Nozzlerocket
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1 WHERE `entry`=48708; -- The Uncrashable
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1 WHERE `entry`=48722; -- Burndl
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1 WHERE `entry`=48723; -- Tanrir
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=48726; -- Bielara Ivyshroud
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=48735; -- Gwen Armstead
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=156, `attackpower`=93, `dmg_multiplier`=1 WHERE `entry`=48740; -- Archmage Maenius
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=48741; -- SI:7 Operative
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=48742; -- SI:7 Assassin
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=48761; -- Salhet
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=48763; -- Forlorn Highborne
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=42, `dmg_multiplier`=1 WHERE `entry`=48764; -- Telarius Voidstrider
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=48765; -- Rimepelt
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=48767; -- Scalding Springsurge
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=48768; -- Boiling Springbubble
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=48797; -- Gim'hila
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=48808; -- Corporal Jeyne
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=48853; -- Samaki
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=48856; -- Husani
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=48857; -- Mosegi
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=48858; -- Ishaq
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=48860; -- Hanbal
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=48861; -- Umi
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=48868; -- Chuma
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=48874; -- Mar'at Guardian
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=48875; -- Agee Tyler
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=48878; -- Stormpike Soldier
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=48880; -- Captain Iceheart
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=48883; -- Ramkahen Guardian
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=48884; -- Jahi
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=48912; -- Stormpike Trainee
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=48918; -- Winterhorn Stag
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=48922; -- Instructor Rufus
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=48923; -- Drill Sergeant Magnus
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=48924; -- Captain Crudbeard
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=202, `dmg_multiplier`=1 WHERE `entry`=48948; -- Sharon Boomgetter
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=48949; -- Kezan Socialite
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=48950; -- The Bogpaddle Bullet
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=48952; -- Frostleaf Treant
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=48959; -- Rusty Anvil
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=48960; -- Frostshard Rumbler
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=48961; -- Kezan Socialite
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=48965; -- Deez Rocksnitch
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=49116; -- Stormpike Engineer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=2.25 WHERE `entry`=49128; -- Daschla
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=2.25 WHERE `entry`=49129; -- Arthura
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=49150; -- Subject Nine
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=180, `attackpower`=224, `dmg_multiplier`=1 WHERE `entry`=49161; -- Altered Beast
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=202, `dmg_multiplier`=1 WHERE `entry`=49178; -- Grolnar the Berserk
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=2.25 WHERE `entry`=49201; -- Nils Beerot
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=49214; -- Raider Lord Havat
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=49217; -- Wintervine Lasher
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1 WHERE `entry`=49224; -- Ramkahen Champion
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=1 WHERE `entry`=49228; -- Ramkahen Sergeant
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=49233; -- Solid Ice
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=49235; -- Icewhomp
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=1 WHERE `entry`=49240; -- Captain Ironhill's Ghost
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=49242; -- Ramkahen Ranger
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=2.25 WHERE `entry`=49243; -- Captain Jekyll
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=49244; -- Ramkahen Ranger Captain
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49245; -- Rampaging Owlbeast
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=49249; -- Vicious Black Bear
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49251; -- Wild Gryphon
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=49263; -- Zephyrus
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=49264; -- Bloodvenom
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=49265; -- Teracula
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=49266; -- Infernus
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=49268; -- D-1000
UPDATE `creature_template` SET `mindmg`=24, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49269; -- Dun Garok Spirit
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=49281; -- Neferset Scout
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=8, `dmg_multiplier`=1 WHERE `entry`=49340; -- Scarlet Corpse
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=49346; -- Coldlurk Creeper
UPDATE `creature_template` SET `mindmg`=91, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=49347; -- Coldlurk Burrower
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=2.25 WHERE `entry`=49348; -- Dreadguard Molina
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=2.25 WHERE `entry`=49349; -- Dreadguard Spiering
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=49394; -- Innkeeper Hershberg
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=49395; -- Shannon Lamb
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=49396; -- Jez Goodgrub
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=49397; -- Kenneth Lamb
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=49398; -- Beauxbeaux
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=49399; -- Remma Curtainfire
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=49400; -- Marcy Curtainfire
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=49401; -- Gordon Finley
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=49402; -- Sana Curtainfire
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=49403; -- Karren Dresner
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=49404; -- Marcus Hagnod
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=49406; -- Yasmin
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=49407; -- Rinno Curtainfire
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=49408; -- Farah Tamina
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=49422; -- Rotbrain Berserker
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=49423; -- Rotbrain Magus
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=49424; -- Marshal Redpath
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=49430; -- Innkeeper Durgens
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=49431; -- Ansel Tunsleworth
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=49433; -- Evan Banlip
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=49434; -- Patricia Eyesley
UPDATE `creature_template` SET `mindmg`=96, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=49435; -- Chesterfield Cobbles
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=172, `attackpower`=218, `dmg_multiplier`=1 WHERE `entry`=49436; -- Francis Morcott
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=49455; -- Dehydrated Prisoner
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=2.25 WHERE `entry`=49477; -- Huntress Sandrya Moonfall
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=49478; -- Dentaria Silverglade
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=49479; -- Dentaria Silverglade
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=49525; -- Salvaged Goods
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=49537; -- Jeb Guthrie
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=49573; -- Karba Blazemaw
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=49622; -- Shok Narnes
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=49635; -- War-Mage Erallier
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=49636; -- Arcanist Arman
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=49639; -- Kartanus
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=49640; -- Ranadae
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=110, `dmg_multiplier`=2.25 WHERE `entry`=49687; -- Brazie the Botanist
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=49691; -- Fertilize-o-tron 2000
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=49692; -- Sunflower
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=49693; -- Rocknut
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=49696; -- Freezya
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=49697; -- Spitter
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=1 WHERE `entry`=49701; -- Bario Matalli
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=37, `dmg_multiplier`=1 WHERE `entry`=49715; -- Dark Cleric Pordon
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49716; -- Thom Denger
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49718; -- Bee Bruxworthy
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49720; -- Eugene Daller
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49736; -- Kallen Stanner
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=49737; -- Shazdar
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49741; -- Sister Darnhald
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49745; -- Sern Hallows
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=49748; -- Hero's Herald
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=37, `dmg_multiplier`=1 WHERE `entry`=49749; -- Priestess Cocoa Anderson
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=49750; -- Warchief's Herald
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49760; -- Zalna Wefhellt
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49769; -- Deborah Fain
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49781; -- Grendin Swiftaxe
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49782; -- Galda Bronzeblade
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=37, `dmg_multiplier`=1 WHERE `entry`=49784; -- Priestess Baerla
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=37, `dmg_multiplier`=1 WHERE `entry`=49786; -- Gindle the Green
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49791; -- Solbin Shadowcog
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49793; -- Faldoc Stonefaith
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49806; -- Belda Wildheart
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=49808; -- Grenhild Darktalon
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=49815; -- Deep Spider
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=49816; -- Deep Spider
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1.5 WHERE `entry`=49822; -- Jadefang
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=107, `attackpower`=142, `dmg_multiplier`=2.25 WHERE `entry`=49856; -- Lord Raymond George
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=49869; -- Stormwind Infantry
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49870; -- Angela Hipple
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=49871; -- Blackrock Battle Worg
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=49874; -- Blackrock Spy
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49875; -- Ace
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=49876; -- Brett "Coins" McQuid
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=49877; -- Captain Lancy Revshon
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=49878; -- Coach Crosscheck
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49879; -- Doc Zapnozzle
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49880; -- Foreman Dampwick
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=49881; -- Geargrinder Gizmo
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49882; -- Grimy Greasefingers
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49883; -- Izzy
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=49884; -- Sally "Salvager" Sandscrew
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=49885; -- KTC Train-a-Tron Deluxe
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49886; -- Missa Spekkies
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=49887; -- Gappy Silvertooth
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49888; -- Szabo
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=49889; -- Bruno Flameretardant
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=49890; -- Frankie Gearslipper
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=49891; -- Jack the Hammer
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=49892; -- Sudsy Magee
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=49893; -- Lisa McKeever
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=49894; -- Bamm Megabomb
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=37, `dmg_multiplier`=1 WHERE `entry`=49895; -- Evol Fingers
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=37, `dmg_multiplier`=1 WHERE `entry`=49896; -- Fizz Lighter
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=49900; -- Maxx Avalanche
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=37, `dmg_multiplier`=1 WHERE `entry`=49901; -- Sister Goldskimmer
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49902; -- Warrior-Matic NX-01
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=49907; -- Dylan Aguilar
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49909; -- Stinky Sharpshiv
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=49910; -- Vermillion Vanguard
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=49912; -- Jafix
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=49917; -- Barkeep Dadams
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=116, `attackpower`=174, `dmg_multiplier`=1 WHERE `entry`=49918; -- Buckslappy
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=112, `attackpower`=162, `dmg_multiplier`=1 WHERE `entry`=49919; -- Loremaster Bret
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=49920; -- Colt Carbine
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=49921; -- Kramer
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=49922; -- Kerwin the Destroyer
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49923; -- Sentinel Moonwing
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=49924; -- Joe Mag
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1 WHERE `entry`=49925; -- Johnny Shinster
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=49926; -- Max Boomson
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49927; -- Lanla Bowleaf
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=49932; -- Boss Mack
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1 WHERE `entry`=49933; -- Nik the Mountain
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1 WHERE `entry`=49934; -- The Great Pisani
UPDATE `creature_template` SET `mindmg`=68, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=49935; -- Ujumqin
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=49936; -- Zak Callabang
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=1 WHERE `entry`=49937; -- Aleesha Killem
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49939; -- Kenral Nightwind
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=37, `dmg_multiplier`=1 WHERE `entry`=49940; -- Irlara Morninglight
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=49941; -- Brann Bronzebeard
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49942; -- Dular
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=49944; -- Jeigh Southie
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=143, `attackpower`=200, `dmg_multiplier`=1 WHERE `entry`=49953; -- Dr. Wizzle Doomsday
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=49956; -- Pebble
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=49958; -- Matt Bruxworthy
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49960; -- Hobbes
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49963; -- Laera Dubois
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=49968; -- Lareth Beld
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=49997; -- Den'chulu
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=37, `dmg_multiplier`=1 WHERE `entry`=49998; -- Gusini
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=37, `dmg_multiplier`=1 WHERE `entry`=50001; -- Parata
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=37, `dmg_multiplier`=1 WHERE `entry`=50002; -- Bomsanchu
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=50004; -- Jamai
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50006; -- Yeniss
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=286, `dmg_multiplier`=1 WHERE `entry`=50007; -- Derithela
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=135, `dmg_multiplier`=1 WHERE `entry`=50010; -- Alaressa
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=50011; -- Cona
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=286, `dmg_multiplier`=1 WHERE `entry`=50012; -- Blood Knight Argaron
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=135, `dmg_multiplier`=1 WHERE `entry`=50013; -- Matron Ismara
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50015; -- Munalti
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=50016; -- Pathstalker Arpalir
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=135, `dmg_multiplier`=1 WHERE `entry`=50017; -- Summoner Durael
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=50018; -- Tarlhir
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=286, `dmg_multiplier`=1 WHERE `entry`=50019; -- Darahu
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=286, `dmg_multiplier`=1 WHERE `entry`=50020; -- Irva
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=135, `dmg_multiplier`=1 WHERE `entry`=50021; -- Raluhi
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50022; -- Brulla
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=286, `dmg_multiplier`=1 WHERE `entry`=50023; -- Ordo
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=135, `dmg_multiplier`=1 WHERE `entry`=50024; -- Salha
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=50025; -- Bratu
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50027; -- Dargad
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50028; -- Chintoka
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50029; -- Egnom
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50031; -- Zumolo
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50032; -- Tarmod
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50033; -- Ging
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=50034; -- Musu Gentlemoon
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=80, `dmg_multiplier`=1 WHERE `entry`=50035; -- Sunwalker Lonaki
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=50036; -- Ramkahen Scout
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=50037; -- Ramkahen Scout
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=50038; -- Captain Hadan
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=50039; -- Goblin Assassin
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=783, `dmg_multiplier`=2.25 WHERE `entry`=50041; -- Myzrael
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=2.25 WHERE `entry`=50043; -- Trade Prince Donais
UPDATE `creature_template` SET `mindmg`=100, `maxdmg`=172, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=50044; -- Hell-Hoot
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=50047; -- Injured Stormwind Infantry
UPDATE `creature_template` SET `mindmg`=528, `maxdmg`=900, `attackpower`=726, `dmg_multiplier`=1 WHERE `entry`=50048; -- Earthwarden Yrsa
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=50092; -- Barbie Cutesazz
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=50094; -- Trapper Kiefer
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=50126; -- Maseel
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=50127; -- Windstalker Ifram
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=135, `dmg_multiplier`=1 WHERE `entry`=50128; -- Fel-Caller Guloto
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=50129; -- Daleohm
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=286, `dmg_multiplier`=1 WHERE `entry`=50130; -- Luknar
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=50136; -- Baelir
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=50137; -- Muha
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=289, `dmg_multiplier`=1 WHERE `entry`=50142; -- Summoner Calwen
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=50144; -- Ranger Eoss
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=50147; -- Pathstalker Rislar
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=289, `dmg_multiplier`=1 WHERE `entry`=50148; -- Matron Ossela
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=50149; -- Vinsun
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=50150; -- Nelur Lightsown
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=50152; -- Hahna Moonscrest
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=50153; -- Murgha the Tempered
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=50155; -- Naseev
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=289, `dmg_multiplier`=1 WHERE `entry`=50156; -- Summoner Nolric
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=50157; -- Ranger Selone
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=50158; -- Pathstalker Ralsir
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=50160; -- Rulen Lightsreap
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=1 WHERE `entry`=50161; -- Dar Rummond
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=289, `dmg_multiplier`=1 WHERE `entry`=50163; -- Matron Alesso
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=1 WHERE `entry`=50164; -- Tulgar Flamefist
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=50171; -- Muran Fairden
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=50174; -- Farseer Lopaa
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=50247; -- Jack "All-Trades" Derrington
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=50250; -- Ice Avatar
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=50251; -- Frostilicus
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=50252; -- Mary Oxworth
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=50258; -- Frostmaul Tumbler
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=50263; -- Jadrag the Slicer
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=50271; -- Melinda Hammond
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=50273; -- Cynthia
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=50274; -- Ashley
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=50275; -- James
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=50291; -- Arcane Tesseract
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=50299; -- Sanath Lim-yo
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=38, `attackpower`=74, `dmg_multiplier`=1 WHERE `entry`=50301; -- Goblin Admirer
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=50302; -- Imported Mottled Boar
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=50304; -- Captain Donald Adams
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=50305; -- Moon Priestess Lasara
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=50306; -- Kadu
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=50307; -- Lord Candren
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=50308; -- Master Tinker Trini
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=50309; -- Captain Stonehelm
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=50310; -- Goblin Captive
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=50311; -- Goblin Captive
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=50312; -- Mana-Compelled Shade
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=180, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=50315; -- Xorothian Eredar
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=180, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=50316; -- K'areshi Trader
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=50317; -- Mana Thirster
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=50318; -- Xorothian Satyr
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=50319; -- Dimensional Ooze
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=50320; -- Lost Ravager
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=50321; -- Xorothian Imp
UPDATE `creature_template` SET `mindmg`=105, `maxdmg`=188, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=50322; -- Arcane Mana-Cluster
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=50323; -- Frizzo Villamar
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=50324; -- Provisioner Arok
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=101, `dmg_multiplier`=1 WHERE `entry`=50325; -- Umbranse the Spiritspeaker
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=50366; -- Nymn
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=50371; -- Captain Broderick
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=38, `dmg_multiplier`=2.25 WHERE `entry`=50372; -- Aradne
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=50374; -- Risen Dead
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=50385; -- Farseer Tooranu
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=50387; -- Baleflame
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=50390; -- Velastrasza
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=50401; -- Titan Mechanism
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=50412; -- Kaitlin
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=50413; -- Mr. Woofles
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=50414; -- Risen Recruit
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=50422; -- Zakk Sinon
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=50424; -- Terran "Justice" Gregory
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=50432; -- Deva Marie
UPDATE `creature_template` SET `mindmg`=38, `maxdmg`=66, `attackpower`=108, `dmg_multiplier`=2.25 WHERE `entry`=50434; -- David Gregory
UPDATE `creature_template` SET `mindmg`=36, `maxdmg`=63, `attackpower`=98, `dmg_multiplier`=1 WHERE `entry`=50435; -- Judy Gregory
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=50463; -- Franny Mertz
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=50471; -- Afflicted Gilnean
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=1 WHERE `entry`=50474; -- Gilneas City Guard
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=50477; -- Champion Uru'zin
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=50480; -- Isabel Jones
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=50482; -- Marith Lazuria
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=50483; -- Brave Tuho
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=50488; -- Stone Guard Nargol
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=1 WHERE `entry`=50497; -- Huntsman Blake
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=50498; -- Loren the Fence
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=50499; -- Myriam Spellwaker
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=50500; -- Sergeant Cleese
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=50501; -- Sister Almyra
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=50502; -- Vitus Darkwalker
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=60, `dmg_multiplier`=2.25 WHERE `entry`=50504; -- Belysra Starbreeze
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=2.25 WHERE `entry`=50505; -- Lyros Swiftwind
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=2.25 WHERE `entry`=50506; -- Talran of the Wild
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=128, `dmg_multiplier`=2.25 WHERE `entry`=50507; -- Vassandra Stormclaw
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50508; -- Carrie Eileen Steen
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50509; -- Jenn Stravaganza
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50510; -- Rachel DeSimone
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=50511; -- Demolitions Analyst I
UPDATE `creature_template` SET `mindmg`=77, `maxdmg`=132, `attackpower`=192, `dmg_multiplier`=1 WHERE `entry`=50512; -- Demolitions Analyst II
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50513; -- Jamie Harriott
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50516; -- Gilnean Refugee
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50517; -- Gilnean Refugee
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50518; -- Gilnean Refugee
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50519; -- Gilnean Refugee
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50520; -- Gilnean Refugee
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=50521; -- Gilnean Refugee
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=1 WHERE `entry`=50524; -- Travis Nichols
UPDATE `creature_template` SET `mindmg`=225, `maxdmg`=385, `attackpower`=478, `dmg_multiplier`=1 WHERE `entry`=50525; -- Chris Miller
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=50526; -- Ian Jolly
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=50527; -- Joanna Jolly
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=50528; -- Samantha Cleland
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=50550; -- Envoy Droln
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=50567; -- Fielding Chesterhill
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=50570; -- Whilsey Bottomtooth
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=50571; -- Tidebreaker Deckhand
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=50572; -- Intrepid Deckhand
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=50573; -- Galerek's Remorse Deckhand
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=50574; -- Amelia Atherton
UPDATE `creature_template` SET `mindmg`=378, `maxdmg`=643, `attackpower`=708, `dmg_multiplier`=1 WHERE `entry`=50575; -- Iron Reaper Deckhand
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=50579; -- Alliance Cannon
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=50580; -- Horde Cannon
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=50594; -- Exploding Sheep
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=50601; -- Snevik the Blade
UPDATE `creature_template` SET `mindmg`=131, `maxdmg`=223, `attackpower`=246, `dmg_multiplier`=2.25 WHERE `entry`=50609; -- Nathanos Blightcaller
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=50613; -- Wailing Guardsman
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=50615; -- Tormented Officer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=50631; -- Prospector Drugan
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=50669; -- Dawn Radue
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=50676; -- Hans Crump
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=112, `dmg_multiplier`=1 WHERE `entry`=50690; -- Tarelvir
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=91, `dmg_multiplier`=1 WHERE `entry`=50714; -- Dyrhara
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=50715; -- Maelir
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=50716; -- Pyromancer Scorchbrew
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=50717; -- Flarna Flametongue
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=50720; -- Lainda Gemgold
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=50723; -- Keric Smolderblade
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=50729; -- Darba the Crone
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=50732; -- Larn Caverndeep
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=50939; -- Domesticated Kodo
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=51081; -- Nathan Stockton
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=2.25 WHERE `entry`=51142; -- Joeyray
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=51143; -- Sunwalker Jo'hsu
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=51193; -- Wild Camel
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=51195; -- Kor'kron Annihilator
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=51217; -- Cavorting Pygmy
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=840, `dmg_multiplier`=1 WHERE `entry`=51230; -- Earthmender
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=51287; -- Colonel Karzag
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=51288; -- Major Marsden
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51346; -- Orgrimmar Wind Rider
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51348; -- Stormwind Gryphon Rider
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51371; -- Darnassus Hippogryph Rider
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51372; -- Thunder Bluff Wind Rider
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51383; -- Ironforge Gryphon Rider
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51384; -- Forsaken Bat
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=51396; -- Stonecore Teleporter
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=51397; -- Stonecore Teleporter
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=51400; -- Haunted Stable Hand
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=51411; -- Neill Penny
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=51440; -- Adam Pressler
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=51496; -- Kim Horn
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=51503; -- Randah Songhorn
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=51504; -- Velia Moonbow
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51509; -- Bilgewater Bruiser
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51521; -- Bambala Headhunter
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51539; -- Flame Crest Bruiser
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51541; -- Sunveil Scout
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=29, `attackpower`=56, `dmg_multiplier`=1 WHERE `entry`=51596; -- Wildhammer Fact Checker
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=51631; -- Witherbark Fisher
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=47, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=51633; -- Witherbark Witch Doctor
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=51637; -- Tarl Cloudsong
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=51638; -- Garn Cloudsong
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=51639; -- Kador Cloudsong
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=51640; -- Lama Cloudsong
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=51648; -- Tabat
UPDATE `creature_template` SET `mindmg`=30, `maxdmg`=53, `attackpower`=90, `dmg_multiplier`=1.5 WHERE `entry`=51663; -- Pogeyan
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=51671; -- Rabid Hyena
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=51672; -- Sweeping Winds
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=51673; -- Venomscale Spitter
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=51674; -- Sand-Husk Scarab
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=51681; -- Frostsaber Cub
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=51709; -- Harris Franklin
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=51753; -- Schnottz Overseer
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=2.25 WHERE `entry`=51759; -- Oathsworn Skinner
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=51760; -- Carrion Bird
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51791; -- Okril'lon Defender
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51819; -- Arathi Deathstalker
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51852; -- Triumph Sentry
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51853; -- Northwatch Guard
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51855; -- Dreadguard
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51861; -- Northwatch Guard
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51867; -- Silverwind Vanquisher
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51899; -- Hammerfall Guardian
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51901; -- Revantusk Watcher
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51913; -- Sen'jin Guardian
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51915; -- Westfall Brigade Guard
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51917; -- Grom'gol Grunt
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51918; -- Stonard Grunt
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51921; -- Lakeshire Guard
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51924; -- Theramore Guard
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=900, `attackpower`=782, `dmg_multiplier`=1 WHERE `entry`=51961; -- Tirisfal Stag
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=51963; -- Fey-Drunk Darter
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=51964; -- Tirisfal Doe
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=24, `attackpower`=40, `dmg_multiplier`=1 WHERE `entry`=51965; -- Tirisfal Fawn
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=51970; -- Anton Buckey
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=51971; -- Jermaine Buckey
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=51972; -- Black Bear
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=51973; -- Young Black Bear
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=51976; -- Dun Morogh Sledder
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=51977; -- Dun Morogh Sledder
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=51978; -- Dun Morogh Sledder
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=51979; -- Dun Morogh Sledder
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=51982; -- Ollie
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=51983; -- Deadwind Widow
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=51986; -- Argent Sentry
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=51987; -- Deadwind Widow
UPDATE `creature_template` SET `mindmg`=27, `maxdmg`=47, `attackpower`=90, `dmg_multiplier`=1 WHERE `entry`=51997; -- Stephanie Krutsick
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=52000; -- Astranaar Sentinel
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52015; -- Captain Jake Sones
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52016; -- Petty Officer Chulok
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=52018; -- Palace Mook
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=138, `attackpower`=196, `dmg_multiplier`=2.25 WHERE `entry`=52021; -- Moltanus
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=101, `dmg_multiplier`=1 WHERE `entry`=52027; -- Larisse Pembraux
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=101, `dmg_multiplier`=1 WHERE `entry`=52028; -- Talric Forthright
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=52029; -- Edlan Halsing
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=101, `dmg_multiplier`=1 WHERE `entry`=52030; -- Liliana Emberfrost
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=52031; -- Sarana Damir
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=52, `dmg_multiplier`=1 WHERE `entry`=52032; -- Sinzi Sparkscribe
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=52033; -- Rogoc
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=52034; -- Togar
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=101, `dmg_multiplier`=1 WHERE `entry`=52036; -- Galra
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=101, `dmg_multiplier`=1 WHERE `entry`=52037; -- Zulna
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52053; -- Zanzil
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52059; -- High Priestess Kilnara
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52060; -- Lognah
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=52061; -- Pride of Bethekk
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=52076; -- Gurubashi Cauldron-Mixer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=52077; -- Gurubashi Berserker
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=52079; -- Gurubashi Bloodrager
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=52081; -- Gurubashi Cauldron-Mixer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=52082; -- Gurubashi Cauldron-Mixer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=52085; -- Razzashi Adder
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=52086; -- Hakkari Witch Doctor
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=52087; -- Zanzil Mindslave
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=52088; -- Gurubashi Cauldron-Mixer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52089; -- Gurubashi Worker
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=52092; -- Izak Mixmaster
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=52093; -- Zanagan Mixmaster
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1.5 WHERE `entry`=52146; -- Chitter
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52155; -- High Priest Venoxis
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=52156; -- Chained Spirit
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=42, `attackpower`=84, `dmg_multiplier`=1 WHERE `entry`=52161; -- Foulweald Pathfinder
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52167; -- Gurubashi Spirit Warrior
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=52170; -- Gizzik Oregrab
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=52171; -- Muddy Tracks
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=52190; -- Twilber Torquewrench
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=52192; -- Brogor
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=52196; -- Burning Blade Bruiser
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=52207; -- Nagala Whipshank
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52222; -- Spirit of Hakkar
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=52227; -- Balgor Whipshank
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=1 WHERE `entry`=52268; -- Riha
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52271; -- Hazza'rah
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=1 WHERE `entry`=52277; -- Droha
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=52278; -- Linsa
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=52292; -- Droha
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52305; -- Toxic Husk
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52306; -- Tangled Vine
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52311; -- Venomguard Destroyer
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=33, `attackpower`=64, `dmg_multiplier`=1 WHERE `entry`=52312; -- Xelnaz
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=52317; -- Mahala Cloudsong
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=52319; -- Mala Skywatcher
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52321; -- Honor Guard Dunstad
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52322; -- Witch Doctor Qu'in
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52323; -- Chosen of Hethiss
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=52325; -- Gurubashi Blood Drinker
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=52327; -- Gurubashi Shadow Hunter
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=52335; -- Dareth
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=52338; -- Burning Blade Attacker
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=52339; -- Lesser Priest of Bethekk
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52340; -- Tiki Lord Mu'Loa
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=52345; -- Pride of Bethekk
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52348; -- Berserking Boulder Roller
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=52356; -- Venture Co. Drudger
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=31, `attackpower`=60, `dmg_multiplier`=1 WHERE `entry`=52357; -- Venture Co. Mercenary
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52362; -- Tiki Lord Zim'wae
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52364; -- Ritual Tiki Mask
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52373; -- Florawing Needler
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=52374; -- Panther Cub
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52375; -- Florawing Needler
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52376; -- Florawing Needler
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52377; -- Florawing Needler
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52379; -- Venomtip Needler
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52380; -- Venomancer Mauri
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52381; -- Venomancer T'Kulu
UPDATE `creature_template` SET `mindmg`=19, `maxdmg`=34, `attackpower`=68, `dmg_multiplier`=1 WHERE `entry`=52386; -- Burning Blade Windrider
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=52392; -- Gurubashi Master Chef
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52402; -- Venomtooth
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52405; -- Mor'Lek the Dismantler
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52409; -- Ragnaros
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=52413; -- Zulian Gnasher
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52414; -- Tor-Tun
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=52417; -- Shredtooth Frenzy
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52418; -- Lost Offspring of Gahz'ranka
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52422; -- Kaulema the Mover
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52427; -- Destroyed Containment Unit
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=52429; -- Zanza the Restless
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52430; -- Hakkar's Chains
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52434; -- Gurubashi Villager
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52435; -- Gurubashi Villager
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52436; -- Gurubashi Refugee
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52437; -- Gurubashi Refugee
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52438; -- Mortaxx
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52440; -- Gub
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52441; -- Curious Jungle Monkey
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52442; -- Florawing Hive Queen
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=52446; -- Ancient Dwarven Artifact
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=52448; -- Gef
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=52449; -- Ancient Dwarven Artifact
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=52450; -- Ancient Elven Artifact
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=52451; -- Ancient Elven Artifact
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=52452; -- Ancient Troll Artifact
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=52453; -- Ancient Troll Artifact
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=52454; -- Ancient Fossil
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=52455; -- Ancient Fossil
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=52456; -- Jesi
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=52457; -- Kodo Calfling
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52571; -- Majordomo Staghelm
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=52584; -- Laida Gembold
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=52586; -- Hanner Gembold
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=52587; -- Neller Fayne
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=52588; -- Sara Lanner
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=52598; -- Gurubashi Soul-Eater
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52606; -- Gurubashi Warmonger
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52624; -- Twisted Spirit
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=52636; -- Tana Lentner
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=52637; -- Hugo Lentner
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=52640; -- Rolf Karner
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=52641; -- Layna Karner
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=52642; -- Foreman Pernic
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=52643; -- Rissa Halding
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=52644; -- Tarien Silverdew
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=52645; -- Aessa Silverdew
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=52651; -- Engineer Palehoof
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=52654; -- Bwemba
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=52655; -- Palehoof's Big Bag of Parts
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=52657; -- Nahari Cloudchaser
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=52658; -- Paku Cloudchaser
UPDATE `creature_template` SET `mindmg`=738, `maxdmg`=1257, `attackpower`=805, `dmg_multiplier`=1 WHERE `entry`=52672; -- Flameward Hippogryph
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52730; -- Gurubashi Spirit
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52806; -- Darkspear Warrior
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52820; -- Helpful Jungle Monkey
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=52830; -- Michelle De Rum
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=52924; -- Vol'jin
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=52925; -- Witch Doctor T'wansi
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52933; -- Blood Guard Hakkuz
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=52938; -- Siame-Quashi
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52939; -- Hazlek
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=52941; -- Bakkalzu
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52956; -- Zandalari Juggernaut
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52958; -- Zandalari Hierophant
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=52962; -- Zandalari Archon
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=53021; -- Amani'shi Peon
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53022; -- Siame-Quashi
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=53023; -- Bloodslayer T'ara
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=53024; -- Bloodslayer Zala
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=53025; -- Bloodslayer Vaena
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=53040; -- Vehini
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53043; -- Briney Boltcutter
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53044; -- Blackwater Ruffian
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=53081; -- Bwemba
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=53088; -- Temple Rat
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53094; -- Patriarch Fire Turtle
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53095; -- Matriarch Fire Turtle
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53096; -- Fire Turtle Hatchling
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53102; -- Inferno Hawk
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53115; -- Molten Lord
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53116; -- Unbound Blazing Elemental
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53119; -- Flamewaker Forward Guard
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53120; -- Flamewaker Pathfinder
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=53121; -- Flamewaker Cauterizer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53127; -- Fire Scorpion
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53128; -- Giant Fire Scorpion
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53129; -- Lava Dweller
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53130; -- Ancient Lava Dweller
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53141; -- Molten Surger
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53151; -- Overseer Blingbang
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53167; -- Unbound Pyrelord
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53185; -- Flamewaker Overseer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=53187; -- Flamewaker Animator
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53188; -- Flamewaker Subjugator
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53206; -- Hell Hound
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53222; -- Flamewaker Centurion
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53223; -- Flamewaker Beast Handler
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53224; -- Flamewaker Taskmaster
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=53231; -- Lava Scion
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53244; -- Flamewaker Trainee
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53308; -- Flamewaker Centurion
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=53309; -- Flamewaker Cauterizer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53310; -- Molten Lord
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=53352; -- Commander Sharp
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53394; -- Blue Dragon
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53396; -- Tarecgosa
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=81, `dmg_multiplier`=1 WHERE `entry`=53403; -- Allen Bright
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=91, `dmg_multiplier`=1 WHERE `entry`=53404; -- Redia Vaunt
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=194, `dmg_multiplier`=1 WHERE `entry`=53405; -- Raedra Windhammer
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=53407; -- Sansha MacVince
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=53409; -- "Kobold" Kerik
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=85, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=53410; -- Lissah Spellwick
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=85, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=53415; -- Theoden Manners
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=53421; -- Faena Woolybush
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=53436; -- Eustace Tanwell
UPDATE `creature_template` SET `mindmg`=49, `maxdmg`=85, `attackpower`=124, `dmg_multiplier`=1 WHERE `entry`=53437; -- Humbert Tanwell
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=53469; -- Flamewaker Incinerator
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53470; -- Enraged Magma Hound
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53476; -- Lava Monstrosity
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53477; -- Cinderweb Skitterer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53478; -- Cinderweb Clutchkeeper
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53479; -- Cinderweb Matriarch
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=53494; -- Baleroc
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=53516; -- Essence of Doubt
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=53517; -- Squirming Slime Mold
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=2.25 WHERE `entry`=53518; -- Thrall
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=53522; -- Baby Octopus
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=53526; -- Brightwater Snail
UPDATE `creature_template` SET `mindmg`=23, `maxdmg`=41, `attackpower`=80, `dmg_multiplier`=2.25 WHERE `entry`=53527; -- Prisoner
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=53528; -- Chef Audrey
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53530; -- Shadow of Doubt
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=53544; -- Squirky
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=53545; -- Molten Spewer
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=53561; -- Stonebull Crayfish
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=53565; -- Aspiring Trainee
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=53575; -- Lava Wielder
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=53616; -- Kar the Everburning
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=53617; -- Molten Erupter
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=53619; -- Druid of the Flame
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=53626; -- Stormwind Orphan
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53631; -- Cinderweb Spiderling
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=53639; -- Flamewaker Cauterizer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53640; -- Flamewaker Sentinel
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=53641; -- Allison Potts
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53642; -- Cinderweb Spinner
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=53646; -- Tides of Longing
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53647; -- Vortex of Longing
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=53694; -- Riplimb
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=53695; -- Rageface
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=53704; -- Corsac Fox
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=53705; -- Pangolin
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53718; -- Cinderweb Matriarch
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53732; -- Unbound Smoldering Elemental
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=53738; -- Aggra
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=53739; -- Element of Patience
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=53793; -- Harbinger of Flame
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=53795; -- Egg Pile
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=53872; -- Cenarius
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=53875; -- Malfurion Stormrage
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=53876; -- Arch Druid Hamuul Runetotem
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=53894; -- Corestone of Patience
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=53991; -- Quincy Cutler
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=54004; -- Jaga
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=54015; -- Majordomo Staghelm
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=54019; -- Captive Druid of the Talon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=54026; -- Dotty
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=54073; -- Flamewaker Hound Master
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=54113; -- Spooks
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=54116; -- Dori'thur
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=54117; -- Vin
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=54130; -- Amberleaf Scamp
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=54131; -- Fe-Feng Hozen
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=54135; -- Master Li Fei
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=54143; -- Molten Flamefather
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=54161; -- Flame Archon
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=54214; -- Scott Keenan
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=54215; -- Lana Ashwin
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=54216; -- Warren Fulton
UPDATE `creature_template` SET `mindmg`=42, `maxdmg`=73, `attackpower`=114, `dmg_multiplier`=1 WHERE `entry`=54217; -- Daphne "The Rose" Bloom
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=54218; -- Daniel Kinsey
UPDATE `creature_template` SET `mindmg`=29, `maxdmg`=50, `attackpower`=92, `dmg_multiplier`=1 WHERE `entry`=54219; -- Sean Copeland
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=54232; -- Mrs. Gant
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=54244; -- Undercity Citizen
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=54254; -- Flashfire
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=54255; -- Hatespark
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=54256; -- Heatflayer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=54299; -- Magma Orb
UPDATE `creature_template` SET `mindmg`=22, `maxdmg`=39, `attackpower`=78, `dmg_multiplier`=1 WHERE `entry`=54308; -- Kirsty Logan
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=7, `dmg_multiplier`=2.25 WHERE `entry`=54313; -- Thrall
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=54334; -- Darkmoon Faire Mystic Mage
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=54345; -- Darkmoon Faire Greeter
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=54348; -- Circle of Thorns Portal
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=54367; -- Circle of Thorns Portal
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=54371; -- Riverpaw Bandit
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=54372; -- Riverpaw Brute
UPDATE `creature_template` SET `mindmg`=13, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=54373; -- Riverpaw Herbalist
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=54401; -- Naresir Stormfury
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=54402; -- Lurah Wrathvine
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=54431; -- Echo of Baine
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=54441; -- Thaumaturge Vashreen
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=54442; -- Warpweaver Hashom
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=54443; -- Vaultkeeper Razhid
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=54445; -- Echo of Jaina
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=54471; -- Thaumaturge Zajir
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=54472; -- Vaultkeeper Jazra
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=54473; -- Warpweaver Dushar
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=54476; -- Nozdormu
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=54485; -- Jessica Rogers
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=54490; -- Dubenko
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=1, `dmg_multiplier`=1 WHERE `entry`=54504; -- Damaged Tonk
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=54510; -- Baby
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=54511; -- Time-Twisted Geist
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=54512; -- Time-Twisted Sentinel
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=54517; -- Time-Twisted Shadowtalon
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=54518; -- Injured Carnie
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=54543; -- Time-Twisted Drake
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=54545; -- Loose Stones
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=54552; -- Time-Twisted Breaker
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=54553; -- Time-Twisted Seer
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=54557; -- Amberglass Crab
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=54567; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=54568; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=54586; -- Huojin Trainee
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=54587; -- Tushui Trainee
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=54595; -- Maggy
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=54596; -- Raan
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=54601; -- Mola
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=54605; -- Finlay Coolshot
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=54608; -- Master Shang Xi
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=54611; -- Jaomin Ro
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=851, `dmg_multiplier`=1 WHERE `entry`=54615; -- Nodd Codejack
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=54617; -- Rell Nightwind
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=54618; -- Nimm Codejack
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=54623; -- Shipwrecked Footman
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=54631; -- Living Air
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=1, `dmg_multiplier`=1 WHERE `entry`=54642; -- Enemy Tonk
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=1, `dmg_multiplier`=1 WHERE `entry`=54643; -- Enemy MiniZep
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=54648; -- Leeni "Smiley" Smalls
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=54649; -- Big Zokk Torquewrench
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=54650; -- Kezzik the Striker
UPDATE `creature_template` SET `mindmg`=200, `maxdmg`=341, `attackpower`=392, `dmg_multiplier`=1 WHERE `entry`=54653; -- Nargle Lashcord
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=54654; -- Ecton Brasstumbler
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=54655; -- Evee Copperspring
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=54656; -- Argex Irongut
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=135, `dmg_multiplier`=2.25 WHERE `entry`=54657; -- Doris Chiltonius
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=1 WHERE `entry`=54658; -- Sergeant Thunderhorn
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=1 WHERE `entry`=54659; -- Blood Guard Zar'shi
UPDATE `creature_template` SET `mindmg`=200, `maxdmg`=341, `attackpower`=392, `dmg_multiplier`=1 WHERE `entry`=54660; -- Lieutenant Tristia
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=1 WHERE `entry`=54661; -- Captain Dirgehammer
UPDATE `creature_template` SET `mindmg`=200, `maxdmg`=341, `attackpower`=392, `dmg_multiplier`=1 WHERE `entry`=54662; -- Knight-Lieutenant T'Maire Sydes
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=54687; -- Time-Twisted Footman
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=54688; -- Time-Twisted Nightsaber
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=2.25 WHERE `entry`=54690; -- Time-Twisted Priest
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=54691; -- Time-Twisted Sorceress
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=54693; -- Time-Twisted Rifleman
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=54699; -- Time-Twisted Nightsaber
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=54700; -- Time-Twisted Nightsaber
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=54701; -- Time-Twisted Huntress
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=54734; -- Master Li Fei
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=54751; -- Nozdormu
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=54780; -- Orchard Fire
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=54855; -- Min Dimwind
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=54856; -- Master Li Fei
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=54867; -- Image of Nozdormu
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=54870; -- General Nazgrim
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=377, `dmg_multiplier`=2.25 WHERE `entry`=54920; -- Infinite Suppressor
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=54923; -- Infinite Warden
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=54943; -- Merchant Lorvo
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=54975; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=54976; -- Barbed Ray
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=54983; -- Treant
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=54985; -- Treant
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=55015; -- Whitefeather Crane
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=55019; -- Tushui Monk
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=55021; -- Jojo Ironbrow
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=55022; -- Steam Fiend
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55054; -- General Nazgrim
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=55072; -- Barum
UPDATE `creature_template` SET `mindmg`=135, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=55088; -- Fire Eater
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=55089; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=55093; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=55117; -- Gargantua
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=55118; -- Jumbo
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=409, `attackpower`=528, `dmg_multiplier`=1 WHERE `entry`=55121; -- Frenzyheart Visitor
UPDATE `creature_template` SET `mindmg`=157, `maxdmg`=409, `attackpower`=528, `dmg_multiplier`=1 WHERE `entry`=55124; -- Oracle Visitor
UPDATE `creature_template` SET `mindmg`=157, `maxdmg`=268, `attackpower`=308, `dmg_multiplier`=1 WHERE `entry`=55125; -- Elder Daftusk
UPDATE `creature_template` SET `mindmg`=188, `maxdmg`=409, `attackpower`=528, `dmg_multiplier`=1 WHERE `entry`=55133; -- Corki
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55135; -- General Nazgrim
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55155; -- Fine Leather Journal
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55167; -- Alliance Banner
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55174; -- Naval Map
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55175; -- Alliance Cannon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55176; -- Alliance Orders
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=245, `attackpower`=292, `dmg_multiplier`=1 WHERE `entry`=55181; -- Arakkoa Visitor
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=55220; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=59, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=55221; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=62, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=55222; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1 WHERE `entry`=55223; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=80, `maxdmg`=180, `attackpower`=224, `dmg_multiplier`=1 WHERE `entry`=55225; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=83, `maxdmg`=213, `attackpower`=248, `dmg_multiplier`=1 WHERE `entry`=55226; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=248, `attackpower`=298, `dmg_multiplier`=1 WHERE `entry`=55229; -- Fire Eater
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=238, `attackpower`=282, `dmg_multiplier`=1 WHERE `entry`=55230; -- Fire Eater
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=55231; -- Fire Eater
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55247; -- Poot
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55248; -- Wallace
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55249; -- Bodie
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55250; -- Duquan
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55251; -- Namond
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55252; -- Michael
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55253; -- Randy
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=55254; -- Ellie Goodup
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55261; -- Darkmoon Parade Creature
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=55264; -- Sazz Coincatcher
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=55266; -- Zina Sharpworth
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55268; -- Aja
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55269; -- Jerrica
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55270; -- Kimber
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55271; -- Shana
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=55272; -- Mountain Horse
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=55273; -- Swift Mountain Horse
UPDATE `creature_template` SET `mindmg`=74, `maxdmg`=127, `attackpower`=188, `dmg_multiplier`=1 WHERE `entry`=55278; -- Boomie Sparks
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=55285; -- Astrid Langstrump
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=55292; -- Fang-she
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=55339; -- Trixi Sharpworth
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=55341; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=65, `maxdmg`=209, `attackpower`=242, `dmg_multiplier`=1 WHERE `entry`=55342; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55347; -- Alliance Citizen
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55348; -- Horde Citizen
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=55382; -- Darkmoon Faire Mystic Mage
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=55396; -- Darkmoon Faire Greeter
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=164, `attackpower`=214, `dmg_multiplier`=1 WHERE `entry`=55397; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=156, `attackpower`=210, `dmg_multiplier`=1 WHERE `entry`=55398; -- Fire Juggler
UPDATE `creature_template` SET `mindmg`=133, `maxdmg`=231, `attackpower`=272, `dmg_multiplier`=1 WHERE `entry`=55399; -- Fire Eater
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=55402; -- Korgol Crushskull
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=55428; -- Jumbo Jr.
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55429; -- Jumbina
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=55440; -- Violet
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=55443; -- Goldwing
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=55444; -- King Leo
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=55446; -- Queen Savannah
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=55448; -- M'ku
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=55449; -- Binti
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=55457; -- Gleok
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=55475; -- Darkmoon Tide Crawler
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=55477; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=55478; -- Jojo Ironbrow
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=55479; -- Gao Summerdraft
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=55483; -- Plump Virmen
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=55504; -- Plump Carrotcruncher
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=55506; -- Raggis
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55539; -- Wugou
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55556; -- Shu
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55558; -- Shu
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=55583; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=55585; -- Jojo Ironbrow
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=55586; -- Master Shang Xi
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55588; -- Elder Shaopai
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55592; -- Dafeng
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=55595; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=55596; -- Careless Carnie
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=55598; -- Krystal
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=55601; -- Fe-Feng Wiseman
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=55602; -- Urefu
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=55603; -- Shingo
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=55604; -- Mabaka
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=55608; -- Crush
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=55609; -- Lizzy
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=55632; -- Fe-Feng Ruffian
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=55633; -- Fe-Feng Firethief
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=55634; -- Ruk-Ruk
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=55640; -- Thornbranch Scamp
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=55650; -- Shang Xi's Hot Air Balloon
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=55672; -- Master Shang Xi
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=55684; -- Jordan Smith
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55714; -- Darkmoon Monkey
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=55715; -- Darkmoon Pony
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=55718; -- Sraaz
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=55719; -- Thomas Miller
UPDATE `creature_template` SET `mindmg`=200, `maxdmg`=341, `attackpower`=175, `dmg_multiplier`=1 WHERE `entry`=55722; -- Breanni
UPDATE `creature_template` SET `mindmg`=137, `maxdmg`=234, `attackpower`=278, `dmg_multiplier`=1 WHERE `entry`=55726; -- Belbi Quikswitch
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55727; -- Topper McNabb
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=55730; -- Nat Pagle
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=55732; -- Champion Faesrol
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=608, `dmg_multiplier`=1 WHERE `entry`=55733; -- Champion Isimode
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=55734; -- Asric
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=55735; -- Jadaar
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=55744; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=55761; -- Phillipe
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55783; -- Ravenholdt Sentry
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=55786; -- Zhao-Ren
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55789; -- Rell Nightwind
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55808; -- Ravenholdt Lookout
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55819; -- Sully "The Pickle" McLeary
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55823; -- Mishka
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55825; -- Amber Kearnen
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55830; -- Nimm Codejack
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=55832; -- Nodd Codejack
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=55869; -- Alizabal
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=55874; -- Zhao-Ren
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=55918; -- Shang Xi's Hot Air Balloon
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=55940; -- Jojo Ironbrow
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=55942; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=55943; -- Wei Palerage
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=55944; -- Delora Lionheart
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=55946; -- Pei-Wu Tiger
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=55999; -- Injured Sailor
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=56007; -- Darkened Horror
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=56008; -- Darkened Terror
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=56009; -- Vordraka, the Deep Sea Nightmare
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=56012; -- Elder Shaopai
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=56013; -- Spirit of Master Shang Xi
UPDATE `creature_template` SET `mindmg`=131, `maxdmg`=223, `attackpower`=262, `dmg_multiplier`=1 WHERE `entry`=56025; -- Cro Threadstrong
UPDATE `creature_template` SET `mindmg`=133, `maxdmg`=227, `attackpower`=268, `dmg_multiplier`=1 WHERE `entry`=56026; -- Darmari
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=56035; -- Kolin
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=56041; -- Rona Greenteeth
UPDATE `creature_template` SET `mindmg`=200, `maxdmg`=341, `attackpower`=175, `dmg_multiplier`=1 WHERE `entry`=56043; -- Martha Weller
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=56068; -- Steven Stagnaro
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=56069; -- Tatia Brine
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=56159; -- Master Shang Xi
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=56160; -- Darkmoon Wolf
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=56172; -- Ash Moth
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=56174; -- Pei-Wu Tiger Cub
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=56180; -- Speckled Trout
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=56195; -- Escaped Horde Engineer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=56236; -- Injured Sailor
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=56241; -- Dai-Lo Farmer
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=56274; -- Guardian of the Elders
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=56335; -- Daenrand Dawncrest
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=56350; -- Disciple of Hate
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=56360; -- Deepscale Tormentor
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=56362; -- Skyseeker Sailor
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=56369; -- Eye of Occu'thar
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=56372; -- Eye of Occu'thar
UPDATE `creature_template` SET `mindmg`=7, `maxdmg`=14, `attackpower`=19, `dmg_multiplier`=1 WHERE `entry`=56393; -- Excited Onlooker
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=56394; -- Mesmerized Child
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=56416; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=56417; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=56418; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=56419; -- Jojo Ironbrow
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56472; -- Fragrant Lotus
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56473; -- Flying Snow
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=56479; -- Legacy of Liu Lang
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=56526; -- Tawnyhide Fawn
UPDATE `creature_template` SET `mindmg`=3830, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56541; -- Master Snowdrift
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=56562; -- Anduin Wrynn
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=851, `dmg_multiplier`=1 WHERE `entry`=56592; -- Pearlfin Aqualyte
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=56622; -- Brazier
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=56637; -- Ook-Ook
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=56653; -- Wild Prowler
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=56655; -- Wild Huntress
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=56661; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=56662; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=56663; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56676; -- Shen-zin Su
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=56686; -- Master Shang Xi
UPDATE `creature_template` SET `mindmg`=3830, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56713; -- Master Snowdrift
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=56717; -- Hoptallus
UPDATE `creature_template` SET `mindmg`=3830, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56719; -- Sha of Violence
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=56730; -- Fe-Feng Brewthief
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56731; -- Habanero Brew
UPDATE `creature_template` SET `mindmg`=3830, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56754; -- Azure Serpent
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56762; -- Yu'lon
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56763; -- Regenerating Sha
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56764; -- Consuming Sha
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56765; -- Destroying Sha
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56767; -- Shado-Pan Fire Archer
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=56783; -- Hozen Hollerer
UPDATE `creature_template` SET `mindmg`=44, `maxdmg`=77, `attackpower`=118, `dmg_multiplier`=1 WHERE `entry`=56796; -- Angela Leifeld
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56849; -- Hozen Bouncer
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56862; -- Drunken Hozen Brawler
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56863; -- Sleepy Hozen Brawler
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=56865; -- Aqua Dancer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=56867; -- Fiery Trickster
UPDATE `creature_template` SET `mindmg`=3830, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56870; -- Master Snowdrift
UPDATE `creature_template` SET `mindmg`=3830, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56884; -- Taran Zhu
UPDATE `creature_template` SET `mindmg`=120, `maxdmg`=205, `attackpower`=238, `dmg_multiplier`=1 WHERE `entry`=56899; -- Ragged Jong
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56915; -- Sun
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=56924; -- Inflamed Hozen Brawler
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=56925; -- Farrah Facet
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=56927; -- Hozen Party Animal
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=57097; -- Hozen Party Animal
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=57132; -- Wu-Song Villager
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=57164; -- Fe-Feng Leaper
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=57205; -- Fe-Feng Hozen
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=57207; -- Nourished Yak
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=57258; -- Field Yak
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=57301; -- Glade Glimmer
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=57317; -- Skyseeker Sailor
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=57414; -- Temple Guard
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=57419; -- Morning Breeze Villager
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=57420; -- Ten Bitterrage
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=57465; -- Fe-Feng Ruffian
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=57466; -- Fe-Feng Firethief
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=57617; -- Lee Sunspark
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=57618; -- Jun Steelbreath
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=57619; -- Cheng Dawnscrive
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=57620; -- Whittler Dewei
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=57621; -- Teamaster Ren
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=57622; -- Lien-Hua Thunderhammer
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=57623; -- Shen Stonecarver
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=57624; -- Yu Sugargale
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=57709; -- Nourished Yak
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=57710; -- Delivery Cart
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=57712; -- Delivery Cart Tender
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=57720; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=57721; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=57739; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=57741; -- Delivery Cart
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=57743; -- Nourished Yak
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=57748; -- Instructor Qun
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=57752; -- Quiet Lam
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=57753; -- Ironfist Zhou
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=57754; -- Instructor Mossthorn
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=57769; -- Shen-zin Su
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=57797; -- Corsac Fox
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=57800; -- Thaumaturge Rafir
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=57801; -- Thaumaturge Altha
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=101, `dmg_multiplier`=1 WHERE `entry`=57850; -- Teleportologist Fozlebub
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=57853; -- Big Bertha
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=897, `dmg_multiplier`=1 WHERE `entry`=57864; -- Alurmi
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=57922; -- Taryssa Lazuria
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=57983; -- Baruma
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=58152; -- Tini Smalls
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=101, `dmg_multiplier`=1 WHERE `entry`=58154; -- Toren Landow
UPDATE `creature_template` SET `mindmg`=110, `maxdmg`=188, `attackpower`=228, `dmg_multiplier`=1 WHERE `entry`=58155; -- Rugok
UPDATE `creature_template` SET `mindmg`=71, `maxdmg`=121, `attackpower`=184, `dmg_multiplier`=1 WHERE `entry`=58167; -- Colton Smith
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=58193; -- Wyrmrest Protector
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=58195; -- Chromie
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=2.25 WHERE `entry`=58199; -- Lord Itharius
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=58219; -- Saltback Yearling
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=2.25 WHERE `entry`=58336; -- Darkmoon Rabbit
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=1 WHERE `entry`=58342; -- Christopher Lesson
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=58542; -- Past Self
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=58646; -- Farmer Yoon
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=58706; -- Gina Mudclaw
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=58712; -- Kol Ironpaw
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=58716; -- Jian Ironpaw
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=58767; -- Pomfruit
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=58786; -- Swirling Pool
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=58794; -- Slain Shado-Pan Defender
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=58803; -- Residual Hatred
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=58807; -- Vestige of Hatred
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=58810; -- Fragment of Hatred
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=58812; -- Hateful Essence
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=58856; -- Haunting Sha
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=58884; -- Sha-Haunted Tigress
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=58885; -- Sha-Haunted Crane
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=58944; -- Kun-Lai Refugee
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=59019; -- Binan Villager
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=59022; -- Admiral Taylor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=59074; -- Uncle Gao
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=59075; -- Ancestral Brewmaster
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=59081; -- Diminutive Cocoon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=59084; -- Pearly Tortoise
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=59154; -- Yak Statuette
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=59156; -- Mogu Effigy
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=59157; -- Granite Quilen
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=59225; -- Ball of Fire
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=59464; -- Hopper
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=59494; -- Yeasty Brew Alemental
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=59497; -- Delivery Cart
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=59499; -- Nourished Yak
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=59518; -- Bloated Brew Alemental
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=59520; -- Fizzy Brew Alemental
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=59551; -- Bopper
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=59577; -- Farmstead Slave
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=59605; -- Sodden Hozen Brawler
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=59607; -- Spectral Guise
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=59642; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=59647; -- Field Hand
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=59649; -- Tushui Monk
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=59684; -- Hozen Party Animal
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=59690; -- Freed Farmhand
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=59704; -- Chen Stormstout
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=59705; -- Scarlet Flamethrower
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=59718; -- Burilgi Despoiler
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=59741; -- Shado-Pan Guardian
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=59746; -- Scarlet Centurion
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=59766; -- Hollow Bloom
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=59781; -- Thunderpaw Initiate
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=59794; -- Krik'thik Disruptor
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=59799; -- Fizzy Bubble
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=59800; -- Krik'thik Rager
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=59804; -- Gripping Hatred
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=59822; -- Auntie Stormstout
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=59824; -- Golden Hopling
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=59834; -- Boss 3 Encounter Add Generator
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=59899; -- Fei
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=59936; -- Armored Carp
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=59960; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=59962; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=59963; -- Jojo Ironbrow
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=59986; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=59988; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=59989; -- Jojo Ironbrow
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=60002; -- Lao Softfoot
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60042; -- Korga Strongmane
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60055; -- Makael Bay
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=60176; -- Huojin Monk
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=60183; -- Trainee Nim
UPDATE `creature_template` SET `mindmg`=7965, `maxdmg`=13541, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=60184; -- Serviteur t
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=60208; -- Hopling
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=60241; -- Lightning Charge
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=60244; -- Trainee Guang
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=60245; -- Brewer Zhen
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=60248; -- Chia-hui Autumnleaf
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=60253; -- Brewer Lin
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=60259; -- Kong
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=60260; -- Jie
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=60276; -- Hozen Clinger
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=60411; -- Water Pincer
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=60415; -- Flak Cannon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=60438; -- Wildfire Spark
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60554; -- Escaped Horde Crewman
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=60669; -- Long-Haired Yak
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=60713; -- Osul Charger
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=60722; -- Deepscale Tormentor
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60729; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=7965, `maxdmg`=13541, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=60731; -- Undying Shadows
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=60739; -- Steppebeast
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60770; -- Horde Druid
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=60780; -- Deepscale Ravager
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60834; -- Horde Druid
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=60851; -- Delora Lionheart
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=60852; -- Korga Strongmane
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60853; -- Makael Bay
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60854; -- Escaped Horde Crewman
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=60858; -- Deepscale Fleshripper
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60873; -- Skyseeker Sailor
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60874; -- Escaped Horde Crewman
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60877; -- Alliance Priest
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60878; -- Alliance Priest
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=60888; -- Korga Strongmane
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=60889; -- Delora Lionheart
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60890; -- Skyseeker Sailor
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60891; -- Escaped Horde Crewman
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60892; -- Jojo Ironbrow
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60893; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60894; -- Makael Bay
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60895; -- Alliance Priest
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60896; -- Horde Druid
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60897; -- Injured Sailor
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60898; -- Injured Horde Crewman
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=60900; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=60913; -- Charge d
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=60917; -- Huo
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=60918; -- Shu
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=60919; -- Wugou
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=60920; -- Dafeng
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=60940; -- Shadowy Minion
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=60958; -- Pinning Arrow
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=60969; -- Furious Paw'don Defender
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=60997; -- The Wrestler
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=61004; -- Healiss
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=61006; -- Tankiss
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=61007; -- Hackiss
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=61077; -- Wayward Lamb
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=61140; -- Gara'jal the Spiritbinder
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=61252; -- Shado-Pan Guard
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=61260; -- Coach Rok Rok
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=61350; -- Graceful Swan
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=61366; -- Rat
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=61411; -- Instructor Zhi
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=61426; -- Shado-Pan Spike Trap
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=61618; -- Longshadow Mushan
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=61994; -- Crow
UPDATE `creature_template` SET `mindmg`=7965, `maxdmg`=13541, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=62003; -- Sectionneur d?
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=62005; -- Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=62210; -- Beast
UPDATE `creature_template` SET `mindmg`=3830, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=62236; -- Ban Bearheart
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=62382; -- Abandoned Kite
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=62419; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=62445; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=62539; -- Ball and Chain
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=62567; -- Cloudrunner Hatchling
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=62601; -- Motherseed
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=62618; -- Etincelle cosmique
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=62855; -- Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=62856; -- Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=62857; -- Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=62858; -- Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=62860; -- Beast
UPDATE `creature_template` SET `mindmg`=3830, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=62865; -- Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=62991; -- Coral Adder
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=62992; -- Bucktooth Flapper
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=62993; -- Julean Tremblay
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=62997; -- Jungle Darter
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=62999; -- Temple Snake
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63001; -- Silkbead Snail
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63002; -- Garden Frog
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63003; -- Masked Tanuki
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63004; -- Grove Viper
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63005; -- Spirebound Crab
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63006; -- Sandy Petrel
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=63025; -- Tsulong
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63062; -- Bandicoon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63064; -- Bandicoon Kit
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63095; -- Malayan Quillrat Pup
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=63238; -- Xi, Friend to the Small
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63239; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63241; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63242; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=63258; -- Bao
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63291; -- Savory Beetle
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63293; -- Spiny Terrapin
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=63296; -- Gato
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63304; -- Jungle Grub
UPDATE `creature_template` SET `mindmg`=12, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=63310; -- Zabrax
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=63327; -- Shoyu
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=63331; -- Laoxi
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=63335; -- Mojo Stormstout
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63358; -- Sifang Otter Pup
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63548; -- Crunchy Scorpion
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63550; -- Alpine Foxling
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63551; -- Alpine Foxling Kit
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63556; -- Mogu Statue
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63558; -- Tolai Hare Pup
UPDATE `creature_template` SET `mindmg`=14, `maxdmg`=24, `attackpower`=44, `dmg_multiplier`=1 WHERE `entry`=63596; -- Audrey Burnhep
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63603; -- Ban's Balloon
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=63605; -- Stonebound Watcher
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=63610; -- Shao-Tien Dominator
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=63611; -- Shao-Tien Soul-Caller
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=63626; -- Varzok
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=63674; -- Mogu Statue
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63715; -- Jumping Spider
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63716; -- Masked Tanuki Pup
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63740; -- Amber Tap
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=63815; -- Puli the Even Handed
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63841; -- Golden Civet
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63842; -- Golden Civet Kitten
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63850; -- Effervescent Glowfly
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=63917; -- Rai
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=63919; -- Leopard Tree Frog
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64040; -- Tina Wang
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64041; -- Mifan
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64042; -- Hara Alebelly
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64043; -- Uncle Gus
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64057; -- Moko Powderrun
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64059; -- Razzie Coalwrench
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64085; -- Cullen Martel-Front
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=64087; -- Marri
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64090; -- Missy M
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=64092; -- Murphy Diremoor
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64118; -- Erni Tanboshi
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64119; -- Dirki Tanboshi
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64148; -- Fitz Togglescrew
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64158; -- Arenji
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64168; -- Vienh Stormbrew
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64169; -- Ranna
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=64238; -- Resilient Roach
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=64246; -- Shy Bandicoon
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64382; -- Thunderpaw Initiate
UPDATE `creature_template` SET `mindmg`=3830, `maxdmg`=6511, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=64387; -- Master Snowdrift
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64449; -- Kim Won Gi
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=64464; -- Andi
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64481; -- Priest Whitebrow
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64482; -- Healer Nan
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64500; -- Brewmaster Chani
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=64505; -- Ji Firepaw
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=64506; -- Aysa Cloudsinger
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=64507; -- Firework Launcher
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=64514; -- Grand Expedition Yak
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64516; -- Cousin Slowhands
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=64524; -- Oil Keg
UPDATE `creature_template` SET `mindmg`=3000, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=64549; -- Shado-Pan Ice Archer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64551; -- Duskwing Crow
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=64554; -- Zhao-Ren
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=64593; -- Korga Strongmane
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=64619; -- Beast
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=64620; -- Beast
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=5101, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64745; -- Lowland Porcupine
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=64804; -- Silent Hedgehog
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64821; -- Tong the Fixer
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64846; -- Springtender Ashani
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=64849; -- Kunchong Hatchling
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=64858; -- Airman Hicks
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=64859; -- Sky Captain Callanan
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=64860; -- Captain Kerwin
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=64861; -- Captain Day
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=64862; -- Airman Ellis
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=64863; -- Sky Mage Harlan
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=64864; -- Airman Clifton
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=64865; -- Airman Rogers
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=64866; -- Corporal Rasmussen
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=64875; -- Lorewalker Amai
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=64876; -- Lorewalker Ruolin
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=64877; -- Tingting
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=64878; -- Young Student
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=64879; -- Yin
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=64880; -- Nan
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=64881; -- Hao
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=64885; -- Lorewalker Zan
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=2.25 WHERE `entry`=64922; -- Brann Bronzebeard
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64924; -- Guyo Crystalgear
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=64925; -- Tivilix Bangalter
UPDATE `creature_template` SET `mindmg`=5, `maxdmg`=10, `attackpower`=13, `dmg_multiplier`=1 WHERE `entry`=64929; -- Lamplighter Deng
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=64939; -- Lamplighter Sunny
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=65008; -- Huojin Monk
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65035; -- Summer Lily
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65037; -- Treeshaper Shu
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65039; -- Eng Dirtplow
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65040; -- Apothecary Jung
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65042; -- Shun the Serene
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65043; -- Elder Oakpaw
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65044; -- Gan Darkcask
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65045; -- Gao Longwagon
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65046; -- Lao Ma Liang
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65047; -- Yi
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=65048; -- Tushui Trainee
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65049; -- Er
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65050; -- San
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=65051; -- Tushui Monk
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65052; -- Shi
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65053; -- Bai Hua
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65054; -- Feverbite Hatchling
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65055; -- Hanshi One-Eye
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65056; -- Longbeard the Liar
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65057; -- Fan Slowplow
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65059; -- Jing Stoutgut
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65062; -- Shao-Li Ironbelly
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65064; -- Ki-Ro the Contemplative
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=65066; -- Jojo Ironbrow
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65067; -- Steepmaster Tira
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65068; -- Old Whitenose
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65070; -- Gokan Sharphoe
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65073; -- Naira Watergarden
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65075; -- Hyacinth
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65077; -- Groundskeeper Amalia
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65080; -- Chi Master Lim
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65081; -- Master Cranewing
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65083; -- Foolish Chao
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65084; -- Crimson Butterfly
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65085; -- Jin
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65086; -- Da Na
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65087; -- Trader Feng
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65088; -- Groundskeeper Shen
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65089; -- Old Yang
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65091; -- Vu Blackbelly
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65093; -- Seeress Weng Wu
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=65102; -- Dafeng
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=65104; -- Wugou
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=65105; -- Shu
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=65107; -- Huo
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65146; -- Zach Chow
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65147; -- Azunyan
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65183; -- Spirit Healer
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65185; -- Mei Li Sparkler
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65187; -- Amber Moth
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65203; -- Emperor Crab
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65215; -- Garden Moth
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65216; -- Shrine Fly
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65248; -- Kaydee Threesong
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65266; -- David Harrington
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65267; -- Mistfall Brew Keg
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65321; -- Wild Crimson Hatchling
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65323; -- Wild Jade Hatchling
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65324; -- Wild Golden Hatchling
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=65362; -- Minion of Doubt
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65375; -- Ancestral Brewmaster
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65376; -- Ancestral Brewmaster
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65384; -- Grant
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=65392; -- Nibbler
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=65467; -- Excited Onlooker
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=65468; -- Tushui Monk
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65469; -- Aspiring Trainee
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65470; -- Huojin Trainee
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65471; -- Tushui Trainee
UPDATE `creature_template` SET `mindmg`=4, `maxdmg`=9, `attackpower`=11, `dmg_multiplier`=1 WHERE `entry`=65472; -- Wu-Song Villager
UPDATE `creature_template` SET `mindmg`=8, `maxdmg`=14, `attackpower`=21, `dmg_multiplier`=1 WHERE `entry`=65473; -- Dai-Lo Farmer
UPDATE `creature_template` SET `mindmg`=2350, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65535; -- Liu Ze
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65537; -- Serpent's Spine Cannoneer
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=65545; -- Huojin Monk
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=65550; -- Tushui Monk
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=2.25 WHERE `entry`=65559; -- Vision of Zhao-Ren
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=65560; -- Vision of Dafeng
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65574; -- Brad Rhodes
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65576; -- Philip Luke
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65579; -- Eric Thibeau
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65596; -- Lil' Canny
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65599; -- H.A.R.V.E.Y.
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=65742; -- Escaped Horde Crewman
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=65744; -- Jun-Jun Edgewater
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=65745; -- Duyi Edgewater
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65762; -- Shao-Tien Cannon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65795; -- Golden Lotus Guard
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65796; -- Golden Lotus Guard
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=65803; -- Cobalt Mine
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65870; -- Mei Blackbrow
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65871; -- Lo Blackbrow
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=65889; -- Su-Shi
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=65976; -- Bartender Tomro
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=66022; -- Turtlemaster Odai
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=66064; -- Bartender Tomro
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=66207; -- Master Hsu
UPDATE `creature_template` SET `mindmg`=57, `maxdmg`=97, `attackpower`=136, `dmg_multiplier`=1 WHERE `entry`=66253; -- Master Kistane
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=66254; -- Master Woo
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=66255; -- Master Yoon
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=66256; -- Master Cheng
UPDATE `creature_template` SET `mindmg`=148, `maxdmg`=252, `attackpower`=304, `dmg_multiplier`=1 WHERE `entry`=66257; -- Master Tsang
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=66258; -- Master Cheng
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=66292; -- Sky Admiral Rogers
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=66297; -- Skyfire Gyrocopter
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=66327; -- Garrosh'ar Shredder
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=66339; -- Horde War Wagon
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=21, `attackpower`=36, `dmg_multiplier`=1 WHERE `entry`=66366; -- Captive Pandaren Cub
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=66396; -- Ga'trul
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=66397; -- Garrosh'ar Shredder
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=66398; -- Garrosh'ar Grunt
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=66407; -- Rell Nightwind
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=66437; -- Arcanist Xu
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=66449; -- Ang the Wise
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=66473; -- Skyfire Gyrocopter
UPDATE `creature_template` SET `mindmg`=1032, `maxdmg`=1756, `attackpower`=930, `dmg_multiplier`=1 WHERE `entry`=66480; -- Skyfire Engineer
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=66509; -- Amber Kearnen
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=66510; -- Sully "The Pickle" McLeary
UPDATE `creature_template` SET `mindmg`=1442, `maxdmg`=2454, `attackpower`=1004, `dmg_multiplier`=1 WHERE `entry`=66527; -- Mishka
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=66602; -- Petulant Pumpkin
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=66654; -- Thunder Hold Supplies
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=66671; -- Piddle
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=66672; -- Puddle
UPDATE `creature_template` SET `mindmg`=3, `maxdmg`=7, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=66673; -- Poople
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=66689; -- Mei Chele
UPDATE `creature_template` SET `mindmg`=10, `maxdmg`=19, `attackpower`=28, `dmg_multiplier`=1 WHERE `entry`=66690; -- Lamplighter Mu
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=66729; -- Krasarang Glider
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3997, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=66750; -- Nurse Shark
UPDATE `creature_template` SET `mindmg`=20, `maxdmg`=36, `attackpower`=70, `dmg_multiplier`=1 WHERE `entry`=66854; -- Initiate Chuang
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=66943; -- Serpent Statue
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=66946; -- Balance Pole
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=66947; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=126, `maxdmg`=217, `attackpower`=252, `dmg_multiplier`=1 WHERE `entry`=66986; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=270, `maxdmg`=461, `attackpower`=642, `dmg_multiplier`=1 WHERE `entry`=67009; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=2.25 WHERE `entry`=67012; -- Taran Zhu
UPDATE `creature_template` SET `mindmg`=34, `maxdmg`=60, `attackpower`=102, `dmg_multiplier`=1 WHERE `entry`=67015; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=67021; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=87, `maxdmg`=149, `attackpower`=206, `dmg_multiplier`=1 WHERE `entry`=67066; -- Monk Trainee
UPDATE `creature_template` SET `mindmg`=1841, `maxdmg`=3132, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=67125; -- Thundergill
UPDATE `creature_template` SET `mindmg`=4888, `maxdmg`=8312, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=67130; -- D.E.N.T.
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=67185; -- Provisioner Phelps
UPDATE `creature_template` SET `mindmg`=11, `maxdmg`=19, `attackpower`=32, `dmg_multiplier`=1 WHERE `entry`=67186; -- Provisioner Drog
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=68555; -- Ka'wi the Gorger
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=68559; -- No-No
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=68560; -- Greyhoof
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=68562; -- Ti'un the Wanderer
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=68563; -- Kafi
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=68566; -- Skitterer Xi'a
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=68846; -- Kun-Lai Runt
UPDATE `creature_template` SET `mindmg`=16, `maxdmg`=28, `attackpower`=54, `dmg_multiplier`=1 WHERE `entry`=68951; -- Little Jiwon
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=68986; -- Li the Tamer
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=70167; -- Maximo
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=70168; -- Noah
UPDATE `creature_template` SET `mindmg`=10166, `maxdmg`=17285, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=70360; -- Vereesa Windrunner
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=70949; -- Jae-Sun Di Fo
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=70950; -- Hilda
UPDATE `creature_template` SET `mindmg`=9, `maxdmg`=17, `attackpower`=24, `dmg_multiplier`=1 WHERE `entry`=70951; -- Tom Wat
UPDATE `creature_template` SET `mindmg`=10166, `maxdmg`=17285, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=71082; -- Grandpa Grumplefloot
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=71988; -- Spectral Campfire
UPDATE `creature_template` SET `mindmg`=15, `maxdmg`=26, `attackpower`=50, `dmg_multiplier`=1 WHERE `entry`=72006; -- Allie
UPDATE `creature_template` SET `mindmg`=6240, `maxdmg`=10608, `attackpower`=0, `dmg_multiplier`=1 WHERE `entry`=72877; -- Ashleaf Sprite
UPDATE `creature_template` SET `mindmg`=6240, `maxdmg`=10608, `attackpower`=0, `dmg_multiplier`=2.25 WHERE `entry`=72894; -- Ordon Fire-Watcher
UPDATE `creature_template` SET `mindmg`=6240, `maxdmg`=10608, `attackpower`=0, `dmg_multiplier`=2 WHERE `entry`=73170; -- Watcher Osu
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=73329; -- Cave-In
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=73368; -- Skywisp Moth
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=73503; -- Chi-Chi, Hatchling of Chi-Ji
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=73505; -- Xu-Fu, Cub of Xuen
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=73506; -- Zao, Calfling of Niuzao
UPDATE `creature_template` SET `mindmg`=25, `maxdmg`=45, `attackpower`=86, `dmg_multiplier`=2.25 WHERE `entry`=73507; -- Yu'la, Broodling of Yu'lon
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=73542; -- Ashwing Moth
UPDATE `creature_template` SET `mindmg`=2, `maxdmg`=6, `attackpower`=10, `dmg_multiplier`=1 WHERE `entry`=73543; -- Flamering Moth
UPDATE `creature_template` SET `mindmg`=6, `maxdmg`=12, `attackpower`=17, `dmg_multiplier`=1 WHERE `entry`=73626; -- Little Tommy Newcomer
