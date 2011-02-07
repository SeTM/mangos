/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
 * This program is free software licensed under GPL version 2
 * Please see the included DOCS/LICENSE.TXT for more information */

#ifndef DEF_NAXXRAMAS_H
#define DEF_NAXXRAMAS_H

enum
{
    MAX_ENCOUNTER               = 15,

    // Kel'Thuzad's taunts after killing Wing Bosses
    SAY_KELTHUZAD_TAUNT1        = -1533090,
    SAY_KELTHUZAD_TAUNT2        = -1533091,
    SAY_KELTHUZAD_TAUNT3        = -1533092,
    SAY_KELTHUZAD_TAUNT4        = -1533093,

    TYPE_ANUB_REKHAN            = 1,
    TYPE_FAERLINA               = 2,
    TYPE_MAEXXNA                = 3,

    TYPE_NOTH                   = 4,
    TYPE_HEIGAN                 = 5,
    TYPE_LOATHEB                = 6,

    TYPE_RAZUVIOUS              = 7,
    TYPE_GOTHIK                 = 8,
    TYPE_FOUR_HORSEMEN          = 9,

    TYPE_PATCHWERK              = 10,
    TYPE_GROBBULUS              = 11,
    TYPE_GLUTH                  = 12,
    TYPE_THADDIUS               = 13,
    TYPE_STALAGG                = 14,
    TYPE_FEUGEN                 = 15,

    TYPE_SAPPHIRON              = 16,
    TYPE_KELTHUZAD              = 17,

    TYPE_BLAUMEUX               = 18,
    TYPE_RIVENDARE              = 19,
    TYPE_ZELIEK                 = 20,
    TYPE_KORTHAZZ               = 21,

    DATA_ANUB_REKHAN            = 30,
    DATA_FAERLINA               = 31,
    DATA_GOTHIK_GATE            = 32,

    DATA_THADDIUS               = 33,
    DATA_STALAGG                = 34,
    DATA_FEUGEN                 = 35,

    DATA_KORTHAZZ               = 36,
    DATA_RIVENDARE              = 37,
    DATA_BLAUMEUX               = 38,
    DATA_ZELIEK                 = 39,
    DATA_HEIGAN_ERUPT           = 40,

    NPC_ANUB_REKHAN             = 15956,
    NPC_FAERLINA                = 15953,

    NPC_THADDIUS                = 15928,
    NPC_STALAGG                 = 15929,
    NPC_FEUGEN                  = 15930,

    NPC_ZELIEK                  = 16063,
    NPC_THANE                   = 16064,
    NPC_BLAUMEUX                = 16065,
    NPC_RIVENDARE               = 30549,

    NPC_KELTHUZAD               = 15990,

    // Faerlina
    NPC_NAXXRAMAS_FOLLOWER      = 16505,
    NPC_NAXXRAMAS_WORSHIPPER    = 16506,

    // Gothik
    NPC_GOTHIK                  = 16060,
    NPC_SUB_BOSS_TRIGGER        = 16137,                    //summon locations
    NPC_UNREL_TRAINEE           = 16124,
    NPC_UNREL_DEATH_KNIGHT      = 16125,
    NPC_UNREL_RIDER             = 16126,
    NPC_SPECT_TRAINEE           = 16127,
    NPC_SPECT_DEATH_KNIGHT      = 16148,
    NPC_SPECT_RIDER             = 16150,
    NPC_SPECT_HORSE             = 16149,

    // End boss adds
    NPC_SOLDIER_FROZEN          = 16427,
    NPC_UNSTOPPABLE_ABOM        = 16428,
    NPC_SOUL_WEAVER             = 16429,
    NPC_GUARDIAN                = 16441,

    // Arachnid Quarter
    GO_ARAC_ANUB_DOOR           = 181126,                   //encounter door
    GO_ARAC_ANUB_GATE           = 181195,                   //open after boss is dead
    GO_ARAC_FAER_WEB            = 181235,                   //encounter door
    GO_ARAC_FAER_DOOR           = 194022,                   //after faerlina, to outer ring
    GO_ARAC_MAEX_INNER_DOOR     = 181197,                   //encounter door
    GO_ARAC_MAEX_OUTER_DOOR     = 181209,                   //right before maex

