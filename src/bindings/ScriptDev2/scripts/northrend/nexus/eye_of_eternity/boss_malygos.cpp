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
SDName: Boss_Malygos
SD%Complete: 60%
SDComment: 
SDAuthor: Tassadar
SDCategory: Nexus, Eye of Eternity
EndScriptData */

#include "precompiled.h"
#include "eye_of_eternity.h"

enum
{
    //First, say what it should do:
    //////////////// PHASE 0 ////////////////
    // Malygost cast SPELL_PORTAL_BEAM on some sort of portal until get aggroed
    // by Focusing iris
    //
    //////////////// PHASE 1 ////////////////
    // Arcane Breath - normal spell
    //
    // Vortex        - ouuu this will be tuff :/ there are spawned some triggers,
    //                 maybe pull between them?
    //
    // Power Spark   - from wowhead: "Spawned throughout the fight and slowly
    //                 shift towards Malygos. Once they reach him, they buff him
    //                 with Power Spark, increasing the damage output by 50% for
    //                   10 seconds, stacking multiplicatively. If killed, they
    //                   instead grant players in proximity the same buff,
    //                   Power Spark, which especially is a great buff for melee
    //                   players close to Malygos."
    //
    //////////////// PHASE 2 //////////////// - 50% of health
    // On beggining of this phase, malygos will take off slowly and let
    // players 10s to DPS him, then he will fly around(?) and cast his deep breath
    // NPCs on flying discs will be spawned, they will fly around and do damage to raid,
    // when is NPC killed, disc does NOT disappear - its vehicle, players use it to!
    // Players also have to cover in protective bubbles(which are spawned continuosly),
    // they shrink over time so players have to run to another one.
    //
    // Arcane Pulse   - Deep Breath....
    // Arcane Storm   - normal spell...
    //
    //////////////// PHASE 3 //////////////// - when all NPCs from previos phase are dead
    // Malygos will destroy platfrom, players are sent to dragon vehicles.
    //
    // Static Field   - trigger with aura
    // Surge of Power - triggers for that are here
    //
    //////////////// PHASE 4 //////////////// - When malygos reach 0%
    // Outro, Alexstrasza will arive
    

    // ******************************** SPELLS ******************************** //
    SPELL_BERSERK                  = 64238, // Maybe wrong but enought :)
    //////////////// PHASE 0 ////////////////
    SPELL_PORTAL_BEAM              = 56046,

    //////////////// PHASE 1 ////////////////
    SPELL_ARCANE_BREATH            = 56272,
    SPELL_ARCANE_BREATH_H          = 60072,
    SPELL_VORTEX_DUMMY             = 56105, // This is for addons, actualy does nothing
    SPELL_VORTEX                   = 56266, // Cast on trigger in middle of the room, this interrupt their cast and deal dmg
    SPELL_VORTEX_AOE_VISUAL        = 55873,    // visual effect around platform
    SPELL_POWER_SPARK              = 56152, // If spark reach malygos then buff him, if killed then to players
    SPELL_POWER_SPARK_PLAYERS      = 55852, // This one is propably for players
    SPELL_POWER_SPARK_VISUAL       = 55845,

    //////////////// PHASE 2 ////////////////
    SPELL_ARCANE_PULSE             = 57432, // Malygos takes a deep breath... cast on trigger
    SPELL_ARCANE_STORM             = 61693, // AOE
    SPELL_ARCANE_STORM_H           = 61694,
    SPELL_ARCANE_OVERLOAD          = 56432, // Cast this on arcane overload NPCs 
    SPELL_DESTROY_PLATFORM_PRE     = 58842, // lights all over the platform
    SPELL_DESTROY_PLATFROM_BOOM    = 59084, // Big Blue boom
    //NPCs spells
    SPELL_ARCANE_SHOCK             = 57058,
    SPELL_ARCANE_SHOCK_H           = 60073,
    SPELL_ARCANE_BARRAGE           = 59381, // 63934 have TARGET_SELF, need implement 56397


    //////////////// PHASE 3 ////////////////
    SPELL_STATIC_FIELD             = 57428, // Summon trigger and cast this on them should be enought
    SPELL_SURGE_OF_POWER           = 56548, // Summon triggers and cast to random targets
    SPELL_SURGE_OF_POWER_H         = 57407,
    SPELL_RIDE_WYRMREST_SKYTALON   = 61245, // Ride vehicle
    //Dragons spells
    SPELL_FLAME_SPIKE              = 56091,
    SPELL_ENGULF_IN_FLAMES         = 56092,
    SPELL_REVIVIFY                 = 57090,
    SPELL_LIFE_BURST               = 57143,
    SPELL_FLAME_SHIELD             = 57108,
    SPELL_BLAZING_SPEED            = 57092,

    // ******************************** NPCs & GObjects ******************************** //
    NPC_MALYGOS                    = 28859,
    ITEM_KEY_TO_FOCUSING_IRIS      = 44582,
    ITEM_KEY_TO_FOCUSING_IRIS_H    = 44581,
    GO_FOCUSING_IRIS               = 193958,
    GO_FOCUSING_IRIS_H             = 193960,
    GO_EXIT_PORTAL                 = 193908,
    //////////////// PHASE 1 ////////////////
    NPC_AOE_TRIGGER                = 22517,
    NPC_VORTEX                     = 30090,
    NPC_POWER_SPARK                = 30084,
    NPC_SPARK_PORTAL               = 30118, // For power sparks

    //////////////// PHASE 2 ////////////////
    NPC_HOVER_DISC                 = 30248, // Maybe wrong, two following NPC flying on them (vehicle)
    DISPLAY_HOVER_DISC             = 26876, // DisplayID of hover disc
    NPC_NEXUS_LORD                 = 30245, // two (?) of them are spawned on beginning of phase 2
    NPC_SCION_OF_ETERNITY          = 30249, // same, but unknow count
    NPC_ARCANE_OVERLOAD            = 30282, // Bubles
    GO_PLATFORM                    = 193070,
    GO_PLATFORM_DESTROY            = 393070,

    //////////////// PHASE 3 ////////////////
    NPC_WYRMREST_SKYTALON          = 30161, // Dragon Vehicle in Third Phase
    NPC_SURGE_OF_POWER             = 30334, // Because its on three targets, malygos cant cast it by hymself
    NPC_STATIC_FIELD               = 30592, // Trigger for that spell. Hope its fly

    //////////////// PHASE 4 ////////////////
    NPC_ALEXSTRASZA                = 32295, // The Life-Binder
    GO_ALEXSTRASZAS_GIFT           = 193905, //Loot chest

