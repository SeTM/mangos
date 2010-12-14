#include "precompiled.h"
#include "ulduar.h"
#include "Spell.h"

float PlasmaLocations[3][4]=
{
    {2730.562f, 2547.082f, 364.314f, 1.093f},
    {2732.091f, 2592.117f, 364.314f, 5.224f},
    {2770.267f, 2568.920f, 364.314f, 3.123f},
};

float PlasmaMoves[9][4]=
{
    {2728.023926f, 2548.022705f, 364.314026f, 0.715062f},
    {2724.964600f, 2550.900879f, 364.314026f, 0.715062f},
    {2723.065918f, 2555.593262f, 364.314026f, 0.404045f},
    {2721.977f, 2559.528f, 364.314f, 0.404f},
    {2720.097656f, 2563.924072f, 364.314026f, 0.404045f},
    {2718.895508f, 2568.858154f, 364.314026f, 0.057684f},
    {2719.282715f, 2574.330566f, 364.314026f, 0.057684f},
    {2721.432129f, 2581.117920f, 364.314026f, 5.959166f},
    {2724.421631f, 2585.947754f, 364.314026f, 5.735327f},
};

float PlasmaMoves2[9][4]=
{
    {2735.051270f, 2593.949219f, 364.313995f, 4.934407f},
    {2739.114014f, 2594.866455f, 364.313995f, 4.934407f},
    {2743.815674f, 2595.384521f, 364.313995f, 4.645380f},
    {2748.425293f, 2595.075195f, 364.313995f, 4.645380f},
    {2753.299316f, 2593.463379f, 364.313995f, 4.645380f},
    {2757.589111f, 2590.688965f, 364.313995f, 4.291166f},
    {2762.483154f, 2587.703125f, 364.313995f, 3.889042f},
    {2765.217041f, 2583.651611f, 364.313995f, 3.889042f},
    {2768.429443f, 2577.902344f, 364.313995f, 3.483776f},
};

float PlasmaMoves3[9][4]=
{
    {2770.800049f, 2565.269043f, 364.313690f, 2.997624f},
    {2769.469727f, 2561.020020f, 364.313690f, 2.783996f},
    {2767.032959f, 2556.438232f, 364.313690f, 2.783996f},
    {2765.476563f, 2552.473145f, 364.313690f, 2.425854f},
    {2762.564453f, 2549.124268f, 364.313690f, 2.177668f},
    {2758.593018f, 2546.461182f, 364.313690f, 2.177668f},
    {2754.350830f, 2544.082520f, 364.313690f, 2.052004f},
    {2749.330566f, 2542.955566f, 364.313690f, 2.052004f},
    {2743.203125f, 2542.535400f, 364.313690f, 1.348287f},
};



struct MANGOS_DLL_DECL boss_mimironAI : public Scripted_NoMovementAI
{   
    boss_mimironAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;
    uint8 m_phase;
    uint32 ReloadTimer;
    uint64 LeviGuid;
    uint64 CannonGuid;
    uint64 AirGuid;
    bool last_phase;
    uint32 LeviTimer;
    uint32 CannonTimer;
    uint32 AirTimer;
    bool m_bIsDeath;
    bool m_bIsDeath2;
    bool m_bIsDeath3;


