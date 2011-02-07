/* Copyright (C) 2006 - 2009 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: boss_leviathan
SD%Complete: 
SDComment: needs vehicles
SDCategory: Ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

/*
#define SAY_AGGRO -1
#define SAY_SLAY -1
*/

#define SPELL_ENRAGE 64238

struct MANGOS_DLL_DECL boss_flameleviatanAI : public ScriptedAI
{
    boss_flameleviatanAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;    
    uint32 m_uiEnrageTimer;

    void Reset()
    {
        m_uiEnrageTimer = 540000;

        if(m_pInstance)
            m_pInstance->SetData(TYPE_LEVIATHAN, NOT_STARTED);
    }

    void KilledUnit(Unit *victim)
    {
    }

    void JustDied(Unit *victim)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_LEVIATHAN, DONE);
    }

    void DamageTaken(Unit *pDoneBy, uint32 &dmg)
    {
        dmg *= 1.5;
    }

    void Aggro(Unit* pWho)
    {
//        DoScriptText(SAY_AGGRO, m_creature);
        m_creature->SetInCombatWithZone();

        if (m_pInstance)
            m_pInstance->SetData(TYPE_LEVIATHAN, IN_PROGRESS);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
//SPELLS TODO:

        if (m_uiEnrageTimer <= diff)
        {
            DoCast(m_creature, SPELL_ENRAGE, true);
            m_uiEnrageTimer = 10000;
        }else m_uiEnrageTimer -= diff;
//
        DoMeleeAttackIfReady();

        //EnterEvadeIfOutOfCombatArea(diff);

    }

};

CreatureAI* GetAI_boss_flameleviatan(Creature* pCreature)
{
    return new boss_flameleviatanAI(pCreature);
}

void AddSC_boss_flameleviatan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_flameleviatan";
    newscript->GetAI = &GetAI_boss_flameleviatan;
    newscript->RegisterSelf();
}
