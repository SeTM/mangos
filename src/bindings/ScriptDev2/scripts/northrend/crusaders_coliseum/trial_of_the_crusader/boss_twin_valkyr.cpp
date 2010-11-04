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
SDName: trial_of_the_crusader
SD%Complete: 80%
SDComment: by /dev/rsa
SDCategory: Crusader Coliseum
EndScriptData */

// Twin pact - heal part not worked now by undefined reason. Need override?
// timers need correct

#include "precompiled.h"
#include "trial_of_the_crusader.h"

enum Equipment
{
    EQUIP_MAIN_1         = 49303,
    EQUIP_OFFHAND_1      = 47146,
    EQUIP_RANGED_1       = 47267,
    EQUIP_MAIN_2         = 45990,
    EQUIP_OFFHAND_2      = 47470,
    EQUIP_RANGED_2       = 47267,
    EQUIP_DONE           = EQUIP_NO_CHANGE,
};

enum Summons
{
    NPC_UNLEASHED_DARK   = 34628,
    NPC_UNLEASHED_LIGHT  = 34630,
};

enum BossSpells
{
    SPELL_TWIN_SPIKE_L     = 66075,
    SPELL_LIGHT_SURGE      = 65766,
    SPELL_SHIELD_LIGHT     = 65858,
    SPELL_TWIN_PACT_L      = 65875,
    SPELL_LIGHT_VORTEX     = 66046,
    SPELL_LIGHT_TOUCH      = 67297,
    SPELL_TWIN_SPIKE_H     = 66069,
    SPELL_DARK_SURGE       = 65768,
    SPELL_SHIELD_DARK      = 65874,
    SPELL_TWIN_PACT_H      = 65876,
    SPELL_DARK_VORTEX      = 66058,
    SPELL_DARK_TOUCH       = 67282,
    SPELL_TWIN_POWER       = 65916,
    SPELL_LIGHT_ESSENCE    = 65686,
    SPELL_DARK_ESSENCE     = 65684,
    SPELL_BERSERK          = 64238,
    SPELL_REMOVE_TOUCH     = 68084,
    SPELL_NONE             = 0,
//
    SPELL_UNLEASHED_DARK   = 65808,
    SPELL_UNLEASHED_LIGHT  = 65795,
};

#define ERROR_INST_DATA           "SD ERROR: Instance Data for IC not set properly; Twin Valkyrias event will not function properly."

struct MANGOS_DLL_DECL mob_twin_valkyrAI : public BSWScriptedAI
{
    mob_twin_valkyrAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

        for(uint8 i = 0; i < 2; ++i)
            Valkyrias[i] = 0;

        Reset();
    }

    ScriptedInstance* m_pInstance;

    uint64 Valkyrias[2];

    uint32 CheckTimer;
    uint32 EndEventTimer;

    uint8 DeathCount;

    bool EventBegun;

    void Reset()
    {
        CheckTimer    = 2000;
        EndEventTimer = 0;
        DeathCount = 0;
        m_creature->SetRespawnDelay(7*HOUR);
        
        EventBegun = false;

        m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
        m_creature->SetDisplayId(11686);

        if (m_pInstance)
        {
            //if already done, not do anything
            if (m_pInstance->GetData(TYPE_VALKIRIES) == DONE)
                return;
            m_pInstance->SetData(TYPE_VALKIRIES, NOT_STARTED);
        }
    }

    void AttackStart(Unit* who) {}
    void MoveInLineOfSight(Unit* who) {}

    void StartEvent(Unit *target)
    {
        if (!m_pInstance)
            return;

        if (target && target->isAlive() && !EventBegun)
        {
            Valkyrias[0] = m_pInstance->GetData64(NPC_LIGHTBANE);
            Valkyrias[1] = m_pInstance->GetData64(NPC_DARKBANE);
            
            for(uint8 i = 0; i < 2; ++i)
            {
                if (Valkyrias[i])
                {
                    Creature* pMember = m_creature->GetMap()->GetCreature(Valkyrias[i]);
                    if (pMember && pMember->isAlive())
                        pMember->AI()->AttackStart(target);
                }
            }

            m_pInstance->SetData(TYPE_VALKIRIES, IN_PROGRESS);
            EventBegun = true;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!EventBegun)
            return;

        if (EndEventTimer)
        {
            if (EndEventTimer <= diff)
            {
                if (DeathCount > 1)
                {
                    if (m_pInstance)
                        m_pInstance->SetData(TYPE_VALKIRIES, DONE);

                    m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                    return;
                }

                Creature* pMember = m_creature->GetMap()->GetCreature(Valkyrias[DeathCount]);
                if (pMember && pMember->isAlive())
                    pMember->DealDamage(pMember, pMember->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);

                ++DeathCount;
                EndEventTimer = 1500;
            }else EndEventTimer -= diff;
        }

        if (CheckTimer)
        {
            if (CheckTimer <= diff)
            {
                uint8 EvadeCheck = 0;
                for(uint8 i = 0; i < 2; ++i)
                {
                    if (Valkyrias[i])
                    {
                        if (Creature* Member = m_creature->GetMap()->GetCreature(Valkyrias[i]))
                        {
                            // This is the evade/death check.
                            if (Member->isAlive() && !Member->SelectHostileTarget())
                                ++EvadeCheck;               //If all members evade, we reset so that players can properly reset the event
                            else if (!Member->isAlive())    //If even one member dies, kill the rest, set instance data, and kill self.
                            {
                                EndEventTimer = 1000;
                                CheckTimer = 0;
                                return;
                            }
                        }
                    }
                }

                if (EvadeCheck > 1)
                    Reset();

                CheckTimer = 2000;
            }else CheckTimer -= diff;
        }
    }
};