    void Reset()
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_MIMIRON, NOT_STARTED);

        if (LeviGuid)
        {
            if (Creature *levi = m_creature->GetMap()->GetCreature(LeviGuid))
            {
                ((TemporarySummon *)levi)->UnSummon();
            }
        } else LeviGuid = 0;

        if (CannonGuid)
        {
            if (Creature *cannon = m_creature->GetMap()->GetCreature(CannonGuid))
            {
                ((TemporarySummon *)cannon)->UnSummon();
            }
        }
        else CannonGuid = 0;

        if (AirGuid)
        {
            if (Creature *Air = m_creature->GetMap()->GetCreature(AirGuid))
            {
                ((TemporarySummon *)Air)->UnSummon();
            }
        }
        else AirGuid = 0;

        m_creature->SetVisibility(VISIBILITY_ON);

        m_phase = 1;
        ReloadTimer = 10000;
        last_phase = false;
        LeviTimer = 1000;
        AirTimer = 1000;
        CannonTimer = 1000;
        m_bIsDeath = false;
        m_bIsDeath2 = false;
        m_bIsDeath3 = false;
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void Aggro(Unit *who) 
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_MIMIRON, IN_PROGRESS);

        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }

    void JustDied(Unit *killer)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_MIMIRON, DONE);

        GameObject* pGo = GetClosestGameObjectWithEntry(m_creature,m_bIsRegularMode ? 194789 : 194956,200.0f);
        if (pGo)
            m_pInstance->DoRespawnGameObject(pGo->GetGUID(),604800);

        if (LeviGuid)
        {
            if (Creature *levi = m_creature->GetMap()->GetCreature(LeviGuid))
            {
                ((TemporarySummon *)levi)->UnSummon();
            }
        } else LeviGuid = 0;

        if (CannonGuid)
        {
            if (Creature *cannon = m_creature->GetMap()->GetCreature(CannonGuid))
            {
                ((TemporarySummon *)cannon)->UnSummon();
            }
        }
        else CannonGuid = 0;

        if (AirGuid)
        {
            if (Creature *Air = m_creature->GetMap()->GetCreature(AirGuid))
            {
                ((TemporarySummon *)Air)->UnSummon();
            }
        }
        else AirGuid = 0;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_phase == 1) // summon Tank
        {
            m_phase = 0;
            m_creature->SetVisibility(VISIBILITY_OFF);
            if (Creature* levi = m_creature->SummonCreature(33432,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),m_creature->GetOrientation(), TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 4000))
                levi->AI()->AttackStart(m_creature->getVictim());
        }        
        else if (m_phase == 2) // summon Big Cannon
        {
            if (ReloadTimer <= diff)
            {
                m_phase = 0;
                if (Creature* cannon = m_creature->SummonCreature(33651,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ(),m_creature->GetOrientation(), TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 4000))
                    cannon->AI()->AttackStart(m_creature->getVictim());
            } else ReloadTimer -= diff;
        }
        else if (m_phase == 3) // Summon Airplane
        {
            if (ReloadTimer <= diff)
            {
                m_phase = 0;
                if (Creature* air = m_creature->SummonCreature(33670,m_creature->GetPositionX(),m_creature->GetPositionY(),m_creature->GetPositionZ() + 9.0f,m_creature->GetOrientation(), TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 4000))
                    air->AI()->AttackStart(m_creature->getVictim());
            } else ReloadTimer -= diff;
        }
        else if (m_phase == 4)
        {
            if (ReloadTimer <= diff)
            {
                m_phase = 0;
                m_pInstance->SetData(TYPE_MIMIRON, SPECIAL);

                float fX, fY, fZ, fO;
                m_creature->GetRespawnCoord(fX,fY,fZ,&fO);

                if (Creature* levi = m_creature->SummonCreature(33432,fX, fY, fZ, fO, TEMPSUMMON_MANUAL_DESPAWN, 4000))
                {
                    LeviGuid = levi->GetGUID();
                    levi->AI()->AttackStart(m_creature->getVictim());
                }

                if (Creature* cannon = m_creature->SummonCreature(33651,fX, fY, fZ+4.9f, fO, TEMPSUMMON_MANUAL_DESPAWN, 4000))
                {
                    CannonGuid = cannon->GetGUID();
                    cannon->AI()->AttackStart(m_creature->getVictim());
                }

                if (Creature* air = m_creature->SummonCreature(33670,fX, fY, fZ+11.0f, fO, TEMPSUMMON_MANUAL_DESPAWN, 4000))
                {
                    AirGuid = air->GetGUID();
                    air->AI()->AttackStart(m_creature->getVictim());
                }

                last_phase = true;
            } else ReloadTimer -= diff;
        }
        else if (m_phase == 5)
            m_creature->DealDamage(m_creature,m_creature->GetHealth(),NULL,DIRECT_DAMAGE,SPELL_SCHOOL_MASK_NORMAL,NULL,false);

        if (last_phase)
        {
            if (LeviTimer <= diff)
            {
                if (LeviGuid)
                {
                    if (Creature *levi = m_creature->GetMap()->GetCreature(LeviGuid))
                        if (!levi->isAlive() && !m_bIsDeath)
                        {
                            m_bIsDeath = true;
                            LeviTimer = 7000;
                        }
                        else if (!levi->isAlive() && m_bIsDeath)
                        {
                            levi->Respawn();
                            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                                levi->AI()->AttackStart(pTarget);
                            m_bIsDeath = false;
                            LeviTimer = 1000;
                        }
                }
            } else LeviTimer -= diff;
            if (AirTimer <= diff)
            {
                if (AirGuid)
                {
                    if (Creature *Air = m_creature->GetMap()->GetCreature(AirGuid))
                        if (!Air->isAlive() && !m_bIsDeath2)
                        {
                            m_bIsDeath2 = true;
                            AirTimer = 7000;
                        }
                        else if (!Air->isAlive() && m_bIsDeath2)
                        {
                            Air->Respawn();
                            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                                Air->AI()->AttackStart(pTarget);
                            m_bIsDeath2 = false;
                            AirTimer = 1000;
                        }
                }
            } else AirTimer -= diff;
            if (CannonTimer <= diff)
            {
                if (CannonGuid)
                {
                    if (Creature *Cannon = m_creature->GetMap()->GetCreature(CannonGuid))
                        if (!Cannon->isAlive() && !m_bIsDeath3)
                        {
                            m_bIsDeath3 = true;
                            CannonTimer = 7000;
                        }
                        else if (!Cannon->isAlive() && m_bIsDeath3)
                        {
                            Cannon->Respawn();
                            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                                Cannon->AI()->AttackStart(pTarget);
                            m_bIsDeath3 = false;
                            CannonTimer = 1000;
                        }
                }
            } else CannonTimer -= diff;
        }

        DoMeleeAttackIfReady();
    }
};


