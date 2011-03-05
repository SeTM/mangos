/* Copyright (C) 2006 - 2010 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Boss_KelThuzud
SD%Complete: 0
SDComment: 90%
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "naxxramas.h"

//when shappiron dies. dialog between kel and lich king (in this order)
#define SAY_SAPP_DIALOG1            -1533084
#define SAY_SAPP_DIALOG2_LICH       -1533085
#define SAY_SAPP_DIALOG3            -1533086
#define SAY_SAPP_DIALOG4_LICH       -1533087
#define SAY_SAPP_DIALOG5            -1533088

//when cat dies
#define SAY_CAT_DIED                -1533089

//when each of the 4 wing bosses dies
#define SAY_TAUNT1                  -1533090
#define SAY_TAUNT2                  -1533091
#define SAY_TAUNT3                  -1533092
#define SAY_TAUNT4                  -1533093

#define SAY_SUMMON_MINIONS          -1533105                //start of phase 1

#define SAY_AGGRO1                  -1533094                //start of phase 2
#define SAY_AGGRO2                  -1533095
#define SAY_AGGRO3                  -1533096

#define SAY_SLAY1                   -1533097
#define SAY_SLAY2                   -1533098

#define SAY_DEATH                   -1533099

#define SAY_CHAIN1                  -1533100
#define SAY_CHAIN2                  -1533101
#define SAY_FROST_BLAST             -1533102

#define SAY_REQUEST_AID             -1533103                //start of phase 3
#define SAY_ANSWER_REQUEST          -1533104                //lich king answer

#define SAY_SPECIAL1_MANA_DET       -1533106
#define SAY_SPECIAL3_MANA_DET       -1533107
#define SAY_SPECIAL2_DISPELL        -1533108

//***THIS SCRIPTS IS UNDER DEVELOPMENT***
/*
DATA.
This script has been made with info taken from wowwikki... so there are things wrong...
like spell timers and Says. Also there's another major problem as there is no aggroed list
I cannot make Kel'thuzad to target specific party members, that is needed for spells like
Mana Detonation... so what I'm doing untill now is just to cast everything on my main aggroed
target. Sorry for him.
Another bug is that there are spells that are actually NOT working... I have to implement
them first.
Need DISPELL efect
I also don't know the emotes
*/

//Positional defines
#define ADDX_LEFT_FAR               3783.272705f
#define ADDY_LEFT_FAR               -5062.697266f
#define ADDZ_LEFT_FAR               143.711203f
#define ADDO_LEFT_FAR               3.617599f

#define ADDX_LEFT_MIDDLE            3730.291260f
#define ADDY_LEFT_MIDDLE            -5027.239258f
#define ADDZ_LEFT_MIDDLE            143.956909f
#define ADDO_LEFT_MIDDLE            4.461900f

#define ADDX_LEFT_NEAR              3683.868652f
#define ADDY_LEFT_NEAR              -5057.281250f
#define ADDZ_LEFT_NEAR              143.183884f
#define ADDO_LEFT_NEAR              5.237086f

#define ADDX_RIGHT_FAR              3759.355225f
#define ADDY_RIGHT_FAR              -5174.128418f
#define ADDZ_RIGHT_FAR              143.802383f
#define ADDO_RIGHT_FAR              2.170104f

#define ADDX_RIGHT_MIDDLE           370.724365f
#define ADDY_RIGHT_MIDDLE           -5185.123047f
#define ADDZ_RIGHT_MIDDLE           143.928024f
#define ADDO_RIGHT_MIDDLE           1.309310f

#define ADDX_RIGHT_NEAR             3665.121094f
#define ADDY_RIGHT_NEAR             -5138.679199f
#define ADDZ_RIGHT_NEAR             143.183212f
#define ADDO_RIGHT_NEAR             0.604023f

#define WALKX_LEFT_FAR              3754.431396f
#define WALKY_LEFT_FAR              -5080.727734f
#define WALKZ_LEFT_FAR              142.036316f
#define WALKO_LEFT_FAR              3.736189f

#define WALKX_LEFT_MIDDLE           3724.396484f
#define WALKY_LEFT_MIDDLE           -5061.330566f
#define WALKZ_LEFT_MIDDLE           142.032700f
#define WALKO_LEFT_MIDDLE           4.564785f

#define WALKX_LEFT_NEAR             3687.158424f
#define WALKY_LEFT_NEAR             -5076.834473f
#define WALKZ_LEFT_NEAR             142.017319f
#define WALKO_LEFT_NEAR             5.237086f