struct MANGOS_DLL_DECL boss_twin_valkyrAI : public BSWScriptedAI
{
    boss_twin_valkyrAI(Creature* pCreature) : BSWScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();

        for(uint8 i = 0; i < 2; ++i)
            Valkyrias[i] = 0;

        LoadedGUIDs = false;
    }

    ScriptedInstance* m_pInstance;

    uint64 Valkyrias[2];

    bool LoadedGUIDs;

    void Aggro(Unit* pWho)
    {
        if (m_pInstance)
        {
            if (Creature* pController = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TWINVALKYRIAS)))
            {
                if (mob_twin_valkyrAI* pControlAI = dynamic_cast<mob_twin_valkyrAI*>(pController->AI()))
                    pControlAI->StartEvent(pWho);
            }

            if (m_creature->GetEntry() == NPC_LIGHTBANE)
            {
                DoScriptText(-1713541,m_creature);
                doCast(SPELL_LIGHT_SURGE);
            }
            else if (m_creature->GetEntry() == NPC_DARKBANE)
            {
                DoScriptText(-1713741,m_creature);
                doCast(SPELL_DARK_SURGE);
            }
        }
        else
        {
            error_log(ERROR_INST_DATA);
            EnterEvadeMode();
        }

        m_creature->SetInCombatWithZone();

        // Load GUIDs on first aggro because the creature guids are only set as the creatures are created in world-
        // this means that for each creature, it will attempt to LoadGUIDs even though some of the other creatures are
        // not in world, and thus have no GUID set in the instance data system. Putting it in aggro ensures that all the creatures
        // have been loaded and have their GUIDs set in the instance data system.
        if (!LoadedGUIDs)
            LoadGUIDs();
    }

    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if(pDoneBy->GetGUID() == m_creature->GetGUID()) 
            return;

        if(pDoneBy->GetTypeId() == TYPEID_PLAYER)
        {
            if(pDoneBy->HasAura(SPELL_LIGHT_ESSENCE))
                m_creature->GetGUID() == Valkyrias[0] ? uiDamage /= 2 : uiDamage += uiDamage/2; // if Light target deal 0.5 damage
            else if(pDoneBy->HasAura(SPELL_DARK_ESSENCE))
                m_creature->GetGUID() == Valkyrias[1] ? uiDamage /= 2 : uiDamage += uiDamage/2;
        }

        //uiDamage /= 2;
        for(uint8 i = 0; i < 2; ++i)
        {
            if (Creature* pValkyria = m_creature->GetMap()->GetCreature(Valkyrias[i]))
            {
                if (pValkyria != m_creature && uiDamage < pValkyria->GetHealth())
                    pValkyria->SetHealth(pValkyria->GetHealth() - uiDamage);
            }
        }
    }

    void LoadGUIDs()
    {
        if (!m_pInstance)
        {
            error_log(ERROR_INST_DATA);
            return;
        }

        Valkyrias[0] = m_pInstance->GetData64(NPC_LIGHTBANE);
        Valkyrias[1] = m_pInstance->GetData64(NPC_DARKBANE);

        LoadedGUIDs = true;
    }
};

