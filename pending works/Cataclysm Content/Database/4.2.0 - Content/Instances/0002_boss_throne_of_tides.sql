UPDATE `instance_template` SET `script`='instance_throne_of_the_tides' WHERE `map`=643 LIMIT 1;
-- Lady Naz'jar
UPDATE `creature_template` SET `ScriptName`='boss_lady_nazjar' WHERE `entry`=40586;
-- Commander Ulthok
UPDATE `creature_template` SET `ScriptName`='boss_commander_ulthok' WHERE `entry`=40765;
-- Erunak Stonespeaker & Mindbender Ghur'sha
UPDATE `creature_template` SET `ScriptName`='boss_erunak_stonespeaker' WHERE `entry`=40825;
UPDATE `creature_template` SET `ScriptName`='boss_mindbender_ghursha' WHERE `entry`=40788;
-- Ozumat & Neptulon
DELETE FROM creature WHERE id = 42172;
UPDATE `creature_template` SET `ScriptName`='npc_neptulon' WHERE `entry`=40792;
UPDATE `creature_template` SET `ScriptName`='boss_ozumat' WHERE `entry`=42172;
-- Texts
REPLACE INTO `script_texts` (`npc_entry`, `entry`, `content_default`, `sound`, `type`, `language`, `emote`, `comment`) VALUES 
(40586, -1643001, 'You have interfered with our plans for the last time, mortals!', 0, 1, 0, 0, 'Lady Naz\'jar SAY_AGGRO'),
(40586, -1643002, 'Take arms, minions! Rise from the icy depths!', 0, 1, 0, 0, 'Lady Naz\'jar SAY_66_PRECENT'),
(40586, -1643003, 'Destroy these intruders! Leave them for the great dark beyond!', 0, 1, 0, 0, 'Lady Naz\'jar SAY_REPETANCE_33_PRECENT'),
(40586, -1643004, 'Ulthok... stop them...', 0, 1, 0, 0, 'Lady Naz\'jar SAY_DEATH'),
(40586, -1643005, 'Depths take you!', 0, 1, 0, 0, 'Lady Naz\'jar SAY_KILL_1'),
(40586, -1643006, 'The abyss awaits!', 0, 1, 0, 0, 'Lady Naz\'jar SAY_KILL_2'),
(40765, -1643007, 'Iilth vwah, uhn\'agth fhssh za.', 18543, 1, 0, 0, 'Commander Ulthok SAY_AGGRO'),
(40765, -1643008, 'Where one falls, many shall take its place...', 0, 5, 0, 0, 'Commander Ulthok SAY_AGGRO_WHISP'),
(40765, -1643009, 'Ywaq maq oou.', 18542, 1, 0, 0, 'Commander Ulthok SAY_DEATH'),
(40765, -1643010, 'They do not die.', 0, 5, 0, 0, 'Commander Ulthok SAY_DEATH_WHISP'),
(40788, -1643011, 'A new host must be found.', 0, 1, 0, 0, 'Mindbender Ghur\'sha SAY_PHASE_1_END_MINDBENDER'),
(40825, -1643012, 'Where one falls, many shall take its place...', 0, 1, 0, 0, 'Erunak Stonespeaker SAY_PHASE_1_END_ERUNAK'),
(40788, -1643013, 'There is only Ghur\'sha.', 0, 1, 0, 0, 'Mindbender Ghur\'sha SAY_MIND_CONTROL_1'),
(40788, -1643014, 'Who are your allies?', 0, 1, 0, 0, 'Mindbender Ghur\'sha SAY_MIND_CONTROL_2'),
(40788, -1643015, 'Give in.', 0, 1, 0, 0, 'Mindbender Ghur\'sha SAY_MIND_CONTROL_3'),
(40765, -1643016, 'Is. This. Reality.', 0, 1, 0, 0, 'Mindbender Ghur\'sha SAY_MIND_FOG'),
(40765, -1643017, 'They are outside the cycle...', 0, 1, 0, 0, 'Mindbender Ghur\'sha SAY_DEATH'),
(40765, -1643018, 'I am... weary. You must carry on without me. I shall take my leave once my strength has returned.', 0, 1, 0, 0, 'Erunak Stonespeaker SAY_WIN_ERUNAK');
