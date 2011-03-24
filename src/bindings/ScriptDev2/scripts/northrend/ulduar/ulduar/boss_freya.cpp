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
SDName: Boss Freya
SD%Complete: 80%
SDComment: 
SDCategory: ulduar
EndScriptData */

#include "precompiled.h"
#include "ulduar.h"

enum
{
    SAY_AGGRO                           = -1603133,
    SAY_AGGRO2                          = -1603134,
    SAY_SLAY1                           = -1603135,
    SAY_SLAY2                           = -1603136,
    SAY_DEFEAT                          = -1603137,
    SAY_BERSERK                         = -1603138,
    SAY_SUMMON1                         = -1603139,
    SAY_SUMMON2                         = -1603140,
    SAY_SUMMON3                         = -1603141,

    SPELL_ATTUNED_TO_NATURE             = 62519,
    SPELL_BERSERK                       = 47008,

    SPELL_TOUCH_OF_EONAR                = 62528,
    H_SPELL_TOUCH_OF_EONAR              = 62892,

    SPELL_SUNBEAM                       = 62623,
    H_SPELL_SUNBEAM                     = 62872,

    SPELL_SUMMON_LIFEBINDERS_GIFT       = 62572,
    H_SPELL_SUMMON_LIFEBINDERS_GIFT     = 62870,

    SPELL_SUMMON_ALLIES                 = 62678,
    H_SPELL_SUMMON_ALLIES               = 62873,

    SPELL_NATURE_BOMB                   = 64604,
    SPELL_NATURE_BOMB_EXPLOSION         = 64587,
    H_SPELL_NATURE_BOMB_EXPLOSION       = 64650,

    //SPELL_ATTUNED_TO_NATURE_REDUCE_2  = 62524,
    //SPELL_ATTUNED_TO_NATURE_REDUCE_10 = 62525,
    //SPELL_ATTUNED_TO_NATURE_REDUCE_25 = 62521,

    SPELL_SUMMON_CHEST_1                = 62950,
    SPELL_SUMMON_CHEST_2                = 62952,
    SPELL_SUMMON_CHEST_3                = 62953,
    SPELL_SUMMON_CHEST_4                = 62954,
    SPELL_SUMMON_CHEST_5                = 62955,
    SPELL_SUMMON_CHEST_6                = 62956,
    SPELL_SUMMON_CHEST_7                = 62957,
    SPELL_SUMMON_CHEST_8                = 62958,

    NPC_NATURE_BOMB                     = 34129,
    GO_NATURE_BOMB                      = 194902,

    //----------------------------------------//
    //               Hard Modes               //
    //----------------------------------------//
    SPELL_DRAINED_OF_POWER              = 62467,

    // Ironbranch Alive
    SPELL_IRON_ROOTS_HM                 = 62862,
    H_SPELL_IRON_ROOTS_HM               = 62439,

    SPELL_IRON_ROOTS_AURA_HM             = 62861,
    H_SPELL_IRON_ROOTS_AURA_HM           = 62438,

    SPELL_IRONBRANCH_ESSENCE_CHANNEL    = 62484,
    H_SPELL_IRONBRANCH_ESSENCE_CHANNEL  = 65588,

    NPC_STRENGTHENED_IRON_ROOTS         = 33168,

    // Stonebark Alive
    SPELL_GROUND_TREMOR_HM              = 62437,
    H_SPELL_GROUND_TREMOR_HM            = 62859,

    SPELL_STONEBARK_ESSENCE_CHANNEL     = 62483,
    H_SPELL_STONEBARK_ESSENCE_CHANNEL   = 65589,

    // Brightleaf Alive
    SPELL_UNSTABLE_SUN_BEAM_HM          = 62450,
    H_SPELL_UNSTABLE_SUN_BEAM_HM        = 62868,

    SPELL_UNSTABLE_ENERGY_HM            = 62451,
    H_SPELL_UNSTABLE_ENERGY_HM          = 62865,

    SPELL_BRIGHTLEAF_ESSENCE_CHANNEL    = 62485,
    H_SPELL_BRIGHTLEAF_ESSENCE_CHANNEL  = 65587,

    NPC_SUN_BEAM                        = 33170,

    //----------------------------------------//
    //                Elders                  //
    //----------------------------------------//
    // Elder Ironbranch
    SAY_IRONBRANCH_AGGRO                = -1603121,
    SAY_IRONBRANCH_SLAY1                = -1603122,
    SAY_IRONBRANCH_SLAY2                = -1603123,
    SAY_IRONBRANCH_DEATH                = -1603124,

    //SPELL_IRONBRANCH_ESSENCE          = 62387,

    SPELL_IMPALE                        = 62310,
    H_SPELL_IMPALE                      = 62928,

    SPELL_THORN_SWARM                   = 64060,
    H_SPELL_THORN_SWARM                 = 64071,

    SPELL_IRON_ROOTS                    = 62275,
    H_SPELL_IRON_ROOTS                  = 62929,

    SPELL_IRON_ROOTS_AURA               = 62283,
    H_SPELL_IRON_ROOTS_AURA             = 62930,

    NPC_IRON_ROOTS                      = 33088,

    // Elder Stonebark
    SAY_STONEBARK_AGGRO                 = -1603125,
    SAY_STONEBARK_SLAY1                 = -1603126,
    SAY_STONEBARK_SLAY2                 = -1603127,
    SAY_STONEBARK_DEATH                 = -1603128,

    //SPELL_STONEBARK_ESSENCE               = 62386,

    SPELL_FISTS_OF_STONE                = 62344,
    SPELL_GROUND_TREMOR                 = 62325,
    H_SPELL_GROUND_TREMOR               = 62932,
    SPELL_PETRIFIED_BARK                = 62337,
    H_SPELL_PETRIFIED_BARK              = 62933,

    // Elder Brightleaf
    SAY_BRIGHTLEAF_AGGRO                = -1603129,
    SAY_BRIGHTLEAF_SLAY1                = -1603130,
    SAY_BRIGHTLEAF_SLAY2                = -1603131,
    SAY_BRIGHTLEAF_DEATH                = -1603132,

    //SPELL_BRIGHTLEAF_ESSENCE          = 62385,

    SPELL_BRIGHTLEAF_FLUX               = 62262,
    SPELL_PHOTOSYNTHESIS                = 62209,
    SPELL_UNSTABLE_SUN_BEAM_AURA        = 62211,

    SPELL_SOLAR_FLARE                   = 62240,
    H_SPELL_SOLAR_FLARE                 = 62920,

    SPELL_SUMMON_UNSTABLE_SUNBEAM       = 62207,
    H_SPELL_SUMMON_UNSTABLE_SUNBEAM     = 62921,

    SPELL_UNSTABLE_ENERGY               = 62217,
    H_SPELL_UNSTABLE_ENERGY             = 62922,