struct MANGOS_DLL_DECL boss_leviathan_mkAI : public ScriptedAI
{   
    boss_leviathan_mkAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance *m_pInstance;
    bool last_phase;
    uint8 m_phase;
    uint32 mines_timer;
    uint32 napalm_timer;
    uint32 plasma_blast_timer;
    uint32 shock_blast_timer;
    bool respawn_timer;
    uint32 respawn_time;


    void Reset()
    {
        if (m_pInstance->GetData(TYPE_MIMIRON) == SPECIAL)
        {
            last_phase = true;
            m_creature->SetHealth(m_creature->GetMaxHealth()/2);
        }
        else last_phase = false;
        mines_timer = 30000;
        napalm_timer = urand(15000, 23000);
        plasma_blast_timer = urand(35000, 45000);
        shock_blast_timer = urand(45000, 55000);
        respawn_timer = false;
        respawn_time = 7000;
    }

    void JustDied(Unit *killer)
    {
        if (Creature *pMimiron = m_pInstance->instance->GetCreature(m_pInstance->GetData64(TYPE_MIMIRON)))
        {
            if (!last_phase)
            {
                ((boss_mimironAI*)pMimiron->AI())->m_phase = 2;
                ((boss_mimironAI*)pMimiron->AI())->ReloadTimer = 10000;
            }
            else if (!GetClosestCreatureWithEntry(m_creature,33670,200) && !GetClosestCreatureWithEntry(m_creature,33651,200))
            {
                ((boss_mimironAI*)pMimiron->AI())->m_phase = 5;
                ((TemporarySummon *)m_creature)->UnSummon();
            }
            else respawn_timer = true;  
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (mines_timer <= diff)
        {
            for (uint8 i = 0; i < 21; i++)
                DoCast(m_creature, 65347, true);
            mines_timer = 35000;
        } else mines_timer -= diff;

        if (napalm_timer <= diff)
        {
            if (!last_phase)
            {
                Unit* target = m_creature->getVictim();
                if (target)
                {
                    for (uint8 i = 0; i < 20; i++)
                    {
                        target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1);
                        if (target && m_creature->GetDistance(target) >= 14)
                            break;
                    }
                    
                    if (target)
                    {
                        m_creature->SetUInt64Value(UNIT_FIELD_TARGET, target->GetGUID());
                        DoCast(target, m_bIsRegularMode ? 63666 : 65026);
                    }
                }
            }
            napalm_timer = urand(15000, 23000);
        } else napalm_timer -= diff;

        if (plasma_blast_timer <= diff)
        {
            if (!last_phase)
            {
                Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,1);
                if (target)
                {
                    m_creature->SetUInt64Value(UNIT_FIELD_TARGET, target->GetGUID());
                    DoCast(target, m_bIsRegularMode ? 62997 : 64529);
                }
            }
            plasma_blast_timer = urand(35000, 45000);
        } else plasma_blast_timer -= diff;