    // Plague Quarter
    GO_PLAG_SLIME01_DOOR        = 181198,                   //not used
    GO_PLAG_SLIME02_DOOR        = 181199,                   //not used
    GO_PLAG_NOTH_ENTRY_DOOR     = 181200,                   //encounter door
    GO_PLAG_NOTH_EXIT_DOOR      = 181201,                   //exit, open when boss dead
    GO_PLAG_HEIG_ENTRY_DOOR     = 181202,
    GO_PLAG_HEIG_EXIT_DOOR      = 181203,                   //exit, open when boss dead
    GO_PLAG_LOAT_DOOR           = 181241,                   //encounter door

    // Military Quarter
    GO_MILI_GOTH_ENTRY_GATE     = 181124,                   //used while encounter is in progress
    GO_MILI_GOTH_EXIT_GATE      = 181125,                   //exit, open at boss dead
    GO_MILI_GOTH_COMBAT_GATE    = 181170,                   //used while encounter is in progress
    GO_MILI_HORSEMEN_DOOR       = 181119,                   //encounter door

    GO_CHEST_HORSEMEN_NORM      = 181366,                   //four horsemen event, DoRespawnGameObject() when event == DONE
    GO_CHEST_HORSEMEN_HERO      = 193426,

    // Construct Quarter
    GO_CONS_PATH_EXIT_DOOR      = 181123,
    GO_CONS_GLUT_EXIT_DOOR      = 181120,
    GO_CONS_THAD_DOOR           = 181121,                   // Thaddius enc door

    // Frostwyrm Lair
    GO_KELTHUZAD_WATERFALL_DOOR = 181225,                   // exit, open after sapphiron is dead
    GO_KELTHUZAD_EXIT_DOOR      = 181228,

    // Eyes
    GO_ARAC_EYE_RAMP            = 181212,
    GO_PLAG_EYE_RAMP            = 181211,
    GO_MILI_EYE_RAMP            = 181210,
    GO_CONS_EYE_RAMP            = 181213,

    // Portals
    GO_ARAC_PORTAL              = 181575,
    GO_PLAG_PORTAL              = 181577,
    GO_MILI_PORTAL              = 181578,
    GO_CONS_PORTAL              = 181576,

    AREATRIGGER_FROSTWYRM       = 4120,                    //not needed here, but AT to be scripted
    AREATRIGGER_KELTHUZAD       = 4112,
    AREATRIGGER_GOTHIK          = 4116
};

enum
{
    //Spiderwing
    ENCOUNT_ANUBREKHAN   = 0,
    ENCOUNT_FAERLINA     = 1,
    ENCOUNT_MAEXXNA      = 2,

    //Abomination Wing
    ENCOUNT_PATCHWERK    = 3,
    ENCOUNT_GROBBULUS    = 4,
    ENCOUNT_GLUTH        = 5,
    ENCOUNT_THADDIUS     = 6,

    //Deathknight Wing
    ENCOUNT_RAZUVIOUS    = 7,
    ENCOUNT_GOTHIK       = 8,
    ENCOUNT_FOURHORSEMAN = 9,

    //Plague Wing
    ENCOUNT_NOTH         = 10,
    ENCOUNT_HEIGAN       = 11,
    ENCOUNT_LOATHEB      = 12,

    //Frostwyrm Lair
    ENCOUNT_SAPPHIRON    = 13,
    ENCOUNT_KELTHUZAD    = 14
};

//Bosses
enum
{
    //Spiderwing
    GUID_ANUBREKHAN   = 100,
    GUID_FAERLINA     = 101,
    GUID_MAEXXNA      = 102,
    GUID_WORSHIPPER1  = 20,
    GUID_WORSHIPPER2  = 21,
    GUID_WORSHIPPER3  = 22,
    GUID_WORSHIPPER4  = 23,

    //Abomination Wing
    GUID_PATCHWERK    = 103,
    GUID_GROBBULUS    = 104,
    GUID_GLUTH        = 105,
    GUID_THADDIUS     = 600,
    GUID_FEUGEN       = 601,
    GUID_STALAGG      = 602,