    NPC_UNSTABLE_SUN_BEAM               = 33050,

    //----------------------------------------//
    //               Trash Mobs               //
    //----------------------------------------//
    SPELL_LIFEBINDERS_GIFT              = 62584,
    H_SPELL_LIFEBINDERS_GIFT            = 64185,

    SPELL_FLAME_LASH                    = 62608,
    SPELL_DETONATE                      = 62598,
    H_SPELL_DETONATE                    = 62937,

    SPELL_LASHER_REVIVE_VISUAL          = 62848,
    SPELL_LASHER_WATER_REVIVE_VISUAL    = 62849,
    SPELL_STORM_LASHER_REVIVE_VISUAL    = 62851,
    SPELL_STORM_LASHER_VISUAL           = 62639,

    SPELL_HEALTHY_SPORE_SUMMON_PERIODIC = 62566,

    NPC_EONAR_GIFT                      = 33228,
    NPC_DETONATING_LASHER               = 32918,
    NPC_ANCIENT_WATER_SPIRIT            = 33202,
    NPC_STORM_LASHER                    = 32919,
    NPC_SNAPLASHER                      = 32916,
    NPC_ANCIENT_CONSERVATOR             = 33203,

    HARD_MODE_IRONBRANCH                = 0x1,
    HARD_MODE_STONEBARK                 = 0x2,
    HARD_MODE_BRIGHTLEAF                = 0x4,

    ACHIEVEMENT_ONE_ELDER_10            = 3177,
    ACHIEVEMENT_ONE_ELDER_25            = 3185,
    ACHIEVEMENT_TWO_ELDERS_10           = 3178,
    ACHIEVEMENT_TWO_ELDERS_25           = 3186,
    ACHIEVEMENT_THREE_ELDERS_10         = 3179,
    ACHIEVEMENT_THREE_ELDERS_25         = 3187,
    ACHIEVEMENT_BACK_TO_NATURE_10       = 2982,
    ACHIEVEMENT_BACK_TO_NATURE_25       = 2983,
};

struct MANGOS_DLL_DECL boss_freya_AI : public ScriptedAI
{
    boss_freya_AI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    ScriptedInstance* m_pInstance;
    uint32 m_uiSunbeamTimer;
    uint32 m_uiModeMask;
    uint32 m_uiUnstableEnergyTimer;
    uint32 m_uiIronRootsTimer;
    uint32 m_uiGroundTremorTimer;
    uint32 m_uiSummonGiftTimer;
    uint32 m_uiSummonTimer;
    uint32 m_uiWaveCount;
    uint32 m_uiNatureBombsTimer;
    uint32 m_uiBerserkTimer;
    uint32 m_uiPrepareTimer;
    uint32 m_uiSummonType;
    uint32 m_uiLastSummonType;
    uint32 m_uiLastSummonType2;

    std::set<ObjectGuid> m_lAdds;

    void Reset()
    {
        m_uiModeMask = 0;
        m_uiBerserkTimer = 10*MINUTE*IN_MILLISECONDS;
        m_uiUnstableEnergyTimer = urand(15000, 20000);
        m_uiIronRootsTimer = urand(40000, 60000);
        m_uiSunbeamTimer = urand(10000, 20000);
        m_uiSummonGiftTimer = 24000;
        m_uiGroundTremorTimer = urand(30000, 45000);
        m_uiWaveCount = 0;
        m_uiSummonTimer = urand(10000, 15000);
        m_uiNatureBombsTimer = urand(25000, 30000);
        m_uiSummonType = urand(0,2);
        m_uiLastSummonType = urand(0,2);
        m_uiLastSummonType2 = urand(0,2);
    }

    void Aggro(Unit*)
    {
        if (m_pInstance)
        {
            if (Creature *pElderIronbranch = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_IRONBRACH)))
            {
                if (pElderIronbranch->isAlive())
                {
                    pElderIronbranch->CastSpell(pElderIronbranch, SPELL_DRAINED_OF_POWER, true);
                    pElderIronbranch->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pElderIronbranch->CastSpell(m_creature, m_bIsRegularMode ? SPELL_IRONBRANCH_ESSENCE_CHANNEL : H_SPELL_IRONBRANCH_ESSENCE_CHANNEL, true);
                    m_uiModeMask |= HARD_MODE_IRONBRANCH;
                }
            }

