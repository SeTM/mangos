/* Copyright (C) 2006 - 2011 ScriptDev2 <https://scriptdev2.svn.sourceforge.net/>
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
SDName: Stranglethorn_Vale
SD%Complete: 100
SDComment: Quest support: 592, 8193
SDCategory: Stranglethorn Vale
EndScriptData */

/* ContentData
mob_yenniku
npc_riggle_bassbait
EndContentData */

#include "precompiled.h"
#include "escort_ai.h"
#include "ObjectMgr.h"
#include "Config/Config.h"
#include "GameEventMgr.h"

enum
{
    SAY_START               = -1510356,
    SAY_WINNER              = -1510357,
    SAY_END                 = -1510358,
    QUEST_MASTER_ANGLER     = 8193,
};

/*######
## mob_yenniku
######*/

struct MANGOS_DLL_DECL mob_yennikuAI : public ScriptedAI
{
    mob_yennikuAI(Creature *c) : ScriptedAI(c)
    {
        bReset = false;
        Reset();
    }

    uint32 Reset_Timer;
    bool bReset;

    void Reset()
    {
        Reset_Timer = 0;
        m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_NONE);
    }

    void SpellHit(Unit *caster, const SpellEntry *spell)
    {
        if (caster->GetTypeId() == TYPEID_PLAYER)
        {
                                                            //Yenniku's Release
            if (!bReset && ((Player*)caster)->GetQuestStatus(592) == QUEST_STATUS_INCOMPLETE && spell->Id == 3607)
            {
                m_creature->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STUN);
                m_creature->CombatStop();                   //stop combat
                m_creature->DeleteThreatList();             //unsure of this
                m_creature->setFaction(83);                 //horde generic

                bReset = true;
                Reset_Timer = 60000;
            }
        }
        return;
    }

    void Aggro(Unit *who) {}

    void UpdateAI(const uint32 diff)
    {
        if (bReset)
            if (Reset_Timer < diff)
        {
            EnterEvadeMode();
            bReset = false;
            m_creature->setFaction(28);                     //troll, bloodscalp
        }
        else Reset_Timer -= diff;

        //Return since we have no target
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim() )
            return;

        DoMeleeAttackIfReady();
    }
};
CreatureAI* GetAI_mob_yenniku(Creature *_Creature)
{
    return new mob_yennikuAI (_Creature);
}

#define GOSSIP_HAVE_CODE       "Я имею код для получения приза"
bool GossipHello_npc_landro_longshot(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isVendor())
        pPlayer->ADD_GOSSIP_ITEM(GOSSIP_ICON_VENDOR, GOSSIP_TEXT_BROWSE_GOODS, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_TRADE);
    pPlayer->ADD_GOSSIP_ITEM_EXTENDED(0, GOSSIP_HAVE_CODE, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1, "", 0, true);
    pPlayer->SEND_GOSSIP_MENU(pPlayer->GetGossipTextId(pCreature), pCreature->GetGUID());
    return true;
}

bool GossipSelect_npc_landro_longshot(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction)
{
    if (uiAction == GOSSIP_ACTION_TRADE)
        pPlayer->SEND_VENDORLIST(pCreature->GetGUID());

    return true;
}

int CheckCode(const char* sCode)
{
    // check if db gone away
    QueryResult *result = SD2Database.PQuery("SELECT 1 FROM sd2_db_version");
    if (!result)
    {
        Config SD2Config;
        SD2Config.SetSource("/home/mangos/bin/wolk/etc/scriptdev2.conf");
        std::string strSD2DBinfo = SD2Config.GetStringDefault("ScriptDev2DatabaseInfo", "");
        SD2Database.Initialize(strSD2DBinfo.c_str());
    }
    delete result;

    int type = 0;
    std::string Code = sCode;
    SD2Database.escape_string(Code);
    QueryResult* pResult = SD2Database.PQuery("SELECT type FROM raffle_code where code = '%s'", Code.c_str());
    if (pResult)
    {
        Field* pFields = pResult->Fetch();
        type = pFields[0].GetUInt32();
    }
    delete pResult;
    return type;
}

