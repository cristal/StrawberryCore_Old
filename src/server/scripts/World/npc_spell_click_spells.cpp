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

#include "ScriptPCH.h"
#include "SpellMgr.h"
#include "Player.h"
#include "Creature.h"

/*########
# npc_deffiant_troll
#########*/

enum NSCSDeffiantTroll
{
    DEFFIANT_KILL_CREDIT               = 34830,
    SPELL_LIGHTNING_VISUAL             = 56328
};

class npc_deffiant_troll : public CreatureScript
{
public:
    npc_deffiant_troll() : CreatureScript("npc_deffiant_troll") { }

    bool OnGossipHello(Player* pPlayer, Creature* pCreature)
    {
            if (pPlayer->GetQuestStatus(14069) == QUEST_STATUS_INCOMPLETE)
            {
                pCreature->CastSpell(pPlayer, 45870, true);
                pPlayer->KilledMonsterCredit(DEFFIANT_KILL_CREDIT, 0);
                pCreature->ForcedDespawn(1500);
                return true;
            }

    return false;
    }
};

void AddSC_npc_spell_click_spells()
{
    new npc_deffiant_troll;
}