        if (shock_blast_timer <= diff)
        {
            DoCast(m_creature,63631);
            shock_blast_timer = urand(45000, 55000);
        } else shock_blast_timer -= diff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL npc_plasma_targetAI : public Scripted_NoMovementAI
{
    npc_plasma_targetAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 Despawn_Timer;
    uint32 MoveTimer;
    int8 step;
    int8 direction;

    void Reset()
    {
        direction = 0;
        step = 0;
        MoveTimer = 4100;
        Despawn_Timer = 15000;
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetVisibility(VISIBILITY_OFF);
    }

    void Aggro(Unit *who) { }
    void AttackStart(Unit* who) { }
    void MoveInLineOfSight(Unit* who) { }
    void EnterCombat(Unit* who)
    {
        return;
    }

    void DamageTaken(Unit *pDoneBy, uint32 &dmg)
    {
        dmg = 0;
    }

    void UpdateAI(const uint32 diff)
    {
        if (MoveTimer <= diff)
        {
            switch (direction)
            {
            case 0: m_creature->GetMap()->CreatureRelocation(m_creature, PlasmaMoves[step][0], PlasmaMoves[step][1], PlasmaMoves[step][2], PlasmaMoves[step][3]); break;
            case 1: m_creature->GetMap()->CreatureRelocation(m_creature, PlasmaMoves2[step][0], PlasmaMoves2[step][1], PlasmaMoves2[step][2], PlasmaMoves2[step][3]); break;
            case 2: m_creature->GetMap()->CreatureRelocation(m_creature, PlasmaMoves3[step][0], PlasmaMoves3[step][1], PlasmaMoves3[step][2], PlasmaMoves3[step][3]); break;
            }

            if (step == 8)
            {
                step = 0;
                MoveTimer = 30000;
            }
            else
            {
                step++;
                MoveTimer = 1000;
            }

        } else MoveTimer -= diff;

        if (Despawn_Timer <= diff)
            m_creature->ForcedDespawn();
        else Despawn_Timer -= diff;
    }
};

struct MANGOS_DLL_DECL boss_vx_001AI : public Scripted_NoMovementAI
{   
    boss_vx_001AI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance *m_pInstance;
    bool m_bIsRegularMode;
    bool plasma;
    bool last_phase;
    uint8 m_phase;
    uint32 plasma_timer;
    uint32 rapid_burst_timer;
    uint32 rocket_timer;
    uint32 plasma_blast_timer;
    uint32 summon_plasma_timer;
    bool respawn_timer;
    uint32 respawn_time;
    

    void Reset()
    {
        if (m_pInstance->GetData(TYPE_MIMIRON) == SPECIAL)
        {
            last_phase = true;
            m_creature->SetHealth(m_creature->GetMaxHealth()/2);
        }
        else last_phase = false;

        m_creature->AddSplineFlag(SPLINEFLAG_FLYING);
        m_creature->SendHeartBeat(false);
        plasma = false;
        plasma_timer = 14000;
        rapid_burst_timer = 1500;
        rocket_timer = urand(11000, 15000);
        plasma_blast_timer = urand(25000, 35000);
        summon_plasma_timer = plasma_blast_timer - 4000;
        respawn_timer = false;
        respawn_time = 7000;
    }

    void Aggro(Unit *who) 
    {
    }