            if (Creature *pElderStonebark = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_STONEBARK)))
            {
                if (pElderStonebark->isAlive())
                {
                    pElderStonebark->CastSpell(pElderStonebark, SPELL_DRAINED_OF_POWER, true);
                    pElderStonebark->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pElderStonebark->CastSpell(m_creature, m_bIsRegularMode ? SPELL_STONEBARK_ESSENCE_CHANNEL : H_SPELL_STONEBARK_ESSENCE_CHANNEL, true);
                    m_uiModeMask |= HARD_MODE_STONEBARK;
                }
            }

            if (Creature *pElderBrightleaf = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_BRIGHTLEAF)))
            {
                if (pElderBrightleaf->isAlive())
                {
                    pElderBrightleaf->CastSpell(pElderBrightleaf, SPELL_DRAINED_OF_POWER, true);
                    pElderBrightleaf->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    pElderBrightleaf->CastSpell(m_creature, m_bIsRegularMode ? SPELL_BRIGHTLEAF_ESSENCE_CHANNEL : H_SPELL_BRIGHTLEAF_ESSENCE_CHANNEL, true);
                    m_uiModeMask |= HARD_MODE_BRIGHTLEAF;
                }
            }

            m_pInstance->SetData(TYPE_FREYA, IN_PROGRESS);
        }

        DoScriptText(m_uiModeMask ? SAY_AGGRO2 : SAY_AGGRO, m_creature);
        m_creature->CastSpell(m_creature, SPELL_ATTUNED_TO_NATURE, true);

        if (Aura* aura = m_creature->GetAura(SPELL_ATTUNED_TO_NATURE, SpellEffectIndex(0)))
            aura->GetHolder()->SetStackAmount(150);

        m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_TOUCH_OF_EONAR : H_SPELL_TOUCH_OF_EONAR, true);
        m_creature->SetInCombatWithZone();
    }

    void ResetElders()
    {

        if (Creature *pElderIronbranch = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_IRONBRACH)))
        {
            if (pElderIronbranch->isAlive())
            {
                pElderIronbranch->RemoveAllAuras();
                pElderIronbranch->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }
        }

        if (Creature *pElderStonebark = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_STONEBARK)))
        {
            if (pElderStonebark->isAlive())
            {
                pElderStonebark->RemoveAllAuras();
                pElderStonebark->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }
        }

        if (Creature *pElderBrightleaf = m_pInstance->instance->GetCreature(m_pInstance->GetData64(NPC_BRIGHTLEAF)))
        {
            if (pElderBrightleaf->isAlive())
            {
                pElderBrightleaf->RemoveAllAuras();
                pElderBrightleaf->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            }
        }
    }

    void JustReachedHome()
    {
        if (m_pInstance)
        {
            m_pInstance->SetData(TYPE_FREYA, FAIL);
            ResetElders();
        }

        for(std::set<ObjectGuid>::const_iterator itr = m_lAdds.begin(); itr != m_lAdds.end(); ++itr)
        {
            if (Creature* pCreature = (Creature*)m_creature->GetMap()->GetUnit(*itr))
            {
                if (pCreature->GetEntry() == NPC_ANCIENT_WATER_SPIRIT || pCreature->GetEntry() == NPC_SNAPLASHER ||
                    pCreature->GetEntry() == NPC_STORM_LASHER)
                    ((TemporarySummon *)pCreature)->UnSummon();
                else if (pCreature->isAlive())
                    pCreature->ForcedDespawn();
            }
        }

        m_lAdds.clear();
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        if (urand(0, 1))
            DoScriptText(urand(0, 1) ? SAY_SLAY1 : SAY_SLAY2, m_creature);
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (uiDamage >= m_creature->GetHealth())
        {
            //m_creature->CombatStop(true);
            //m_creature->RemoveAllAuras();
            //m_creature->ClearAllReactives();
            //m_creature->ClearComboPointHolders();
            //m_creature->StopMoving();
            //m_creature->GetMotionMaster()->Clear();
            //m_creature->GetMotionMaster()->MoveIdle();
            //m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            //m_creature->setFaction(35);
            //uiDamage = 0;

            // Summon Freya's Gift here

            DoScriptText(SAY_DEFEAT, m_creature);

            if (m_pInstance)
                m_pInstance->SetData(TYPE_FREYA, DONE);
        }
    }

    void JustDied(Unit* pKiller)
    {
        GameObject* pGo = GetClosestGameObjectWithEntry(m_creature,m_bIsRegularMode ? 194325 : 194329,200.0f);
        if (pGo)
            m_pInstance->DoRespawnGameObject(pGo->GetGUID(),604800);
    }

    void JustSummoned(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
            case NPC_ANCIENT_CONSERVATOR:
            case NPC_ANCIENT_WATER_SPIRIT:
            case NPC_STORM_LASHER:
            case NPC_SNAPLASHER:
            case NPC_DETONATING_LASHER:
                pSummoned->SetInCombatWithZone();
            case NPC_EONAR_GIFT:
                m_lAdds.insert(pSummoned->GetObjectGuid());
                break;
        };

        // Visuals, etc.
        switch(pSummoned->GetEntry())
        {
            case NPC_SNAPLASHER:
                pSummoned->CastSpell(pSummoned, SPELL_LASHER_REVIVE_VISUAL, true);
                break;
            case NPC_STORM_LASHER:
                pSummoned->CastSpell(pSummoned, SPELL_STORM_LASHER_REVIVE_VISUAL, true);
                pSummoned->CastSpell(pSummoned, SPELL_STORM_LASHER_VISUAL, true);
                break;
            case NPC_ANCIENT_WATER_SPIRIT:
                pSummoned->CastSpell(pSummoned, SPELL_LASHER_WATER_REVIVE_VISUAL, true);
                break;
            case NPC_ANCIENT_CONSERVATOR:
                //pSummoned->CastSpell(pSummoned, SPELL_HEALTHY_SPORE_SUMMON_PERIODIC, true);
                break;
        };
    }

    // TODO: Use spells instead
    void RemoveAttunedToNatureStacks(uint8 numStacks)
    {
        if (Aura* aura = m_creature->GetAura(SPELL_ATTUNED_TO_NATURE, SpellEffectIndex(0)))
        {
            uint8 stacks = aura->GetStackAmount();
            stacks = stacks > numStacks ? stacks - numStacks : 0;

            if (stacks)
                aura->GetHolder()->SetStackAmount(stacks);
            else
                m_creature->RemoveAurasDueToSpell(SPELL_ATTUNED_TO_NATURE);
        }
    }

    void CheckAliveMobs()
    {
        bool m1, m2, m3;
        m1 = false;
        m2 = false;
        m3 = false;
        for(std::set<ObjectGuid>::const_iterator itr = m_lAdds.begin(); itr != m_lAdds.end(); ++itr)
        {
            if (Creature* pCreature = (Creature*)m_creature->GetMap()->GetUnit(*itr))
            {
                if (pCreature->GetEntry() == NPC_ANCIENT_WATER_SPIRIT && !pCreature->isAlive())
                    m1 = true;
                else if (pCreature->GetEntry() == NPC_SNAPLASHER  && !pCreature->isAlive())
                    m2 = true;
                else if (pCreature->GetEntry() == NPC_STORM_LASHER && !pCreature->isAlive())
                    m3 = true;
            }
        }

        if (m1 && m2 && m3)
        {
            RemoveAttunedToNatureStacks(30);
            for(std::set<ObjectGuid>::const_iterator itr = m_lAdds.begin(); itr != m_lAdds.end(); ++itr)
            {
                if (Creature* pCreature = (Creature*)m_creature->GetMap()->GetUnit(*itr))
                {
                    if (pCreature->GetEntry() == NPC_ANCIENT_WATER_SPIRIT && !pCreature->isAlive())
                        ((TemporarySummon *)pCreature)->UnSummon();
                    else if (pCreature->GetEntry() == NPC_SNAPLASHER  && !pCreature->isAlive())
                        ((TemporarySummon *)pCreature)->UnSummon();
                    else if (pCreature->GetEntry() == NPC_STORM_LASHER && !pCreature->isAlive())
                        ((TemporarySummon *)pCreature)->UnSummon();
                }
            }
        }
    }

    void SummonedCreatureJustDied(Creature* pSummoned)
    {
        switch(pSummoned->GetEntry())
        {
            case NPC_ANCIENT_CONSERVATOR:
                m_lAdds.erase(pSummoned->GetObjectGuid());
                RemoveAttunedToNatureStacks(25);
                pSummoned->ForcedDespawn();
                break;
            case NPC_ANCIENT_WATER_SPIRIT:
            case NPC_STORM_LASHER:
            case NPC_SNAPLASHER:
                m_lAdds.erase(pSummoned->GetObjectGuid());
                RemoveAttunedToNatureStacks(10);
                pSummoned->ForcedDespawn();
                break;
            case NPC_DETONATING_LASHER:
                m_lAdds.erase(pSummoned->GetObjectGuid());
                RemoveAttunedToNatureStacks(2);
                pSummoned->ForcedDespawn();
                break;
            case NPC_EONAR_GIFT:
                m_lAdds.erase(pSummoned->GetObjectGuid());
                pSummoned->ForcedDespawn();
                break;
        };
    }

    void SpellHitTarget(Unit*, const SpellEntry* spell)
    {
        switch(spell->Id)
        {
            case 62685: DoScriptText(SAY_SUMMON1, m_creature); break;
            case 62686: DoScriptText(SAY_SUMMON2, m_creature); break;
            case 62688: DoScriptText(SAY_SUMMON3, m_creature); break;
        };
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiSunbeamTimer < uiDiff)
        {
            CheckAliveMobs();
            if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                if (DoCastSpellIfCan(target, m_bIsRegularMode ? SPELL_SUNBEAM : H_SPELL_SUNBEAM) == CAST_OK)
                    m_uiSunbeamTimer = urand(10000, 18000);
            }
        }
        else
            m_uiSunbeamTimer -= uiDiff;

        if (m_uiSummonGiftTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SUMMON_LIFEBINDERS_GIFT : H_SPELL_SUMMON_LIFEBINDERS_GIFT) == CAST_OK)
                m_uiSummonGiftTimer = urand(35000, 48000);
        }
        else
            m_uiSummonGiftTimer -= uiDiff;

        // Phase 1
        if (m_uiWaveCount < 6)
        {
            if (m_uiSummonTimer < uiDiff)
            {
                do
                {
                    m_uiSummonType = (rand()%3);
                }
                while ( (m_uiSummonType == m_uiLastSummonType) || (m_uiSummonType == m_uiLastSummonType2));
                // small freeze
                
                switch (m_uiSummonType)
                {
                case 0:
                    {
                        for (uint8 i = 0; i < 12; i++)
                        {
                            Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                            if (!target)
                                continue;
                            m_creature->SummonCreature(NPC_DETONATING_LASHER, target->GetPositionX(), target->GetPositionY(), target->GetPositionZ(),target->GetAngle(m_creature),TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 20000);
                        }
                        break;
                    }
                case  1:
                    {
                        Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                        if (!target)
                            break;
                        m_creature->SummonCreature(NPC_ANCIENT_WATER_SPIRIT,target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), target->GetAngle(m_creature), TEMPSUMMON_MANUAL_DESPAWN, 20000);
                        m_creature->SummonCreature(NPC_STORM_LASHER,target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), target->GetAngle(m_creature), TEMPSUMMON_MANUAL_DESPAWN, 20000);
                        m_creature->SummonCreature(NPC_SNAPLASHER,target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), target->GetAngle(m_creature), TEMPSUMMON_MANUAL_DESPAWN, 20000);
                        break;
                    }
                case  2:
                    {
                        Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
                        if (!target)
                            break;
                        m_creature->SummonCreature(NPC_ANCIENT_CONSERVATOR,target->GetPositionX(),target->GetPositionY(),target->GetPositionZ(), target->GetAngle(m_creature), TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 20000);                    
                        break;
                    }
                default:
                    break;
                }
                
                if (m_uiWaveCount == 0 || m_uiWaveCount == 3)
                    m_uiLastSummonType = m_uiSummonType;
                else if (m_uiWaveCount == 1 || m_uiWaveCount == 4)
                    m_uiLastSummonType2 = m_uiSummonType;
                else 
                    m_uiSummonType = (rand()%3);

                ++m_uiWaveCount;
                m_uiSummonTimer = 60000;
            }
            else
                m_uiSummonTimer -= uiDiff;
        }
        else // Phase 2
        {
            if (m_uiNatureBombsTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, SPELL_NATURE_BOMB) == CAST_OK)
                    m_uiNatureBombsTimer = urand(15000, 16000);
            }
            else
                m_uiNatureBombsTimer -= uiDiff;
        }

        if (m_uiBerserkTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BERSERK) == CAST_OK)
                m_uiBerserkTimer = 30*MINUTE*IN_MILLISECONDS;
        }
        else
            m_uiBerserkTimer -= uiDiff;

        if (m_uiModeMask & HARD_MODE_IRONBRANCH)
        {
            if (m_uiIronRootsTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_IRON_ROOTS_HM : H_SPELL_IRON_ROOTS_HM) == CAST_OK)
                    m_uiIronRootsTimer = urand(50000, 70000);
            }
            else
                m_uiIronRootsTimer -= uiDiff;
        }

        if (m_uiModeMask & HARD_MODE_STONEBARK)
        {
            if (m_uiGroundTremorTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_GROUND_TREMOR_HM : H_SPELL_GROUND_TREMOR_HM) == CAST_OK)
                    m_uiGroundTremorTimer = urand(25000, 45000);
            }
            else
                m_uiGroundTremorTimer -= uiDiff;
        }

        if (m_uiModeMask & HARD_MODE_BRIGHTLEAF)
        {
            if (m_uiUnstableEnergyTimer < uiDiff)
            {
                if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_UNSTABLE_SUN_BEAM_HM : H_SPELL_UNSTABLE_SUN_BEAM_HM) == CAST_OK)
                    m_uiUnstableEnergyTimer = urand(30000, 50000);
            }
            else
                m_uiUnstableEnergyTimer -= uiDiff;
        }

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_elder_ironbranch_AI : public ScriptedAI
{
    mob_elder_ironbranch_AI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 m_uiImpaleTimer;
    uint32 m_uiIronRootsTimer;
    uint32 m_uiThornSwarmTimer;

    void Reset()
    {
        m_uiImpaleTimer = 15000;
        m_uiIronRootsTimer = urand(10000, 20000);
        m_uiThornSwarmTimer = urand(5000, 10000);
    }

    void Aggro(Unit*)
    {
        DoScriptText(SAY_IRONBRANCH_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        if (urand(0, 1))
            DoScriptText(urand(0, 1) ? SAY_IRONBRANCH_SLAY1 : SAY_IRONBRANCH_SLAY2, m_creature);
    }

    void JustDied(Unit*)
    {
        DoScriptText(SAY_IRONBRANCH_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiImpaleTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), m_bIsRegularMode ? SPELL_IMPALE : H_SPELL_IMPALE) == CAST_OK)
                m_uiImpaleTimer = urand(20000, 25000);
        }
        else
            m_uiImpaleTimer -= uiDiff;

        if (m_uiIronRootsTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_IRON_ROOTS : H_SPELL_IRON_ROOTS) == CAST_OK)
                m_uiIronRootsTimer = urand(18000, 28000);
        }
        else
            m_uiIronRootsTimer -= uiDiff;

        if (m_uiThornSwarmTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0)) // probably casted only on non-melee targets
            {
                if (DoCastSpellIfCan(pTarget, m_bIsRegularMode ? SPELL_THORN_SWARM : H_SPELL_THORN_SWARM) == CAST_OK)
                    m_uiThornSwarmTimer = urand(6000, 10000);
            }
        }
        else
            m_uiThornSwarmTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_elder_stonebark_AI : public ScriptedAI
{
    mob_elder_stonebark_AI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 m_uiFistsOfStoneTimer;
    uint32 m_uiGroundTremorTimer;
    uint32 m_uiPetrifiedBarkTimer;

    void Reset()
    {
        m_uiFistsOfStoneTimer = urand(20000, 25000);
        m_uiGroundTremorTimer = urand(6000, 12000);
        m_uiPetrifiedBarkTimer = urand(10000, 25000);
    }

    void Aggro(Unit*)
    {
        DoScriptText(SAY_STONEBARK_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        if (urand(0, 1))
            DoScriptText(urand(0, 1) ? SAY_STONEBARK_SLAY1 : SAY_STONEBARK_SLAY2, m_creature);
    }

    void JustDied(Unit*)
    {
        DoScriptText(SAY_STONEBARK_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFistsOfStoneTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_FISTS_OF_STONE) == CAST_OK)
                m_uiFistsOfStoneTimer = urand(38000, 50000);
        }
        else
            m_uiFistsOfStoneTimer -= uiDiff;

        if (m_uiGroundTremorTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_GROUND_TREMOR : H_SPELL_GROUND_TREMOR) == CAST_OK)
                m_uiGroundTremorTimer = urand(15000, 35000);
        }
        else
            m_uiGroundTremorTimer -= uiDiff;

        if (m_uiPetrifiedBarkTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_PETRIFIED_BARK : H_SPELL_PETRIFIED_BARK) == CAST_OK)
                m_uiPetrifiedBarkTimer = urand(25000, 40000);
        }
        else
            m_uiPetrifiedBarkTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_elder_brightleaf_AI : public ScriptedAI
{
    mob_elder_brightleaf_AI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 m_uiBrightleafFluxTimer;
    uint32 m_uiSolarFlareTimer;
    uint32 m_uiSummonSunBeamTimer;
    uint32 m_uiBeamCheckTimer;

    void Reset()
    {
        m_uiBrightleafFluxTimer = 5000;
        m_uiSolarFlareTimer = urand(8000, 10000);
        m_uiSummonSunBeamTimer = urand(6000, 15000);
        m_uiBeamCheckTimer = 7000;
    }

    void Aggro(Unit*)
    {
        DoScriptText(SAY_BRIGHTLEAF_AGGRO, m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim->GetTypeId() != TYPEID_PLAYER)
            return;

        if (urand(0, 1))
            DoScriptText(urand(0, 1) ? SAY_BRIGHTLEAF_SLAY1 : SAY_BRIGHTLEAF_SLAY2, m_creature);
    }

    void JustDied(Unit*)
    {
        DoScriptText(SAY_BRIGHTLEAF_DEATH, m_creature);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiBeamCheckTimer < uiDiff)
        {
            if (Creature* pSunBeam = GetClosestCreatureWithEntry(m_creature, NPC_UNSTABLE_SUN_BEAM, 4.0f))
            {
                if (!m_creature->HasAura(SPELL_PHOTOSYNTHESIS))
                    m_creature->CastSpell(m_creature, SPELL_PHOTOSYNTHESIS, true);
            }
            else
                m_creature->RemoveAurasDueToSpell(SPELL_PHOTOSYNTHESIS);

            m_uiBeamCheckTimer = 3000;
        }
        else
            m_uiBeamCheckTimer -= uiDiff;

        if (m_uiBrightleafFluxTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, SPELL_BRIGHTLEAF_FLUX) == CAST_OK)
                m_uiBrightleafFluxTimer = 5000;
        }
        else
            m_uiBrightleafFluxTimer -= uiDiff;

        if (m_uiSolarFlareTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SOLAR_FLARE : H_SPELL_SOLAR_FLARE) == CAST_OK)
                m_uiSolarFlareTimer = urand(5000, 10000);
        }
        else
            m_uiSolarFlareTimer -= uiDiff;

        if (m_uiSummonSunBeamTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_SUMMON_UNSTABLE_SUNBEAM : H_SPELL_SUMMON_UNSTABLE_SUNBEAM) == CAST_OK)
                m_uiSummonSunBeamTimer = urand(15000, 25000);
        }
        else
            m_uiSummonSunBeamTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }
};

