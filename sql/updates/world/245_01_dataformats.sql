/*
Navicat MySQL Data Transfer

Source Server         : local
Source Server Version : 60011
Source Host           : localhost:3306
Source Database       : world

Target Server Type    : MYSQL
Target Server Version : 60011
File Encoding         : 65001

Date: 2011-12-29 02:19:14
*/

SET FOREIGN_KEY_CHECKS=0;
-- ----------------------------
-- Table structure for `dataformats`
-- ----------------------------
DROP TABLE IF EXISTS `dataformats`;
CREATE TABLE `dataformats` (
  `version` smallint(5) unsigned NOT NULL,
  `name` varchar(64) NOT NULL DEFAULT '',
  `format` longtext NOT NULL,
  PRIMARY KEY (`name`)
) ENGINE=MyISAM DEFAULT CHARSET=latin1;

-- ----------------------------
-- Records of dataformats
-- ----------------------------
INSERT INTO dataformats VALUES ('15050', 'Achievementfmt', 'niixsxiixixxii');
INSERT INTO dataformats VALUES ('15050', 'AchievementCriteriafmt', 'niiiiiiiixsiiiiixxxxxxx');
INSERT INTO dataformats VALUES ('15050', 'AreaTableEntryfmt', 'iiinixxxxxisixxxxxxxxxxxxx');
INSERT INTO dataformats VALUES ('15050', 'AreaGroupEntryfmt', 'niiiiiii');
INSERT INTO dataformats VALUES ('15050', 'AreaPOIEntryfmt', 'niiiiiiiiiiiffixixxixx');
INSERT INTO dataformats VALUES ('15050', 'AreaTriggerEntryfmt', 'nifffxxxfffff');
INSERT INTO dataformats VALUES ('15050', 'ArmorLocationfmt', 'nfffff');
INSERT INTO dataformats VALUES ('15050', 'AuctionHouseEntryfmt', 'niiix');
INSERT INTO dataformats VALUES ('15050', 'BankBagSlotPricesEntryfmt', 'ni');
INSERT INTO dataformats VALUES ('15050', 'BarberShopStyleEntryfmt', 'nixxxiii');
INSERT INTO dataformats VALUES ('15050', 'BattlemasterListEntryfmt', 'niiiiiiiiixsiiiiiiii');
INSERT INTO dataformats VALUES ('15050', 'CharStartOutfitEntryfmt', 'diiiiiiiiiiiiiiiiiiiiiiiiixxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx');
INSERT INTO dataformats VALUES ('15050', 'CharTitlesEntryfmt', 'nxsxix');
INSERT INTO dataformats VALUES ('15050', 'ChatChannelsEntryfmt', 'nixsx');
INSERT INTO dataformats VALUES ('15050', 'ChrClassesEntryfmt', 'nixsxxxixiixxx');
INSERT INTO dataformats VALUES ('15050', 'ChrRacesEntryfmt', 'nxixiixixxxxixsxxxxxixxx');
INSERT INTO dataformats VALUES ('15050', 'CinematicSequencesEntryfmt', 'nxxxxxxxxx');
INSERT INTO dataformats VALUES ('15050', 'CreatureDisplayInfofmt', 'nxxxfxxxxxxxxxxxx');
INSERT INTO dataformats VALUES ('15050', 'CreatureFamilyfmt', 'nfifiiiiixsx');
INSERT INTO dataformats VALUES ('15050', 'CreatureSpellDatafmt', 'niiiixxxx');
INSERT INTO dataformats VALUES ('15050', 'CreatureTypefmt', 'nxx');
INSERT INTO dataformats VALUES ('15050', 'CurrencyTypesfmt', 'nxxxxxxiixx');
INSERT INTO dataformats VALUES ('15050', 'DestructibleModelDatafmt', 'ixxixxxixxxixxxixxxxxxxx');
INSERT INTO dataformats VALUES ('15050', 'DungeonEncounterfmt', 'iiixisxx');
INSERT INTO dataformats VALUES ('15050', 'DurabilityCostsfmt', 'niiiiiiiiiiiiiiiiiiiiiiiiiiiii');
INSERT INTO dataformats VALUES ('15050', 'DurabilityQualityfmt', 'nf');
INSERT INTO dataformats VALUES ('15050', 'EmotesEntryfmt', 'nxxiiixx');
INSERT INTO dataformats VALUES ('15050', 'EmotesTextEntryfmt', 'nxixxxxxxxxxxxxxxxx');
INSERT INTO dataformats VALUES ('15050', 'FactionEntryfmt', 'niiiiiiiiiiiiiiiiiiffixsxx');
INSERT INTO dataformats VALUES ('15050', 'FactionTemplateEntryfmt', 'niiiiiiiiiiiii');
INSERT INTO dataformats VALUES ('15050', 'GameObjectDisplayInfofmt', 'nxxxxxxxxxxxffffffxxx');
INSERT INTO dataformats VALUES ('15050', 'GemPropertiesEntryfmt', 'nixxix');
INSERT INTO dataformats VALUES ('15050', 'GlyphPropertiesfmt', 'niii');
INSERT INTO dataformats VALUES ('15050', 'GlyphSlotfmt', 'nii');
INSERT INTO dataformats VALUES ('15050', 'GtBarberShopCostBasefmt', 'xf');
INSERT INTO dataformats VALUES ('15050', 'GtCombatRatingsfmt', 'xf');
INSERT INTO dataformats VALUES ('15050', 'GtChanceToMeleeCritBasefmt', 'xf');
INSERT INTO dataformats VALUES ('15050', 'GtChanceToMeleeCritfmt', 'xf');
INSERT INTO dataformats VALUES ('15050', 'GtChanceToSpellCritBasefmt', 'xf');
INSERT INTO dataformats VALUES ('15050', 'GtChanceToSpellCritfmt', 'xf');
INSERT INTO dataformats VALUES ('15050', 'GtOCTClassCombatRatingScalarfmt', 'df');
INSERT INTO dataformats VALUES ('15050', 'GtOCTHpPerStaminafmt', 'xf');
INSERT INTO dataformats VALUES ('15050', 'GtRegenMPPerSptfmt', 'xf');
INSERT INTO dataformats VALUES ('15050', 'GtSpellScalingfmt', 'nf');
INSERT INTO dataformats VALUES ('15050', 'Holidaysfmt', 'nxiixxxxxxxxixxxxxxxxxxxxxxxixxxxxxxxxxxiixxxxxxxxxsxix');
INSERT INTO dataformats VALUES ('15050', 'ItemArmorQualityfmt', 'nfffffffi');
INSERT INTO dataformats VALUES ('15050', 'ItemArmorShieldfmt', 'nifffffff');
INSERT INTO dataformats VALUES ('15050', 'ItemArmorTotalfmt', 'niffff');
INSERT INTO dataformats VALUES ('15050', 'ItemBagFamilyfmt', 'nx');
INSERT INTO dataformats VALUES ('15050', 'ItemDamagefmt', 'nfffffffi');
INSERT INTO dataformats VALUES ('15050', 'ItemLimitCategoryEntryfmt', 'nxii');
INSERT INTO dataformats VALUES ('15050', 'ItemRandomPropertiesfmt', 'nxiiiiis');
INSERT INTO dataformats VALUES ('15050', 'ItemRandomSuffixfmt', 'nsxiiiiiiiiii');
INSERT INTO dataformats VALUES ('15050', 'ItemSetEntryfmt', 'dsiiiiiiiiiixxxxxxxiiiiiiiiiiiiiiiiii');
INSERT INTO dataformats VALUES ('15050', 'LFGDungeonEntryfmt', 'nxiiiiiiixixxixixxxxx');
INSERT INTO dataformats VALUES ('15050', 'LockEntryfmt', 'niiiiiiiiiiiiiiiiiiiiiiiixxxxxxxx');
INSERT INTO dataformats VALUES ('15050', 'MailTemplateEntryfmt', 'nxs');
INSERT INTO dataformats VALUES ('15050', 'MapEntryfmt', 'nxixxxsixxixiffxiixx');
INSERT INTO dataformats VALUES ('15050', 'MapDifficultyEntryfmt', 'diisiix');
INSERT INTO dataformats VALUES ('15050', 'MountCapabilityfmt', 'niixxiii');
INSERT INTO dataformats VALUES ('15050', 'MountTypefmt', 'niiiiiiiiiiiiiiiiixxxxxxx');
INSERT INTO dataformats VALUES ('15050', 'MovieEntryfmt', 'nxxx');
INSERT INTO dataformats VALUES ('15050', 'OverrideSpellDatafmt', 'niiiiiiiiiixx');
INSERT INTO dataformats VALUES ('15050', 'QuestSortEntryfmt', 'nx');
INSERT INTO dataformats VALUES ('15050', 'QuestXPfmt', 'niiiiiiiiii');
INSERT INTO dataformats VALUES ('15050', 'QuestFactionRewardfmt', 'niiiiiiiiii');
INSERT INTO dataformats VALUES ('15050', 'PvPDifficultyfmt', 'diiiii');
INSERT INTO dataformats VALUES ('15050', 'RandomPropertiesPointsfmt', 'niiiiiiiiiiiiiii');
INSERT INTO dataformats VALUES ('15050', 'ScalingStatDistributionfmt', 'niiiiiiiiiiiiiiiiiiiixi');
INSERT INTO dataformats VALUES ('15050', 'ScalingStatValuesfmt', 'iniiiiiiiiiiiiiiiiiiiixxxxxxxxxxxxxxxxxxxxxxxxx');
INSERT INTO dataformats VALUES ('15050', 'SkillLinefmt', 'nisxixi');
INSERT INTO dataformats VALUES ('15050', 'SkillLineAbilityfmt', 'niiiixxiiiiixx');
INSERT INTO dataformats VALUES ('15050', 'SoundEntriesfmt', 'nxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx');
INSERT INTO dataformats VALUES ('15050', 'SpellCastTimefmt', 'nixx');
INSERT INTO dataformats VALUES ('15050', 'SpellDifficultyfmt', 'niiii');
INSERT INTO dataformats VALUES ('15050', 'SpellDurationfmt', 'niii');
INSERT INTO dataformats VALUES ('15050', 'SpellEntryfmt', 'niiiiiiiiixxiiiifiiiissxxiixxixiiiiiiixiiiiiiiix');
INSERT INTO dataformats VALUES ('15050', 'True_SpellEntryfmt', 'iiiiiiiiiixiiiifiiiisixxiixxixiiiiiiixiiiiixiixiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiifffiiiiiiiiifffiiiiiiiiiiiiiiiiiifffiiifffiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiffffffffffiiiiiiiiiii');
INSERT INTO dataformats VALUES ('15050', 'SpellAuraOptionsEntryfmt', 'niiii');
INSERT INTO dataformats VALUES ('15050', 'SpellAuraRestrictionsEntryfmt', 'niiiiiiii');
INSERT INTO dataformats VALUES ('15050', 'SpellCastingRequirementsEntryfmt', 'nixxixi');
INSERT INTO dataformats VALUES ('15050', 'SpellCategoriesEntryfmt', 'niiiiii');
INSERT INTO dataformats VALUES ('15050', 'SpellClassOptionsEntryfmt', 'nxiiiix');
INSERT INTO dataformats VALUES ('15050', 'SpellCooldownsEntryfmt', 'niii');
INSERT INTO dataformats VALUES ('15050', 'SpellEffectEntryfmt', 'nifiiiffiiiiiifixfiiiiiiiix');
INSERT INTO dataformats VALUES ('15050', 'SpellEquippedItemsEntryfmt', 'niii');
INSERT INTO dataformats VALUES ('15050', 'SpellInterruptsEntryfmt', 'nixixi');
INSERT INTO dataformats VALUES ('15050', 'SpellLevelsEntryfmt', 'niii');
INSERT INTO dataformats VALUES ('15050', 'SpellPowerEntryfmt', 'niiiixxx');
INSERT INTO dataformats VALUES ('15050', 'SpellReagentsEntryfmt', 'niiiiiiiiiiiiiiii');
INSERT INTO dataformats VALUES ('15050', 'SpellScalingEntryfmt', 'niiiiffffffffffi');
INSERT INTO dataformats VALUES ('15050', 'SpellShapeshiftEntryfmt', 'nixixx');
INSERT INTO dataformats VALUES ('15050', 'SpellTargetRestrictionsEntryfmt', 'dfiiii');
INSERT INTO dataformats VALUES ('15050', 'SpellTotemsEntryfmt', 'niiii');
INSERT INTO dataformats VALUES ('15050', 'SpellFocusObjectfmt', 'nx');
INSERT INTO dataformats VALUES ('15050', 'SpellItemEnchantmentfmt', 'nxiiiiiixxxiiisiiiiiiix');
INSERT INTO dataformats VALUES ('15050', 'SpellItemEnchantmentConditionfmt', 'nbbbbbxxxxxbbbbbbbbbbiiiiiXXXXX');
INSERT INTO dataformats VALUES ('15050', 'SpellRadiusfmt', 'nfxf');
INSERT INTO dataformats VALUES ('15050', 'SpellRangefmt', 'nffffixx');
INSERT INTO dataformats VALUES ('15050', 'SpellRuneCostfmt', 'niiii');
INSERT INTO dataformats VALUES ('15050', 'SpellShapeshiftFormfmt', 'nxxiixiiixxiiiiiiiixx');
INSERT INTO dataformats VALUES ('15050', 'SummonPropertiesfmt', 'niiiii');
INSERT INTO dataformats VALUES ('15050', 'GuildPerksfmt', 'nii');
INSERT INTO dataformats VALUES ('15050', 'TalentEntryfmt', 'niiiiiiiiixxixxxxxx');
INSERT INTO dataformats VALUES ('15050', 'TalentTabEntryfmt', 'nxxiiixxxii');
INSERT INTO dataformats VALUES ('15050', 'TalentTreePrimarySpellsfmt', 'niix');
INSERT INTO dataformats VALUES ('15050', 'TaxiNodesEntryfmt', 'nifffsiixxx');
INSERT INTO dataformats VALUES ('15050', 'TaxiPathEntryfmt', 'niii');
INSERT INTO dataformats VALUES ('15050', 'TaxiPathNodeEntryfmt', 'diiifffiiii');
INSERT INTO dataformats VALUES ('15050', 'TotemCategoryEntryfmt', 'nxii');
INSERT INTO dataformats VALUES ('15050', 'VehicleEntryfmt', 'niffffiiiiiiiifffffffffffffffssssfifiixx');
INSERT INTO dataformats VALUES ('15050', 'VehicleSeatEntryfmt', 'niiffffffffffiiiiiifffffffiiifffiiiiiiiffiiiiixxxxxxxxxxxxxxxxxxxx');
INSERT INTO dataformats VALUES ('15050', 'WMOAreaTableEntryfmt', 'niiixxxxxiixxxx');
INSERT INTO dataformats VALUES ('15050', 'WorldMapAreaEntryfmt', 'xinxffffixxxxx');
INSERT INTO dataformats VALUES ('15050', 'WorldMapOverlayEntryfmt', 'nxiiiixxxxxxxxx');
INSERT INTO dataformats VALUES ('15050', 'WorldSafeLocsEntryfmt', 'nifffx');
INSERT INTO dataformats VALUES ('15050', 'PhaseEntryfmt', 'nsi');
INSERT INTO dataformats VALUES ('15050', 'Itemfmt', 'niiiiiii');
INSERT INTO dataformats VALUES ('15050', 'ItemCurrencyCostfmt', 'di');
INSERT INTO dataformats VALUES ('15050', 'ItemExtendedCostEntryfmt', 'nxxiiiiiiiiiiiixiiiiiiiiiixxxxx');
INSERT INTO dataformats VALUES ('15050', 'ItemReforgefmt', 'nifif');
INSERT INTO dataformats VALUES ('15050', 'ChrClassesXPowerTypesfmt', 'nii');
