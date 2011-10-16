/*
 * Copyright (C) 2008-2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010-2011 Strawberry-Pr0jcts <http://www.strawberry-pr0jcts.com/>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 */

#ifndef DEF_GRIMBATOL_H
#define DEF_GRIMBATOL_H

enum Npcs
{
    // General Umbriss
    BOSS_GENERAL_UMBRISS              = 39625,
    
    // Forgemaster Throngus
    BOSS_FORGEMASTER_THRONGUS         = 40177,
    
    // Drahga Shadowburner
    BOSS_DRAHGA_SHADOWBURNER          = 40319,
    
    // Erudax
    BOSS_ERUDAX                       = 40484,
    
};

enum Data
{
    DATA_GENERAL_UMBRISS_EVENT         = 0,
    DATA_FORGEMASTER_THRONGUS_EVENT    = 1,
    DATA_DRAHGA_SHADOWBURNER_EVENT     = 2,
    DATA_ERUDAX_EVENT                  = 3,
};

#define MAX_ENCOUNTER 4

enum Data64
{
    DATA_GENERAL_UMBRISS,
    DATA_FORGEMASTER_THRONGUS,
    DATA_DRAHGA_SHADOWBURNER,
    DATA_ERUDAX,
};

enum Achievements
{
    ACHIEV_UMBRAGE_FOR_UMBRISS      = 5297,
};

#endif