struct MANGOS_DLL_DECL mob_detonating_lasher_AI : public ScriptedAI
{
    mob_detonating_lasher_AI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    uint32 m_uiChangeTargetTimer;
    uint32 m_uiFlameLashTimer;

    void Reset()
    {
        m_uiChangeTargetTimer = urand(10000, 15000);
        m_uiFlameLashTimer = urand(8000, 16000);
    }

    void DamageTaken(Unit* pDoneBy, uint32& uiDamage)
    {
        if (uiDamage >= m_creature->GetHealth())
            DoCast(m_creature, m_bIsRegularMode ? SPELL_DETONATE : H_SPELL_DETONATE, true);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiFlameLashTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature->getVictim(), SPELL_FLAME_LASH) == CAST_OK)
                m_uiFlameLashTimer = urand(12000, 20000);
        }
        else
            m_uiFlameLashTimer -= uiDiff;

        if (m_uiChangeTargetTimer < uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 1))
            {
                DoResetThreat();
                m_creature->AddThreat(pTarget, 10000.0f);
                AttackStart(pTarget);
                m_uiChangeTargetTimer = urand(6000, 12000);
            }
        }
        else
            m_uiChangeTargetTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit*)
    {
        if (Creature *pFreya = m_pInstance->instance->GetCreature(m_pInstance->GetData64(TYPE_FREYA)))
            if (pFreya->isAlive())
                ((boss_freya_AI*)pFreya->AI())->RemoveAttunedToNatureStacks(2);
        m_creature->ForcedDespawn();        
    }
};

