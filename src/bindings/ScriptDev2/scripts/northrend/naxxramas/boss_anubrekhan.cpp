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
SDName: Boss_Anubrekhan
SD%Complete: 90
SDComment: timers may be not offi
SDCategory: Naxxramas
EndScriptData */

#include "precompiled.h"
#include "def_naxxramas.h"

//Anub'Rekhan speech
enum Speech
{
    SAY_GREET  = -1533000,
    SAY_AGGRO1 = -1533001,
    SAY_AGGRO2 = -1533002,
    SAY_AGGRO3 = -1533003,
    SAY_TAUNT1 = -1533004,
    SAY_TAUNT2 = -1533005,
    SAY_TAUNT3 = -1533006,
    SAY_TAUNT4 = -1533007,
    SAY_SLAY   = -1533008
};

enum
{
//Anub'Rekhan spels
    SPELL_IMPALE       = 28783,                           //May be wrong spell id. Causes more dmg than I expect
    SPELL_IMPALE_H     = 56090,
    SPELL_LOCUSTSWARM  = 28785,                           //This is a self buff that triggers the dmg debuff
    SPELL_LOCUSTSWARM_H = 54021,
    SPELL_BERSERK      = 46587,                           
    SPELL_SELF_SPAWN_5 = 29105,                           //This spawns 5 corpse scarabs ontop of us (most likely the player casts this on death)

// Adds
    MOB_CRYPT_GUARD    = 16573,
    MOB_CORPSE_SCARAB  = 16698,

//Crypt Guard spells
    SPELL_CG_ACID_SPIT  = 28969,
    SPELL_CG_CLEAVE     = 40504,
    SPELL_CG_FRENZY     = 8269,
    SPELL_SELF_SPAWN_10 = 28864                           //This spawns 10 corpse scarabs. This is used by the crypt guards when they die
};

static const uint32 MAX_CRYPT_GUARDS = 8;

//Anub'Rekhan AI
struct MANGOS_DLL_DECL boss_anubrekhanAI : public ScriptedAI
{
    boss_anubrekhanAI(Creature *c) : ScriptedAI(c) 
    {
        pInstance = ((ScriptedInstance*)c->GetInstanceData());
        m_bIsRegularMode = c->GetMap()->IsRegularDifficulty();
        
        for (int i = 0; i < MAX_CRYPT_GUARDS; i++)
            guidCryptGuards[i] = 0;
        HasTaunted = false;
        Reset();
    }

    ScriptedInstance *pInstance;

    bool m_ach_10ppl;
    bool m_ach_25ppl;
    bool m_bIsRegularMode;
    uint32 m_count_ppl;
    uint32 Ach_Timer;
    uint32 Impale_Timer;
    uint32 LocustSwarm_Timer;
    uint32 SummonFirst_Timer;
    uint32 Berserk_Timer;
    uint32 RiseFromCorpse_Timer;
    uint32 m_uiEvadeCheckCooldown;

    ObjectGuid guidCryptGuards[MAX_CRYPT_GUARDS];
    uint32 CryptGuard_count;

    bool HasTaunted;                //in fact says we entered the room with big beetle
    bool swarm;                        //is active spell LocustSwarm or not, when active he cannot use other spels

    void Reset()
    {
        m_ach_10ppl = true;
        m_ach_25ppl = true;
        Ach_Timer = 10000;
        m_count_ppl = 0;
        m_uiEvadeCheckCooldown = 2000;
        Impale_Timer = 15000;                               //15 seconds
        LocustSwarm_Timer = 80000 + (rand()%40000);         //Random time between 80 seconds and 2 minutes for initial cast
        SummonFirst_Timer = 15000;                            //45 seconds after initial locust swarm
        Berserk_Timer = 600000;
        RiseFromCorpse_Timer = 20000 + (rand()%20000);
        swarm = false;
        CryptGuard_count = 0;

        // Remove all Crypt Guards
        for (int i = 0; i < MAX_CRYPT_GUARDS; i++)
        {
            //delete creature
            Unit* pUnit = m_creature->GetMap()->GetUnit(guidCryptGuards[i]);
            if (pUnit)
                pUnit->AddObjectToRemoveList();
            guidCryptGuards[i] = 0;
        }
        //Remove all corpse scarabs

        std::list<Creature*> CorpseScarabs = GetCreaturesByEntry(MOB_CORPSE_SCARAB);
        if (!CorpseScarabs.empty())
            for(std::list<Creature*>::iterator itr = CorpseScarabs.begin(); itr != CorpseScarabs.end(); ++itr)
                (*itr)->AddObjectToRemoveList();
    }

