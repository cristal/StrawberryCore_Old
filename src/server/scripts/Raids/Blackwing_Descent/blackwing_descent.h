/*
 * Copyright (C) 2011 Project Voragine <http://www.projectvoragine.com/>
 *
 * Copyright (C) 2011 TrilliumEMU <http://www.trilliumemu.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef BLACKWING_DESCENT_H
#define BLACKWING_DESCENT_H

#define BWDScriptName "instance_blackwing_descent"

enum SharedSpells
{
    SPELL_BERSERK1 = 26662,
    SPELL_BERSERK2 = 64238,
};

enum Data
{
    /* Encounters */
    DATA_MAGMAW                  = 0,
    DATA_OMNOTRON_DEFENSE_SYSTEM = 1,
    DATA_MALORIAK                = 2,
    DATA_ATRAMEDES               = 3,
    DATA_CHIMAERON               = 4,
    DATA_NEFARIAN                = 5,

    /* Additional Entities */
    DATA_ARCANOTRON_GUID         = 6,
    DATA_ELECTRON_GUID           = 7,
    DATA_MAGMATRON_GUID          = 8,
    DATA_TOXITRON_GUID           = 9,
    DATA_ONYXIA_GUID             = 10,
    DATA_BILE_O_TRON_800         = 11,
};

enum Creatures
{
    NPC_MAGMAW               = 41570,
    NPC_ARCANOTRON           = 42166,
    NPC_ELECTRON             = 42179,
    NPC_MAGMATRON            = 42178,
    NPC_TOXITRON             = 42180,
    NPC_MALORIAK             = 41378,
    NPC_ATRAMEDES            = 41442,
    NPC_CHIMAERON            = 43296,
    NPC_BILE_O_TRON_800      = 44418,
    NPC_NEFARIAN             = 41376,
    NPC_ONYXIA               = 41270,
    NPC_LORD_VICTOR_NEFARIAN = 41379,
};

enum GameObjects
{
    GO_ELEVATOR_HALL = 203716,
};

#endif      