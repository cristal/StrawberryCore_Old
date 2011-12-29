/*
Navicat MySQL Data Transfer

Source Server         : local
Source Server Version : 60011
Source Host           : localhost:3306
Source Database       : world

Target Server Type    : MYSQL
Target Server Version : 60011
File Encoding         : 65001

Date: 2011-12-29 01:37:33
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `opcodes`
-- ----------------------------
DROP TABLE IF EXISTS `opcodes`;
CREATE TABLE `opcodes` (
  `OpcodeName` varchar(255) NOT NULL DEFAULT '',
  `OpcodeValue` mediumint(5) NOT NULL DEFAULT '0',
  `ClientBuild` mediumint(5) NOT NULL DEFAULT '15050',
  `StructureBuild` mediumint(5) NOT NULL,
  PRIMARY KEY (`OpcodeName`,`OpcodeValue`)
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of opcodes
-- ----------------------------
INSERT INTO opcodes VALUES ('CMSG_AUTH_SESSION', '4352', '15050', '15050');
INSERT INTO opcodes VALUES ('CMSG_REALM_SPLIT_STATE', '5760', '15050', '15050');
INSERT INTO opcodes VALUES ('CMSG_REQUEST_CHARACTER_CREATE', '10886', '15050', '15050');
INSERT INTO opcodes VALUES ('CMSG_REQUEST_CHARACTER_DELETE', '14500', '15050', '15050');
INSERT INTO opcodes VALUES ('CMSG_REQUEST_CHARACTER_ENUM', '7808', '15050', '15050');
INSERT INTO opcodes VALUES ('MSG_WOW_CONNECTION', '20311', '15050', '15050');
INSERT INTO opcodes VALUES ('SMSG_AUTH_CHALLENGE', '5923', '15050', '15050');
INSERT INTO opcodes VALUES ('SMSG_AUTH_RESPONSE', '5126', '15050', '15050');
INSERT INTO opcodes VALUES ('SMSG_CHAR_CREATE', '10882', '15050', '15050');
INSERT INTO opcodes VALUES ('SMSG_CHAR_DELETE', '10754', '15050', '15050');
INSERT INTO opcodes VALUES ('SMSG_CHAR_ENUM', '1196', '15050', '15050');
INSERT INTO opcodes VALUES ('SMSG_REALM_SPLIT_MSG', '6176', '15050', '15050');
INSERT INTO opcodes VALUES ('SMSG_UPDATE_OBJECT', '17926', '15050', '15050');