    void JustRespawned()
    {
        JustReachedHome();
    }

    void JustReachedHome()
    {
        if (pInstance)
            pInstance->SetData(ENCOUNT_ANUBREKHAN, NOT_STARTED);
    }


    void JustDied(Unit*)
    {
        if (!pInstance)
            return;
        //Anubrekhan is slayed -> open all doors to Faerlina
        pInstance->SetData(ENCOUNT_ANUBREKHAN, DONE);

        Map::PlayerList const &PlList = pInstance->instance->GetPlayers();
        if (PlList.isEmpty())
            return;
        for(Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
        {
            if (Player* pPlayer = i->getSource())
            {
                if (!m_creature->IsWithinDistInMap(pPlayer,200))
                    continue;

                if (m_bIsRegularMode && m_ach_10ppl)
                    pPlayer->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE,m_creature->GetEntry(),1,0,0,7146);
                else if (!m_bIsRegularMode && m_ach_25ppl)
                    pPlayer->UpdateAchievementCriteria(ACHIEVEMENT_CRITERIA_TYPE_KILL_CREATURE,m_creature->GetEntry(),1,0,0,7159);
            }
        }
    }

    void KilledUnit(Unit* Victim)
    {
        DoScriptText(SAY_SLAY, m_creature);
    }

    void CheckAch()
    {
        if (!pInstance)
            return;

        m_count_ppl = 0;
        Map::PlayerList const &PlList = pInstance->instance->GetPlayers();
        if (PlList.isEmpty())
            return;
        for(Map::PlayerList::const_iterator i = PlList.begin(); i != PlList.end(); ++i)
        {
            if (Player* pPlayer = i->getSource())
            {
                if (pPlayer->isGameMaster())
                    continue;
                ++m_count_ppl;
            }
        }
        if (m_bIsRegularMode)
        {
            if(m_count_ppl>8)
                m_ach_10ppl = false;
        }
        else
        {
            if(m_count_ppl>20)
                m_ach_25ppl = false;
        }
    }

    void Aggro(Unit *who)
    {
        //Close the room for boss fight
        if(pInstance)
            pInstance->SetData(ENCOUNT_ANUBREKHAN, IN_PROGRESS);

        switch(rand()%3)
        {
            case 0: DoScriptText(SAY_AGGRO1, m_creature); break;
            case 1: DoScriptText(SAY_AGGRO2, m_creature); break;
            case 2: DoScriptText(SAY_AGGRO3, m_creature); break;
        }
        CheckAch();
    }
    
    bool IsVisible(Unit* who) const
    {
        if (!who)
            return false;
        return m_creature->IsWithinDistInMap(who, 100.0f);
    }

    void MoveInLineOfSight(Unit *who)
    {
        if (!HasTaunted)
        {
            DoScriptText(SAY_GREET, m_creature);
            HasTaunted = true;
        }
        
        ScriptedAI::MoveInLineOfSight(who);
    }
    
    std::list<Creature*> GetCreaturesByEntry(uint32 entry)
    {
        CellPair pair(MaNGOS::ComputeCellPair(m_creature->GetPositionX(), m_creature->GetPositionY()));
        Cell cell(pair);
        cell.data.Part.reserved = ALL_DISTRICT;
        cell.SetNoCreate();

        std::list<Creature*> creatureList;

        AllCreaturesOfEntryInRange check(m_creature, entry, 100);
        MaNGOS::CreatureListSearcher<AllCreaturesOfEntryInRange> searcher(m_creature, creatureList, check);
        TypeContainerVisitor<MaNGOS::CreatureListSearcher<AllCreaturesOfEntryInRange>, GridTypeMapContainer> visitor(searcher);

        cell.Visit(pair, visitor, *(m_creature->GetMap()));

        return creatureList;
    }

