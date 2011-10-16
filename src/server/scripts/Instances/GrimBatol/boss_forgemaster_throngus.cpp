/*
 * Copyright (C) 2008-2011 Trinity <http://www.trinitycore.org/>
 *
 * Copyright (C) 2010-2011 Strawberry Project <http://www.strawberry-pr0jcts.com/>
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

#include "grimbatol.h"
#include "ScriptPCH.h"

enum Texts
{
    SAY_AGGRO         = 1,
    SAY_DUAL_SWORDS,
    SAY_MAZE,
    SAY_SHIELD,
    SAY_DEATH,
};

enum Spells
{
    SPELL_MIGHTY_STOMP      = 74984,
    SPELL_PICK_WEAPON       = 75000,
    SPELL_ENCUMBERED        = 75007, // Only maze
    SPELL_IMPALING_SLAM     = 90756, // Only maze
    SPELL_DISORIENTING_ROAR = 74976, // Only dual-blades
    
};
class boss_forgemaster_throngus : public CreatureScript
{
    public:
        boss_forgemaster_throngus() : CreatureScript("boss_forgemaster_throngus") { }

        struct boss_forgemaster_throngusAI : public BossAI
        {
            boss_forgemaster_throngusAI(Creature* creature) : BossAI(creature, DATA_FORGEMASTER_THRONGUS)
            {
                
            }

            void Reset()
            {
                _Reset();
                
            }

            void EnterCombat(Unit* who)
            {
                me->setActive(true);
                Talk(SAY_AGGRO);
            }
            
            void JustDied(Unit* killer)
            {
                _JustDied();
                Talk(SAY_DEATH);
            }

            void JustReachedHome()
            {
                _JustReachedHome();
            }

            void KilledUnit(Unit* victim)
            {
                
            }

            void EnterEvadeMode()
            {
                
            }

            void UpdateAI(const uint32 diff)
            {
                if (!UpdateVictim())
                    return;

                events.Update(diff);

                /*while (uint32 eventId = events.ExecuteEvent())
                {
                    switch (eventId)
                    {
                        // ToDo: Add event ids
                    }
                }*/

                DoMeleeAttackIfReady();
            }
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new boss_forgemaster_throngusAI(creature);
        }
};

void AddSC_forgemaster_throngus()
{
    new boss_forgemaster_throngus();
}