    SAY_INTRO1                     = -1616000,
    SAY_INTRO2                     = -1616001,
    SAY_INTRO3                     = -1616002,
    SAY_INTRO4                     = -1616003,
    SAY_INTRO5                     = -1616004,
    SAY_INTRO_PHASE3               = -1616018,
    SAY_AGGRO1                     = -1616005,
    SAY_AGGRO2                     = -1616013,
    SAY_AGGRO3                     = -1616019,
    SAY_VORTEX                     = -1616006,
    SAY_POWER_SPARK                = -1616007,
    SAY_DEATH                      = -1616011,
    SAY_KILL1_1                    = -1616008,
    SAY_KILL1_2                    = -1616009,
    SAY_KILL1_3                    = -1616010,
    SAY_KILL2_1                    = -1616020,
    SAY_KILL2_2                    = -1616021,
    SAY_KILL2_3                    = -1616022,
    SAY_KILL3_1                    = -1616023,
    SAY_KILL3_2                    = -1616024,
    SAY_KILL3_3                    = -1616025,
    SAY_END_PHASE1                 = -1616012,
    SAY_END_PHASE2                 = -1616017,
    SAY_ARCANE_PULSE               = -1616014,
    SAY_ARCANE_PULSE_WARN          = -1616015,
    SAY_ARCANE_OVERLOAD            = -1616016,

    SHELL_MIN_X                    = 722,
    SHELL_MAX_X                    = 768,
    SHELL_MIN_Y                    = 1290,
    SHELL_MAX_Y                    = 1339,

    NEXUS_LORD_COUNT               = 2,
    NEXUS_LORD_COUNT_H             = 4,
    SCION_OF_ETERNITY_COUNT        = 6,
    SCION_OF_ETERNITY_COUNT_H      = 8,

    PHASE_NOSTART                  = 0,
        SUBPHASE_FLY_UP            = 01,
        SUBPHASE_UP                = 03,
        SUBPHASE_FLY_DOWN1         = 04,
        SUBPHASE_FLY_DOWN2         = 05,
    PHASE_FLOOR                    = 1,
        SUBPHASE_VORTEX            = 11,
    PHASE_ADDS                     = 2,
        SUBPHASE_TALK              = 21,
    PHASE_DRAGONS                  = 3,
        SUBPHASE_DESTROY_PLATFORM1 = 31,
        SUBPHASE_DESTROY_PLATFORM2 = 32,
        SUBPHASE_DESTROY_PLATFORM3 = 33,
    PHASE_OUTRO                    = 4,
        SUBPHASE_DIE               = 41,
};
struct Locations
{
    float x, y, z, o;
    uint32 id;
};
struct LocationsXY
{
    float x, y;
    uint32 id;
};
static Locations GOPositions[]=
{
    {754.346f, 1300.87f, 256.249f, 3.14159f},   // Raid Platform position
    {754.731f, 1300.12f, 266.171f, 5.01343f},   // Focusing iris
    {724.684f, 1332.92f, 267.234f, -0.802851f}, // Exit Portal
};
static LocationsXY SparkLoc[]=
{
    {652.417f, 1200.52f},
    {847.67f, 1408.05f},
    {647.675f, 1403.8f},
    {843.182f, 1215.42f},
};
static LocationsXY VortexLoc[]=
{
    {754, 1311},
    {734, 1334},
    {756, 1339},
    {781, 1329},
    {791, 1311},
    {790, 1283},
    {768, 1264},
    {739, 1261},
    {720, 1280},
    {714, 1299},
    {716, 1318},
    {734, 1334},
    {756, 1339},
    {781, 1329},
    {791, 1311},
    {790, 1283},
    {768, 1264},
    {739, 1261},
    {720, 1280},
    {714, 1299},
    {716, 1318},
};
#define MAX_VORTEX              21
#define VORTEX_Z                268

#define FLOOR_Z                 269.17f
#define AIR_Z                   297.24f   
/*######
## boss_malygos
######*/

struct MANGOS_DLL_DECL boss_malygosAI : public ScriptedAI
{
    boss_malygosAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
        pAlexstrasza = NULL;
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;

    Creature *pAlexstrasza;
    
    uint8 m_uiPhase; //Fight Phase
    uint8 m_uiSubPhase; //Subphase if needed
    uint8 m_uiSpeechCount;
    uint8 m_uiVortexPhase;
    std::list<ObjectGuid> m_lSparkGUIDList;

    uint32 m_uiEnrageTimer;
    uint32 m_uiSpeechTimer[5];
    uint32 m_uiTimer;
    uint32 m_uiVortexTimer;
    uint32 m_uiArcaneBreathTimer;
    uint32 m_uiPowerSparkTimer;
    uint32 m_uiDeepBreathTimer;
    uint32 m_uiShellTimer;
    uint32 m_uiArcaneStormTimer;
    uint32 m_uiStaticFieldTimer;

    bool fly_fix;
    
    void Reset()
    {
        m_uiPhase = PHASE_NOSTART;
        m_uiSubPhase = 0;
        m_uiSpeechCount = 0;
        m_uiVortexPhase = 0;

        fly_fix = true;

        m_uiEnrageTimer = 800000;
        m_uiSpeechTimer[0] = 2000;
        m_uiSpeechTimer[1] = 10000;
        m_uiSpeechTimer[2] = 11000;
        m_uiSpeechTimer[3] = 13000;
        m_uiSpeechTimer[4] = 10000;
        m_uiSpeechTimer[5] = 7000;
        m_uiTimer = 7000;
        m_uiVortexTimer = 60000;
        m_uiArcaneBreathTimer = 15000;
        m_uiPowerSparkTimer = 20000;
        m_uiDeepBreathTimer = 60000;
        m_uiShellTimer = 0;
        m_uiArcaneStormTimer = 15000;
        m_uiStaticFieldTimer = 15000;
        
        m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);