    //Deathknight Wing
    GUID_RAZUVIOUS    = 107,
    GUID_GOTHIK       = 108,
    GUID_KORTHAZZ     = 901,
    GUID_BLAUMEUX     = 902,
    GUID_RIVENDARE    = 903,
    GUID_ZELIEK       = 904,

    //Plague Wing
    GUID_NOTH          = 110,
    GUID_HEIGAN       = 111,
    GUID_LOATHEB      = 112,

    //Frostwyrm Lair
    GUID_SAPPHIRON    = 113,
    GUID_KELTHUZAD    = 114
};

struct GothTrigger
{
    bool bIsRightSide;
    bool bIsAnchorHigh;
};

class MANGOS_DLL_DECL instance_naxxramas : public ScriptedInstance
{
    public:
        instance_naxxramas(Map* pMap);
        ~instance_naxxramas() {}

        void Initialize();

        bool IsEncounterInProgress() const;

        void OnCreatureCreate(Creature* pCreature);
        void OnObjectCreate(GameObject* pGo);

        void SetData(uint32 uiType, uint32 uiData);
        uint32 GetData(uint32 uiType);
        uint64 GetData64(uint32 uiData);

        const char* Save() { return strInstData.c_str(); }
        void Load(const char* chrIn);

        // goth
        void SetGothTriggers();
        Creature* GetClosestAnchorForGoth(Creature* pSource, bool bRightSide);
        void GetGothSummonPointCreatures(std::list<Creature*> &lList, bool bRightSide);
        bool IsInRightSideGothArea(Unit* pUnit);

        // kel
        void SetChamberCenterCoords(float fX, float fY, float fZ);
        void GetChamberCenterCoords(float &fX, float &fY, float &fZ) { fX = m_fChamberCenterX; fY = m_fChamberCenterY; fZ = m_fChamberCenterZ; }
        void DoTaunt();
        
    protected:
        uint32 m_auiEncounter[MAX_ENCOUNTER];
        std::string strInstData;

        uint64 m_uiAracEyeRampGUID;
        uint64 m_uiPlagEyeRampGUID;
        uint64 m_uiMiliEyeRampGUID;
        uint64 m_uiConsEyeRampGUID;

        uint64 m_uiAracPortalGUID;
        uint64 m_uiPlagPortalGUID;
        uint64 m_uiMiliPortalGUID;
        uint64 m_uiConsPortalGUID;

        uint64 m_uiAnubRekhanGUID;
        uint64 m_uiFaerlinanGUID;

        uint64 m_uiZeliekGUID;
        uint64 m_uiThaneGUID;
        uint64 m_uiBlaumeuxGUID;
        uint64 m_uiRivendareGUID;

        uint64 m_uiThaddiusGUID;
        uint64 m_uiStalaggGUID;
        uint64 m_uiFeugenGUID;

        uint64 m_uiKelthuzadGUID;

        uint64 m_uiPathExitDoorGUID;
        uint64 m_uiGlutExitDoorGUID;
        uint64 m_uiThadDoorGUID;

        uint64 m_uiAnubDoorGUID;
        uint64 m_uiAnubGateGUID;
        uint64 m_uiFaerDoorGUID;
        uint64 m_uiFaerWebGUID;
        uint64 m_uiMaexOuterGUID;
        uint64 m_uiMaexInnerGUID;
        std::list<uint64> m_lFaerlinaAddGUIDs;

        uint64 m_uiGothikGUID;
        uint64 m_uiGothCombatGateGUID;
        uint64 m_uiGothikEntryDoorGUID;
        uint64 m_uiGothikExitDoorGUID;
        std::list<uint64> m_lGothTriggerList;
        UNORDERED_MAP<uint64, GothTrigger> m_mGothTriggerMap;

        uint64 m_uiHorsemenDoorGUID;
        uint64 m_uiHorsemenChestGUID;

        uint64 m_uiNothEntryDoorGUID;
        uint64 m_uiNothExitDoorGUID;
        uint64 m_uiHeigEntryDoorGUID;
        uint64 m_uiHeigExitDoorGUID;
        uint64 m_uiLoathebDoorGUID;

        uint64 m_uiKelthuzadDoorGUID;
        uint64 m_uiKelthuzadExitDoorGUID;

        float m_fChamberCenterX;
        float m_fChamberCenterY;
        float m_fChamberCenterZ;
};

#endif
