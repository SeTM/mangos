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
SDName: Boss_Noth
SD%Complete: 40
SDComment: Missing Balcony stage
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "def_naxxramas.h"
enum
{
    SAY_AGGRO1                          = -1533075,
    SAY_AGGRO2                          = -1533076,
    SAY_AGGRO3                          = -1533077,
    SAY_SUMMON                          = -1533078,
    SAY_SLAY1                           = -1533079,
    SAY_SLAY2                           = -1533080,
    SAY_DEATH                           = -1533081,

    SPELL_BLINK                         = 29211,            //29208, 29209 and 29210 too
    SPELL_CRIPPLE                       = 29212,
    SPELL_CRIPPLE_H                     = 54814,
    SPELL_CURSE_PLAGUEBRINGER           = 29213,
    SPELL_CURSE_PLAGUEBRINGER_H         = 54835,
    SPELL_BERSERK                       = 64238,

    SPELL_SUMMON_CHAMPION_AND_CONSTRUCT = 29240,
    SPELL_SUMMON_GUARDIAN_AND_CONSTRUCT = 29269,

    NPC_PLAGUED_WARRIOR                 = 16984,
    NPC_PLAGUED_CHAMPION                = 16983,
    NPC_PLAGUED_GUARDIANS               = 16981,
};

uint32 m_auiSpellSummonPlaguedWarrior[]=
{
    29247, 29248, 29249
};

uint32 m_auiSpellSummonPlaguedChampion[]=
{
    29217, 29224, 29225, 29227, 29238, 29255, 29257, 29258, 29262, 29267
};

uint32 m_auiSpellSummonPlaguedGuardian[]=
{
    29226, 29239, 29256, 29268
};

float SpawnLocations[4][4]=
{
    {2646.818, -3462.727, 263.412, 5.280},
    {2670.403, -3461.881, 262.879, 4.800},
    {2704.462, -3460.483, 262.884, 4.338},
    {2725.451, -3463.500, 263.295, 3.983},
};

// Teleport position of Noth on his balcony
#define TELE_X 2631.370
#define TELE_Y -3529.680
#define TELE_Z 274.040
#define TELE_O 6.277

// IMPORTANT: BALCONY TELEPORT NOT ADDED YET! WILL BE ADDED SOON!
// Dev note 26.12.2008: When is soon? :)

struct MANGOS_DLL_DECL boss_nothAI : public ScriptedAI
{
    boss_nothAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsHeroicMode = pCreature->GetMap()->IsHeroic();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsHeroicMode;

    uint32 Blink_Timer;
    uint32 Curse_Timer;
    uint32 Summon_Timer;
    uint32 Balcony_Timer;
    uint32 Ground_Timer;
    uint32 Wave1_Timer;
    uint32 m_Stage;
    uint32 Stage;
    uint32 Creature_Summon;
    uint8 max;

    void Reset()
    {
        max = 0;
        Stage = 1;
        m_Stage = 1;
        Blink_Timer = 30000;
        Curse_Timer = 4000;
        Summon_Timer = 20000;
        Balcony_Timer = 90000;
        Ground_Timer = 70000;
        Wave1_Timer = 10000;
        Creature_Summon = 0;
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);