struct MANGOS_DLL_DECL mob_iron_roots_AI : public Scripted_NoMovementAI
{
    mob_iron_roots_AI(Creature *pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;

    void Reset() {}
    void MoveInLineOfSight(Unit*) {}
    void UpdateAI(const uint32) {}

    void JustDied(Unit*)
    {
        //if (!m_creature->isTemporarySummon())
            //return;

        TemporarySummon* summon = (TemporarySummon*)m_creature;

        if (Unit* pSummoner = m_creature->GetMap()->GetUnit(summon->GetSummonerGuid()))
        {
            switch(m_creature->GetEntry())
            {
                case NPC_IRON_ROOTS:
                    pSummoner->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_IRON_ROOTS_AURA : H_SPELL_IRON_ROOTS_AURA);
                case NPC_STRENGTHENED_IRON_ROOTS:
                    pSummoner->RemoveAurasDueToSpell(m_bIsRegularMode ? SPELL_IRON_ROOTS_AURA_HM : H_SPELL_IRON_ROOTS_AURA_HM);
                    break;
            };
        }
    }
};

struct MANGOS_DLL_DECL mob_eonars_gift_AI : public Scripted_NoMovementAI
{
    mob_eonars_gift_AI(Creature *pCreature) : Scripted_NoMovementAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    uint32 m_uiHealTimer;
    uint32 m_uiGrowTimer;
    float m_fSize;