#define WALKX_RIGHT_FAR             3687.571777f
#define WALKY_RIGHT_FAR             -5126.831055f
#define WALKZ_RIGHT_FAR             142.017807f
#define WALKO_RIGHT_FAR             0.604023f

#define WALKX_RIGHT_MIDDLE          3707.990733f
#define WALKY_RIGHT_MIDDLE          -5151.450195f
#define WALKZ_RIGHT_MIDDLE          142.032562f
#define WALKO_RIGHT_MIDDLE          1.376855f

#define WALKX_RIGHT_NEAR            3739.500000f
#define WALKY_RIGHT_NEAR            -5141.883989f
#define WALKZ_RIGHT_NEAR            142.0141130f
#define WALKO_RIGHT_NEAR            2.121412f

//spells to be casted
#define SPELL_FROST_BOLT            28478
#define H_SPELL_FROST_BOLT          55802
#define SPELL_FROST_BOLT_NOVA       28479
#define H_SPELL_FROST_BOLT_NOVA     55807

#define SPELL_CHAINS_OF_KELTHUZAD   28410                   //casted spell should be 28408. Also as of 303, heroic only
#define SPELL_MANA_DETONATION       27819
#define SPELL_SHADOW_FISURE         27810
#define SPELL_FROST_BLAST           27808

float AddNpc[7][4]=
{
    {3763.591553f, -5074.789062f, 143.182022f, 3.698747f},
    {3727.601074f, -5049.215332f, 143.281937f, 4.492000f},
    {3685.690430f, -5062.457520f, 143.179794f, 5.501236f},
    {3660.546875f, -5096.548828f, 143.284775f, 6.029805f},
    {3668.957764f, -5139.075684f, 143.181763f, 0.743290f},
    {3704.820312f, -5164.700684f, 143.285233f, 1.363758f},
    {3749.049805f, -5155.220703f, 143.182693f, 2.124021f},
};

float AddLocations[42][4]=
{
    // banshe
    {3757.66f, -5150.64f, 143.565f, 2.0745f},
    {3760,    -5158.89f, 143.591f, 2.0745f},
    {3742.31f, -5160.77f, 143.431f, 2.06665f},
    {3701.89f, -5163.56f, 143.385f, 1.15951f},
    {3666.62f, -5150.67f, 143.652f, 0.561034f},
    {3668.46f, -5141.2f,  143.18f,  0.546897f},
    {3662.76f, -5135.29f, 143.354f, 0.351333f},
    {3659.97f, -5093.71f, 143.321f, 6.00542f},
    {3677.67f, -5064.14f, 143.294f, 5.22316f},
    {3691.84f, -5055.26f, 143.229f, 5.55617f},
    {3725.44f, -5037.12f, 143.668f, 4.49668f},
    {3729.52f, -5049.37f, 143.336f, 4.40243f},
    // aboms
    {3750.27f, -5158.44f, 143.184f, 2.16641f},
    {3712.03f, -5170.32f, 143.687f, 1.36845f},
    {3704.83f, -5175.21f, 143.573f, 1.36845f},
    {3706.34f, -5167.82f, 143.416f, 1.36845f},
    {3696.52f, -5165.81f, 143.639f, 1.36845f},
    {3663.72f, -5144.91f, 143.339f, 0.656883f},
    {3683.92f, -5059.68f, 143.182f, 5.37085f},
    {3676.37f, -5056.48f, 143.462f, 5.37085f},
    {3685.22f, -5049.63f, 143.474f, 5.37085f},
    {3735.63f, -5042.05f, 143.622f, 4.36632f},
    {3722.02f, -5042.97f, 143.63f,  4.75588f},
    {3767.75f, -5071.23f, 143.229f, 3.64297f},
    // skelets
    {3761.25f, -5065.75f, 143.558f, 3.92887f},
    {3772.62f, -5066.32f, 143.461f, 4.0671f},
    {3753.59f, -5155.48f, 143.336f, 2.17114f},
    {3745.85f, -5160.78f, 143.385f, 2.17114f},
    {3702.45f, -5166.98f, 143.377f, 1.03231f},
    {3664.42f, -5138.49f, 143.18f,  0.57835f},
    {3670.32f, -5146.32f, 143.453f, 0.587775f},
    {3660.21f, -5087.45f, 143.606f, 6.12091f},
    {3649.8f,  -5090.29f, 143.428f, 1.6677f},
    {3650.77f, -5098.65f, 143.557f, 2.88978f},
    {3657.9f,  -5098.26f, 143.412f, 4.62787f},
    {3655.47f, -5091.61f, 143.441f, 4.62787f},
    {3681.22f, -5057.02f, 143.181f, 5.45725f},
    {3728.03f, -5044.25f, 143.371f, 4.56739f},
    {3733.38f, -5046.63f, 143.527f, 4.56739f},
    {3773.44f, -5072.12f, 143.389f, 5.44939f},
    {3766.86f, -5067.02f, 143.449f, 4.0671f},
    {3769.91f, -5075.19f, 143.327f, 3.74979f},
};