    void JustDied(Unit *killer)
    {
        if (Creature *pMimiron = m_pInstance->instance->GetCreature(m_pInstance->GetData64(TYPE_MIMIRON)))
        {
            if (!last_phase)
            {
                ((boss_mimironAI*)pMimiron->AI())->m_phase = 3;
                ((boss_mimironAI*)pMimiron->AI())->ReloadTimer = 10000;
            }
            else if (!GetClosestCreatureWithEntry(m_creature,33670,200) && !GetClosestCreatureWithEntry(m_creature,33432,200))
            {
                ((boss_mimironAI*)pMimiron->AI())->m_phase = 5;
                ((TemporarySummon *)m_creature)->UnSummon();
            }
            else respawn_timer = true;   
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_creature->HasAura(64534) && !last_phase)
            DoCast(m_creature,64534,true);

        if (plasma)
        {
            if (Creature* target = GetClosestCreatureWithEntry(m_creature, 33835, 120))
            {
                m_creature->SetOrientation(m_creature->GetAngle(target));
                m_creature->AttackStop();
                m_creature->AI()->AttackStart(target);
                m_creature->AddThreat(target, 10000000.0f);
            }

            if (plasma_timer <= diff)
            {
                plasma = false;
                plasma_timer = 14000;
            } else plasma_timer -= diff;
        }

        if (rapid_burst_timer <= diff)
        {
            if (!plasma)
            {
                if (Spell* spell = m_creature->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                    if (spell->m_spellInfo->Id == 63274)
                        return;

                Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);

                if (target)
                {
                    if (!last_phase)
                        DoCast(target, 63382);
                    else
                        DoCast(target, m_bIsRegularMode ? 64348 : 64536);
                }
            }
            
            rapid_burst_timer = 3200;
        } else rapid_burst_timer -= diff;

        if (rocket_timer <= diff)
        {
            if (Spell* spell = m_creature->GetCurrentSpell(CURRENT_CHANNELED_SPELL))
                if (spell->m_spellInfo->Id != 63274)
                    m_creature->InterruptNonMeleeSpells(false);

            uint8 count;
            count = last_phase ? 2 : 1;
            if (!m_bIsRegularMode)
                count *= 2;

            for (uint8 i = 0; i < count; i++)
            {
                Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                if (target)
                    DoCast(target, 63036, true);
            }

            rapid_burst_timer = 800;
            rocket_timer = urand(11000, 15000);
        } else rocket_timer -= diff;

        if (summon_plasma_timer <= diff)
        {
            if (!plasma)
            {
                m_creature->HandleEmoteCommand(124);
                DoScriptText(-1750000, m_creature);
                uint8 j = (rand()%3);
                if (Creature* target = m_creature->SummonCreature(33835,PlasmaLocations[j][0],PlasmaLocations[j][1],PlasmaLocations[j][2],PlasmaLocations[j][3],TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 15000))
                {
                    m_creature->RemoveAllAttackers();
                    m_creature->AttackStop();
                    m_creature->AI()->AttackStart(target);
                    ((npc_plasma_targetAI*)target->AI())->direction = j;
                }
                plasma = true;
            }
            summon_plasma_timer = 40000;
        } else summon_plasma_timer -= diff;

        if (plasma_blast_timer <= diff)
        {
            if (Creature* pTarget = GetClosestCreatureWithEntry(m_creature, 33835, 120))
                DoCast(pTarget,63274);
            plasma_blast_timer = urand(25000, 35000) + 10000;
            summon_plasma_timer = plasma_blast_timer - 4000;
        } else plasma_blast_timer -= diff;
    }
};

struct MANGOS_DLL_DECL boss_aerial_command_unitAI : public ScriptedAI
{   
    boss_aerial_command_unitAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    bool stop;
    bool last_phase;
    ScriptedInstance *m_pInstance;
    uint8 m_phase;
    uint32 plasma_ball_timer;
    uint32 summon_assault_timer;
    uint32 summon_bomb_timer;
    uint32 summon_junk_timer;
    bool respawn_timer;
    uint32 respawn_time;


    void Reset()
    {
        if (m_pInstance->GetData(TYPE_MIMIRON) == SPECIAL)
        {
            last_phase = true;
            m_creature->SetHealth(m_creature->GetMaxHealth()/2);
        }
        else last_phase = false;

        stop = false;
        m_creature->AddSplineFlag(SPLINEFLAG_FLYING);
        m_creature->SendHeartBeat(false);
        
        plasma_ball_timer = 3000;
        
        summon_junk_timer = urand(10000, 13000);
        summon_bomb_timer = urand(10000, 12000);
        summon_assault_timer = urand(8000, 10000);
        respawn_timer = false;
        respawn_time = 7000;
    }