    void Reset()
    {
        m_uiHealTimer = 12000;
        m_uiGrowTimer = 0;
        m_fSize = 0.2f;
        m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, m_fSize);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_uiHealTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_LIFEBINDERS_GIFT : H_SPELL_LIFEBINDERS_GIFT) == CAST_OK)
                m_uiHealTimer = 12000;
            //m_creature->ForcedDespawn();
        }
        else
            m_uiHealTimer -= uiDiff;

        if (m_fSize < 2.5f)
        {
            if (m_uiGrowTimer < uiDiff)
            {
                m_fSize += 0.1f;
                m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, m_fSize);
                m_uiGrowTimer = 500;
            }
            else
                m_uiGrowTimer -= uiDiff;
        }
    }

    void JustDied(Unit*)
    {
        m_creature->ForcedDespawn();        
    }
};

struct MANGOS_DLL_DECL mob_nature_bomb_AI : public ScriptedAI
{
    mob_nature_bomb_AI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    bool m_bNeedDespawn;
    uint32 m_uiExplodeTimer;

    void Reset()
    {
        m_bNeedDespawn = false;
        m_uiExplodeTimer = 10000;
        m_creature->CastSpell(m_creature, 64600, true);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bNeedDespawn)
            return;

        if (m_uiExplodeTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_NATURE_BOMB_EXPLOSION : H_SPELL_NATURE_BOMB_EXPLOSION) == CAST_OK)
            {
                if (GameObject* pGO = GetClosestGameObjectWithEntry(m_creature, GO_NATURE_BOMB, 5.0f))
                    pGO->UseDoorOrButton();

                m_creature->ForcedDespawn(5000);
                m_bNeedDespawn = true;
            }
        }
        else
            m_uiExplodeTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL mob_healthy_spore_AI : public ScriptedAI
{
    mob_healthy_spore_AI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bNeedDespawn;
    bool m_bIsRegularMode;
    uint32 m_uiGrowTimer;
    uint32 m_uiDespawnTimer;
    float m_fSize;

    void Reset()
    {
        m_bNeedDespawn = false;
        m_uiGrowTimer = 0;
        m_fSize = 0.2f;
        m_uiDespawnTimer = 25000;
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, m_fSize);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bNeedDespawn)
            return;

        if (!m_creature->HasAura(62541))
            DoCast(m_creature,62541,true);

        if (!m_creature->HasAura(62538))
            DoCast(m_creature,62538,true);

        if (m_fSize < 3.0f)
        {
            if (m_uiGrowTimer < uiDiff)
            {
                m_fSize += 0.1f;
                m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, m_fSize);
                m_uiGrowTimer = 500;
            }
            else
                m_uiGrowTimer -= uiDiff;
        }

        if (m_uiDespawnTimer < uiDiff)
        {
            m_creature->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.2f);
            m_creature->ForcedDespawn(2000);
            m_bNeedDespawn = true;
        }
        else
            m_uiDespawnTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL mob_unstable_sun_beam_AI : public ScriptedAI
{
    mob_unstable_sun_beam_AI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;
    bool m_bNeedDespawn;
    uint32 m_uiUnstableEnergyTimer;

    void Reset()
    {
        m_bNeedDespawn = false;
        m_creature->CastSpell(m_creature, SPELL_UNSTABLE_SUN_BEAM_AURA, true);
        m_uiUnstableEnergyTimer = urand(8000, 25000);
    }

    void MoveInLineOfSight(Unit*) {}
    void AttackStart(Unit*) {}

    void UpdateAI(const uint32 uiDiff)
    {
        if (m_bNeedDespawn)
            return;

        if (m_uiUnstableEnergyTimer < uiDiff)
        {
            if (DoCastSpellIfCan(m_creature, m_bIsRegularMode ? SPELL_UNSTABLE_ENERGY : H_SPELL_UNSTABLE_ENERGY) == CAST_OK)
            {
                m_bNeedDespawn = true;
                m_creature->ForcedDespawn(2000);
            }
        }
        else
            m_uiUnstableEnergyTimer -= uiDiff;
    }
};

struct MANGOS_DLL_DECL mob_sun_beam_AI : public ScriptedAI
{
    mob_sun_beam_AI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    bool m_bIsRegularMode;

    void Reset()
    {
        m_creature->CastSpell(m_creature, m_bIsRegularMode ? SPELL_UNSTABLE_ENERGY_HM : H_SPELL_UNSTABLE_ENERGY_HM, true);
    }

    void MoveInLineOfSight(Unit*) {}
    void AttackStart(Unit*) {}
    void UpdateAI(const uint32) {}
};

struct MANGOS_DLL_DECL mob_ancient_conservator_AI : public ScriptedAI
{
    mob_ancient_conservator_AI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    uint32 m_uiNatureFuryTimer;
    uint32 m_uiGripTimer;
    uint32 m_uiSporeTimer;
    uint32 m_uiSporeCount;

    void Reset()
    {
        m_uiNatureFuryTimer = urand(8000, 12000);
        m_uiGripTimer = 3500;
        m_uiSporeTimer = 600;
        m_uiSporeCount = 0;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiGripTimer <= uiDiff)
        {
            DoCast(m_creature, 62532,true);
            m_uiGripTimer = 80000;
        } else m_uiGripTimer -= uiDiff;

        if (m_uiSporeTimer <= uiDiff)
        {
            // хз почему не работает
            /*switch (rand()%4)
            {
            case 0:
                DoCast(m_creature, 62591, true);
                break;
            case 1:
                DoCast(m_creature, 62592, true);
                break;
            case 2:
                DoCast(m_creature, 62593, true);
                break;
            case 3:
                DoCast(m_creature, 62582, true);
                break;
            default:
                break;
            }*/

            for (int8 i = 0; i < 3; i++)
            {
                float radius = 30.0f;
                radius *= sqrt(rand_norm());
                float angle = 2.0 * M_PI * rand_norm();
                float dest_x = m_creature->GetPositionX() + cos(angle) * radius;
                float dest_y = m_creature->GetPositionY() + sin(angle) * radius;
                float dest_z = m_creature->GetPositionZ();
                m_creature->UpdateGroundPositionZ(dest_x, dest_y, dest_z);
                m_creature->SummonCreature(33215, dest_x, dest_y, dest_z, angle, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 25000);
            }

            m_uiSporeCount++;
            if (m_uiSporeCount < (m_bIsRegularMode ? 10 : 15))
                m_uiSporeTimer = 3000;
            else
                m_uiSporeTimer = 7000;
        } else m_uiSporeTimer -= uiDiff;