        if(m_pInstance)
            m_pInstance->SetData(TYPE_MALYGOS, NOT_STARTED);
    }

    void JustReachedHome()
    {
        Reset();
        //Summon Platform
        if(!GetClosestGameObjectWithEntry(m_creature, GO_PLATFORM, 120.0f))
            m_creature->SummonGameObject(GO_PLATFORM, GOPositions[0].x, GOPositions[0].y, GOPositions[0].z, GOPositions[0].o, 0.0f, 0.0f, 0.0f, 0.0f, 25000);

        //Summon focusing iris
        if(m_bIsRegularMode && !GetClosestGameObjectWithEntry(m_creature, GO_FOCUSING_IRIS, 120.0f))
            m_creature->SummonGameObject(GO_FOCUSING_IRIS, GOPositions[1].x, GOPositions[1].y, GOPositions[1].z, GOPositions[1].o, 0.0f, 0.0f, 0.0f, 0.0f, 25000);

        if(!m_bIsRegularMode && !GetClosestGameObjectWithEntry(m_creature, GO_FOCUSING_IRIS_H, 120.0f))
            m_creature->SummonGameObject(GO_FOCUSING_IRIS_H, GOPositions[1].x, GOPositions[1].y, GOPositions[1].z, GOPositions[1].o, 0.0f, 0.0f, 0.0f, 0.0f, 25000);
        
        //Summon exit portal
        if(!GetClosestGameObjectWithEntry(m_creature, GO_EXIT_PORTAL, 120.0f))
            m_creature->SummonGameObject(GO_EXIT_PORTAL, GOPositions[2].x, GOPositions[2].y, GOPositions[2].z, GOPositions[2].o, 0.0f, 0.0f, 0.0f, 0.0f, 25000);
    }

    void AttackStart(Unit* pWho)
    {
        if(m_uiPhase != PHASE_FLOOR && m_uiPhase != PHASE_DRAGONS)
            return;

        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            m_creature->GetMotionMaster()->MoveChase(pWho);
        }
    }

    void Aggro(Unit* pWho)
    {
        DoScriptText(SAY_AGGRO1, m_creature);
        //Despawn exit portal
        if(GameObject *pPortal = GetClosestGameObjectWithEntry(m_creature, GO_EXIT_PORTAL, 120.0f))
            pPortal->Delete();
        if(m_pInstance)
            m_pInstance->SetData(TYPE_MALYGOS, IN_PROGRESS);
    }

    /*void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (m_uiPhase == PHASE_OUTRO && m_uiSubPhase != SUBPHASE_DIE)
        {
            uiDamage = 0;
            return;
        }

        if (uiDamage > m_creature->GetHealth())
        {
            m_uiPhase = PHASE_OUTRO;

            if (m_creature->IsNonMeleeSpellCasted(false))
                m_creature->InterruptNonMeleeSpells(false);

            m_creature->RemoveAllAuras();
            SetCombatMovement(false);

            if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == CHASE_MOTION_TYPE)
                m_creature->GetMotionMaster()->MovementExpired();

            DespawnCreatures(NPC_SURGE_OF_POWER, 120.0f);
            DespawnCreatures(NPC_STATIC_FIELD, 120.0f);

            uiDamage = 0;
            m_creature->SetHealth(1);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_uiSubPhase = 0;
            m_uiSpeechCount = 0;
            m_uiSpeechTimer[0] = 2000;
            m_uiSpeechTimer[1] = 10000;
            m_uiSpeechTimer[2] = 11000;
            m_uiSpeechTimer[3] = 13000;
            m_uiSpeechTimer[4] = 10000;
            m_uiSpeechTimer[5] = 7000;
            if(Creature *pTemp = m_creature->SummonCreature(NPC_ALEXSTRASZA, m_creature->GetPositionX()+40, m_creature->GetPositionY(), m_creature->GetPositionZ(), 0, TEMPSUMMON_CORPSE_DESPAWN, 0))
            {
                pTemp->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
                pTemp->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
                pTemp->SetFacingToObject(m_creature);
                m_creature->SetFacingToObject(pTemp);
                pAlexstrasza = pTemp;
            }
        }
    }*/
    void JustDied(Unit* pKiller)
    {
        if(m_pInstance)
            m_pInstance->SetData(TYPE_MALYGOS, DONE);

        //Summon exit portal
        if(!GetClosestGameObjectWithEntry(m_creature, GO_EXIT_PORTAL, 120.0f))
            m_creature->SummonGameObject(GO_EXIT_PORTAL, GOPositions[2].x, GOPositions[2].y, GOPositions[2].z, GOPositions[2].o, 0.0f, 0.0f, 0.0f, 0.0f, 25000);

    }
    void KilledUnit(Unit* pVictim)
    {
        uint8 text = 0;
        switch(m_uiPhase)
        {
            case PHASE_FLOOR:
                text = urand(0, 2);
                break;
            case PHASE_ADDS:
                text = urand(3, 5);
                break;
            case PHASE_DRAGONS:
                text = urand(6, 8);
                break;
        }
        switch(text)
        {
            case 0: DoScriptText(SAY_KILL1_1, m_creature); break;
            case 1: DoScriptText(SAY_KILL1_2, m_creature); break;
            case 2: DoScriptText(SAY_KILL1_3, m_creature); break;

            case 3: DoScriptText(SAY_KILL2_1, m_creature); break;
            case 4: DoScriptText(SAY_KILL2_2, m_creature); break;
            case 5: DoScriptText(SAY_KILL2_3, m_creature); break;

            case 6: DoScriptText(SAY_KILL3_1, m_creature); break;
            case 7: DoScriptText(SAY_KILL3_2, m_creature); break;
            case 8: DoScriptText(SAY_KILL3_3, m_creature); break;
        }
            
    }
    void SpellHit(Unit* pCaster, const SpellEntry* pSpell)
    {
        if(pSpell->Id == SPELL_POWER_SPARK)
            DoScriptText(SAY_POWER_SPARK, m_creature);
    }
    void SummonedCreatureDespawn(Creature* pDespawned)
    {
        float x,y,z;
        pDespawned->GetPosition(x,y,z);
        z = FLOOR_Z;
        /*if(Vehicle *pDisc = m_creature->SummonVehicle(NPC_HOVER_DISC, x, y, z, 0))
            ((Creature*)pDisc)->SetSpeedRate(MOVE_FLIGHT, 3.5f, true);*/
    }
    void DoMovement(float x, float y, float z, uint32 time, bool tofly = false, bool movepoint = true)
    {        
        if(tofly)
        {
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
        }
        else{
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
            m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
        }
//        WorldPacket heart;
//        m_creature->BuildHeartBeatMsg(&heart);
//        m_creature->SendMessageToSet(&heart, false);
        if(movepoint)
            m_creature->GetMotionMaster()->MovePoint(0, x,y,z);
        else
        {
            m_creature->GetMap()->CreatureRelocation(m_creature, x, y, z, m_creature->GetOrientation());
            m_creature->SendMonsterMove(x, y, z, SPLINETYPE_FACINGTARGET, SPLINEFLAG_FLYING, 1);
        }
    }
    void DoVortex(uint8 phase)
    {
        if(phase == 0)
        {
            m_creature->GetMotionMaster()->Clear(false);
            SetCombatMovement(false);
            float x, y, z;
            m_creature->GetPosition(x, y, z);
            z = z + 20;
            DoMovement(x, y, z, 0, true, false);
        }
        else if(phase == 1)
        {
            if(Creature *pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_AOE_TRIGGER, 60.0f))
                pTrigger->CastSpell(pTrigger, SPELL_VORTEX_AOE_VISUAL, false);

            Map* pMap = m_creature->GetMap();
            if(!pMap)
                return;

            Map::PlayerList const &lPlayers = pMap->GetPlayers();
            for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
            {
                if(!itr->getSource()->isAlive())
                    continue;
                itr->getSource()->NearTeleportTo(VortexLoc[0].x, VortexLoc[0].y, VORTEX_Z, 0); 
                itr->getSource()->CastSpell(itr->getSource(), SPELL_VORTEX, true);    
            }        
        }
        else if(phase > 1 && phase < 26){
            Map* pMap = m_creature->GetMap();
            if(!pMap)
                return;
            if(Creature *pVortex = m_creature->SummonCreature(NPC_VORTEX, VortexLoc[phase-1].x, VortexLoc[phase-1].y, VORTEX_Z, 0, TEMPSUMMON_TIMED_DESPAWN, 10000))
            {
                pVortex->SetVisibility(VISIBILITY_OFF);
                Map::PlayerList const &lPlayers = pMap->GetPlayers();
                for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
                {
                    if(!itr->getSource()->isAlive())
                        continue;

                    float z = itr->getSource()->GetPositionZ() - VORTEX_Z;
                    itr->getSource()->KnockBackFrom(pVortex,-float(pVortex->GetDistance2d(itr->getSource())),7);
                }
            }
        }else if (phase == 30 || phase == 31)
        {    
            if(phase == 31)
            {
                if(m_creature->getVictim())
                    m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                
                m_uiSubPhase = 0;
                return;
            }
            m_creature->GetMotionMaster()->Clear(false);
            SetCombatMovement(true);
            float x, y, z;
            m_creature->GetPosition(x, y, z);
            z = FLOOR_Z;
            DoMovement(x, y, z, 0, false, false);
        }
        
    }
    void PowerSpark(uint8 action)
    {
        if(action == 1) //Summon
        {
            uint8 random = urand(0, 3);
            if(Creature *pSpark = m_creature->SummonCreature(NPC_POWER_SPARK, SparkLoc[random].x, SparkLoc[random].y, FLOOR_Z, 0, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 40000))
            {
                pSpark->CastSpell(pSpark, SPELL_POWER_SPARK_VISUAL, false);
                pSpark->GetMotionMaster()->MoveFollow(m_creature, 0, 0);
                //m_creature->AddSplineFlag(SPLINEFLAG_TRAJECTORY_FLY);
                m_lSparkGUIDList.push_back(pSpark->GetObjectGuid());
            }
        }
        else if(action == 2 || action == 3) // Start/stop movement
        {
            if(action == 3)
                m_creature->RemoveAurasDueToSpell(SPELL_POWER_SPARK);

            if (m_lSparkGUIDList.empty())
                return;

            for(std::list<ObjectGuid>::iterator itr = m_lSparkGUIDList.begin(); itr != m_lSparkGUIDList.end(); ++itr)
            {
                if (Creature* pTemp = (Creature*)m_creature->GetMap()->GetUnit(*itr))
                {
                    if (pTemp->isAlive())
                    {
                        if(action == 2)
                            pTemp->GetMotionMaster()->MoveFollow(m_creature, 0, 0);
                        else
                            pTemp->GetMotionMaster()->MovementExpired(false);
                    }
                }
            }
        }
    }
    void DoSpawnAdds()
    {
        //Nexus lords
        int max_lords = m_bIsRegularMode ? NEXUS_LORD_COUNT : NEXUS_LORD_COUNT_H;
        for(int i=0; i < max_lords;i++)
        {
            if(Creature *pLord = m_creature->SummonCreature(NPC_NEXUS_LORD, m_creature->getVictim()->GetPositionX()-5+rand()%10, m_creature->getVictim()->GetPositionY()-5+rand()%10, m_creature->getVictim()->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000))
                pLord->AI()->AttackStart(m_creature->getVictim());
        }
        //Scions of eternity
        int max_scions = m_bIsRegularMode ? SCION_OF_ETERNITY_COUNT : SCION_OF_ETERNITY_COUNT_H;
        for(int i=0; i < max_scions;i++)
        {
            uint32 x = urand(SHELL_MIN_X, SHELL_MAX_X);
            uint32 y = urand(SHELL_MIN_Y, SHELL_MAX_Y);
            if(Creature *pScion = m_creature->SummonCreature(NPC_SCION_OF_ETERNITY, x,y, m_creature->getVictim()->GetPositionZ(), 0, TEMPSUMMON_TIMED_OR_CORPSE_DESPAWN, 30000))
                if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                    pScion->AI()->AttackStart(pTarget);
        }       
    }
    bool IsThereAnyAdd()
    {
        //Search for Nexus lords
        if(GetClosestCreatureWithEntry(m_creature, NPC_NEXUS_LORD, 80.0f))
            return true;

        //Search for Scions of eternity
        if(GetClosestCreatureWithEntry(m_creature, NPC_SCION_OF_ETERNITY, 80.0f))
            return true;

        return false;
    }
    void DoSpawnShell()
    {
        uint32 x = urand(SHELL_MIN_X, SHELL_MAX_X);
        uint32 y = urand(SHELL_MIN_Y, SHELL_MAX_Y);
        if(Creature *pShell = m_creature->SummonCreature(NPC_ARCANE_OVERLOAD, x, y, FLOOR_Z, 0, TEMPSUMMON_TIMED_DESPAWN, 45000))
            pShell->CastSpell(pShell, SPELL_ARCANE_OVERLOAD, false);
    }
    /*void MountPlayers()
    {
        Map *pMap = m_creature->GetMap();

        if(!pMap)
            return;

        Map::PlayerList const &lPlayers = pMap->GetPlayers();
        if (lPlayers.isEmpty())
            return;
        
        for(Map::PlayerList::const_iterator itr = lPlayers.begin(); itr != lPlayers.end(); ++itr)
        {
            if (Player* pPlayer = itr->getSource())
            {
                if(pPlayer->GetVehicleGUID())
                    pPlayer->ExitVehicle();

                if(Vehicle *pTemp = m_creature->SummonVehicle(NPC_WYRMREST_SKYTALON, pPlayer->GetPositionX(), pPlayer->GetPositionY(), pPlayer->GetPositionZ(), 0))
                {
                    ((Creature*)pTemp)->SetUInt32Value(UNIT_CREATED_BY_SPELL, SPELL_RIDE_WYRMREST_SKYTALON);
                    ((Creature*)pTemp)->SetCreatorGUID(pPlayer->GetGUID());
                    pPlayer->CastSpell((Unit*)pTemp, SPELL_RIDE_WYRMREST_SKYTALON, true);

                    //Propably some calculations
                    ((Creature*)pTemp)->SetMaxHealth(100000);
                    ((Creature*)pTemp)->SetHealth(100000);
                    ((Creature*)pTemp)->SetArmor(pPlayer->GetArmor());
                    pPlayer->EnterVehicle(pTemp, 0, false);
                }
            }
        }
    }*/
    void DespawnCreatures(uint32 entry, float distance)
    {
        std::list<Creature*> m_pCreatures;
        GetCreatureListWithEntryInGrid(m_pCreatures, m_creature, entry, distance);

        if (m_pCreatures.empty())
            return;

        for(std::list<Creature*>::iterator iter = m_pCreatures.begin(); iter != m_pCreatures.end(); ++iter)
            (*iter)->ForcedDespawn();            
    }
    void UpdateAI(const uint32 uiDiff)
    {
        /*if (fly_fix)
        {
            float x, y, z;
            if(Creature *pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_AOE_TRIGGER, 180.0f))
            {
                pTrigger->GetPosition(x, y, z);
                z = m_creature->GetPositionZ();
                DoMovement(x, y, z, 0, true);
            }
            fly_fix = false;
        }*/

        if(m_uiPhase == PHASE_NOSTART)
        {
            if(m_uiSubPhase == SUBPHASE_FLY_UP)
            {
                float x, y, z;
                if(Creature *pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_AOE_TRIGGER, 180.0f))
                {
                    pTrigger->GetPosition(x, y, z);
                    z = m_creature->GetPositionZ();
                    DoMovement(x, y, z, 0, true);
                }
                m_uiSubPhase = SUBPHASE_UP;
            }
            else if(m_uiSubPhase == SUBPHASE_UP)
            {
                if(m_uiSpeechTimer[m_uiSpeechCount] <= uiDiff)
                {
                    DoScriptText(SAY_INTRO1-m_uiSpeechCount, m_creature);
                    m_uiSpeechCount++;
                    if(m_uiSpeechCount == 5){
                        m_uiSubPhase = SUBPHASE_FLY_DOWN1;
                    }
                }else m_uiSpeechTimer[m_uiSpeechCount] -= uiDiff;
            }
            else if(m_uiSubPhase == SUBPHASE_FLY_DOWN1)
            {
                if(m_uiTimer <= uiDiff)
                {
                    float x, y, z;
                    m_creature->GetPosition(x, y, z);
                    DoMovement(x, y, FLOOR_Z, 0, false, false);
                    m_uiSubPhase = SUBPHASE_FLY_DOWN2;
                    m_uiTimer = 1500;
                }else m_uiTimer -= uiDiff;
            }
            else if(m_uiSubPhase == SUBPHASE_FLY_DOWN2)
            {
                if(m_uiTimer <= uiDiff)
                {
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    m_uiSubPhase = 0;
                    m_uiPhase = PHASE_FLOOR;
                }else m_uiTimer -= uiDiff;
            }    
        }
        
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        //Enrage timer.....
        if(m_uiEnrageTimer <= uiDiff)
        {
            DoCast(m_creature, SPELL_BERSERK,true);
            m_uiEnrageTimer = 600000;
        }else m_uiEnrageTimer -= uiDiff;

        if(m_uiPhase == PHASE_FLOOR)
        {
            if(m_uiSubPhase == SUBPHASE_VORTEX)
            {
                if(m_uiTimer <= uiDiff)
                {
                    DoVortex(m_uiVortexPhase);
                    
                    if(m_uiVortexPhase == 1 || m_uiVortexPhase == 11)
                    {
                        if(m_uiVortexPhase == 1)
                            DoCast(m_creature, SPELL_VORTEX_DUMMY);
                        m_uiTimer = 300;
                    }else
                        m_uiTimer = 500;

                    if(m_uiVortexPhase >= MAX_VORTEX && m_uiVortexPhase < 29)
                    {
                        m_uiVortexPhase = 30;
                        DoVortex(m_uiVortexPhase);
                        PowerSpark(2);
                        m_uiTimer = 1000;
                        return;
                    }
                    m_uiVortexPhase++;
                }else m_uiTimer -= uiDiff;
                return;
            }

            //Vortex
            if(m_uiVortexTimer <= uiDiff)
            {
                PowerSpark(3);
                DoVortex(0);
                m_uiVortexPhase = 1;
                m_uiSubPhase = SUBPHASE_VORTEX;
                m_uiVortexTimer = 50000;
                m_uiTimer = 1000;
                DoScriptText(SAY_VORTEX, m_creature);
                return;
            }else m_uiVortexTimer -= uiDiff;

            //Arcane Breath
            if(m_uiArcaneBreathTimer <= uiDiff && m_uiSubPhase != SUBPHASE_VORTEX)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_ARCANE_BREATH : SPELL_ARCANE_BREATH_H);
                m_uiArcaneBreathTimer = 15000 + urand(3000, 8000);
            }else m_uiArcaneBreathTimer -= uiDiff;

            //PowerSpark
            if(m_uiPowerSparkTimer<= uiDiff)
            {
                PowerSpark(1);
                m_uiPowerSparkTimer = 30000;
            }else m_uiPowerSparkTimer -= uiDiff;

            //Health check
            if(m_uiTimer<= uiDiff)
            {
                uint8 health = m_creature->GetHealth()*100 / m_creature->GetMaxHealth();                    
                if(health <= 50)
                {
                    m_creature->InterruptNonMeleeSpells(true);
                    SetCombatMovement(false);
                    m_creature->GetMotionMaster()->MovementExpired(false);
                    DoScriptText(SAY_END_PHASE1, m_creature);
                    float x, y, z;
                    if(Creature *pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_AOE_TRIGGER, 60.0f))
                    {
                        pTrigger->GetPosition(x, y, z);
                        DoMovement(x, y, z+40, 0, true);
                    }
                    //Despawn power sparks
                    DespawnCreatures(NPC_POWER_SPARK, 120.0f);
                    m_uiPhase = PHASE_ADDS;
                    m_uiSubPhase = SUBPHASE_TALK;
                    m_uiTimer = 23000;
                    return;
                }
                m_uiTimer = 1500;
            }else m_uiTimer -= uiDiff;  

            DoMeleeAttackIfReady();
        }else if(m_uiPhase == PHASE_ADDS)
        {
            if(m_uiSubPhase == SUBPHASE_TALK)
            {
                if(m_uiTimer <= uiDiff)
                {
                    DoScriptText(SAY_AGGRO2, m_creature);
                    m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    DoSpawnAdds();
                    DoSpawnShell();
                    m_uiShellTimer = 30000;
                    m_uiSubPhase = 0;
                    m_uiTimer = 15000;
                }else m_uiTimer -= uiDiff;
                return;
            }
            
            //Arcane overload (bubble)
            if(m_uiShellTimer <= uiDiff)
            {
                DoSpawnShell();
                DoScriptText(SAY_ARCANE_OVERLOAD, m_creature);
                m_uiShellTimer = 30000;
            }else m_uiShellTimer -= uiDiff;

            // Arcane Pulse
            if(m_uiDeepBreathTimer <= uiDiff)
            {
                DoScriptText(SAY_ARCANE_PULSE, m_creature);
                DoScriptText(SAY_ARCANE_PULSE_WARN, m_creature);
                if(Creature *pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_AOE_TRIGGER, 60.0f))
                    pTrigger->CastSpell(pTrigger, SPELL_ARCANE_PULSE, true);

                m_uiDeepBreathTimer = 60000;
            }else m_uiDeepBreathTimer -= uiDiff;

            // Arcane Storm
            if(m_uiArcaneStormTimer <= uiDiff)
            {
                DoCast(m_creature, m_bIsRegularMode ? SPELL_ARCANE_STORM : SPELL_ARCANE_STORM_H);
                m_uiArcaneStormTimer = 20000;
            }else m_uiArcaneStormTimer -= uiDiff;

            if(m_uiTimer <= uiDiff)
            {
                if(!IsThereAnyAdd())
                {
                    m_uiPhase = PHASE_DRAGONS;
                    m_uiSubPhase = SUBPHASE_DESTROY_PLATFORM1;
                    DoScriptText(SAY_END_PHASE2, m_creature);
                    //if(Creature *pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_AOE_TRIGGER, 60.0f))
                        //pTrigger->CastSpell(pTrigger, SPELL_DESTROY_PLATFORM_PRE, false);
                    m_uiTimer = 6500;
                    return;
                }
                m_uiTimer = 5000;
            }else m_uiTimer -= uiDiff;  
        }
        else if(m_uiPhase == PHASE_DRAGONS)
        {
            if(m_uiSubPhase == SUBPHASE_DESTROY_PLATFORM1)
            {
                if(m_uiTimer<= uiDiff)
                {
                    //Destroy Platform
                    if(Creature *pTrigger = GetClosestCreatureWithEntry(m_creature, NPC_AOE_TRIGGER, 60.0f))
                        pTrigger->CastSpell(pTrigger, SPELL_DESTROY_PLATFROM_BOOM, false);
//                    if(GameObject *pPlatform = GetClosestGameObjectWithEntry(m_creature, GO_PLATFORM, 120.0f))
                        //pPlatform->Delete();                
                    
                    //Mount Players
                    //MountPlayers();

                    //Despawn bubbles and discs
                    DespawnCreatures(NPC_ARCANE_OVERLOAD, 70.0f);
                    DespawnCreatures(NPC_HOVER_DISC, 70.0f);

                    m_uiTimer = 2000;
                    m_uiSubPhase = SUBPHASE_DESTROY_PLATFORM2;
                }else m_uiTimer -= uiDiff;
                return;
            }
            else if(m_uiSubPhase == SUBPHASE_DESTROY_PLATFORM2)
            {
                if(m_uiTimer<= uiDiff){
                    float x, y, z;
                    m_creature->GetPosition(x, y, z);
                    DoMovement(x, y, z, 0, true);
                    m_uiSubPhase = SUBPHASE_DESTROY_PLATFORM3;
                    DoScriptText(SAY_INTRO_PHASE3, m_creature);
                    m_uiTimer = 14900;
                }else m_uiTimer -= uiDiff;
                return;
            }
            else if(m_uiSubPhase == SUBPHASE_DESTROY_PLATFORM3)
            {
                if(m_uiTimer<= uiDiff)
                {
                    m_uiSubPhase = 0;
                    m_creature->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    SetCombatMovement(true);
                    m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 0);
                    m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 0);
                    if(m_creature->getVictim())
                        m_creature->GetMotionMaster()->MoveChase(m_creature->getVictim());
                    /*if(Unit *pVehicle = ((Unit*)m_creature->GetMap()->GetUnit(m_creature->getVictim()->GetVehicleGUID())))
                    {
                        DoResetThreat();
                        m_creature->AI()->AttackStart(pVehicle);
                        m_creature->AddThreat(pVehicle, 1.0f);
                        m_creature->GetMotionMaster()->MoveChase(pVehicle);
                    }*/
                    DoScriptText(SAY_AGGRO3, m_creature);
                }
                else m_uiTimer -= uiDiff;
                return;
            }
            //Static field
            /*if(m_uiStaticFieldTimer <= uiDiff)
            {
                int32 x = urand(uint32(m_creature->GetPositionX())-50, uint32(m_creature->GetPositionX())+50);
                int32 y = urand(uint32(m_creature->GetPositionY())-50, uint32(m_creature->GetPositionY())+50);
                if(Creature *pField = m_creature->SummonCreature(NPC_STATIC_FIELD, x, y, m_creature->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN, 30000))
                    pField->CastSpell(pField, SPELL_STATIC_FIELD, true);
                m_uiStaticFieldTimer = 20000+rand()%10000;
            }else m_uiStaticFieldTimer -= uiDiff;*/
            DoMeleeAttackIfReady();
        }
        //Outro!
        else if(m_uiPhase == PHASE_OUTRO)
        {
            if(m_uiSpeechTimer[m_uiSpeechCount] <= uiDiff)
            {
                Creature *pSpeaker = NULL;
                if(m_uiSpeechCount == 0)
                    pSpeaker = m_creature;
                else
                    pSpeaker = pAlexstrasza;

                //if(pSpeaker && pSpeaker->isAlive())
                    //DoScriptText(SAY_OUTRO1-m_uiSpeechCount, pSpeaker);

                switch(m_uiSpeechCount)
                {
                    case 1:
                        m_creature->SetVisibility(VISIBILITY_OFF);
                        break;
                    case 5:
                        m_uiSubPhase = SUBPHASE_DIE;
                        if(pAlexstrasza && pAlexstrasza->isAlive())
                            pAlexstrasza->ForcedDespawn();
                        //Summon exit portal
                        if(!GetClosestGameObjectWithEntry(m_creature, GO_EXIT_PORTAL, 120.0f))
                            m_creature->SummonGameObject(GO_EXIT_PORTAL, GOPositions[2].x, GOPositions[2].y, GOPositions[2].z, GOPositions[2].o, 0, 0, 0, 0, 25000);
                        m_creature->DealDamage(m_creature, m_creature->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
                        break;

                }
                m_uiSpeechCount++;
            }else m_uiSpeechTimer[m_uiSpeechCount] -= uiDiff;
            
        }
    }
};
/*######
## mob_power_spark
######*/
struct MANGOS_DLL_DECL mob_power_sparkAI : public ScriptedAI
{
    mob_power_sparkAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    bool isDead;
    uint32 m_uiCheckTimer;
    Creature *pMalygos;

