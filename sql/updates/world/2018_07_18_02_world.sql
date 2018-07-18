-- Add Language Common to all Alliance
DELETE FROM playercreateinfo_spell WHERE spell=668;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|1|4|8|64|1024|2097152|16777216, classmask|1|2|4|8|16|32|64|128|256|512|1024, 668, 'Language Common');
-- Add Language Orcish to all Horde
DELETE FROM playercreateinfo_spell WHERE spell=669;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|2|16|32|128|256|512|33554432, classmask|1|2|4|8|16|32|64|128|256|512|1024, 669, 'Language Orcish');
-- Add Language Language Dwarven to Dwarfs
DELETE FROM playercreateinfo_spell WHERE spell=672;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|4, classmask|1|2|4|8|16|32|64|128|256|512, 672, 'Language Dwarven');
-- Add Languages (Racial Passive) Common, Dwarven
DELETE FROM playercreateinfo_spell WHERE spell=79739;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|4, classmask|1|2|4|8|16|32|64|128|256|512, 79739, 'Languages (Racial Passive)');
-- Add Language Language Darnassian to Night Elves
DELETE FROM playercreateinfo_spell WHERE spell=671;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|8, classmask|1|4|8|16|32|128|512|1024, 671, 'Language Darnassian');
-- Add Languages (Racial Passive) Common, Darnassian
DELETE FROM playercreateinfo_spell WHERE spell=76252;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|8, classmask|1|4|8|16|32|128|512|1024, 76252, 'Languages (Racial Passive)');
-- Add Language Gutterspeak to Undeads
DELETE FROM playercreateinfo_spell WHERE spell=17737;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|16, classmask|1|4|8|16|32|512, 17737, 'Language Gutterspeak');
-- Add Languages (Racial Passive) Orcish, Forsaken
DELETE FROM playercreateinfo_spell WHERE spell=79747;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|16, classmask|1|4|8|16|32|512, 79747, 'Languages (Racial Passive)');
-- Add Language Taurahe to Taurens
DELETE FROM playercreateinfo_spell WHERE spell=670;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|32, classmask|1|2|4|16|32|64|512|1024, 670, 'Language Taurahe');
-- Add Languages (Racial Passive) Orcish, Taurahe
DELETE FROM playercreateinfo_spell WHERE spell=79746;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|32, classmask|1|2|4|16|32|64|512|1024, 79746, 'Languages (Racial Passive)');
-- Add Language Gnomish to Gnomes
DELETE FROM playercreateinfo_spell WHERE spell=7340;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|64, classmask|1|8|16|32|128|256|512, 7340, 'Language Gnomish');
-- Add Languages (Racial Passive) Common, Gnomish
DELETE FROM playercreateinfo_spell WHERE spell=79740;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|64, classmask|1|8|16|32|128|256|512, 79740, 'Languages (Racial Passive)');
-- Add Language Troll to Trolls
DELETE FROM playercreateinfo_spell WHERE spell=7341;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|128, classmask|1|4|8|16|32|64|128|256|512|1024, 7341, 'Language Troll');
-- Add Languages (Racial Passive) Orcish, Zandali
DELETE FROM playercreateinfo_spell WHERE spell=79744;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|128, classmask|1|4|8|16|32|64|128|256|512|1024, 79744, 'Languages (Racial Passive)');
-- Add Language Goblin to Goblins
DELETE FROM playercreateinfo_spell WHERE spell=69269;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|256, classmask|1|4|8|16|32|64|128|256, 69269, 'Language Goblin');
-- Add Languages (Racial Passive) Orcish, Goblin
DELETE FROM playercreateinfo_spell WHERE spell=79749;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|256, classmask|1|4|8|16|32|64|128|256, 79749, 'Languages (Racial Passive)');
-- Add Language Thalassian to Blood Elves
DELETE FROM playercreateinfo_spell WHERE spell=813;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|512, classmask|1|2|4|8|16|32|128|256|512, 813, 'Language Thalassian');
-- Add Languages (Racial Passive) Orcish, Thalassian
DELETE FROM playercreateinfo_spell WHERE spell=79748;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|512, classmask|1|2|4|8|16|32|128|256|512, 79748, 'Languages (Racial Passive)');
-- Add Language Draenei to Draenei
DELETE FROM playercreateinfo_spell WHERE spell=29932;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|1024, classmask|1|2|4|16|32|64|128|512, 29932, 'Language Draenei');
-- Add Languages (Racial Passive) Common, Draenei
DELETE FROM playercreateinfo_spell WHERE spell=79741;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|1024, classmask|1|2|4|16|32|64|128|512, 79741, 'Languages (Racial Passive)');
-- Add Language Gilnean to Worgens
DELETE FROM playercreateinfo_spell WHERE spell=69270;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|2097152, classmask|1|4|8|16|32|128|256|1024, 69270, 'Language Gilnean');
-- Add Languages (Racial Passive) Common
DELETE FROM playercreateinfo_spell WHERE spell=79742;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|2097152, classmask|1|4|8|16|32|128|256|1024, 79742, 'Languages (Racial Passive)');
-- Add Language Pandaren Neutral to Neutral Pandaren
DELETE FROM playercreateinfo_spell WHERE spell=108127;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|8388608, classmask|1|4|8|16|64|128|512, 108127, 'Language Pandaren Neutral');
-- Add Languages (Racial Passive) Pandaren
DELETE FROM playercreateinfo_spell WHERE spell=131701;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|8388608, classmask|1|4|8|16|64|128|512, 131701, 'Languages (Racial Passive)');
-- Add Language Pandaren Alliance to Alliance Pandaren
DELETE FROM playercreateinfo_spell WHERE spell=108130;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|16777216, classmask|1|4|8|16|64|128|512, 108130, 'Language Pandaren Alliance');
-- Add Languages (Racial Passive) Pandaren, Common
DELETE FROM playercreateinfo_spell WHERE spell=143368;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|16777216, classmask|1|4|8|16|64|128|512, 143368, 'Languages (Racial Passive)');
-- Add Language Pandaren Horde to Horde Pandaren
DELETE FROM playercreateinfo_spell WHERE spell=108131;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|33554432, classmask|1|4|8|16|64|128|512, 108131, 'Language Pandaren Horde');
-- Add Languages (Racial Passive) Pandaren, Orcish
DELETE FROM playercreateinfo_spell WHERE spell=143369;
INSERT INTO playercreateinfo_spell (racemask, classmask, Spell, Note) VALUES
(racemask|33554432, classmask|1|4|8|16|64|128|512, 143369, 'Languages (Racial Passive)');