struct MANGOS_DLL_DECL boss_kelthuzadAI : public ScriptedAI
{
    boss_kelthuzadAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        GuardiansOfIcecrown[0] = 0;
        GuardiansOfIcecrown[1] = 0;
        GuardiansOfIcecrown[2] = 0;
        GuardiansOfIcecrown[3] = 0;
        GuardiansOfIcecrown[4] = 0;
        GuardiansOfIcecrown_Count = 0;
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    ObjectGuid GuardiansOfIcecrown[5];
    ObjectGuid MobsArround[42];
    bool m_bIsRegularMode;

    uint32 AbomsTimer;
    uint32 BansheTimer;
    uint32 SkeletalTimer;

    uint32 GuardiansOfIcecrown_Count;
    uint32 GuardiansOfIcecrown_Timer;
    uint32 FrostBolt_Timer;
    uint32 FrostBoltNova_Timer;
    uint32 ChainsOfKelthuzad_Timer;
    uint32 ManaDetonation_Timer;
    uint32 ShadowFisure_Timer;
    uint32 FrostBlast_Timer;
    uint32 ChainsOfKelthuzad_Targets;
    uint32 Phase1_Timer;
    bool Phase1;
    bool Phase2;
    bool Phase3;

    void Reset()
    {
        FrostBolt_Timer = (rand()%60)*1000;                 //It won't be more than a minute without cast it
        FrostBoltNova_Timer = 15000;                        //Cast every 15 seconds
        ChainsOfKelthuzad_Timer = (rand()%30+30)*1000;      //Cast no sooner than once every 30 seconds
        ManaDetonation_Timer = 20000;                       //Seems to cast about every 20 seconds
        ShadowFisure_Timer = 25000;                         //25 seconds
        FrostBlast_Timer = (rand()%30+30)*1000;             //Random time between 30-60 seconds
        GuardiansOfIcecrown_Timer = 5000;                   //5 seconds for summoning each Guardian of Icecrown in phase 3
        GuardiansOfIcecrown_Count = 0;

        SkeletalTimer = 3000; // 71
        AbomsTimer = 10000; // 8
        BansheTimer = 10000; // 8

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        for(int i=0; i<5; i++)
        {
            if (!GuardiansOfIcecrown[i].IsEmpty())
            {
                //delete creature
                if (Creature* pGuardian = (Creature*)m_creature->GetMap()->GetUnit(GuardiansOfIcecrown[i]))
                {
                    if (pGuardian->isAlive())
                        pGuardian->ForcedDespawn();
                }

                GuardiansOfIcecrown[i] = 0;
            }
        }

        for(int i=0; i<42; i++)
        {
            if (!MobsArround[i].IsEmpty())
            {
                //delete creature
                if (Creature* pGuardian = (Creature*)m_creature->GetMap()->GetUnit(MobsArround[i]))
                {
                    if (pGuardian->isAlive())
                        pGuardian->ForcedDespawn();
                }

                MobsArround[i] = 0;
            }
        }

        Phase1_Timer = 228000;                              //Phase 1 lasts 3 minutes and 48 seconds
        Phase1 = true;
        Phase2 = false;
        Phase3 = false;
        m_creature->HandleEmoteCommand(EMOTE_ONESHOT_NONE);
    }

    void JustRespawned()
    {
        JustReachedHome();
    }