    void Reset()
    {
        isDead = false;
        pMalygos = GetClosestCreatureWithEntry(m_creature, NPC_MALYGOS, 150.0f);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_0, 50331648);
        m_creature->SetUInt32Value(UNIT_FIELD_BYTES_1, 50331648);
        m_creature->AddSplineFlag(SPLINEFLAG_FLYING);
        //WorldPacket heart;
        //m_creature->BuildHeartBeatMsg(&heart);
        //m_creature->SendMessageToSet(&heart, false);
        m_uiCheckTimer = 2500;
    }
    void AttackStart(Unit *pWho)
    {
        return;
    }
    void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
    {
        if (isDead)
        {
            uiDamage = 0;
            return;
        }

        if (uiDamage > m_creature->GetHealth())
        {
            isDead = true;

            if (m_creature->IsNonMeleeSpellCasted(false))
                m_creature->InterruptNonMeleeSpells(false);

            m_creature->RemoveAllAuras();
            m_creature->AttackStop();
            SetCombatMovement(false);

            if (m_creature->GetMotionMaster()->GetCurrentMovementGeneratorType() == CHASE_MOTION_TYPE)
                m_creature->GetMotionMaster()->MovementExpired();

            uiDamage = 0;
            m_creature->SetHealth(1);
            m_creature->CastSpell(m_creature, SPELL_POWER_SPARK_PLAYERS, true);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
            m_creature->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
            m_uiCheckTimer = 25000;
            if(pMalygos && pMalygos->isAlive())
                ((boss_malygosAI*)pMalygos->AI())->m_lSparkGUIDList.clear();
        }
    }
    void UpdateAI(const uint32 uiDiff)
    {
        if(m_uiCheckTimer <= uiDiff)
        {
            if(isDead)
            {
                m_creature->ForcedDespawn();
                return;
            }
            pMalygos = GetClosestCreatureWithEntry(m_creature, NPC_MALYGOS, 150.0f);
            if(pMalygos && pMalygos->isAlive() && m_creature->GetVisibility() == VISIBILITY_ON)
            {
                //WorldPacket heart;
                //m_creature->BuildHeartBeatMsg(&heart);
                //m_creature->SendMessageToSet(&heart, false);
                if(m_creature->IsWithinDist(pMalygos, 3.0f, false))
                {                    
                    ((boss_malygosAI*)pMalygos->AI())->m_lSparkGUIDList.clear();
                    m_creature->CastSpell(pMalygos, SPELL_POWER_SPARK, true);
                    m_creature->SetVisibility(VISIBILITY_OFF);
                }
            }
            m_uiCheckTimer = 2500;
        }else m_uiCheckTimer -= uiDiff;
    }
};
/*######
## mob_scion_of_eternity
######*/
struct MANGOS_DLL_DECL mob_scion_of_eternityAI : public ScriptedAI
{
    mob_scion_of_eternityAI(Creature* pCreature) : ScriptedAI(pCreature)
    {
        m_pInstance = (ScriptedInstance*)pCreature->GetInstanceData();
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();
    }