    void JustDied(Unit *killer)
    {
        if (Creature *pMimiron = m_pInstance->instance->GetCreature(m_pInstance->GetData64(TYPE_MIMIRON)))
        {
            if (!last_phase)
            {
                ((boss_mimironAI*)pMimiron->AI())->m_phase = 4;
                ((boss_mimironAI*)pMimiron->AI())->ReloadTimer = 10000;
            }
            else if (!GetClosestCreatureWithEntry(m_creature,33651,200) && !GetClosestCreatureWithEntry(m_creature,33432,200))
            {
                ((boss_mimironAI*)pMimiron->AI())->m_phase = 5;
                ((TemporarySummon *)m_creature)->UnSummon();
            }
            else respawn_timer = true;                
        }
    }

    void EnterEvadeMode()
    {
        m_creature->RemoveAllAuras();
        m_creature->DeleteThreatList();
        m_creature->CombatStop(true);

        if (m_creature->isAlive())
            m_creature->GetMotionMaster()->MoveTargetedHome();

        m_creature->SetLootRecipient(NULL);
    }

    void JustSummoned(Creature* who)
    {
        if (who->GetEntry() == 33836)
            if (m_creature->getVictim())
                who->AI()->AttackStart(m_creature->getVictim());
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_creature->GetDistance(m_creature->getVictim()) <= 30 && !stop && !m_creature->HasAura(64436))
        {
            m_creature->GetMotionMaster()->MoveIdle();
            m_creature->StopMoving();
            stop = true;
        }
        else if (stop && m_creature->GetDistance(m_creature->getVictim()) > 30 && !m_creature->HasAura(64436))
        {
            m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
            stop = false;
        }

        if (plasma_ball_timer <= diff)
        {
            if (!m_creature->HasAura(64436))
                DoCast(m_creature->getVictim(), m_bIsRegularMode ? 63689 : 64535);
            plasma_ball_timer = 2500;
        } else plasma_ball_timer -= diff;

        if (!last_phase)
        {
            if (m_creature->HasAura(64436) && m_creature->GetPositionZ() > 366.0f)
            {
                m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), 364.0f, m_creature->GetOrientation());
                m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), 364.0f, SPLINETYPE_NORMAL, SPLINEFLAG_NONE, 1000);  
            }

            if (!m_creature->HasAura(64436) && m_creature->GetPositionZ() < 366.0f)
            {
                m_creature->GetMap()->CreatureRelocation(m_creature, m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() + 10.0f, m_creature->GetOrientation());
                m_creature->SendMonsterMove(m_creature->GetPositionX(), m_creature->GetPositionY(), m_creature->GetPositionZ() + 10.0f, SPLINETYPE_NORMAL, SPLINEFLAG_NONE, 1000);
            }

            if (summon_assault_timer <= diff)
            {
                if (Creature* assault = m_creature->SummonCreature(34057, 2726.807f, 2569.337f, 364.314f, m_creature->GetOrientation(), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 10000))
                    assault->AI()->AttackStart(m_creature->getVictim());
                summon_assault_timer = urand(28000, 31000);
            } else summon_assault_timer -= diff;

            if (summon_junk_timer <= diff)
            {
                if (Creature* Junk = m_creature->SummonCreature(33855, 2715.661f, 2569.088f, 364.314f, m_creature->GetOrientation(), TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 10000))
                    Junk->AI()->AttackStart(m_creature->getVictim());
                summon_junk_timer = urand(5000, 8000);
            } else summon_junk_timer -= diff;

            if (summon_bomb_timer <= diff)
            {
                m_creature->InterruptNonMeleeSpells(false);
                plasma_ball_timer = 2500;
                if (!m_creature->HasAura(64436))
                    DoCast(m_creature->getVictim(),63811);
                summon_bomb_timer = urand(10000, 15000);
            } else summon_bomb_timer -= diff;
        }

    }
};

struct MANGOS_DLL_DECL npc_rocket_strikeAI : public Scripted_NoMovementAI
{
    npc_rocket_strikeAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 Despawn_Timer;

    void Reset()
    {
        Despawn_Timer = 5500;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }
    void Aggro(Unit *who)
    {
        return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->HasAura(64064))
            DoCast(m_creature,64064,true);

        if (Despawn_Timer <= diff)
            m_creature->ForcedDespawn();
        else Despawn_Timer -= diff;
    }
};


struct MANGOS_DLL_DECL npc_mk_mineAI : public Scripted_NoMovementAI
{
    npc_mk_mineAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 Despawn_Timer;
    uint32 Despawn_Timer2;
    bool explosion;