    void JustReachedHome()
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KELTHUZAD, NOT_STARTED);
    }

    void KilledUnit()
    {
        if (rand()%2)
            DoScriptText(SAY_SLAY1, m_creature);
        else
            DoScriptText(SAY_SLAY2, m_creature);
    }

    void JustDied(Unit* Killer)
    {
        if (m_pInstance)
            m_pInstance->SetData(TYPE_KELTHUZAD, DONE);

        DoScriptText(SAY_DEATH, m_creature);

        for(int i=0; i<5; i++)
        {
            if (!GuardiansOfIcecrown[i].IsEmpty())
            {
                Creature* pGuardian = (Creature*)m_creature->GetMap()->GetUnit(GuardiansOfIcecrown[i]);

                if (!pGuardian || !pGuardian->isAlive())
                    continue;

                pGuardian->CombatStop();

                float Walk_Pos_X = 0.0f;
                float Walk_Pos_Y = 0.0f;
                float Walk_Pos_Z = 0.0f;

                switch(rand()%6)
                {
                    case 0:
                        Walk_Pos_X = ADDX_LEFT_FAR;
                        Walk_Pos_Y = ADDY_LEFT_FAR;
                        Walk_Pos_Z = ADDZ_LEFT_FAR;
                        break;
                    case 1:
                        Walk_Pos_X = ADDX_LEFT_MIDDLE;
                        Walk_Pos_Y = ADDY_LEFT_MIDDLE;
                        Walk_Pos_Z = ADDZ_LEFT_MIDDLE;
                        break;
                    case 2:
                        Walk_Pos_X = ADDX_LEFT_NEAR;
                        Walk_Pos_Y = ADDY_LEFT_NEAR;
                        Walk_Pos_Z = ADDZ_LEFT_NEAR;
                        break;
                    case 3:
                        Walk_Pos_X = ADDX_RIGHT_FAR;
                        Walk_Pos_Y = ADDY_RIGHT_FAR;
                        Walk_Pos_Z = ADDZ_RIGHT_FAR;
                        break;
                    case 4:
                        Walk_Pos_X = ADDX_RIGHT_MIDDLE;
                        Walk_Pos_Y = ADDY_RIGHT_MIDDLE;
                        Walk_Pos_Z = ADDZ_RIGHT_MIDDLE;
                        break;
                    case 5:
                        Walk_Pos_X = ADDX_RIGHT_NEAR;
                        Walk_Pos_Y = ADDY_RIGHT_NEAR;
                        Walk_Pos_Z = ADDZ_RIGHT_NEAR;
                        break;
                }

                pGuardian->SendMonsterMoveWithSpeed(Walk_Pos_X, Walk_Pos_Y, Walk_Pos_Z);
                pGuardian->ForcedDespawn();
            }
        }
    }

    void SpawnNpc()
    {
        // spawn banshe
        for (int i = 0; i < 12; i++)
        {
            if (Creature* pBanshe = m_creature->SummonCreature(16429,AddLocations[i][0],AddLocations[i][1],AddLocations[i][2],AddLocations[i][3],TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,600000))
                MobsArround[i] = pBanshe->GetObjectGuid();
        }
        
        // spawn aboms
        for (int i = 12; i < 24; i++)
        {
            if (Creature* pAboms = m_creature->SummonCreature(16428,AddLocations[i][0],AddLocations[i][1],AddLocations[i][2],AddLocations[i][3],TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,600000))
                MobsArround[i] = pAboms->GetObjectGuid();
        }

        // spawn skelets
        for (int i = 24; i < 42; i++)
        {
            if (Creature* pSkelets = m_creature->SummonCreature(16427,AddLocations[i][0],AddLocations[i][1],AddLocations[i][2],AddLocations[i][3],TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,600000))
                MobsArround[i] = pSkelets->GetObjectGuid();
        }
    }

    void Aggro(Unit* who)
    {
        DoScriptText(SAY_SUMMON_MINIONS, m_creature);
        SpawnNpc();
        SetCombatMovement(false);
        m_creature->SetInCombatWithZone();

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);

        if (m_pInstance)
            m_pInstance->SetData(TYPE_KELTHUZAD, IN_PROGRESS);
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (Phase1)
        {
            if (!m_creature->HasAura(29423))
                DoCast(m_creature,29423,true);

            if (SkeletalTimer <= diff)
            {
                uint8 j = (rand()%7);
                if (Creature* pSkelet = m_creature->SummonCreature(16427,AddNpc[j][0],AddNpc[j][1],AddNpc[j][2],AddNpc[j][3],TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,20000))
                    if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                    {
                        pSkelet->SetNoCallAssistance(true);
                        pSkelet->AI()->AttackStart(target);
                    }
                
                SkeletalTimer = 4000;
            }else SkeletalTimer -= diff;

            if (AbomsTimer <= diff)
            {
                uint8 j = (rand()%7);
                if (Creature* pAbom = m_creature->SummonCreature(16428,AddNpc[j][0],AddNpc[j][1],AddNpc[j][2],AddNpc[j][3],TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,20000))
                    if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                    {
                        pAbom->SetNoCallAssistance(true);
                        pAbom->AI()->AttackStart(target);
                    }
                AbomsTimer = 25000;
            }else AbomsTimer -= diff;

            if (BansheTimer <= diff)
            {
                uint8 j = (rand()%7);
                if (Creature* pBanshe = m_creature->SummonCreature(16429,AddNpc[j][0],AddNpc[j][1],AddNpc[j][2],AddNpc[j][3],TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,20000))
                    if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                    {
                        pBanshe->SetNoCallAssistance(true);
                        pBanshe->AI()->AttackStart(target);
                    }

                BansheTimer = 23000;
            }else BansheTimer -= diff;


            if (Phase1_Timer - 15000 <= diff)
            {
                switch(rand()%3)
                {
                case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
                case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
                case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
                }

                for(int i=0; i<42; i++)
                {
                    if (!MobsArround[i].IsEmpty())
                    {
                        //delete creature
                        if (Creature* pGuardian = (Creature*)m_creature->GetMap()->GetUnit(MobsArround[i]))
                            if (pGuardian->isAlive())
                                pGuardian->ForcedDespawn();

                        MobsArround[i] = 0;
                    }
                }
            }

            if (Phase1_Timer <= diff)
            {
                Phase1 = false;
                Phase2 = true;
                
                m_creature->RemoveAurasDueToSpell(29423);
                m_creature->HandleEmoteCommand(EMOTE_ONESHOT_NONE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                SetCombatMovement(true);
                m_creature->GetMotionMaster()->Clear();
                m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            } else Phase1_Timer -= diff;
        }

        if (Phase2)
        {
            //Check for Frost Bolt
            if (FrostBolt_Timer < diff)
            {
                DoCast(m_creature->getVictim(),!m_bIsRegularMode?H_SPELL_FROST_BOLT:SPELL_FROST_BOLT);
                FrostBolt_Timer = (rand()%60)*1000;
            }else FrostBolt_Timer -= diff;

            //Check for Frost Bolt Nova
            if (FrostBoltNova_Timer < diff)
            {
                DoCast(m_creature->getVictim(),!m_bIsRegularMode?H_SPELL_FROST_BOLT_NOVA:SPELL_FROST_BOLT_NOVA);
                FrostBoltNova_Timer = 15000;
            }else FrostBoltNova_Timer -= diff;

            //Check for Chains Of Kelthuzad
            if (ChainsOfKelthuzad_Timer < diff)
            {
                //DoCast(m_creature->getVictim(),SPELL_CHAINS_OF_KELTHUZAD);

                if (!m_bIsRegularMode)
                {

                    if (rand()%2)
                        DoScriptText(SAY_CHAIN1, m_creature);
                    else
                        DoScriptText(SAY_CHAIN2, m_creature);

                    DoResetThreat(); // 50% chance WIPE
                }
                
                ChainsOfKelthuzad_Timer = (rand()%30+30)*1000;
            }else ChainsOfKelthuzad_Timer -= diff;

            //Check for Mana Detonation
            if (ManaDetonation_Timer < diff)
            {
                Unit* pTarget = NULL;
                ThreatList const& t_list = m_creature->getThreatManager().getThreatList();
                std::vector<Unit *> target_list;
                for(ThreatList::const_iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
                {
                    pTarget = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());
                    if (pTarget && pTarget->GetTypeId() == TYPEID_PLAYER && pTarget->getPowerType() == POWER_MANA)
                        target_list.push_back(pTarget);
                    pTarget = NULL;
                }
                if (target_list.size())
                    pTarget = *(target_list.begin()+rand()%target_list.size());

                DoCast(pTarget,SPELL_MANA_DETONATION, true);

                if (rand()%2)
                    DoScriptText(SAY_SPECIAL1_MANA_DET, m_creature);

                ManaDetonation_Timer = 20000;
            }else ManaDetonation_Timer -= diff;

            //Check for Shadow Fissure
            if (ShadowFisure_Timer < diff)
            {
                DoCast(m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0),SPELL_SHADOW_FISURE, true);

                if (rand()%2)
                    DoScriptText(SAY_SPECIAL3_MANA_DET, m_creature);

                ShadowFisure_Timer = 25000;
            }else ShadowFisure_Timer -= diff;

            //Check for Frost Blast
            if (FrostBlast_Timer < diff)
            {
                Unit* pTarget = NULL;
                ThreatList const& t_list = m_creature->getThreatManager().getThreatList();
                std::vector<Unit *> target_list;
                // find random player target
                for(ThreatList::const_iterator itr = t_list.begin(); itr!= t_list.end(); ++itr)
                {
                    pTarget = m_creature->GetMap()->GetUnit((*itr)->getUnitGuid());
                    if (pTarget && pTarget->GetTypeId() == TYPEID_PLAYER)
                        target_list.push_back(pTarget);
                    pTarget = NULL;
                }
                if (target_list.size())
                    pTarget = *(target_list.begin()+rand()%target_list.size());

                DoCast(pTarget,SPELL_FROST_BLAST, true);

                if (rand()%2)
                    DoScriptText(SAY_FROST_BLAST, m_creature);

                FrostBlast_Timer = (rand()%30+30)*1000;
            }else FrostBlast_Timer -= diff;
        }

        //start phase 3 when we are 40% health
        if (!Phase3 && (m_creature->GetHealth()*100 / m_creature->GetMaxHealth()) < 40)
        {
            Phase3 = true;
            DoScriptText(SAY_REQUEST_AID, m_creature);
            //here Lich King should respond to KelThuzad but I don't know which creature to make talk
            //so for now just make Kelthuzad says it.
            DoScriptText(SAY_ANSWER_REQUEST, m_creature);
        }

        if (Phase3 && (GuardiansOfIcecrown_Count < (!m_bIsRegularMode ? 4:2) ))
        {
            if (GuardiansOfIcecrown_Timer < diff)
            {
                //Summon a Guardian of Icecrown in a random alcove (Creature # 16441)
                //uint32 TimeToWalk;
                Creature* pGuardian = NULL;

                float Walk_Pos_X;
                float Walk_Pos_Y;
                float Walk_Pos_Z;

                switch(rand()%6)
                {
                    case 0:
                        pGuardian = m_creature->SummonCreature(16441,ADDX_LEFT_FAR,ADDY_LEFT_FAR,ADDZ_LEFT_FAR,ADDO_LEFT_FAR,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,12000);
                        //Setting walk position
                        Walk_Pos_X = WALKX_LEFT_FAR;
                        Walk_Pos_Y = WALKY_LEFT_FAR;
                        Walk_Pos_Z = WALKZ_LEFT_FAR;
                        break;
                    case 1:
                        pGuardian = m_creature->SummonCreature(16441,ADDX_LEFT_MIDDLE,ADDY_LEFT_MIDDLE,ADDZ_LEFT_MIDDLE,ADDO_LEFT_MIDDLE,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,12000);
                        //Start moving guardian towards the center of the room
                        Walk_Pos_X = WALKX_LEFT_MIDDLE;
                        Walk_Pos_Y = WALKY_LEFT_MIDDLE;
                        Walk_Pos_Z = WALKZ_LEFT_MIDDLE;
                        break;
                    case 2:
                        pGuardian = m_creature->SummonCreature(16441,ADDX_LEFT_NEAR,ADDY_LEFT_NEAR,ADDZ_LEFT_NEAR,ADDO_LEFT_NEAR,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,12000);
                        //Start moving guardian towards the center of the room
                        Walk_Pos_X = WALKX_LEFT_NEAR;
                        Walk_Pos_Y = WALKY_LEFT_NEAR;
                        Walk_Pos_Z = WALKZ_LEFT_NEAR;
                        break;
                    case 3:
                        pGuardian = m_creature->SummonCreature(16441,ADDX_RIGHT_FAR,ADDY_RIGHT_FAR,ADDZ_RIGHT_FAR,ADDO_RIGHT_FAR,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,12000);
                        //Start moving guardian towards the center of the room
                        Walk_Pos_X = WALKX_RIGHT_FAR;
                        Walk_Pos_Y = WALKY_RIGHT_FAR;
                        Walk_Pos_Z = WALKZ_RIGHT_FAR;
                        break;
                    case 4:
                        pGuardian = m_creature->SummonCreature(16441,ADDX_RIGHT_MIDDLE,ADDY_RIGHT_MIDDLE,ADDZ_RIGHT_MIDDLE,ADDO_RIGHT_MIDDLE,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,12000);
                        //Start moving guardian towards the center of the room
                        Walk_Pos_X = WALKX_RIGHT_MIDDLE;
                        Walk_Pos_Y = WALKY_RIGHT_MIDDLE;
                        Walk_Pos_Z = WALKZ_RIGHT_MIDDLE;
                        break;
                    case 5:
                        pGuardian = m_creature->SummonCreature(16441,ADDX_RIGHT_NEAR,ADDY_RIGHT_NEAR,ADDZ_RIGHT_NEAR,ADDO_RIGHT_NEAR,TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN,12000);
                        //Start moving guardian towards the center of the room
                        Walk_Pos_X = WALKX_RIGHT_NEAR;
                        Walk_Pos_Y = WALKY_RIGHT_NEAR;
                        Walk_Pos_Z = WALKZ_RIGHT_NEAR;
                        break;
                }

                if (pGuardian)
                {
                    //if we find no one to figth walk to the center
                    if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
                        pGuardian->AI()->AttackStart(target);

                    //Safe storing of creatures
                    GuardiansOfIcecrown[GuardiansOfIcecrown_Count] = pGuardian->GetObjectGuid();

                    //Update guardian count
                    GuardiansOfIcecrown_Count++;
                }

                //5 seconds until summoning next guardian
                GuardiansOfIcecrown_Timer = 5000;
            }else GuardiansOfIcecrown_Timer -= diff;
        }

        if (!Phase1)
            DoMeleeAttackIfReady();
    }
};