    ScriptedInstance* m_pInstance;
    bool m_bIsRegularMode;
    uint32 m_uiArcaneBarrageTimer;

    void Reset()
    {
        m_creature->SetSpeedRate(MOVE_WALK, 3.5f, true);
        m_creature->SetSpeedRate(MOVE_RUN, 3.5f, true);
        m_creature->SetSpeedRate(MOVE_FLIGHT, 3.5f, true);
        DoNextMovement();
        m_uiArcaneBarrageTimer = 6000 + rand()%5000;

    }
    void AttackStart(Unit *pWho)
    {
        if (m_creature->Attack(pWho, true))
        {
            m_creature->AddThreat(pWho);
            m_creature->SetInCombatWith(pWho);
            pWho->SetInCombatWith(m_creature);
            //m_creature->GetMotionMaster()->MoveChase(pWho, 15.0f);
        }
    }
    void DoNextMovement()
    {
        uint32 x = urand(SHELL_MIN_X, SHELL_MAX_X);
        uint32 y = urand(SHELL_MIN_Y, SHELL_MAX_Y);
        uint32 z = urand(uint32(FLOOR_Z), uint32(FLOOR_Z)+10);
        m_creature->GetMotionMaster()->MovePoint(0, x, y, z);
    }
    void MovementInform(uint32 uiType, uint32 uiPointId)
    {
        if(uiType != POINT_MOTION_TYPE)
            return;

        switch(uiPointId)
        {
            case 0:
                DoNextMovement();
                break;
        }
    }