        if (m_uiNatureFuryTimer <= uiDiff)
        {
            Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0);
            if (target)
                DoCast(target, m_bIsRegularMode?62589:63571, true);
            m_uiNatureFuryTimer = 10000;
        } else m_uiNatureFuryTimer -= uiDiff;

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit*)
    {
        if (!m_pInstance)
            return;
        if (Creature *pFreya = m_pInstance->instance->GetCreature(m_pInstance->GetData64(TYPE_FREYA)))
            if (pFreya->isAlive())
                ((boss_freya_AI*)pFreya->AI())->RemoveAttunedToNatureStacks(25);
        m_creature->ForcedDespawn();        
    }
};


struct MANGOS_DLL_DECL mob_ancient_water_spirit_AI : public ScriptedAI
{
    mob_ancient_water_spirit_AI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    uint32 m_uiChangeTargetTimer;
    uint32 DeathCheck_Timer;
    ObjectGuid StormLasherGuid;
    ObjectGuid SnaplasherGuid;
    bool m_bIsDeath;
    bool m_bIsDeath2;

    void Reset()
    {
        m_bIsDeath = false;
        m_bIsDeath2 = false;
        StormLasherGuid = 0;
        SnaplasherGuid = 0;
        DeathCheck_Timer = 1000;
        m_uiChangeTargetTimer = urand(4000, 10000);
    }

    void Aggro(Unit *who)
    {
        if (Creature* Snaplasher = GetClosestCreatureWithEntry(m_creature,32916,250))
            SnaplasherGuid = Snaplasher->GetObjectGuid();
        if (Creature* StormLasher = GetClosestCreatureWithEntry(m_creature,32919,250))
            StormLasherGuid = StormLasher->GetObjectGuid();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_pInstance)
            return;

        if (m_uiChangeTargetTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(pTarget, m_bIsRegularMode ? 62653 : 62935);
                m_uiChangeTargetTimer = urand(8000, 12000);
            }
        }
        else
            m_uiChangeTargetTimer -= uiDiff;

        if (DeathCheck_Timer < uiDiff)
        {
            if (Creature* Snaplasher = m_pInstance->instance->GetCreature(SnaplasherGuid))
            {
                if (!Snaplasher->isAlive() && !m_bIsDeath)
                {
                    m_bIsDeath = true;
                }
                else if (!Snaplasher->isAlive() && m_bIsDeath)
                {
                    Snaplasher->Respawn();
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        Snaplasher->Attack(pTarget, true);
                    m_bIsDeath = false;
                }
            }
            if (Creature* StormLasher = m_pInstance->instance->GetCreature(StormLasherGuid))
            {
                if (!StormLasher->isAlive() && !m_bIsDeath2)
                {
                    m_bIsDeath2 = true;
                }
                else if (!StormLasher->isAlive() && m_bIsDeath2)
                {
                    StormLasher->Respawn();
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        StormLasher->Attack(pTarget, true);
                    m_bIsDeath2 = false;
                }
            }

            if (m_bIsDeath2 || m_bIsDeath)
                DeathCheck_Timer = 8000;
            else
                DeathCheck_Timer = 1000;
        }else DeathCheck_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit*)
    {
        /*if (Creature *pFreya = m_pInstance->instance->GetCreature(m_pInstance->GetData64(TYPE_FREYA)))
            if (pFreya->isAlive())
                ((boss_freya_AI*)pFreya->AI())->RemoveAttunedToNatureStacks(10);*/
        //m_creature->ForcedDespawn();        
    }
};

struct MANGOS_DLL_DECL mob_storm_lasher_AI : public ScriptedAI
{
    mob_storm_lasher_AI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    uint32 m_uiStormboltTimer;
    uint32 m_uiLightningLashTimer;
    uint32 DeathCheck_Timer;
    ObjectGuid AncientWaterSpiritGuid;
    ObjectGuid SnaplasherGuid;
    bool m_bIsDeath;
    bool m_bIsDeath2;

    void Reset()
    {
        m_bIsDeath = false;
        m_bIsDeath2 = false;
        AncientWaterSpiritGuid = 0;
        SnaplasherGuid = 0;
        DeathCheck_Timer = 1000;
        m_uiStormboltTimer = urand(4000, 6000);
        m_uiLightningLashTimer = urand(6000, 10000);
    }

    void Aggro(Unit *who)
    {
        if (Creature* Snaplasher = GetClosestCreatureWithEntry(m_creature,32916,250))
            SnaplasherGuid = Snaplasher->GetObjectGuid();
        if (Creature* AncientWaterSpirit = GetClosestCreatureWithEntry(m_creature,33202,250))
            AncientWaterSpiritGuid = AncientWaterSpirit->GetObjectGuid();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_pInstance)
            return;

        if (m_uiStormboltTimer <= uiDiff)
        {
            DoCast(m_creature->getVictim(), m_bIsRegularMode ? 62649 : 62938);
            m_uiStormboltTimer = urand(4000, 7000);
        }
        else
            m_uiStormboltTimer -= uiDiff;

        if (m_uiLightningLashTimer <= uiDiff)
        {
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
            {
                DoCast(pTarget, m_bIsRegularMode ? 62648 : 62939, true);
                m_uiLightningLashTimer = urand(8000, 12000);
            }
        }
        else
            m_uiLightningLashTimer -= uiDiff;

        if (DeathCheck_Timer < uiDiff)
        {
            if (Creature* Snaplasher = m_pInstance->instance->GetCreature(SnaplasherGuid))
            {
                if (!Snaplasher->isAlive() && !m_bIsDeath)
                {
                    m_bIsDeath = true;
                }
                else if (!Snaplasher->isAlive() && m_bIsDeath)
                {
                    Snaplasher->Respawn();
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        Snaplasher->Attack(pTarget, true);
                    m_bIsDeath = false;
                }
            }
            if (Creature* AncientWaterSpirit = m_pInstance->instance->GetCreature(AncientWaterSpiritGuid))
            {
                if (!AncientWaterSpirit->isAlive() && !m_bIsDeath2)
                {
                    m_bIsDeath2 = true;
                }
                else if (!AncientWaterSpirit->isAlive() && m_bIsDeath2)
                {
                    AncientWaterSpirit->Respawn();
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        AncientWaterSpirit->Attack(pTarget, true);
                    m_bIsDeath2 = false;
                }
            }

            if (m_bIsDeath2 || m_bIsDeath)
                DeathCheck_Timer = 8000;
            else
                DeathCheck_Timer = 1000;
        }else DeathCheck_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit*)
    {
        /*if (Creature *pFreya = m_pInstance->instance->GetCreature(m_pInstance->GetData64(TYPE_FREYA)))
            if (pFreya->isAlive())
                ((boss_freya_AI*)pFreya->AI())->RemoveAttunedToNatureStacks(10);*/
        //m_creature->ForcedDespawn();        
    }
};