void CleanupCode(const char* sCode)
{
    // check if db gone away
    QueryResult *result = SD2Database.PQuery("SELECT 1 FROM sd2_db_version");
    if (!result)
    {
        Config SD2Config;
        SD2Config.SetSource("/home/mangos/bin/wolk/etc/scriptdev2.conf");
        std::string strSD2DBinfo = SD2Config.GetStringDefault("ScriptDev2DatabaseInfo", "");
        SD2Database.Initialize(strSD2DBinfo.c_str());
    }
    delete result;

    std::string Code = sCode;
    SD2Database.escape_string(Code);
    SD2Database.PExecute("DELETE FROM raffle_code WHERE code = ('%s')", Code.c_str());
}

void RewardPlayer(Player *pPlayer, const char* sCode, uint32 item, uint32 spell)
{
    ItemPosCountVec dest;
    uint8 msg = pPlayer->CanStoreNewItem(NULL_BAG, NULL_SLOT, dest, item, 1, false);
    if( msg == EQUIP_ERR_OK )
    {
        if (spell)
            pPlayer->CastSpell(pPlayer,spell,true);
        else
            pPlayer->StoreNewItem(dest, item, 1, true);

        CleanupCode(sCode);
    }
    else
    {
        pPlayer->SendEquipError( msg, NULL, NULL );
    }
}

bool GossipSelectWithCode_npc_landro_longshot(Player* pPlayer, Creature* pCreature, uint32 uiSender, uint32 uiAction, const char* sCode)
{
    if (uiSender == GOSSIP_SENDER_MAIN)
    {
        switch (uiAction)
        {
        case GOSSIP_ACTION_INFO_DEF+1:
            {
                int prize = 0;
                prize = CheckCode(sCode);
                if (prize)
                {
                    switch (prize)
                    {
                    case 1:
                        RewardPlayer(pPlayer, sCode, 49284, 0);
                        break;
                    case 2:
                        RewardPlayer(pPlayer, sCode, 49286, 0);
                        break;
                    case 3:
                        RewardPlayer(pPlayer, sCode, 35227, 0);
                        break;
                    case 4:
                        RewardPlayer(pPlayer, sCode, 23713, 0);
                        break;
                    case 5:
                        RewardPlayer(pPlayer, sCode, 32588, 0);
                        break;
                    case 6:
                        RewardPlayer(pPlayer, sCode, 54811, 0);
                        break;
                    case 7:
                        RewardPlayer(pPlayer, sCode, 54069, 0);
                        break;
                    case 8:
                        RewardPlayer(pPlayer, sCode, 800000, 0);
                        break;
                    case 9:
                        RewardPlayer(pPlayer, sCode, 46017, 0);
                        break;
                    }
                }
                pPlayer->CLOSE_GOSSIP_MENU();
                break;
            }
        }
        return true;
    }
    return false;
}

/*######
##npc_riggle_bassbait
######*/
/**
 *  AI for Riggle Bassbait.
 *  This is the AI for Riggle Bassbait, see http://www.wowhead.com/?npc=15077
 *  @see ScriptedAI
 *  @author burned, gotisch
 */
struct MANGOS_DLL_DECL npc_riggle_bassbaitAI : public ScriptedAI
{
    /**
     *  Constructor of the Creature.
     *  This is called when the creature is spawned.
     *  @param c The Creature that this AI is for
     */
    npc_riggle_bassbaitAI(Creature *c) : ScriptedAI(c)
    {
        // This will keep the NPC active even if there are no players around!
        c->SetActiveObjectState(true);
        bEventAnnounced = bEventIsOver = bEventWinnerFound = false;
        Reset();
    }
    /**
     *  Flag to check if event was announced. True if event was announced.
     */
    bool bEventAnnounced;
    /**
     *  Flag to check if event is over. True if event is over.
     */
    bool bEventIsOver;
    /**
     *  Flag to check if someone won the event. True if someone has won.
     */
    bool bEventWinnerFound;