    void UpdateAI(const uint32 uiDiff)
    {
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;
        
        if(m_uiArcaneBarrageTimer <= uiDiff)
        {
            m_creature->GetMotionMaster()->MovementExpired(false);
            if (Unit* pTarget = m_creature->SelectAttackingTarget(ATTACKING_TARGET_RANDOM, 0))
                DoCast(pTarget, SPELL_ARCANE_BARRAGE, true);
            m_uiArcaneBarrageTimer = 3000 + rand()%5000;
            DoNextMovement();
        }else m_uiArcaneBarrageTimer -= uiDiff;
    }
};
/*######
## go_focusing_iris
######*/
bool GOHello_go_focusing_iris(Player* pPlayer, GameObject* pGo)
{
    bool m_bIsRegularMode = pGo->GetMap()->IsRegularDifficulty();

    bool hasItem = false;
    if (m_bIsRegularMode)
    {
        if(pPlayer->HasItemCount(ITEM_KEY_TO_FOCUSING_IRIS, 1) || pPlayer->HasItemCount(ITEM_KEY_TO_FOCUSING_IRIS_H, 1)) 
            hasItem = true;
    }else{
        if(pPlayer->HasItemCount(ITEM_KEY_TO_FOCUSING_IRIS_H, 1))
            hasItem = true;        
    }
    if(!hasItem)        
        return false;

    if(Creature *pMalygos = GetClosestCreatureWithEntry(pGo, NPC_MALYGOS, 150.0f))
    {
        ((boss_malygosAI*)pMalygos->AI())->m_uiSubPhase = SUBPHASE_FLY_UP;
    }

    pGo->Delete();
    return false;
}

