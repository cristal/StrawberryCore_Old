ALTER TABLE `creature_template`
    ADD COLUMN `type_flags2` int(19) NOT NULL DEFAULT '0' AFTER `type_flags`;