struct MANGOS_DLL_DECL boss_fjolaAI : public boss_twin_valkyrAI
{
    boss_fjolaAI(Creature* pCreature) : boss_twin_valkyrAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 stage;

    void Reset() 
    {
        if(!m_pInstance) 
            return;
        SetEquipmentSlots(false, EQUIP_MAIN_1, EQUIP_OFFHAND_1, EQUIP_RANGED_1);
        m_creature->SetRespawnDelay(7*HOUR);
        m_pInstance->SetData(DATA_CASTING_VALKYRS, SPELL_NONE);
        stage = 0;
    }

    void JustReachedHome()
    {
        if (!m_pInstance) 
            return;
        m_pInstance->SetData(TYPE_VALKIRIES, FAIL);
        m_pInstance->SetData(DATA_CASTING_VALKYRS, SPELL_NONE);
        if (Creature* pController = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_TWINVALKYRIAS)))
        {
            ((TemporarySummon *)pController)->UnSummon();
        }
        ((TemporarySummon *)m_creature)->UnSummon();
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) 
            return;
        DoScriptText(-1713547,m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim != m_creature)
            DoScriptText(-1713544,m_creature,pVictim);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance) 
            return;
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        
        switch (stage)
        {
         case 0:
             timedCast(SPELL_TWIN_SPIKE_L, uiDiff);

             if (timedQuery(SPELL_LIGHT_TOUCH, uiDiff))
             {
                 if (currentDifficulty == RAID_DIFFICULTY_25MAN_HEROIC || RAID_DIFFICULTY_10MAN_HEROIC)
                     if (Unit* pTarget = doSelectRandomPlayer(SPELL_LIGHT_ESSENCE, false, 50.0f))
                         doCast(SPELL_LIGHT_TOUCH, pTarget);
                 doCast(NPC_UNLEASHED_LIGHT);
             }
             if (m_pInstance->GetData(DATA_CASTING_VALKYRS) == SPELL_NONE )
             {
                 if (timedQuery(SPELL_LIGHT_VORTEX, uiDiff))
                 {
                     m_pInstance->SetData(DATA_CASTING_VALKYRS, SPELL_LIGHT_VORTEX);
                     DoScriptText(-1713538,m_creature);
                     stage = 1;
                     break;
                 }

                 if (timedQuery(SPELL_TWIN_PACT_H, uiDiff)
                     && m_creature->GetHealthPercent() <= 50.0f)
                 {
                     m_creature->InterruptNonMeleeSpells(true);
                     doCast(SPELL_SHIELD_LIGHT);
                     m_pInstance->SetData(DATA_CASTING_VALKYRS, SPELL_TWIN_PACT_H);
                     DoScriptText(-1713539,m_creature);
                     stage = 3;
                 }
             }
             if (m_pInstance->GetData(DATA_CASTING_VALKYRS) == SPELL_TWIN_PACT_L)
                 if (!m_creature->HasAura(65949)) // need fix HasAura for heroic spells...
                     doCast(SPELL_TWIN_POWER);
             break;
         case 1:
             doCast(SPELL_LIGHT_VORTEX);
             stage = 2;
             break;
         case 2:
             if (!m_creature->HasAura(SPELL_LIGHT_VORTEX) 
                 && timedQuery(SPELL_SHIELD_LIGHT, uiDiff)) 
             {
                 m_pInstance->SetData(DATA_CASTING_VALKYRS, SPELL_NONE);
                 stage = 0;
             }
             break;
         case 3:
             doCast(SPELL_TWIN_PACT_H);
             stage = 4;
             break;
         case 4:
             if ((!m_creature->HasAura(SPELL_SHIELD_LIGHT) && !m_creature->HasAura(67259) &&
                 !m_creature->HasAura(67260) && !m_creature->HasAura(67261))
                 && timedQuery(SPELL_SHIELD_LIGHT, uiDiff))
             {
                 m_pInstance->SetData(DATA_CASTING_VALKYRS, SPELL_NONE);
                 stage = 0;
             }
         default:
             break;
         }

        timedCast(SPELL_BERSERK, uiDiff);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_fjola(Creature* pCreature)
{
    return new boss_fjolaAI(pCreature);
}

struct MANGOS_DLL_DECL boss_eydisAI : public boss_twin_valkyrAI
{
    boss_eydisAI(Creature* pCreature) : boss_twin_valkyrAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint8 stage;