/*######
## Mob Arcane Overload
######*/

struct MANGOS_DLL_DECL mob_arcane_overloadAI : public ScriptedAI
{
    mob_arcane_overloadAI(Creature* pCreature) : ScriptedAI(pCreature) 
    {
        m_bIsRegularMode = pCreature->GetMap()->IsRegularDifficulty();
        Reset();         
    }

    uint32 m_uiVoidBlast;
    bool m_bIsRegularMode;

    void Aggro(Unit* who)
    {
        DoStopAttack();
        SetCombatMovement(false);
    }

    void Reset()
    {

    }

    void UpdateAI(const uint32 uiDiff)
    {
    }
};

CreatureAI* GetAI_mob_arcane_overload(Creature* pCreature)
{
    return new mob_arcane_overloadAI(pCreature);
}

CreatureAI* GetAI_boss_malygos(Creature* pCreature)
{
    return new boss_malygosAI(pCreature);
}
CreatureAI* GetAI_mob_power_spark(Creature* pCreature)
{
    return new mob_power_sparkAI(pCreature);
}
CreatureAI* GetAI_mob_scion_of_eternity(Creature* pCreature)
{
    return new mob_scion_of_eternityAI(pCreature);
}


void AddSC_boss_malygos()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "boss_malygos";
    newscript->GetAI = &GetAI_boss_malygos;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_power_spark";
    newscript->GetAI = &GetAI_mob_power_spark;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_scion_of_eternity";
    newscript->GetAI = &GetAI_mob_scion_of_eternity;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "go_focusing_iris";
    newscript->pGOUse = &GOHello_go_focusing_iris;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "mob_arcane_overload";
    newscript->GetAI = &GetAI_mob_arcane_overload;
    newscript->RegisterSelf();
}
/*
INSERT IGNORE INTO npc_spellclick_spells VALUES (30161, 60968, 0, 0, 0, 1), (30248, 60968, 0, 0, 0, 1);

INSERT INTO `vehicle_data` (
`entry` ,
`flags` ,
`Spell1` ,
`Spell2` ,
`Spell3` ,
`Spell4` ,
`Spell5` ,
`Spell6` ,
`Spell7` ,
`Spell8` ,
`Spell9` ,
`Spell10` ,
`req_aura` 
)
VALUES (
'264', '24', '56091', '56092', '57090', '57143', '57108', '57092', '0', '0', '0', '0', '0'
)
INSERT INTO `vehicle_seat_data` (
`seat` ,
`flags` 
)
VALUES (
'2449', '1'
);


UPDATE gameobject_template SET ScriptName="go_focusing_iris" WHERE entry IN (193960, 193958);
UPDATE creature_template SET ScriptName="boss_malygos" WHERE entry=28859;
UPDATE creature_template SET ScriptName="mob_power_spark", faction_A = 16, faction_H = 16, `InhabitType` = '7', speed ='1' WHERE entry=30084;
UPDATE creature_template SET ScriptName="mob_scion_of_eternity", `InhabitType` = '7', faction_A = 16, faction_H = 16 WHERE entry=30249;
UPDATE `creature_template` SET `AIName` = 'NullAI' WHERE `entry` =30282;

SELECT * FROM creature_addon WHERE guid IN (SELECT guid FROM creature WHERE id=28859)
SELECT * FROM creature_template WHERE entry=28859;
UPDATE `creature_template` SET `InhabitType` = '3' WHERE `entry` =28859 LIMIT 1 ;
UPDATE `mangostest`.`creature` SET `spawndist` = '0',
`MovementType` = '0' WHERE `creature`.`guid` =132313 LIMIT 1 ;
INSERT INTO creature_template_addon (entry, vehicle_id) VALUES (30161, 194);
INSERT INTO creature_template_addon (entry, vehicle_id) VALUES (32535, 264);

INSERT INTO `vehicle_data` (`entry`, `flags`, `Spell1`, `Spell2`, `Spell3`, `Spell4`, `Spell5`, `Spell6`, `Spell7`, `Spell8`, `Spell9`, `Spell10`, `req_aura`) VALUES
(194, 280, 56091, 56092, 57090, 57143, 57108, 57092, 0, 0, 0, 0, 0);
INSERT INTO `vehicle_seat_data` (`seat`, `flags`) VALUES ('2102', '1');

INSERT INTO `script_texts` (`entry`, `content_default`, `content_loc1`, `content_loc2`, `content_loc3`, `content_loc4`, `content_loc5`, `content_loc6`, `content_loc7`, `content_loc8`, `sound`, `type`, `language`, `emote`, `comment`) VALUES
('-1616000', 'Lesser beings, intruding here! A shame that your excess courage does not compensate for your stupidity!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14512', '1', '0', '457', NULL),
('-1616001', 'None but the blue dragonflight are welcome here! Perhaps this is the work of Alexstrasza? Well then, she has sent you to your deaths.', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14513', '1', '0', '457', NULL),
('-1616002', 'What could you hope to accomplish, to storm brazenly into my domain? To employ MAGIC? Against ME? <Laughs>', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14514', '1', '0', '457', NULL),
('-1616003', 'I am without limits here... the rules of your cherished reality do not apply... In this realm, I am in control...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14515', '1', '0', '457', NULL),
('-1616004', 'I give you one chance. Pledge fealty to me, and perhaps I won''t slaughter you for your insolence!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14516', '1', '0', '457', NULL),
('-1616005', 'My patience has reached its limit, I WILL BE RID OF YOU!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14517', '1', '0', '1', NULL),
('-1616006', 'Watch helplessly as your hopes are swept away...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14525', '1', '0', '1', NULL),
('-1616007', 'I AM UNSTOPPABLE!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14533', '1', '0', '1', NULL),
('-1616008', 'Your stupidity has finally caught up to you!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14519', '1', '0', '1', NULL),
('-1616009', 'More artifacts to confiscate...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14520', '1', '0', '1', NULL),
('-1616010', '<Laughs> How very... naive...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14521', '1', '0', '1', NULL),
('-1616011', 'UNTHINKABLE! The mortals will destroy... e-everything... my sister... what have you-', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14540', '1', '0', '1', NULL),
('-1616012', 'I had hoped to end your lives quickly, but you have proven more...resilient then I had anticipated. Nonetheless, your efforts are in vain, it is you reckless, careless mortals who are to blame for this war! I do what I must...And if it means your...extinction...THEN SO BE IT!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14522', '1', '0', '1', NULL),
('-1616013', 'Few have experienced the pain I will now inflict upon you!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14523', '1', '0', '1', NULL),
('-1616014', 'YOU WILL NOT SUCCEED WHILE I DRAW BREATH!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14523', '1', '0', '1', NULL),
('-1616015', 'Malygos takes a deep breath...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14523', '1', '0', '1', NULL),
('-1616016', 'I will teach you IGNORANT children just how little you know of magic...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14524', '1', '0', '1', NULL),
('-1616017', 'ENOUGH! If you intend to reclaim Azeroth\'s magic, then you shall have it...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14529', '1', '0', '1', NULL),
('-1616018', 'Now your benefactors make their appearance...But they are too late. The powers contained here are sufficient to destroy the world ten times over! What do you think they will do to you?', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14530', '1', '0', '1', NULL),
('-1616019', 'SUBMIT!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14531', '1', '0', '1', NULL),
('-1616020', 'Your energy will be put to good use!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14526', '1', '0', '1', NULL),
('-1616021', 'I AM THE SPELL-WEAVER! My power is INFINITE!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14527', '1', '0', '1', NULL),
('-1616022', 'Your spirit will linger here forever!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14528', '1', '0', '1', NULL),
('-1616023', 'Alexstrasza! Another of your brood falls!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14534', '1', '0', '1', NULL),
('-1616024', 'Little more then gnats!', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14535', '1', '0', '1', NULL),
('-1616025', 'Your red allies will share your fate...', NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, '14536', '1', '0', '1', NULL);


UPDATE `creature_template` SET `AIName` = 'NullAI' WHERE `creature_template`.`entry` =22517 LIMIT 1 ;

UPDATE `creature_template` SET `AIName` = 'NullAI' WHERE `creature_template`.`entry` =30090 LIMIT 1 ;

UPDATE `creature_template` SET `AIName` = 'NullAI' WHERE `creature_template`.`entry` =30118 LIMIT 1 ;

UPDATE `creature_template` SET `AIName` = 'NullAI' WHERE `creature_template`.`entry` =30334 LIMIT 1 ;

UPDATE `creature_template` SET `AIName` = 'NullAI' WHERE `creature_template`.`entry` =31253 LIMIT 1 ;

UPDATE `creature_template` SET `AIName` = 'NullAI' WHERE `creature_template`.`entry` =32448 LIMIT 1 ;

UPDATE `creature_template` SET 

UPDATE `creature_template` SET `modelid_A` = '11686',
`modelid_A2` = '11686',
`modelid_H` = '11686',
`modelid_H2` = '11686',
`minhealth` = '44120',
`maxhealth` = '44120',
minlevel=80,
maxlevel=80,
`AIName` = '',
`flags_extra` = '2' WHERE `creature_template`.`entry` =22517 LIMIT 1 ;

INSERT INTO `spell_script_target` (`entry`, `type`, `targetEntry`) VALUES ('56152', '1', '28859');
UPDATE `creature_model_info` SET `combat_reach` = '30' WHERE `modelid` =26752;

UPDATE `creature_template` SET `InhabitType` = '4' WHERE `entry` IN (30118, 30334, 30592);

INSERT INTO `gameobject_template` (`entry`, `type`, `displayId`, `name`, `IconName`, `castBarCaption`, `unk1`, `faction`, `flags`, `size`, `questItem1`, `questItem2`, `questItem3`, `questItem4`, `questItem5`, `questItem6`, `data0`, `data1`, `data2`, `data3`, `data4`, `data5`, `data6`, `data7`, `data8`, `data9`, `data10`, `data11`, `data12`, `data13`, `data14`, `data15`, `data16`, `data17`, `data18`, `data19`, `data20`, `data21`, `data22`, `data23`, `ScriptName`) VALUES
(393070, 0, 8390, 'Nexus Raid Platform - custom destroyed', '', '', '', 35, 36, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '');

*/