    void JustSummoned(Creature* temp) 
    {
        if (!temp)
            return;

        //Summoned Crypt Guard will target random player
        guidCryptGuards[CryptGuard_count++] = temp->GetObjectGuid();
        if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,0))
        {
            temp->AddThreat(target,0.0f);
            m_creature->SetInCombatWithZone();
        }

        switch (rand()%4)
        {
            case 0: DoScriptText(SAY_TAUNT1, m_creature); break;
            case 1: DoScriptText(SAY_TAUNT2, m_creature); break;
            case 2: DoScriptText(SAY_TAUNT3, m_creature); break;
            case 3: DoScriptText(SAY_TAUNT4, m_creature); break;
        }
    }

    void UpdateAI(const uint32 diff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        if (m_uiEvadeCheckCooldown < diff)
        {
            if (m_creature->GetDistance2d(3307.02f, -3476.27f) > 125.0f)
                EnterEvadeMode();
            m_uiEvadeCheckCooldown = 2000;
        }
        else
            m_uiEvadeCheckCooldown -= diff;

        //Berserk_Timer
        if (Berserk_Timer < diff)
        {
            DoCast(m_creature,SPELL_BERSERK);
            Berserk_Timer = 300000;
        }else Berserk_Timer -= diff;

        if (m_bIsRegularMode)
        {
            //SumonFirstCryptGuard_Timer
            if (SummonFirst_Timer < diff)
            {
                if (CryptGuard_count < MAX_CRYPT_GUARDS)
                    m_creature->SummonCreature(MOB_CRYPT_GUARD,3330.0f,-3477.0f,288.0f,3.2f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,10000);
                SummonFirst_Timer = 2000000;
            }else SummonFirst_Timer -= diff;
        }

        //RiseFromCorpse_Timer
        if (RiseFromCorpse_Timer < diff)
        {
            RiseFromCorpse_Timer = 60000 + (rand()%10000);
            std::list<Creature*> CryptGuards = GetCreaturesByEntry(MOB_CRYPT_GUARD);
            if (!CryptGuards.empty())
                for(std::list<Creature*>::iterator itr = CryptGuards.begin(); itr != CryptGuards.end(); ++itr)
                    if ((*itr)->isDead())
                    {
                        (*itr)->CastSpell((*itr),SPELL_SELF_SPAWN_10,true);
                        (*itr)->AddObjectToRemoveList();
                    }

            const Map::PlayerList &players = m_creature->GetMap()->GetPlayers();
            if (players.isEmpty())
                return;

            for(Map::PlayerList::const_iterator i = players.begin(); i != players.end(); ++i)
                if (Player* pPlayer = i->getSource())
                {
                    if (pPlayer->isGameMaster())
                        continue;

                    if (pPlayer->isDead())
                        pPlayer->CastSpell(pPlayer,SPELL_SELF_SPAWN_5,true);
                }
            CryptGuards.clear();
        }else RiseFromCorpse_Timer -= diff;

        if(!swarm)
        {
            //Impale_Timer
            if (Impale_Timer < diff)
            {
                //Cast Impale on a random target
                //Do NOT cast it when we are afflicted by locust swarm
                if (!m_creature->HasAura(SPELL_LOCUSTSWARM,SpellEffectIndex(1)))
                    if (Unit* target = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM,SpellEffectIndex(1)))
                        DoCast(target,m_bIsRegularMode ? SPELL_IMPALE : SPELL_IMPALE_H);
                Impale_Timer = 15000;
            }else Impale_Timer -= diff;

            //LocustSwarm_Timer
            if (LocustSwarm_Timer < diff)
            {
                //Cast Locust Swarm buff on ourselves
                DoCast(m_creature, !m_bIsRegularMode ? SPELL_LOCUSTSWARM_H:SPELL_LOCUSTSWARM);
                swarm = true;
                //Summon Crypt Guard immidietly after Locust Swarm
                if (CryptGuard_count < MAX_CRYPT_GUARDS)
                    m_creature->SummonCreature(MOB_CRYPT_GUARD,3330.0f,-3477.0f,288.0f,3.2f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,10000);
                LocustSwarm_Timer = 20000;
            }else LocustSwarm_Timer -= diff;
        }
        else
        {
            if (LocustSwarm_Timer < diff)
            {            
                swarm = false;
                LocustSwarm_Timer = 85000;
            }else LocustSwarm_Timer -= diff;
        }

        if (Ach_Timer<diff)
        {
            if (m_bIsRegularMode && m_ach_10ppl)
                CheckAch();
            else if (!m_bIsRegularMode && m_ach_25ppl)
                CheckAch();
            Ach_Timer = 10000;
        }else Ach_Timer -= diff;            

        DoMeleeAttackIfReady();
    }
};

CreatureAI* GetAI_boss_anubrekhan(Creature *_Creature)
{
    return new boss_anubrekhanAI (_Creature);
}

void AddSC_boss_anubrekhan()
{
    Script *newscript;
    newscript = new Script;
    newscript->Name = "boss_anubrekhan";
    newscript->GetAI = &GetAI_boss_anubrekhan;
    newscript->RegisterSelf();
}