    void Reset() { }

    void Aggro(Unit *who) {}

    void UpdateAI(const uint32 diff)
    {
        // Announce the event max 1 minute after being spawned. But only if Fishing extravaganza is running.
        if (!bEventAnnounced && time(NULL) % 60 == 0 && IsHolidayActive(HOLIDAY_FISHING_EXTRAVAGANZA))
        {
            debug_log("SD2: npc_riggle_bassbait announce HOLIDAY_FISHING_EXTRAVAGANZA contest");
            DoScriptText(SAY_START, m_creature);
            m_creature->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER); //Quest&Gossip are now active
            bEventAnnounced = true;
        }
        // The Event was started (announced) & It was not yet ended & One minute passed & the Fish are gone
        if ( bEventAnnounced && !bEventIsOver && time(NULL) % 60 == 0 && !IsHolidayActive(HOLIDAY_FISHING_EXTRAVAGANZA))
        {
            debug_log("SD2: npc_riggle_bassbait end HOLIDAY_FISHING_EXTRAVAGANZA contest");
            DoScriptText(SAY_END, m_creature);
            bEventIsOver = true;
        }
        if (!m_creature->SelectHostileTarget() || !m_creature->getVictim())
            return;

        DoMeleeAttackIfReady();
    }
};
/**
 * GossipHello for NPC Riggle Bassbait. 
 * This is called each time a Player tries to talk with the NPC.
 */
bool GossipHello_npc_riggle_bassbait(Player* pPlayer, Creature* pCreature)
{
    if (pCreature->isQuestGiver()) // If the quest is still running.
    {
        pPlayer->PrepareQuestMenu(pCreature->GetGUID());
        pPlayer->SEND_GOSSIP_MENU(7614, pCreature->GetGUID());
        return true;
    }
    // The Quest is not there anymore 
    // There is a winner!
    pPlayer->SEND_GOSSIP_MENU(7714, pCreature->GetGUID());
    return true;
}

bool ChooseReward_npc_riggle_bassbait(Player* pPlayer, Creature* pCreature, const Quest* pQuest)
{
    // TODO: check if this can only be called if NPC has QUESTGIVER flag.
    if (pQuest->GetQuestId() == QUEST_MASTER_ANGLER && ((npc_riggle_bassbaitAI*)(pCreature->AI()))->bEventWinnerFound == false)
    {
        DoScriptText(SAY_WINNER, pCreature,pPlayer);
        pCreature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
        ((npc_riggle_bassbaitAI*)(pCreature->AI()))->bEventWinnerFound = true;
        Creature* creature2 = GetClosestCreatureWithEntry(pCreature,15087,60.0f);
        if (creature2)
        {
            creature2->SetFlag(UNIT_NPC_FLAGS,UNIT_NPC_FLAG_QUESTGIVER);
        } else {
            debug_log("Could not change flag of Jang");
        }
        return true;
    }
    return true;
}

CreatureAI* GetAI_npc_riggle_bassbait(Creature* pCreature)
{
    return new npc_riggle_bassbaitAI(pCreature);
}

void AddSC_stranglethorn_vale()
{
    Script *newscript;

    newscript = new Script;
    newscript->Name = "mob_yenniku";
    newscript->GetAI = &GetAI_mob_yenniku;
    newscript->RegisterSelf();

    newscript = new Script;
    newscript->Name = "npc_landro_longshot";
    newscript->pGossipHello = &GossipHello_npc_landro_longshot;
    newscript->pGossipSelect = &GossipSelect_npc_landro_longshot;
    newscript->pGossipSelectWithCode = &GossipSelectWithCode_npc_landro_longshot;
    newscript->RegisterSelf();
    
    newscript = new Script;
    newscript->Name = "npc_riggle_bassbait";
    newscript->GetAI = &GetAI_npc_riggle_bassbait;
    newscript->pGossipHello = &GossipHello_npc_riggle_bassbait;
    newscript->pQuestRewardedNPC = &ChooseReward_npc_riggle_bassbait;
    newscript->RegisterSelf();
}