    void Reset() 
    {
        if(!m_pInstance) 
            return;
        SetEquipmentSlots(false, EQUIP_MAIN_2, EQUIP_OFFHAND_2, EQUIP_RANGED_2);
        m_creature->SetRespawnDelay(7*DAY);
        m_pInstance->SetData(DATA_CASTING_VALKYRS, SPELL_NONE);
        stage = 0;
    }

    void JustReachedHome()
    {
        if (!m_pInstance) 
            return;
        m_pInstance->SetData(TYPE_VALKIRIES, FAIL);
        m_pInstance->SetData(DATA_CASTING_VALKYRS, SPELL_NONE);
        ((TemporarySummon *)m_creature)->UnSummon();
    }

    void JustDied(Unit* pKiller)
    {
        if (!m_pInstance) 
            return;
        DoScriptText(-1713547,m_creature);
    }

    void KilledUnit(Unit* pVictim)
    {
        if (pVictim != m_creature)
            DoScriptText(-1713543,m_creature,pVictim);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        switch (stage)
        {
        case 0:
            timedCast(SPELL_TWIN_SPIKE_H, uiDiff);

            if (timedQuery(SPELL_DARK_TOUCH, uiDiff))
            {
                if (currentDifficulty == RAID_DIFFICULTY_25MAN_HEROIC || RAID_DIFFICULTY_10MAN_HEROIC)
                    if (Unit* pTarget = doSelectRandomPlayer(SPELL_DARK_ESSENCE, false, 50.0f))
                        doCast(SPELL_DARK_TOUCH, pTarget);
                doCast(NPC_UNLEASHED_DARK);
            }
            if (m_pInstance->GetData(DATA_CASTING_VALKYRS) == SPELL_NONE )
            {
                if (timedQuery(SPELL_DARK_VORTEX, uiDiff))
                {
                    m_pInstance->SetData(DATA_CASTING_VALKYRS, SPELL_DARK_VORTEX);
                    DoScriptText(-1713540,m_creature);
                    stage = 1;
                    break;
                }
                if (timedQuery(SPELL_TWIN_PACT_L, uiDiff)
                    && m_creature->GetHealthPercent() <= 50.0f)
                {
                    m_creature->InterruptNonMeleeSpells(true);
                    doCast(SPELL_SHIELD_DARK);
                    m_pInstance->SetData(DATA_CASTING_VALKYRS, SPELL_TWIN_PACT_L);
                    DoScriptText(-1713539,m_creature);
                    stage = 3;
                    break;
                }
            }
            if (m_pInstance->GetData(DATA_CASTING_VALKYRS) == SPELL_TWIN_PACT_H)
                if (!m_creature->HasAura(65949)) // need fix HasAura for heroic spells...
                    doCast(SPELL_TWIN_POWER);
            break;
        case 1:
            doCast(SPELL_DARK_VORTEX);
            stage = 2;
            break;
        case 2:
            if (!m_creature->HasAura(SPELL_DARK_VORTEX)
                && timedQuery(SPELL_SHIELD_DARK, uiDiff)) 
            {
                m_pInstance->SetData(DATA_CASTING_VALKYRS, SPELL_NONE);
                stage = 0;
            }
            break;
        case 3:
            doCast(SPELL_TWIN_PACT_L);
            stage = 4;
            break;
        case 4:
            if ((!m_creature->HasAura(SPELL_SHIELD_DARK) && 
                !m_creature->HasAura(67256) &&
                !m_creature->HasAura(67257) && 
                !m_creature->HasAura(67258))
                && timedQuery(SPELL_SHIELD_DARK, uiDiff)) 
            {
                m_pInstance->SetData(DATA_CASTING_VALKYRS, SPELL_NONE);
                stage = 0;
            };
        default:
            break;
        }

        timedCast(SPELL_BERSERK, uiDiff);

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_eydis(Creature* pCreature)
{
    return new boss_eydisAI(pCreature);
}

struct MANGOS_DLL_DECL mob_light_essenceAI : public ScriptedAI
{
    mob_light_essenceAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }
    ScriptedInstance* m_pInstance;

    void Reset() 
    {
        m_creature->SetRespawnDelay(3*MINUTE);
        m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance)
            m_creature->ForcedDespawn();
        if (m_pInstance->GetData(TYPE_VALKIRIES) == DONE || m_pInstance->GetData(TYPE_VALKIRIES) == FAIL) 
        {
            Map* pMap = m_creature->GetMap();
            Map::PlayerList const &lPlayers = pMap->GetPlayers();
            for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            {
                Unit* pPlayer = itr->getSource();
                if (!pPlayer) 
                    continue;
                if (pPlayer->isAlive())
                    pPlayer->RemoveAurasDueToSpell(SPELL_LIGHT_ESSENCE);
            }

            m_creature->ForcedDespawn();
        }
        return;
    }
};