    void Reset()
    {
        explosion = false;
        Despawn_Timer = 35000;
        Despawn_Timer2 = 45000;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }
    void Aggro(Unit *who)
    {
        return;
    }

    void MoveInLineOfSight(Unit* who)
    {
        if (explosion)
            return;

        if (!who)
            return;

        if (who->GetTypeId() != TYPEID_PLAYER)
            return;

        if (m_creature->GetDistance(who) <= 2.5f)
        {
            explosion = true;
            DoCast(m_creature, m_bIsRegularMode ? 66351:63009, true);
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (Despawn_Timer <= diff && !explosion)
        {
            DoCast(m_creature, m_bIsRegularMode ? 66351:63009, true);
            explosion = true;
        }
        else Despawn_Timer -= diff;

        if (Despawn_Timer2 <= diff)
        {
            m_creature->ForcedDespawn();
        } else Despawn_Timer2 -= diff;
    }
};

struct MANGOS_DLL_DECL npc_magnetic_coreAI : public Scripted_NoMovementAI
{
    npc_magnetic_coreAI(Creature* pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 Despawn_Timer;
    uint32 CastTimer;

    void Reset()
    {
        CastTimer = 1000;
        Despawn_Timer = 10000;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
    }
    void Aggro(Unit *who)
    {
        return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (CastTimer <= diff)
        {
            DoCast(m_creature, 64436, true);
        } else CastTimer -= diff;

        if (Despawn_Timer <= diff)
        {
            m_creature->ForcedDespawn();
        } else Despawn_Timer -= diff;
    }
};

struct MANGOS_DLL_DECL npc_bomb_botAI : public ScriptedAI
{
    npc_bomb_botAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 Despawn_Timer;
    uint32 CastTimer;

    void Reset()
    {
        Despawn_Timer = 5000;
    }
    void Aggro(Unit *who)
    {
        return;
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
        {
            if (Despawn_Timer <= diff)
                m_creature->ForcedDespawn();
            else Despawn_Timer -= diff;

            return;
        }

        if (m_creature->HasAura(63801))
            m_creature->ForcedDespawn();

        if (!m_creature->HasAura(63767))
            DoCast(m_creature,63767,true);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_mimiron(Creature* pCreature)
{
    return new boss_mimironAI(pCreature);
}

CreatureAI* GetAI_boss_leviathan_mk(Creature* pCreature)
{
    return new boss_leviathan_mkAI(pCreature);
}

CreatureAI* GetAI_npc_mk_mine(Creature* pCreature)
{
    return new npc_mk_mineAI(pCreature);
}

CreatureAI* GetAI_boss_vx_001(Creature* pCreature)
{
    return new boss_vx_001AI(pCreature);
}

CreatureAI* GetAI_boss_aerial_command_unit(Creature* pCreature)
{
    return new boss_aerial_command_unitAI(pCreature);
}

CreatureAI* GetAI_npc_rocket_strike(Creature* pCreature)
{
    return new npc_rocket_strikeAI(pCreature);
}

CreatureAI* GetAI_npc_magnetic_core(Creature* pCreature)
{
    return new npc_magnetic_coreAI(pCreature);
}

CreatureAI* GetAI_npc_bomb_bot(Creature* pCreature)
{
    return new npc_bomb_botAI(pCreature);
}

CreatureAI* GetAI_npc_plasma_target(Creature* pCreature)
{
    return new npc_plasma_targetAI(pCreature);
}

void AddSC_boss_mimiron()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_mimiron";
    newscript->GetAI = &GetAI_boss_mimiron;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_leviathan_mk";
    newscript->GetAI = &GetAI_boss_leviathan_mk;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_mk_mine";
    newscript->GetAI = &GetAI_npc_mk_mine;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_vx_001";
    newscript->GetAI = &GetAI_boss_vx_001;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_rocket_strike";
    newscript->GetAI = &GetAI_npc_rocket_strike;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_plasma_target";
    newscript->GetAI = &GetAI_npc_plasma_target;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_aerial_command_unit";
    newscript->GetAI = &GetAI_boss_aerial_command_unit;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_magnetic_core";
    newscript->GetAI = &GetAI_npc_magnetic_core;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_bomb_bot";
    newscript->GetAI = &GetAI_npc_bomb_bot;
    newscript->RegisterSelf();
}