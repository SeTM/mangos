/* Copyright (C) 2006 - 2011 ScriptDev2 <http://www.scriptdev2.com/>
* This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

/* ScriptData
SDName: boss_headless_horseman
SD%Complete: 0
SDComment: Place Holder
SDCategory: Scarlet Monastery
EndScriptData */

#include "precompiled.h"

enum
{
    SAY_ENTRANCE           = -1189022,
    SAY_REJOINED           = -1189023,
    SAY_BODY_DEFEAT        = -1189024,
    SAY_LOST_HEAD          = -1189025,
    SAY_CONFLAGRATION      = -1189026,
    SAY_SPROUTING_PUMPKINS = -1189027,
    SAY_SLAY               = -1189028,
    SAY_DEATH              = -1189029,

    EMOTE_LAUGH            = -1189030,

    SAY_PLAYER1            = -1189031,
    SAY_PLAYER2            = -1189032,
    SAY_PLAYER3            = -1189033,
    SAY_PLAYER4            = -1189034
};

struct MANGOS_DLL_DECL boss_headless_horsemanAI : public ScriptedAI
{
    boss_headless_horsemanAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset()
    {
        m_stage = 0;
        m_confuse = false;
        m_conflagate_timer = 5000;
        m_remove_whirl = 5000;
        m_creature->Mount(22653);
    }

    uint32 m_stage;
    uint32 m_conflagate_timer;
    uint32 m_remove_whirl;
    bool m_confuse;

    void Aggro(Unit* pWho)
    {
        m_creature->SetInCombatWithZone();
        DoCast(m_creature,43873);
   }

    void KilledUnit(Unit* pVictim)
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

    void DamageTaken(Unit *done_by, uint32 &damage)
    {
        if (m_confuse && damage > m_creature->GetHealth())                // Don't let ourselves be slain before we do our death speech
        {
            m_creature->ModifyHealth(damage);
            damage = 0;
        }
    }

    void JustDied(Unit* pKiller)
    {
        DoScriptText(SAY_DEATH, m_creature);
    }

    void HeadKill()
    {
        m_confuse = false;
        m_creature->SetHealth(m_creature->GetMaxHealth());
        m_creature->RemoveAurasDueToSpell(43105);
        m_creature->RemoveAurasDueToSpell(42403);
        m_creature->RemoveAurasDueToSpell(42556);
        if(Creature* Head = GetClosestCreatureWithEntry(m_creature,23775,200))
            Head->ForcedDespawn();
        DoCast(m_creature,43116,true);
        m_creature->Mount(22653);
        m_creature->GetMotionMaster()->Clear();
        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->HasAura(42413))
            DoCast(m_creature,42413,true);

        if (m_creature->HasAura(43116))
        {
            if (m_remove_whirl <= uiDiff)
            {
                m_creature->RemoveAurasDueToSpell(43116);
                m_remove_whirl = 5000;
            }else m_remove_whirl -= uiDiff;
        }

        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_confuse && m_creature->GetHealth() == m_creature->GetMaxHealth())
            HeadKill();

        if (!m_confuse && m_stage != 3 && m_creature->GetHealth() <= (m_creature->GetMaxHealth() * 0.05f))
        {
            m_confuse = true;
            m_stage++;
            float x,y,z = 0.0;
            m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 20, x, y, z);
            if (Creature* Head = m_creature->SummonCreature(23775,x,y,z,0,TEMPSUMMON_CORPSE_DESPAWN,600000))
            {
                switch (m_stage)
                {
                case 2: Head->SetHealth(Head->GetMaxHealth()*0.67f); break;
                case 3: Head->SetHealth(Head->GetMaxHealth()*0.33f); break;
                default: Head->SetHealth(Head->GetMaxHealth()); break;
                }
            }
            DoStopAttack();
            m_creature->GetMotionMaster()->Clear(false);     // Stop he from moving as well
            m_creature->GetMotionMaster()->MoveIdle();
            DoCast(m_creature,43105,true);
            DoCast(m_creature,42403,true);
            DoCast(m_creature,42556,true);
            m_creature->Unmount();
        }

        if (!m_confuse)
        {
            // need right spell and summons
            if (m_stage > 0)
            {
                if (m_conflagate_timer <= uiDiff)
                {
                    DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1),31340);
                    m_conflagate_timer = 5000;
                }else m_conflagate_timer -= uiDiff;
            }
        }else
        {
            if (!m_creature->HasAura(43105))
                DoCast(m_creature,43105,true);            
            if (!m_creature->HasAura(42403))
                DoCast(m_creature,42403,true);
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_head_horsemanAI : public ScriptedAI
{
    mob_head_horsemanAI(Creature* pCreature) : ScriptedAI(pCreature) {Reset();}

    void Reset()
    {
        m_movetimer = 0;
        if(Creature* Horseman = GetClosestCreatureWithEntry(m_creature,23682,200))
        {
            m_stage = ((boss_headless_horsemanAI*)Horseman->AI())->m_stage;
        }else m_stage = 1;
    }

    uint32 m_movetimer;
    uint32 m_stage;

    void MoveInLineOfSight(Unit *who)
    {
        return;
    }

    void Aggro(Unit* who)
    {
        DoStopAttack();
    }

    void JustDied(Unit* who)
    {
        if(Creature* Horseman = GetClosestCreatureWithEntry(m_creature,23682,200))
            ((boss_headless_horsemanAI*)Horseman->AI())->HeadKill();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->HasAura(42413))
            DoCast(m_creature,42413,true);

        if (m_movetimer <= uiDiff)
        {
            float x,y,z = 0.0;
            m_creature->GetRandomPoint(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ(), 15, x, y, z);
            m_creature->GetMotionMaster()->MovePoint(0, x, y, z);
            m_movetimer = 5000;
        }else m_movetimer -= uiDiff;

        if (m_stage == 1 && m_creature->GetHealth() <= (m_creature->GetMaxHealth() * 0.67f))
        {             
            if(Creature* Horseman = GetClosestCreatureWithEntry(m_creature,23682,200))
                ((boss_headless_horsemanAI*)Horseman->AI())->HeadKill();
            m_creature->ForcedDespawn();
        }
        if (m_stage == 2 && m_creature->GetHealth() <= (m_creature->GetMaxHealth() * 0.33f))
        {             
            if(Creature* Horseman = GetClosestCreatureWithEntry(m_creature,23682,200))
                ((boss_headless_horsemanAI*)Horseman->AI())->HeadKill();
            m_creature->ForcedDespawn();
        }
    }
};

CreatureAI* GetAI_mob_head_horseman(Creature* pCreature)
{
    return new mob_head_horsemanAI(pCreature);
}

CreatureAI* GetAI_boss_headless_horseman(Creature* pCreature)
{
    return new boss_headless_horsemanAI(pCreature);
}

void AddSC_boss_headless_horseman()
{
    Script* NewScript;

    NewScript = new Script;
    NewScript->Name = "boss_headless_horseman";
    NewScript->GetAI = GetAI_boss_headless_horseman;
    NewScript->RegisterSelf();

    NewScript = new Script;
    NewScript->Name = "mob_head_horseman";
    NewScript->GetAI = GetAI_mob_head_horseman;
    NewScript->RegisterSelf();
}