CreatureAI* GetAI_mob_light_essence(Creature* pCreature)
{
    return new mob_light_essenceAI(pCreature);
};

bool GossipHello_mob_light_essence(Player *player, Creature* pCreature)
{
    ScriptedInstance *pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    if(!pInstance) 
        return true;
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
    player->RemoveAurasDueToSpell(SPELL_DARK_ESSENCE);
    //        player->CastSpell(player,SPELL_REMOVE_TOUCH,false); // Not worked now
    player->CastSpell(player,SPELL_LIGHT_ESSENCE,false);
    player->RemoveAurasDueToSpell(SPELL_LIGHT_TOUCH); // Override for REMOVE_TOUCH
    player->RemoveAurasDueToSpell(67296); // Override for REMOVE_TOUCH
    player->RemoveAurasDueToSpell(67298); // Override for REMOVE_TOUCH
    player->RemoveAurasDueToSpell(65950); // Override for REMOVE_TOUCH
    player->CLOSE_GOSSIP_MENU();
    return true;
};

struct MANGOS_DLL_DECL mob_dark_essenceAI : public ScriptedAI
{
    mob_dark_essenceAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }
    ScriptedInstance* m_pInstance;

    void Reset() 
    {
        m_creature->SetRespawnDelay(3*MINUTE);
        m_creature->AddSplineFlag(SPLINEFLAG_WALKMODE);
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance) 
            m_creature->ForcedDespawn();
        if (m_pInstance->GetData(TYPE_VALKIRIES) == DONE || m_pInstance->GetData(TYPE_VALKIRIES) == FAIL) 
        {
            Map* pMap = m_creature->GetMap();
            Map::PlayerList const &lPlayers = pMap->GetPlayers();
            for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            {
                Unit* pPlayer = itr->getSource();
                if (!pPlayer) 
                    continue;
                if (pPlayer->isAlive())
                    pPlayer->RemoveAurasDueToSpell(SPELL_DARK_ESSENCE);
            }
            m_creature->ForcedDespawn();
        }
        return;
    }
};

CreatureAI* GetAI_mob_dark_essence(Creature* pCreature)
{
    return new mob_dark_essenceAI(pCreature);
};

bool GossipHello_mob_dark_essence(Player *player, Creature* pCreature)
{
    ScriptedInstance *pInstance = (ScriptedInstance *) pCreature->GetInstanceData();
    if(!pInstance) return true;
    player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, pCreature->GetGUID());
    player->RemoveAurasDueToSpell(SPELL_LIGHT_ESSENCE);
    //        player->CastSpell(player,SPELL_REMOVE_TOUCH,false); // Not worked now
    player->CastSpell(player,SPELL_DARK_ESSENCE,false);
    player->RemoveAurasDueToSpell(SPELL_DARK_TOUCH); // Override for REMOVE_TOUCH
    player->RemoveAurasDueToSpell(66001); // Override for REMOVE_TOUCH
    player->RemoveAurasDueToSpell(67281); // Override for REMOVE_TOUCH
    player->RemoveAurasDueToSpell(67283); // Override for REMOVE_TOUCH
    player->CLOSE_GOSSIP_MENU();
    return true;
}

struct MANGOS_DLL_DECL mob_unleashed_darkAI : public ScriptedAI
{
    mob_unleashed_darkAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiRangeCheck_Timer;
    Creature* pboss1;
    Creature* pboss2;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false); 
        m_creature->GetMotionMaster()->MoveRandom();
        m_uiRangeCheck_Timer = 1000;
        pboss1 = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_DARKBANE));
        pboss2 = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_LIGHTBANE));
    }

    void AttackStart(Unit *pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance)
            m_creature->ForcedDespawn();

        if (m_pInstance->GetData(TYPE_VALKIRIES) == DONE || m_pInstance->GetData(TYPE_VALKIRIES) == FAIL)
            m_creature->ForcedDespawn();
 
        if (m_uiRangeCheck_Timer < uiDiff)
        {
            Map* pMap = m_creature->GetMap();
            Map::PlayerList const &lPlayers = pMap->GetPlayers();
            for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            {
                Unit* pPlayer = itr->getSource();
                if (!pPlayer) continue;
                if (pPlayer->isAlive() && pPlayer->IsWithinDistInMap(m_creature, 3.0f))
                {
                    m_creature->CastSpell(m_creature, SPELL_UNLEASHED_DARK, true);
                    m_creature->ForcedDespawn();
                }

            }
            if (pboss1 && pboss1->isAlive() && pboss1->IsWithinDistInMap(m_creature, 3.0f))
            {
                m_creature->CastSpell(m_creature, SPELL_UNLEASHED_DARK, true);
                m_creature->ForcedDespawn();
            }
            if (pboss2 && pboss2->isAlive() && pboss2->IsWithinDistInMap(m_creature, 3.0f))
            {
                m_creature->CastSpell(m_creature, SPELL_UNLEASHED_DARK, true);
                m_creature->ForcedDespawn();
            }
            m_uiRangeCheck_Timer = 1000;
        }
        else m_uiRangeCheck_Timer -= uiDiff;
    }
};