/*######
## Mob Shadow Fissure
######*/

struct MANGOS_DLL_DECL mob_shadow_fisureAI : public ScriptedAI
{
    mob_shadow_fisureAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();         
    }

    uint32 m_uiDespawn;
    uint32 m_uiVoidTimer;
    bool m_bIsRegularMode;

    void Aggro(Unit* who)
    {
        DoStopAttack();
        SetCombatMovement(false);
    }

    void Reset()
    {
        m_uiDespawn = 5000;
        m_uiVoidTimer = 3500;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiVoidTimer <= uiDiff)
        {
            DoCast(m_creature,27812,true);
            m_uiVoidTimer = 3000;
        }else m_uiVoidTimer -= uiDiff;

        if (m_uiDespawn <= uiDiff)
            m_creature->ForcedDespawn();
        else m_uiDespawn -= uiDiff;
    }
};

/*######
## Mob Soul Weaver
######*/

struct MANGOS_DLL_DECL mob_soul_weaverAI : public ScriptedAI
{
    mob_soul_weaverAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();         
    }

    void Reset()
    {
    }


    bool m_bIsRegularMode;

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->HasAura(!m_bIsRegularMode ? 55717 : 28460))
            DoCast(m_creature, !m_bIsRegularMode ? 55717 : 28460, true);

        DoMeleeAttackIfReady();
    }
};