        if(m_pInstance && m_creature->isAlive())
            m_pInstance->SetData(ENCOUNT_NOTH, NOT_STARTED);
    }

    void Aggro(Unit *who)
    {
        switch (rand()%3)
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }
        if(m_pInstance)
            m_pInstance->SetData(ENCOUNT_NOTH, IN_PROGRESS);
    }

    void JustSummoned(Creature* summoned)
    {
        if (!summoned)
            return;
        if (Unit* target = SelectUnit(SELECT_TARGET_RANDOM,0))
            summoned->AddThreat(target,0.0f);
    }

    void KilledUnit(Unit* victim)
    {
        switch (rand()%2)
        {
            case 0: DoScriptText(SAY_SLAY1, m_creature); break;
            case 1: DoScriptText(SAY_SLAY2, m_creature); break;
        }
    }

    void JustDied(Unit* Killer)
    {
        DoScriptText(SAY_DEATH, m_creature);
        if(m_pInstance)
            m_pInstance->SetData(ENCOUNT_NOTH, DONE);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostilTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetPositionX()>(2737))
        {
            EnterEvadeMode();
            return;
        }
        if (m_creature->GetPositionY()<(-3560))
        {
            EnterEvadeMode();
            return;
        }

        switch(Stage)
        {
        case 1:
            {
                //Blink_Timer
                if (Blink_Timer < diff)
                {
                    DoCast(m_creature->getVictim(),m_bIsHeroicMode?SPELL_CRIPPLE_H:SPELL_CRIPPLE);
                    if (m_bIsHeroicMode)
                    {
                        DoResetThreat();
                        DoCast(m_creature,SPELL_BLINK);
                    }
                    Blink_Timer = 30000;
                }else Blink_Timer -= diff;

                //Curse_Timer
                if (Curse_Timer < diff)
                {
                    DoCast(m_creature->getVictim(),m_bIsHeroicMode?SPELL_CURSE_PLAGUEBRINGER_H:SPELL_CURSE_PLAGUEBRINGER);
                    Curse_Timer = 55000;
                }else Curse_Timer -= diff;

                //Summon_Timer
                if (Summon_Timer < diff)
                {
                    DoScriptText(SAY_SUMMON, m_creature);
                    max = m_bIsHeroicMode?3:2;
                    for(uint8 i = 0; i < max; i++)
                    {
                        uint8 j = (rand()%4);
                        m_creature->SummonCreature(NPC_PLAGUED_WARRIOR,SpawnLocations[j][0],SpawnLocations[j][1],SpawnLocations[j][2],SpawnLocations[j][3],TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                    }

                    Summon_Timer = 30500;
                }else Summon_Timer -= diff;

                if (Balcony_Timer < diff)
                {
                    m_creature->GetMotionMaster()->Clear(false);     // Stop he from moving as well
                    m_creature->GetMotionMaster()->MoveIdle();
                    m_creature->GetMap()->CreatureRelocation(m_creature, TELE_X, TELE_Y, TELE_Z, TELE_O);
                    m_creature->SendMonsterMove(TELE_X, TELE_Y, TELE_Z, 0, MONSTER_MOVE_NONE, 0);                    
                    m_creature->RemoveAllAuras();
                    m_creature->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
                    ++m_Stage;
                    Balcony_Timer = m_Stage == 2 ? 110000 : 180000;
                    Wave1_Timer = 10000;
                    Stage = 2;
                }else Balcony_Timer -= diff;

                DoMeleeAttackIfReady();
                break;
            }
        case 2:
            {
                if (Wave1_Timer < diff)
                {
                    max = m_bIsHeroicMode?4:2;
                    for(uint8 i = 0; i < max; i++)
                    {
                        if (m_Stage < 4)
                            Creature_Summon = NPC_PLAGUED_CHAMPION;
                        if (!m_bIsHeroicMode)
                        {
                            if (m_Stage > 2 && i == 1)
                                Creature_Summon = NPC_PLAGUED_GUARDIANS;
                        }
                        else
                        {
                            if (m_Stage > 2 && i > 1)
                                Creature_Summon = NPC_PLAGUED_GUARDIANS;
                        }
                        if (m_Stage > 3)
                            Creature_Summon = NPC_PLAGUED_GUARDIANS;
                        uint8 j = (rand()%4);
                        m_creature->SummonCreature(Creature_Summon,SpawnLocations[j][0],SpawnLocations[j][1],SpawnLocations[j][2],SpawnLocations[j][3],TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,80000);
                    }
                    Wave1_Timer = 30000;
                }else Wave1_Timer -= diff;

                if (Ground_Timer < diff)
                {
                    m_creature->GetMap()->CreatureRelocation(m_creature, 2684.830, -3503.489, 261.308, 0.24);
                    m_creature->SendMonsterMove(2684.830, -3503.489, 261.308, 0, MONSTER_MOVE_NONE, 0);
                    m_creature->GetMotionMaster()->Clear();
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_NON_ATTACKABLE);
                    if (m_Stage == 4)
                        DoCast(m_creature,SPELL_BERSERK);
                    Ground_Timer = m_Stage == 2 ? 95000 : 120000;
                    Stage = 1;
                }else Ground_Timer -= diff;
                break;
                DoStopAttack();
            }
        }
    }
};

CreatureAI* GetAI_boss_noth(Creature* pCreature)
{
    return new boss_nothAI(pCreature);
}

void AddSC_boss_noth()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_noth";
    newscript->GetAI = &GetAI_boss_noth;
    newscript->RegisterSelf();
}