CreatureAI* GetAI_mob_unleashed_dark(Creature *pCreature)
{
    return new mob_unleashed_darkAI(pCreature);
}

struct MANGOS_DLL_DECL mob_unleashed_lightAI : public ScriptedAI
{
    mob_unleashed_lightAI(Creature *pCreature) : ScriptedAI(pCreature) 
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    uint32 m_uiRangeCheck_Timer;
    Creature* pboss1;
    Creature* pboss2;

    void Reset()
    {
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        SetCombatMovement(false); 
        m_creature->GetMotionMaster()->MoveRandom();
        m_uiRangeCheck_Timer = 1000;
        pboss1 = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_DARKBANE));
        pboss2 = m_creature->GetMap()->GetCreature(m_pInstance->GetData64(NPC_LIGHTBANE));
    }

    void AttackStart(Unit *pWho)
    {
        return;
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_pInstance)
            m_creature->ForcedDespawn();

        if (m_pInstance->GetData(TYPE_VALKIRIES) == DONE || m_pInstance->GetData(TYPE_VALKIRIES) == FAIL)
            m_creature->ForcedDespawn();
 
        if (m_uiRangeCheck_Timer < uiDiff)
        {
            Map* pMap = m_creature->GetMap();
            Map::PlayerList const &lPlayers = pMap->GetPlayers();
            for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            {
                Unit* pPlayer = itr->getSource();
                if (!pPlayer) continue;
                if (pPlayer->isAlive() && pPlayer->IsWithinDistInMap(m_creature, 3.0f))
                {
                    m_creature->CastSpell(m_creature, SPELL_UNLEASHED_LIGHT, true);
                    m_creature->ForcedDespawn();
                }
            }
            if (pboss1 && pboss1->isAlive() && pboss1->IsWithinDistInMap(m_creature, 3.0f))
            {
                m_creature->CastSpell(m_creature, SPELL_UNLEASHED_LIGHT, true);
                m_creature->ForcedDespawn();
            }
            if (pboss2 && pboss2->isAlive() && pboss2->IsWithinDistInMap(m_creature, 3.0f))
            {
                m_creature->CastSpell(m_creature, SPELL_UNLEASHED_LIGHT, true);
                m_creature->ForcedDespawn();
            }
            m_uiRangeCheck_Timer = 1000;
        }
        else m_uiRangeCheck_Timer -= uiDiff;
    }

};

CreatureAI* GetAI_mob_unleashed_light(Creature *pCreature)
{
    return new mob_unleashed_lightAI(pCreature);
}

CreatureAI* GetAI_mob_twin_valkyr(Creature *pCreature)
{
    return new mob_twin_valkyrAI(pCreature);
}

void AddSC_twin_valkyr()
{
    Script* newscript;

    newscript = new Script;
    newscript->Name = "mob_twin_valkyr";
    newscript->GetAI = &GetAI_mob_twin_valkyr;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_fjola";
    newscript->GetAI = &GetAI_boss_fjola;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "boss_eydis";
    newscript->GetAI = &GetAI_boss_eydis;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_unleashed_light";
    newscript->GetAI = &GetAI_mob_unleashed_light;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_unleashed_dark";
    newscript->GetAI = &GetAI_mob_unleashed_dark;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_light_essence";
    newscript->GetAI = &GetAI_mob_light_essence;
    newscript->pGossipHello = &GossipHello_mob_light_essence;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_dark_essence";
    newscript->GetAI = &GetAI_mob_dark_essence;
    newscript->pGossipHello = &GossipHello_mob_dark_essence;
    newscript->RegisterSelf();
}