/*######
## Mob Frozen Wastes
######*/

struct MANGOS_DLL_DECL mob_frozen_wastesAI : public ScriptedAI
{
    mob_frozen_wastesAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();         
    }

    bool m_bIsRegularMode;

    void Reset()
    {
    }


    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->HasAura(!m_bIsRegularMode ? 55713 : 28458))
            DoCast(m_creature, !m_bIsRegularMode ? 55713 : 28458, true);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_mob_frozen_wastes(Creature* pCreature)
{
    return new mob_frozen_wastesAI(pCreature);
}

CreatureAI* GetAI_mob_soul_weaver(Creature* pCreature)
{
    return new mob_soul_weaverAI(pCreature);
}

CreatureAI* GetAI_mob_shadow_fisure(Creature* pCreature)
{
    return new mob_shadow_fisureAI(pCreature);
}

CreatureAI* GetAI_boss_kelthuzadAI(Creature* pCreature)
{
    return new boss_kelthuzadAI(pCreature);
}

void AddSC_boss_kelthuzad()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_kelthuzad";
    newscript->GetAI = &GetAI_boss_kelthuzadAI;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_shadow_fisure";
    newscript->GetAI = &GetAI_mob_shadow_fisure;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_soul_weaver";
    newscript->GetAI = &GetAI_mob_soul_weaver;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_frozen_wastes";
    newscript->GetAI = &GetAI_mob_frozen_wastes;
    newscript->RegisterSelf();
}