struct MANGOS_DLL_DECL mob_snaplasher_AI : public ScriptedAI
{
    mob_snaplasher_AI(Creature *pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    uint32 DeathCheck_Timer;
    ObjectGuid StormLasherGuid;
    ObjectGuid AncientWaterSpiritGuid;
    bool m_bIsDeath;
    bool m_bIsDeath2;

    void Reset()
    {
        m_bIsDeath = false;
        m_bIsDeath2 = false;
        StormLasherGuid = 0;
        AncientWaterSpiritGuid = 0;
        DeathCheck_Timer = 1000;
    }

    void Aggro(Unit *who)
    {
        if (Creature* AncientWaterSpirit = GetClosestCreatureWithEntry(m_creature,33202,250))
            AncientWaterSpiritGuid = AncientWaterSpirit->GetObjectGuid();
        if (Creature* StormLasher = GetClosestCreatureWithEntry(m_creature,32919,250))
            StormLasherGuid = StormLasher->GetObjectGuid();
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (!m_pInstance)
            return;

        if (!m_creature->HasAura(m_bIsRegularMode?62664:64191))
            DoCast(m_creature,m_bIsRegularMode?62664:64191, true );

        if (DeathCheck_Timer < uiDiff)
        {
            if (Creature* AncientWaterSpirit = m_pInstance->instance->GetCreature(AncientWaterSpiritGuid))
            {
                if (!AncientWaterSpirit->isAlive() && !m_bIsDeath)
                {
                    m_bIsDeath = true;
                }
                else if (!AncientWaterSpirit->isAlive() && m_bIsDeath)
                {
                    AncientWaterSpirit->Respawn();
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        AncientWaterSpirit->Attack(pTarget, true);
                    m_bIsDeath = false;
                }
            }
            if (Creature* StormLasher = m_pInstance->instance->GetCreature(StormLasherGuid))
            {
                if (!StormLasher->isAlive() && !m_bIsDeath2)
                {
                    m_bIsDeath2 = true;
                }
                else if (!StormLasher->isAlive() && m_bIsDeath2)
                {
                    StormLasher->Respawn();
                    if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                        StormLasher->Attack(pTarget, true);
                    m_bIsDeath2 = false;
                }
            }

            if (m_bIsDeath2 || m_bIsDeath)
                DeathCheck_Timer = 8000;
            else
                DeathCheck_Timer = 1000;
        }else DeathCheck_Timer -= uiDiff;

        DoMeleeAttackIfReady();
    }

    void JustDied(Unit*)
    {
        /*if (Creature *pFreya = m_pInstance->instance->GetCreature(m_pInstance->GetData64(TYPE_FREYA)))
            if (pFreya->isAlive())
                ((boss_freya_AI*)pFreya->AI())->RemoveAttunedToNatureStacks(10);*/
        //m_creature->ForcedDespawn();        
    }
};

CreatureAI* GetAI_boss_freya(Creature* pCreature)
{
    return new boss_freya_AI(pCreature);
}

CreatureAI* GetAI_mob_elder_ironbranch(Creature* pCreature)
{
    return new mob_elder_ironbranch_AI(pCreature);
}

CreatureAI* GetAI_mob_elder_stonebark(Creature* pCreature)
{
    return new mob_elder_stonebark_AI(pCreature);
}

CreatureAI* GetAI_mob_elder_brightleaf(Creature* pCreature)
{
    return new mob_elder_brightleaf_AI(pCreature);
}

CreatureAI* GetAI_mob_detonating_lasher(Creature* pCreature)
{
    return new mob_detonating_lasher_AI(pCreature);
}

CreatureAI* GetAI_mob_iron_roots(Creature* pCreature)
{
    return new mob_iron_roots_AI(pCreature);
}

CreatureAI* GetAI_mob_unstable_sun_beam(Creature* pCreature)
{
    return new mob_unstable_sun_beam_AI(pCreature);
}

CreatureAI* GetAI_mob_eonars_gift(Creature* pCreature)
{
    return new mob_eonars_gift_AI(pCreature);
}

CreatureAI* GetAI_mob_healthy_spore(Creature* pCreature)
{
    return new mob_healthy_spore_AI(pCreature);
}

CreatureAI* GetAI_mob_nature_bomb(Creature* pCreature)
{
    return new mob_nature_bomb_AI(pCreature);
}

CreatureAI* GetAI_mob_sun_beam(Creature* pCreature)
{
    return new mob_sun_beam_AI(pCreature);
}

CreatureAI* GetAI_mob_ancient_conservator(Creature* pCreature)
{
    return new mob_ancient_conservator_AI(pCreature);
}
CreatureAI* GetAI_mob_ancient_water_spirit(Creature* pCreature)
{
    return new mob_ancient_water_spirit_AI(pCreature);
}

CreatureAI* GetAI_mob_storm_lasher(Creature* pCreature)
{
    return new mob_storm_lasher_AI(pCreature);
}
CreatureAI* GetAI_mob_snaplasher(Creature* pCreature)
{
    return new mob_snaplasher_AI(pCreature);
}

void AddSC_boss_freya()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_freya";
    newscript->GetAI = &GetAI_boss_freya;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_elder_ironbranch";
    newscript->GetAI = &GetAI_mob_elder_ironbranch;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_elder_stonebark";
    newscript->GetAI = &GetAI_mob_elder_stonebark;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_elder_brightleaf";
    newscript->GetAI = &GetAI_mob_elder_brightleaf;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_detonating_lasher";
    newscript->GetAI = &GetAI_mob_detonating_lasher;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_iron_roots";
    newscript->GetAI = &GetAI_mob_iron_roots;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_unstable_sun_beam";
    newscript->GetAI = &GetAI_mob_unstable_sun_beam;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_eonars_gift";
    newscript->GetAI = &GetAI_mob_eonars_gift;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_healthy_spore";
    newscript->GetAI = &GetAI_mob_healthy_spore;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "mob_nature_bomb";
    newscript->GetAI = &GetAI_mob_nature_bomb;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_sun_beam";
    newscript->GetAI = &GetAI_mob_sun_beam;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ancient_conservator";
    newscript->GetAI = &GetAI_mob_ancient_conservator;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_ancient_water_spirit";
    newscript->GetAI = &GetAI_mob_ancient_water_spirit;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_storm_lasher";
    newscript->GetAI = &GetAI_mob_storm_lasher;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_snaplasher";
    newscript->GetAI = &GetAI_mob_snaplasher;
    newscript->RegisterSelf();
}
