/*
 * Copyright (C) 2008-2011 TrinityCore <http://www.trinitycore.org/>
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "ScriptPCH.h"
#include "Spell.h"
#include "Vehicle.h"
#include "ScriptedEscortAI.h"

/*#####################
# go_not_a_bug (193939)
######################*/

class go_not_a_bug : public GameObjectScript
{
public:
    go_not_a_bug() : GameObjectScript("go_not_a_bug") { }

    enum SummoningStoneData
    {
        QUEST_NOT_A_BUG_A = 13342,
        QUEST_NOT_A_BUG_H = 13358,
        QUEST_NOT_A_BUG_A_D = 13344,
        QUEST_NOT_A_BUG_H_D = 13365,
        NPC_DARK_MESSENGER = 32316,
        NPC_NOT_A_BUG_KC = 32314
    };

    bool OnGossipHello(Player* player, GameObject* go)
    {
        if (player->GetQuestStatus(QUEST_NOT_A_BUG_A) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_NOT_A_BUG_H) == QUEST_STATUS_INCOMPLETE ||
            player->GetQuestStatus(QUEST_NOT_A_BUG_A_D) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_NOT_A_BUG_H_D) == QUEST_STATUS_INCOMPLETE)
        {
            go->SummonCreature(NPC_DARK_MESSENGER,8028.225098f,2077.155518f,499.729156f,3.947409f,TEMPSUMMON_TIMED_OR_DEAD_DESPAWN,5000);
            player->KilledMonsterCredit(NPC_NOT_A_BUG_KC,0);
        }
        return true;
    }
};

/*#############################
# item_alumeths_remains (44480)
###############################*/

class item_alumeths_remains : public ItemScript
{
    public:

        item_alumeths_remains() : ItemScript("item_alumeths_remains") { }

        enum AlumethsRemainsData
        {
            QUEST_NO_REST_FOR_THE_WICKED_A = 13346,
            QUEST_NO_REST_FOR_THE_WICKED_H = 13367,
            QUEST_NO_REST_FOR_THE_WICKED_A_D = 13350,
            QUEST_NO_REST_FOR_THE_WICKED_H_D = 13368,
            NPC_ALUMETH = 32300
        };

        bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& /*targets*/)
        {
            if (player->GetQuestStatus(QUEST_NO_REST_FOR_THE_WICKED_A) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_NO_REST_FOR_THE_WICKED_H) == QUEST_STATUS_INCOMPLETE ||
                player->GetQuestStatus(QUEST_NO_REST_FOR_THE_WICKED_A_D) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_NO_REST_FOR_THE_WICKED_H_D) == QUEST_STATUS_INCOMPLETE)
                player->SummonCreature(NPC_ALUMETH,8229.606445f,2179.215820f,499.736694f,3.161045f,TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT,30000);
                return true;
        }
};

/*##################################
# npc_q13355_q13320_trigger (97001)
###################################*/

class npc_q13355_q13320_trigger : public CreatureScript
{
public:
    npc_q13355_q13320_trigger() : CreatureScript("npc_q13355_q13320_trigger") { }

    enum q13355q13320TriggerData
    {
        GO_TEMP_CULTISTS_CAULDRON = 300240,
        BLUE_CAULDRON_GUID = 99719,
        BLACK_CAULDRON_GUID = 99718,
        GREEN_CAULDRON_GUID = 99720,
        BLUE_SAMPLE_KC = 32242,
        BLACK_SAMPLE_KC = 32245,
        GREEN_SAMPLE_KC = 32244,
        SPELL_COLLECT_SAMPLE = 60256
    };

    struct npc_q13355_q13320_triggerAI : public ScriptedAI
    {
        npc_q13355_q13320_triggerAI(Creature* creature) : ScriptedAI(creature) { }

        void Reset() {}

        void SpellHit(Unit* caster, const SpellInfo* spell)
        {
            if (caster->GetTypeId() != TYPEID_PLAYER)
                return;

            if (caster->GetTypeId() == TYPEID_PLAYER && spell->Id == SPELL_COLLECT_SAMPLE)
            {
                if (GameObject* go = me->FindNearestGameObject(GO_TEMP_CULTISTS_CAULDRON,10.0f))
                {
                    switch(go->GetGUIDLow())
                    {
                        case BLUE_CAULDRON_GUID: caster->ToPlayer()->KilledMonsterCredit(BLUE_SAMPLE_KC,0); break;
                        case BLACK_CAULDRON_GUID: caster->ToPlayer()->KilledMonsterCredit(BLACK_SAMPLE_KC,0); break;
                        case GREEN_CAULDRON_GUID: caster->ToPlayer()->KilledMonsterCredit(GREEN_SAMPLE_KC,0); break;
                    }
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_q13355_q13320_triggerAI(creature);
    }
};

/*#################################
# npc_brokendown_shredders (27354)
##################################*/

class npc_brokendown_shredders : public CreatureScript
{
public:
    npc_brokendown_shredders() : CreatureScript("npc_brokendown_shredders") { }

    struct npc_brokendown_shreddersAI : public ScriptedAI
    {
        npc_brokendown_shreddersAI(Creature* creature) : ScriptedAI(creature) { }

        void UpdateAI(const uint32 /*diff*/)
        {
            Unit* player = me->GetVehicleKit()->GetPassenger(0);
            if (!player && ((me->GetPositionX() != me->GetHomePosition().GetPositionX()) && (me->GetPositionY() != me->GetHomePosition().GetPositionY()) && (me->GetPositionZ() != me->GetHomePosition().GetPositionZ())))
            {
                me->DisappearAndDie();
                me->Respawn();
                return;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_brokendown_shreddersAI(creature);
    }
};

/*#################################
# npc_shredders_taker (27371, 2742)
##################################*/

class npc_shredders_taker : public CreatureScript
{
public:
    npc_shredders_taker() : CreatureScript("npc_shredders_taker") { }

    struct npc_shredders_takerAI : public ScriptedAI
    {
        npc_shredders_takerAI(Creature* creature) : ScriptedAI(creature) { }

        void MoveInLineOfSight(Unit* who)
        {
            if(!who)
                return;

            if (who->GetTypeId() != TYPEID_UNIT || who->GetEntry() != 27354 || !who->isCharmedOwnedByPlayerOrPlayer())
                return;

            Player* player = who->GetCharmerOrOwner()->ToPlayer();

            if (player && me->IsWithinDistInMap(who, 15.0f))
            {
                player->KilledMonsterCredit(27396, 0);
                player->ExitVehicle();
                who->ToCreature()->DisappearAndDie();
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_shredders_takerAI(creature);
    }
};

/*##########################
# item_writhing_mass (44304)
###########################*/

class item_writhing_mass : public ItemScript
{
    public:

        item_writhing_mass() : ItemScript("item_writhing_mass") { }

        enum WrithingMassData
        {
            QUEST_RETEST_NOW_A = 13321,
            QUEST_RETEST_NOW_H = 13356,
            QUEST_RETEST_NOW_A_D = 13322,
            QUEST_RETEST_NOW_H_D = 13357,
            NPC_RETEST_NOW_KC = 32266
        };

        bool OnUse(Player* player, Item* /*item*/, SpellCastTargets const& /*targets*/)
        {
            if (player->GetQuestStatus(QUEST_RETEST_NOW_A) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_RETEST_NOW_H) == QUEST_STATUS_INCOMPLETE ||
                player->GetQuestStatus(QUEST_RETEST_NOW_A_D) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_RETEST_NOW_H_D) == QUEST_STATUS_INCOMPLETE)
                player->KilledMonsterCredit(NPC_RETEST_NOW_KC,0);
                return true;
        }
};

/*##########################
# npc_rocket_warhead (27593)
###########################*/

class npc_rocket_warhead : public CreatureScript
{
public:
    npc_rocket_warhead() : CreatureScript("npc_rocket_warhead") { }

    struct npc_rocket_warheadAI : public ScriptedAI
    {
        npc_rocket_warheadAI(Creature* creature) : ScriptedAI(creature) { }

        bool occupied;

        void Reset()
        {
            occupied = false;
            me->SetHover(true);
        }

        void UpdateAI(const uint32 /*diff*/)
        {
            if(!occupied)
                return;

            Unit* player = me->GetVehicleKit()->GetPassenger(0);

            if(!player)
            {
                me->DisappearAndDie();
                return;
            }

            if(player && !me->HasAura(49181, 0)) // Time-Out and no boat reached
            {
                player->ExitVehicle();
                DoCast(71945);            // Visual
                me->DespawnOrUnsummon(1500);
                return;
            }

            Unit* pBombTarget;
            switch(player->ToPlayer()->GetTeam())
            {
                case ALLIANCE: pBombTarget = me->FindNearestCreature(27702, 8.0f); break;
                case HORDE: pBombTarget = me->FindNearestCreature(27688, 8.0f); break;
            }

            if(player && pBombTarget)
            {
                player->ToPlayer()->KilledMonsterCredit(pBombTarget->GetEntry(), 0);
                player->ExitVehicle();
                DoCast(71945);            // Visual
                me->DespawnOrUnsummon(1500);
            }
        }

        void PassengerBoarded(Unit* /*passenger*/, int8 /*seatId*/, bool /*apply*/)
        {
            me->AddAura(49181, me);
            occupied = true;
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_rocket_warheadAI(creature);
    }
};

/*###########################
# npc_father_kamaros (31279)
###########################*/

enum NotDeadYet
{
    QUEST_NOT_DEAD_YET_A = 13221,
    QUEST_NOT_DEAD_YET_H = 13229,
    NPC_NOT_DEAD_YET_TRIG = 97002,
    SPELL_PW_FORTITUDE = 23947,
    SPELL_PW_SHIELD = 20697
};

const Position KamarosWp[54] =
{
    {6718.48f, 3439.88f, 682.24f, 0.00f},
    {6723.54f, 3433.79f, 682.20f, 0.00f},
    {6734.04f, 3436.02f, 682.03f, 0.00f},
    {6737.66f, 3439.00f, 680.97f, 0.00f},
    {6742.07f, 3442.76f, 679.93f, 0.00f},
    {6745.65f, 3446.09f, 678.85f, 0.00f},
    {6750.47f, 3450.57f, 677.51f, 0.00f},
    {6755.94f, 3455.28f, 675.78f, 0.00f},
    {6759.58f, 3458.61f, 674.71f, 0.00f},
    {6765.04f, 3463.30f, 673.37f, 0.00f},
    {6769.22f, 3467.40f, 673.00f, 0.00f},
    {6771.95f, 3471.55f, 673.01f, 0.00f},
    {6777.21f, 3477.04f, 672.99f, 0.00f},
    {6782.91f, 3482.89f, 674.34f, 0.00f},
    {6779.54f, 3479.36f, 673.32f, 0.00f},
    {6773.88f, 3473.81f, 673.03f, 0.00f},
    {6786.32f, 3484.85f, 675.62f, 0.00f},
    {6792.51f, 3484.66f, 677.20f, 0.00f},
    {6797.39f, 3484.49f, 678.76f, 0.00f},
    {6800.64f, 3484.44f, 680.03f, 0.00f},
    {6805.98f, 3484.26f, 682.17f, 0.00f},
    {6810.80f, 3484.14f, 683.97f, 0.00f},
    {6816.15f, 3483.58f, 686.18f, 0.00f},
    {6822.91f, 3483.01f, 688.17f, 0.00f},
    {6829.09f, 3481.91f, 689.49f, 0.00f},
    {6836.49f, 3480.96f, 690.69f, 0.00f},
    {6844.57f, 3480.25f, 691.89f, 0.00f},
    {6853.38f, 3479.60f, 692.98f, 0.00f},
    {6859.95f, 3479.59f, 693.60f, 0.00f},
    {6867.60f, 3479.74f, 694.21f, 0.00f},
    {6875.25f, 3479.95f, 694.86f, 0.00f},
    {6882.69f, 3480.14f, 695.82f, 0.00f},
    {6889.85f, 3479.97f, 697.36f, 0.00f},
    {6896.19f, 3479.88f, 698.54f, 0.00f},
    {6902.49f, 3479.66f, 699.74f, 0.00f},
    {6908.08f, 3479.89f, 700.81f, 0.00f},
    {6913.89f, 3480.32f, 701.86f, 0.00f},
    {6920.57f, 3482.18f, 703.10f, 0.00f},
    {6925.61f, 3481.11f, 704.38f, 0.00f},
    {6930.92f, 3480.19f, 705.91f, 0.00f},
    {6936.04f, 3479.73f, 707.06f, 0.00f},
    {6942.06f, 3479.21f, 708.06f, 0.00f},
    {6947.22f, 3477.91f, 709.01f, 0.00f},
    {6953.44f, 3477.53f, 709.63f, 0.00f},
    {6959.23f, 3477.21f, 710.30f, 0.00f},
    {6967.35f, 3474.84f, 711.05f, 0.00f},
    {6980.54f, 3470.97f, 710.86f, 0.00f},
    {6985.85f, 3469.15f, 708.75f, 0.00f},
    {6991.36f, 3467.22f, 705.94f, 0.00f},
    {6997.30f, 3465.15f, 702.91f, 0.00f},
    {7004.14f, 3462.46f, 699.36f, 0.00f},
    {7009.42f, 3460.61f, 696.67f, 0.00f},
    {7018.43f, 3455.71f, 696.67f, 0.00f},
    {7029.21f, 3447.94f, 696.17f, 0.00f}
};

class npc_father_kamaros : public CreatureScript
{
public:
    npc_father_kamaros() : CreatureScript("npc_father_kamaros") { }

    bool OnQuestAccept(Player* player, Creature* creature, Quest const* quest)
    {
        if (quest->GetQuestId() == QUEST_NOT_DEAD_YET_A || quest->GetQuestId() == QUEST_NOT_DEAD_YET_H)
        {
            creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            creature->AI()->SetGUID(player->GetGUID());
            creature->CastSpell(player,SPELL_PW_FORTITUDE,true);
            player->CastSpell(player,SPELL_PW_SHIELD,true);
        }
        return true;
    }

    struct npc_father_kamarosAI : public ScriptedAI
    {
        npc_father_kamarosAI(Creature* creature) : ScriptedAI(creature) { }

        uint8 count;
        bool wp_reached;
        bool movementStarted;
        bool finished;
        uint64 uiPlayer;
        Player* player;

        void SetGUID(const uint64 &uiGuid, int32 /*iId*/)
        {
            movementStarted = true;
            me->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_OOC_NOT_ATTACKABLE);
            me->SetReactState(REACT_PASSIVE);
            me->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
            me->GetMotionMaster()->MovePoint(0, KamarosWp[0]);
            uiPlayer = uiGuid;
        }

        void Reset()
        {
            count = 0;
            wp_reached = false;
            movementStarted = false;
            finished = false;
            uiPlayer = 0;
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE || id != count)
                return;

            if (movementStarted && id < 54)
            {
                ++count;
                wp_reached = true;
            }
        }

        void UpdateAI(const uint32 /*diff*/)
        {
            if (movementStarted)
            {
                player = me->GetPlayer(*me, uiPlayer);

                if (!player || !player->isAlive() || me->GetDistance(player) > 30.0f)
                    me->DisappearAndDie();

                if (player->isInCombat() && player->GetUInt64Value(UNIT_FIELD_TARGET))
                    me->Attack(me->GetUnit(*me, player->GetUInt64Value(UNIT_FIELD_TARGET)), true);
            }

            if (wp_reached && !finished)
            {
                wp_reached = false;
                me->SetUnitMovementFlags(MOVEMENTFLAG_WALKING);
                me->GetMotionMaster()->MovePoint(count, KamarosWp[count]);
            }
        }

        void MoveInLineOfSight(Unit* who)
        {
            if (who->GetEntry() == NPC_NOT_DEAD_YET_TRIG && me->IsWithinDistInMap(who,10.0f,true))
            {
                if (Player* player = me->GetPlayer(*me, uiPlayer))
                {
                    switch (player->GetTeam())
                    {
                        case ALLIANCE: player->GroupEventHappens(QUEST_NOT_DEAD_YET_A, me); break;
                        case HORDE: player->GroupEventHappens(QUEST_NOT_DEAD_YET_H, me); break;
                    }
                    movementStarted = false;
                    finished = true;
                    me->DespawnOrUnsummon(10000);
                }
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_father_kamarosAI(creature);
    }
};

/*#####
# npc_spring_rabbit
#####*/

enum rabbitSpells
{
    SPELL_SPRING_FLING = 61875,
    SPELL_SPRING_RABBIT_JUMP = 61724,
    SPELL_SPRING_RABBIT_WANDER = 61726,
    SPELL_SUMMON_BABY_BUNNY = 61727,
    SPELL_SPRING_RABBIT_IN_LOVE = 61728,
    NPC_SPRING_RABBIT = 32791
};

class npc_spring_rabbit : public CreatureScript
{
public:
    npc_spring_rabbit() : CreatureScript("npc_spring_rabbit") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_spring_rabbitAI(creature);
    }

    struct npc_spring_rabbitAI : public ScriptedAI
    {
        npc_spring_rabbitAI(Creature* c) : ScriptedAI(c) { }

        bool inLove;
        uint32 jumpTimer;
        uint32 bunnyTimer;
        uint32 searchTimer;
        uint64 rabbitGUID;

        void Reset()
        {
            inLove = false;
            rabbitGUID = 0;
            jumpTimer = urand(5000, 10000);
            bunnyTimer = urand(10000, 20000);
            searchTimer = urand(5000, 10000);
            if (Unit* owner = me->GetOwner())
                me->GetMotionMaster()->MoveFollow(owner, PET_FOLLOW_DIST, PET_FOLLOW_ANGLE);
        }

        void EnterCombat(Unit * /*who*/) { }

        void DoAction(const int32 /*param*/)
        {
            inLove = true;
            if (Unit* owner = me->GetOwner())
                owner->CastSpell(owner, SPELL_SPRING_FLING, true);
        }

        void UpdateAI(const uint32 diff)
        {
            if (inLove)
            {
                if (jumpTimer <= diff)
                {
                    if (Unit* rabbit = Unit::GetUnit(*me, rabbitGUID))
                        DoCast(rabbit, SPELL_SPRING_RABBIT_JUMP);
                    jumpTimer = urand(5000, 10000);
                }else jumpTimer -= diff;

                if (bunnyTimer <= diff)
                {
                    DoCast(SPELL_SUMMON_BABY_BUNNY);
                    bunnyTimer = urand(20000, 40000);
                }else bunnyTimer -= diff;
            }
            else
            {
                if (searchTimer <= diff)
                {
                    if (Creature* rabbit = me->FindNearestCreature(NPC_SPRING_RABBIT, 10.0f))
                    {
                        if (rabbit == me || rabbit->HasAura(SPELL_SPRING_RABBIT_IN_LOVE))
                            return;

                        me->AddAura(SPELL_SPRING_RABBIT_IN_LOVE, me);
                        DoAction(1);
                        rabbit->AddAura(SPELL_SPRING_RABBIT_IN_LOVE, rabbit);
                        rabbit->AI()->DoAction(1);
                        rabbit->CastSpell(rabbit, SPELL_SPRING_RABBIT_JUMP, true);
                        rabbitGUID = rabbit->GetGUID();
                    }
                    searchTimer = urand(5000, 10000);
                }else searchTimer -= diff;
            }
        }
    };
};

enum ghoulSpells
{
    SPELL_SHINING_LIGTH = 43202,
    SPELL_SHINING_LIGTH_HIT_VISUAL = 46400,
};

class npc_decomposing_ghoul : public CreatureScript
{
public:
    npc_decomposing_ghoul() : CreatureScript("npc_decomposing_ghoul") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_decomposing_ghoulAI(creature);
    }

    struct npc_decomposing_ghoulAI : public ScriptedAI
    {
        npc_decomposing_ghoulAI(Creature* c) : ScriptedAI(c) { }


        void EnterCombat(Unit * who)
        {
            if (who->HasAura(SPELL_SHINING_LIGTH))
            {
                DoCast(SPELL_SHINING_LIGTH_HIT_VISUAL);
                EnterEvadeMode();
            }
        }
    };
};

enum irulonData
{
    QUEST_GUIDED_BY_HONOR = 11289,
    NPC_TIRION            = 24232,
    NPC_CLERIC            = 24233,
    ITEM_ASHBRINGER       = 13262,

    SAY_TIRION_1          = -1002000,
    EMOTE_TIRION_1        = -1002001,
    SAY_IRULON_1          = -1002002,
    SAY_TIRION_2          = -1002003,
    SAY_TIRION_3          = -1002004,
    SAY_IRULON_2          = -1002005,
    EMOTE_TIRION_2        = -1002006,
    SAY_TIRION_4          = -1002007,
    SAY_TIRION_5          = -1002008,
    EMOTE_TIRION_3        = -1002009,
    YELL_TIRION           = -1002010,

    ACTION_START,
};

enum iluronEvents
{
    EVENT_NONE,
    EVENT_00,
    EVENT_01,
    EVENT_02,
    EVENT_03,
    EVENT_04,
    EVENT_05,
    EVENT_06,
    EVENT_07,
    EVENT_08,
    EVENT_09,
    EVENT_10,
};

class npc_irulon_trueblade : public CreatureScript
{
public:
    npc_irulon_trueblade() : CreatureScript("npc_irulon_trueblade") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_irulon_truebladeAI(creature);
    }

    bool OnGossipHello(Player* player, Creature* me)
    {
        if (me->isQuestGiver())
            player->PrepareQuestMenu(me->GetGUID());

        player->GroupEventHappens(QUEST_GUIDED_BY_HONOR, me);

        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE, me->GetGUID());
        return true;
    }
    bool OnQuestReward(Player* /*player*/, Creature* me, Quest const* quest, uint32 /*item*/)
    {
        if (quest->GetQuestId() == QUEST_GUIDED_BY_HONOR)
            me->AI()->DoAction(ACTION_START);
        return true;
    }

    struct npc_irulon_truebladeAI : public ScriptedAI
    {
        npc_irulon_truebladeAI(Creature* c) : ScriptedAI(c) { }

        EventMap events;
        uint64 uiTirion;

        void Reset()
        {
            uiTirion = 0;
            events.Reset();
        }

        void DoAction(const int32 actionId)
        {
            switch(actionId)
            {
                case ACTION_START:
                    uiTirion = 0;
                    events.ScheduleEvent(EVENT_00, 1500);
                    break;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);
            switch(events.ExecuteEvent())
            {
                case EVENT_00:
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);

                    if(Creature* pTirion = me->FindNearestCreature(NPC_CLERIC, 7.0f))
                    {
                        uiTirion = pTirion->GetGUID();
                        DoScriptText(SAY_TIRION_1, pTirion);
                        pTirion->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
                        pTirion->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
                        pTirion->GetMotionMaster()->MovePoint(0, me->GetPositionX() + 3.0f, me->GetPositionY() + 3.0f, me->GetPositionZ() + 0.5f);
                    }
                    events.ScheduleEvent(EVENT_01, 2000);
                    break;
                case EVENT_01:
                    if(Creature* pTirion = me->GetCreature(*me, uiTirion))
                    {
                        DoScriptText(EMOTE_TIRION_1, pTirion);
                        pTirion->UpdateEntry(NPC_TIRION);
                        pTirion->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 0);
                    }
                    events.ScheduleEvent(EVENT_02, 1000);
                    break;
                case EVENT_02:
                    DoScriptText(SAY_IRULON_1, me);
                    events.ScheduleEvent(EVENT_03, 2000);
                    break;
                case EVENT_03:
                    if(Creature* pTirion = me->GetCreature(*me, uiTirion))
                        DoScriptText(SAY_TIRION_2, pTirion);
                    events.ScheduleEvent(EVENT_04, 3000);
                    break;
                case EVENT_04:
                    if(Creature* pTirion = me->GetCreature(*me, uiTirion))
                        DoScriptText(SAY_TIRION_3, pTirion);
                    events.ScheduleEvent(EVENT_05,1000);
                    break;
                case EVENT_05:
                    DoScriptText(SAY_IRULON_2, me);
                    events.ScheduleEvent(EVENT_06, 2500);
                    break;
                case EVENT_06:
                    if(Creature* pTirion = me->GetCreature(*me, uiTirion))
                        DoScriptText(EMOTE_TIRION_2, pTirion);
                    events.ScheduleEvent(EVENT_07,1000);
                    break;
                case EVENT_07:
                    if(Creature* pTirion = me->GetCreature(*me, uiTirion))
                        DoScriptText(SAY_TIRION_4, pTirion);
                    events.ScheduleEvent(EVENT_08,1500);
                    break;
                case EVENT_08:
                    if(Creature* pTirion = me->GetCreature(*me, uiTirion))
                        DoScriptText(SAY_TIRION_5, pTirion);
                    events.ScheduleEvent(EVENT_09,1500);
                    break;
                case EVENT_09:
                    if(Creature* pTirion = me->GetCreature(*me, uiTirion))
                    {
                        DoScriptText(EMOTE_TIRION_3, pTirion);
                        pTirion->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, ITEM_ASHBRINGER);
                    }
                    events.ScheduleEvent(EVENT_10,2000);
                    break;
                case EVENT_10:
                    if(Creature* pTirion = me->GetCreature(*me, uiTirion))
                    {
                        DoScriptText(YELL_TIRION, pTirion);
                        pTirion->SetUInt32Value(UNIT_VIRTUAL_ITEM_SLOT_ID, 0);
                        pTirion->DespawnOrUnsummon(5000);
                    }
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER);
                    me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    break;
            }
        }
    };
};

enum q11429Data
{
    QUEST_DROP_IT_ROCK_IT = 11429,
    NPC_WINTERSKORN_DEFENDER = 24015,
};

class npc_banner_q11429 : public CreatureScript
{
public:
    npc_banner_q11429() : CreatureScript("npc_banner_q11429") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_banner_q11429AI(creature);
    }

    struct npc_banner_q11429AI : public ScriptedAI
    {
        npc_banner_q11429AI(Creature* c) : ScriptedAI(c) { }

        uint32 uiWaveTimer;
        uint8 killCounter;

        void Reset()
        {
            killCounter = 0;
            uiWaveTimer = 2000;
            me->SetReactState(REACT_PASSIVE);
            me->GetMotionMaster()->MoveIdle();
        }

        void JustDied(Unit* /*killer*/)
        {
            if (Player* player = me->GetOwner()->ToPlayer())
                player->FailQuest(QUEST_DROP_IT_ROCK_IT);
        }

        void UpdateAI(const uint32 diff)
        {
            if(uiWaveTimer < diff)
            {
                if(Creature* pVrykul = me->SummonCreature(NPC_WINTERSKORN_DEFENDER, (1476.85f + rand()%20), (-5327.56f + rand()%20), (194.8f  + rand()%2), 0.0f, TEMPSUMMON_CORPSE_DESPAWN))
                {
                    pVrykul->AI()->AttackStart(me);
                    pVrykul->GetMotionMaster()->Clear();
                    pVrykul->GetMotionMaster()->MoveChase(me);
                }
                uiWaveTimer = urand(8000, 16000);
            }
            else
                uiWaveTimer -= diff;
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            if (summon->GetEntry() == NPC_WINTERSKORN_DEFENDER)
                killCounter++;

            if(killCounter >= 3)
            {
                if (Player* player = me->GetOwner()->ToPlayer())
                    player->GroupEventHappens(QUEST_DROP_IT_ROCK_IT, me);

                me->DespawnOrUnsummon(2000);
            }
        }
    };
};

enum ancientSkullPileData
{
    ITEM_TIMELOST_OFFERING = 32720,
    NPC_TEROKK             = 21838,
};

class go_ancient_skull_pile : public GameObjectScript
{
    public:
        go_ancient_skull_pile() : GameObjectScript("go_ancient_skull_pile") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            if (player->HasItemCount(ITEM_TIMELOST_OFFERING, 1))
                player->ADD_GOSSIP_ITEM( 9, "Invocar a Terokk", GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

            player->SEND_GOSSIP_MENU(player->GetGossipTextId(go), go->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, GameObject* go, uint32 /*uiSender*/, uint32 uiAction)
        {
            if(uiAction = GOSSIP_ACTION_INFO_DEF+1)
            {
                if(Creature* pTerokk = player->SummonCreature(NPC_TEROKK, go->GetPositionX() + 4.0f, go->GetPositionY() + 2.0f, go->GetPositionZ(), go->GetOrientation(), TEMPSUMMON_CORPSE_TIMED_DESPAWN, 30000))
                {
                    pTerokk->AI()->AttackStart(player);
                    pTerokk->GetMotionMaster()->Clear();
                    pTerokk->GetMotionMaster()->MoveChase(player);
                    player->DestroyItemCount(ITEM_TIMELOST_OFFERING, 1, true);
                }
            }
            player->CLOSE_GOSSIP_MENU();
            return true;
        }
};

enum ymironData
{
    QUEST_ECHO_YMIRON     = 11343,
    NPC_ANCIENT_VRYKUL_M  = 24314,
    NPC_ANCIENT_VRYKUL_F  = 24315,
    NPC_LICHKING_HF       = 24248,
    NPC_VALKYR            = 24327,

    SAY_VRYKUL_1          = -1002011, // Male
    EMOTE_VRYKUL_1        = -1002012, // Male
    EMOTE_VRYKUL_2        = -1002013, // Female
    SAY_VRYKUL_2          = -1002014, // Male
    SAY_VRYKUL_3          = -1002015, // Female
    SAY_VRYKUL_4          = -1002016, // Male
    SAY_VRYKUL_5          = -1002017, // Female

    EMOTE_LK_1            = -1002018,
    EMOTE_LK_2            = -1002019,
    SAY_LK_1              = -1002020,
    SAY_LK_2              = -1002021, //Val'kyr
    SAY_LK_3              = -1002022,
    SAY_LK_4              = -1002023,
    SAY_LK_5              = -1002024,
    SAY_LK_6              = -1002025,

    SPELL_ECHO_YMIRON     = 42786,
    SPELL_MAGNETIC_PULL   = 29661,
    SPELL_LK_GRASP        = 43489,
    SPELL_LK_WRATH        = 43488,
};

class npc_ancient_vrykul : public CreatureScript
{
public:
    npc_ancient_vrykul() : CreatureScript("npc_ancient_vrykul") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ancient_vrykulAI(creature);
    }
    struct npc_ancient_vrykulAI : public ScriptedAI
    {
        npc_ancient_vrykulAI(Creature* c) : ScriptedAI(c) { }

        EventMap events;
        Creature* pMale;
        uint64 uiPlayer;
        bool active;

        void Reset()
        {
            uiPlayer = 0;
            active = false;
            events.Reset();

            // Set both passive
            me->SetReactState(REACT_PASSIVE);

            if(Creature* male = me->FindNearestCreature(NPC_ANCIENT_VRYKUL_M, 5.0f))
            {
                male->SetReactState(REACT_PASSIVE);
                pMale = male;
            }
        }

        void SetGUID(const uint64 &uiGuid, int32 /*iId*/)
        {
            if (active)
                return;

            uiPlayer = uiGuid;
            events.ScheduleEvent(1, 2000);
            active = true;
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);
            switch(events.ExecuteEvent())
            {
                case 1:
                    DoScriptText(SAY_VRYKUL_1, pMale);
                    events.ScheduleEvent(2, 2000);
                    break;
                case 2:
                    DoScriptText(EMOTE_VRYKUL_1, pMale);
                    events.ScheduleEvent(3, 1000);
                    break;
                case 3:
                    DoScriptText(EMOTE_VRYKUL_2, me);
                    events.ScheduleEvent(4, 2000);
                    break;
                case 4:
                    DoScriptText(SAY_VRYKUL_2, pMale);
                    events.ScheduleEvent(5, 3000);
                    break;
                case 5:
                    DoScriptText(SAY_VRYKUL_3, me);
                    events.ScheduleEvent(6,1000);
                    break;
                case 6:
                    DoScriptText(SAY_VRYKUL_4, pMale);
                    events.ScheduleEvent(7, 2500);
                    break;
                case 7:
                    DoScriptText(SAY_VRYKUL_5, me);
                    events.ScheduleEvent(8, 1000);
                    break;
                case 8:
                    if(Player* player = me->GetPlayer(*me, uiPlayer))
                        player->GroupEventHappens(QUEST_ECHO_YMIRON, me);
                    // Set long timer for reset, preventes restarting the event
                    events.ScheduleEvent(9,20000);
                    break;
                case 9:
                    Reset();
                    break;
            }
        }
    };
};

class at_ymiron_house : public AreaTriggerScript
{
    public:
        at_ymiron_house() : AreaTriggerScript("at_ymiron_house") {}

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
        {
            if(player->HasAura(SPELL_ECHO_YMIRON))
            {
                if(Creature * pVrykul = player->FindNearestCreature(NPC_ANCIENT_VRYKUL_F, 10.0f))
                    pVrykul->AI()->SetGUID(player->GetGUID());
            }
            return true;
        }
};

class npc_lich_king_hfjord : public CreatureScript
{
public:
    npc_lich_king_hfjord() : CreatureScript("npc_lich_king_hfjord") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_lich_king_hfjordAI(creature);
    }
    struct npc_lich_king_hfjordAI : public ScriptedAI
    {
        npc_lich_king_hfjordAI(Creature* c) : ScriptedAI(c) { }

        EventMap events;
        Player* player;
        bool active;

        void Reset()
        {
            player = NULL;
            active = false;
            events.Reset();

            me->SetReactState(REACT_PASSIVE);
            me->SetOrientation(3.29914f);
        }


        void UpdateAI(const uint32 diff)
        {
            if (!active)
            {
                if(Unit *pSelect = me->SelectNearestTarget(20.0f))
                {
                    if (pSelect->GetTypeId() == TYPEID_PLAYER)
                    {
                        player = pSelect->ToPlayer();
                        DoScriptText(EMOTE_LK_1, me);
                        me->CastSpell(player, SPELL_MAGNETIC_PULL, true);
                        me->CastSpell(player, SPELL_LK_GRASP, true);
                        active = true;
                        events.ScheduleEvent(1, 2000);
                     }
                }
            }

            events.Update(diff);
            switch(events.ExecuteEvent())
            {
                case 1:
                    DoScriptText(EMOTE_LK_2, me);
                    events.ScheduleEvent(2, 1000);
                    break;
                case 2:
                    DoScriptText(SAY_LK_1, me);
                    events.ScheduleEvent(3, 2500);
                    break;
                case 3:
                    if (Creature* pValkyr = me->FindNearestCreature(NPC_VALKYR, 5.0f))
                        DoScriptText(SAY_LK_2, pValkyr);
                    events.ScheduleEvent(4, 3000);
                    break;
                case 4:
                    DoScriptText(SAY_LK_3, me);
                    events.ScheduleEvent(5, 300);
                    break;
                case 5:
                    DoScriptText(SAY_LK_4, me);
                    events.ScheduleEvent(6,5500);
                    break;
                case 6:
                    DoScriptText(SAY_LK_5, me);
                    events.ScheduleEvent(7, 5500);
                    break;
                case 7:
                    DoScriptText(SAY_LK_6, me);
                    events.ScheduleEvent(8, 3000);
                    break;
                case 8:
                    if(player)
                        me->CastSpell(player, SPELL_LK_WRATH, true);
                    events.ScheduleEvent(9, 500);
                case 9:
                    if(player)
                        me->Kill(player);
                    // Set long timer for reset, preventes restarting the event
                    events.ScheduleEvent(10,20000);
                    break;
                case 10:
                    Reset();
                    break;
            }
        }
    };
};

enum ironRuneConstructData
{
    QUEST_IRON_RUNE_CONST_JUMP  = 11485,
    QUEST_IRON_RUNE_CONST_DATA  = 11489,
    QUEST_IRON_RUNE_CONST_BLUFF = 11491,

    NPC_IRON_RUNE_CONST_JUMP    = 24823,
    NPC_IRON_RUNE_CONST_DATA    = 24821,
    NPC_IRON_RUNE_CONST_BLUFF   = 24825,

    SPELL_BLUFF                 = 44562,
};

class npc_iron_rune_construct : public CreatureScript
{
public:
    npc_iron_rune_construct() : CreatureScript("npc_iron_rune_construct") { }

    struct npc_iron_rune_constructAI : public ScriptedAI
    {
        npc_iron_rune_constructAI(Creature* creature) : ScriptedAI(creature) { }

        bool ocuppied;
        uint8 seatID;

        void UpdateAI(const uint32 /*diff*/)
        {
            Unit* player = me->GetVehicleKit()->GetPassenger(seatID);

            if (!player)
                me->DisappearAndDie();
        }

        void PassengerBoarded(Unit* passenger, int8 seatId, bool /*apply*/)
        {
            seatID = seatId;
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_iron_rune_constructAI(creature);
    }
};

class npc_lebronski : public CreatureScript
{
public:
    npc_lebronski() : CreatureScript("npc_lebronski") { }

    struct npc_lebronskiAI : public ScriptedAI
    {
        npc_lebronskiAI(Creature* creature) : ScriptedAI(creature) { }

        void SpellHit(Unit* caster, SpellInfo const* spell)
        {
            if(spell->Id == SPELL_BLUFF)
                caster->GetVehicleKit()->GetPassenger(0)->ToPlayer()->GroupEventHappens(QUEST_IRON_RUNE_CONST_BLUFF, me);
        }

    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_lebronskiAI(creature);
    }
};

class go_iron_rune_construct_workbank : public GameObjectScript
{
    public:
        go_iron_rune_construct_workbank() : GameObjectScript("go_iron_rune_construct_workbank") { }

        bool OnGossipHello(Player* player, GameObject* go)
        {
            if (player->GetQuestStatus(QUEST_IRON_RUNE_CONST_JUMP) == QUEST_STATUS_INCOMPLETE)
            {
                if(Creature* pConstruct = player->SummonCreature(NPC_IRON_RUNE_CONST_JUMP, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation()))
                {
                    pConstruct->SetLevel(player->getLevel());
                    player->EnterVehicle(pConstruct);
                    player->GroupEventHappens(QUEST_IRON_RUNE_CONST_JUMP, pConstruct);
                }
                return true;
            }

            if (player->GetQuestStatus(QUEST_IRON_RUNE_CONST_DATA) == QUEST_STATUS_INCOMPLETE)
            {
                if(Creature* pConstruct = player->SummonCreature(NPC_IRON_RUNE_CONST_DATA, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation()))
                {
                    pConstruct->SetLevel(player->getLevel());
                    player->EnterVehicle(pConstruct);
                }
                return true;
            }

            if (player->GetQuestStatus(QUEST_IRON_RUNE_CONST_BLUFF) == QUEST_STATUS_INCOMPLETE)
            {
                if(Creature* pConstruct = player->SummonCreature(NPC_IRON_RUNE_CONST_BLUFF, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ(), player->GetOrientation()))
                {
                    pConstruct->SetLevel(player->getLevel());
                    player->EnterVehicle(pConstruct);
                }
                return true;
            }

            return true;
        }
};

/*############
# npc_terokk
#############*/

enum TerokkData
{
    SPELL_CLEAVE = 15284,
    SPELL_DIVINE_SHIELD = 40733,
    SPELL_FRENZY = 28747,
    SPELL_SHADOW_BOLT_VOLLEY = 40721,
    SPELL_WILL_OF_THE_ARAKKOA = 40722,
    SPELL_VISUAL_MARKER = 40656,
    NPC_MARKER_TRIGGER = 97016,
    SAY_SUMMONED = 0,
    SAY_CHOSEN = 1,
    SAY_IMMUNE = 2
};

class npc_terokk : public CreatureScript
{
public:
    npc_terokk() : CreatureScript("npc_terokk") {}

    struct npc_terokkAI : public ScriptedAI
    {
        npc_terokkAI(Creature* creature) : ScriptedAI(creature) {}

        bool inCombat;
        bool Chosen;
        bool isImmune;
        uint32 CleaveTimer;
        uint32 VolleyTimer;
        uint32 ShieldTimer;
        uint32 MarkerTimer;

        void Reset()
        {
            inCombat = false;
            Chosen = false;
            isImmune = false;
            CleaveTimer = 5000;
            VolleyTimer = 3000;
            ShieldTimer = 999999;
            MarkerTimer = 15000;
        }

        void EnterCombat (Unit* /*who*/)
        {
            if (!inCombat)
            {
                Talk(SAY_SUMMONED);
                inCombat = true;
            }
        }

        void UpdateAI (const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (CleaveTimer <= diff)
            {
                DoCast(me->getVictim(),SPELL_CLEAVE);
                CleaveTimer = urand(9000,15000);
            } else CleaveTimer -= diff;

            if (VolleyTimer <= diff)
            {
                DoCast(me, SPELL_SHADOW_BOLT_VOLLEY);
                VolleyTimer = urand(12000,20000);
            } else VolleyTimer -= diff;

            if (MarkerTimer <= diff)
            {
                switch (urand(1,4))
                {
                    case 1:
                        if (Creature* trigger = me->SummonCreature(NPC_MARKER_TRIGGER,me->GetPositionX()+30,me->GetPositionY(),me->GetPositionZ(),0.0f,TEMPSUMMON_TIMED_DESPAWN,30000))
                            trigger->AddAura(SPELL_VISUAL_MARKER,trigger);
                            break;
                    case 2:
                        if (Creature* trigger = me->SummonCreature(NPC_MARKER_TRIGGER,me->GetPositionX(),me->GetPositionY()+30,me->GetPositionZ(),0.0f,TEMPSUMMON_TIMED_DESPAWN,30000))
                            trigger->AddAura(SPELL_VISUAL_MARKER,trigger);
                            break;
                    case 3:
                        if (Creature* trigger = me->SummonCreature(NPC_MARKER_TRIGGER,me->GetPositionX()-30,me->GetPositionY(),me->GetPositionZ(),0.0f,TEMPSUMMON_TIMED_DESPAWN,30000))
                            trigger->AddAura(SPELL_VISUAL_MARKER,trigger);
                            break;
                    case 4:
                        if (Creature* trigger = me->SummonCreature(NPC_MARKER_TRIGGER,me->GetPositionX(),me->GetPositionY()-30,me->GetPositionZ(),0.0f,TEMPSUMMON_TIMED_DESPAWN,30000))
                            trigger->AddAura(SPELL_VISUAL_MARKER,trigger);
                            break;
                }
                MarkerTimer = urand(25000,30000);
            } else MarkerTimer -= diff;

            if (HealthBelowPct(50) && !Chosen)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0))
                {
                    if (target && target->isAlive())
                    {
                        Talk(SAY_CHOSEN);
                        DoCast(target, SPELL_WILL_OF_THE_ARAKKOA);
                        Chosen = true;
                    }
                }
            }

            if (HealthBelowPct(25) && !isImmune)
            {
                if (ShieldTimer <= diff)
                {
                    Talk(SAY_IMMUNE);
                    DoCast(me, SPELL_DIVINE_SHIELD);
                    ShieldTimer = 60000;
                    isImmune = true;
                } else ShieldTimer -= diff;
            }

            if (me->FindNearestCreature(NPC_MARKER_TRIGGER,5.0f,true) && isImmune)
            {
                me->RemoveAura(SPELL_DIVINE_SHIELD);
                DoCast(me, SPELL_FRENZY);
                isImmune = false;
            }
            DoMeleeAttackIfReady();
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_terokkAI(creature);
    }
};

/*###################
# spell_fumping_39238
####################*/

enum FumpingSpellData
{
    SPELL_FUMPING = 39238,
    NPC_MATURE_BONE_SIFTER = 22482,
    NPC_SAND_GNOME = 22483
};

class spell_fumping_39238 : public SpellScriptLoader
{
public:
    spell_fumping_39238() : SpellScriptLoader("spell_fumping_39238") {}

    class spell_fumping_39238SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_fumping_39238SpellScript)

        bool Validate(SpellInfo const * /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_FUMPING))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* player = GetCaster())
            {
                switch (urand(1,2))
                {
                    case 1: player->SummonCreature(NPC_MATURE_BONE_SIFTER, player->GetPositionX()+rand()%10, player->GetPositionY()+rand()%10, player->GetPositionZ()+5, 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0); break;
                    case 2: player->SummonCreature(NPC_SAND_GNOME, player->GetPositionX()+rand()%10, player->GetPositionY()+rand()%10, player->GetPositionZ()+5, 0.0f, TEMPSUMMON_DEAD_DESPAWN, 0); break;
                }
            }
        }
        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_fumping_39238SpellScript::HandleDummy, EFFECT_ALL, SPELL_EFFECT_ANY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_fumping_39238SpellScript();
    }
};

/*###################
# spell_fumping_39246
####################*/

enum AnotherFumpingSpellData
{
    SPELL_FUMPING2 = 39246,
    NPC_HAISHULUD = 22038
};

class spell_fumping_39246 : public SpellScriptLoader
{
public:
    spell_fumping_39246() : SpellScriptLoader("spell_fumping_39246") {}

    class spell_fumping_39246SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_fumping_39246SpellScript)

        bool Validate(SpellInfo const * /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_FUMPING2))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* player = GetCaster())
            {
                switch (urand(1,3))
                {
                    case 1:
                        player->SummonCreature(NPC_MATURE_BONE_SIFTER, player->GetPositionX()+rand()%10, player->GetPositionY()+rand()%10, player->GetPositionZ()+5, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);
                        player->SummonCreature(NPC_MATURE_BONE_SIFTER, player->GetPositionX()+rand()%10, player->GetPositionY()-rand()%10, player->GetPositionZ()+5, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);
                        player->SummonCreature(NPC_MATURE_BONE_SIFTER, player->GetPositionX()-rand()%10, player->GetPositionY()+rand()%10, player->GetPositionZ()+5, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);
                        break;
                    case 2:
                        player->SummonCreature(NPC_SAND_GNOME, player->GetPositionX()+rand()%10, player->GetPositionY()+rand()%10, player->GetPositionZ()+5, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);
                        player->SummonCreature(NPC_SAND_GNOME, player->GetPositionX()+rand()%10, player->GetPositionY()-rand()%10, player->GetPositionZ()+5, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);
                        player->SummonCreature(NPC_SAND_GNOME, player->GetPositionX()-rand()%10, player->GetPositionY()+rand()%10, player->GetPositionZ()+5, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 300000);
                        break;
                    case 3:
                        player->SummonCreature(NPC_HAISHULUD, player->GetPositionX()+rand()%5, player->GetPositionY()+rand()%5, player->GetPositionZ()+5, 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 600000);
                        break;
                }
            }
        }
        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_fumping_39246SpellScript::HandleDummy, EFFECT_ALL, SPELL_EFFECT_ANY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_fumping_39246SpellScript();
    }
};

enum keristraszaData
{
    QUEST_SPRINGING_TRAP = 11969,

    NPC_KERISTRASZA = 26237,
    NPC_SARAGOSA    = 26299,
    NPC_MALYGOS     = 26310,
    GO_SIGNAL_FIRE  = 194151,

    SPELL_RAELORASZ_SPARK = 62272,
    SPELL_FIRE_CORPSE     = 39199,
    SPELL_FIRE_BREATH     = 31962,
    SPELL_MALYGOS_EARTHQ  = 46853,
    SPELL_TAXI_KERISTASZA = 46814,
    SPELL_ICE_BLOCK       = 56644,
    SPELL_FROSTBOLT       = 61461,

    SAY_KERISTRASZA_1     = -1002030,
    YELL_KERISTRASZA_1    = -1002031,
    YELL_KERISTRASZA_2    = -1002032,
    YELL_KERISTRASZA_3    = -1002033,
    SAY_KERISTRASZA_2     = -1002034,

    //After Fligth
    YELL_MALYGOS_1        = -1002035,
    SAY_KERISTRASZA_3     = -1002036,
    YELL_MALYGOS_2        = -1002037,
    YELL_KERISTRASZA_4    = -1002038,
};
const Position posKeristrasza[6] =
{
    {4157.00f, 7035.00f, 215.87f, 0.00f}, // Summon position
    {4063.72f, 7084.12f, 174.86f, 0.00f}, // Land position
    {4054.51f, 7084.29f, 168.12f, 0.00f}, // Burn Corpse positon
    {4048.90f, 7083.94f, 168.21f, 0.00f}, // Saragosa Corpse Spawn
    {3800.47f, 6557.50f, 170.98f, 1.55f}, // Keristrasza 2nd Spawn
    {3791.76f, 6603.61f, 179.91f, 0.00f}, // Malygos Spawn
};
class npc_signal_fire : public CreatureScript
{
public:
    npc_signal_fire() : CreatureScript("npc_signal_fire") { }

    struct npc_signal_fireAI : public ScriptedAI
    {
        npc_signal_fireAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;
        Creature* pKeristrasza;
        Creature* pSaragosa;
        Player* player;
        bool eventRunning;

        void Reset()
        {
            // Reset al variables
            events.Reset();
            pKeristrasza, pSaragosa, player = NULL;
            eventRunning = false;
            // Unlit signal
            if(GameObject* pGo = me->FindNearestGameObject(GO_SIGNAL_FIRE, 2.0f))
                pGo->SetGoState(GO_STATE_READY);
        }

        void SpellHit(Unit* caster, SpellInfo const* spell)
        {
            if (eventRunning)
                return;

            // The invisible trigger handles the event until the Fly of Keristrasza
            if(spell->Id == SPELL_RAELORASZ_SPARK)
            {
                player = caster->ToPlayer();
                // Lit the fire signal
                if(GameObject* pGo = me->FindNearestGameObject(GO_SIGNAL_FIRE, 2.0f))
                    pGo->SetGoState(GO_STATE_ACTIVE);
                // Summon Keristrasza
                if(Creature* pFind = me->SummonCreature(NPC_KERISTRASZA, posKeristrasza[0]))
                {
                    pKeristrasza = pFind;
                    pFind->SetFlying(true);
                    pFind->GetMotionMaster()->MovePoint(1, posKeristrasza[1]);
                }
                eventRunning = true;
                events.ScheduleEvent(1, 6000);
            }
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            switch(events.ExecuteEvent())
            {
            case 1:
                pKeristrasza->SetFlying(false);
                DoScriptText(SAY_KERISTRASZA_1, pKeristrasza, player);
                events.ScheduleEvent(2, 3000);
                break;
            case 2:
                DoScriptText(YELL_KERISTRASZA_1, pKeristrasza);
                pKeristrasza->GetMotionMaster()->MovePoint(1, posKeristrasza[2]);
                events.ScheduleEvent(3, 5000);
                break;
            case 3:
                DoScriptText(YELL_KERISTRASZA_2, pKeristrasza);
                // Summon Saragosa and make her die
                if(Creature* pCorpse = me->SummonCreature(NPC_SARAGOSA, posKeristrasza[3]))
                    pSaragosa = pCorpse;
                events.ScheduleEvent(4, 3000);
                break;
            case 4:
                DoScriptText(YELL_KERISTRASZA_3, pKeristrasza);
                pKeristrasza->CastSpell(pSaragosa, SPELL_FIRE_BREATH, true);
                events.ScheduleEvent(5, 1000);
                break;
            case 5:
                me->AddAura(SPELL_FIRE_CORPSE, pSaragosa);
                events.ScheduleEvent(6, 1000);
                break;
            case 6:
                player->CastSpell(player, SPELL_MALYGOS_EARTHQ, true);
                events.ScheduleEvent(7, 3000);
                break;
            case 7:
                DoScriptText(SAY_KERISTRASZA_2, pKeristrasza, player);
                events.ScheduleEvent(8, 3000);
                break;
            case 8:
                // Passes the control to Keristrasza
                player->CastSpell(player, SPELL_TAXI_KERISTASZA, true);
                pKeristrasza->AI()->SetGUID(player->GetGUID());
                pSaragosa->DespawnOrUnsummon(10000);
                events.ScheduleEvent(9, 20000);
                break;
            case 9:
                Reset();
                break;
            }
        }

    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_signal_fireAI(creature);
    }
};

class npc_keristrasza_coldarra : public CreatureScript
{
public:
    npc_keristrasza_coldarra() : CreatureScript("npc_keristrasza_coldarra") { }

    struct npc_keristrasza_coldarraAI : public ScriptedAI
    {
        npc_keristrasza_coldarraAI(Creature* creature) : ScriptedAI(creature) { }

        EventMap events;
        uint64 uiPlayer;
        bool waiting;
        bool finishedWay;
        Creature* pMalygos;

        void SetGUID(const uint64 &guid, int32 /*iId*/)
        {
            me->NearTeleportTo(posKeristrasza[4].GetPositionX(), posKeristrasza[4].GetPositionY(), posKeristrasza[4].GetPositionZ(), posKeristrasza[4].GetOrientation());
            me->SetVisible(false);
            uiPlayer = guid;
            waiting = true;
        }
        void Reset()
        {
            events.Reset();
            me->SetSpeed(MOVE_FLIGHT, 3.2f, true);
            pMalygos = NULL;
            uiPlayer = 0;
            waiting = false;
            finishedWay = false;
            me->SetReactState(REACT_PASSIVE);
        }


        void UpdateAI(const uint32 diff)
        {
            if(!waiting)
                return;

            if(!finishedWay)
            {
                if(Player* player = me->GetPlayer(*me, uiPlayer))
                {
                    if (!player->isInFlight())
                    {
                        if(me->IsWithinDist(player, 10.0f))
                        {
                            finishedWay = true;
                            me->SetVisible(true);
                            player->ToPlayer()->KilledMonsterCredit(NPC_KERISTRASZA, 0);
                            events.ScheduleEvent(1, 0);
                        }else
                            me->DespawnOrUnsummon(0);
                    }
                    else
                        return;
                }
            }

            events.Update(diff);

            switch(events.ExecuteEvent())
            {
            case 1:
                me->SetFlying(false);
                if(Player* player = me->GetPlayer(*me, uiPlayer))
                    DoScriptText(SAY_KERISTRASZA_3, me, player);
                events.ScheduleEvent(2, 5000);
                break;
            case 2:
                if(Creature* pSumm = me->SummonCreature(NPC_MALYGOS, posKeristrasza[5]))
                {
                    pMalygos = pSumm;
                    pSumm->SetFlying(true);
                    pSumm->SetReactState(REACT_PASSIVE);
                    pSumm->SetFlag(UNIT_FIELD_FLAGS,UNIT_FLAG_OOC_NOT_ATTACKABLE);
                    pSumm->SetFloatValue(OBJECT_FIELD_SCALE_X, 0.4f);
                    DoScriptText(YELL_MALYGOS_2, pMalygos);
                    pMalygos->SetUInt64Value(UNIT_FIELD_TARGET, me->GetGUID());
                    me->SetUInt64Value(UNIT_FIELD_TARGET, pMalygos->GetGUID());
                }
                events.ScheduleEvent(3, 6000);
                break;
            case 3:
                DoScriptText(YELL_KERISTRASZA_4, me);
                me->AddAura(SPELL_ICE_BLOCK, me);

                if(pMalygos)
                    pMalygos->DespawnOrUnsummon(7000);
                me->DespawnOrUnsummon(7000);
                break;
            }

        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_keristrasza_coldarraAI(creature);
    }
};

enum evergroveData
{
    SPELL_DRUID_SIGNAL = 38782,
    SPELL_DRUID_FORM   = 39158,
    SPELL_CROW_FORM    = 38776,

    QUEST_DEATH_DOOR   = 10910,
    ITEM_DRUID_SIGNAL  = 31763,
};
class npc_evergrove_druid : public CreatureScript
{
public:
    npc_evergrove_druid() : CreatureScript("npc_evergrove_druid") { }

    struct npc_evergrove_druidAI : public ScriptedAI
    {
        npc_evergrove_druidAI(Creature* creature) : ScriptedAI(creature) { }

        Player* player;
        Position playerPos;
        bool summoned;
        float x,y,z;

        void Reset()
        {
            // Reset all variables
            me->SetFlying(true);
            me->SetSpeed(MOVE_FLIGHT, 3.2f, true);
            me->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER | UNIT_NPC_FLAG_GOSSIP);
            me->SetVisible(false);
            player = NULL;
            summoned = false;
        }

        void SpellHit(Unit* caster, SpellInfo const* spell)
        {
            if(summoned)
                return;

            if(spell->Id == SPELL_DRUID_SIGNAL)
            {
                summoned = true;

                if(caster)
                    player = caster->ToPlayer();

                // Prepare visuals
                me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_QUESTGIVER | UNIT_NPC_FLAG_GOSSIP);
                me->SetUInt64Value(UNIT_FIELD_TARGET, player->GetGUID());
                me->SetVisible(true);
                me->CastSpell(me, SPELL_CROW_FORM, true);

                // Move to player
                if(player)
                {
                    x = player->GetPositionX(); y = player->GetPositionY(); z = player->GetPositionZ();
                    me->GetMotionMaster()->MovePoint(0, x, y, z);
                }
            }
        }

        void MovementInform(uint32 type, uint32 id)
        {
            if(!player)
                return;

            me->UpdatePosition( x, y, z, 0.0f, true);
            if(!player->IsFlying())
            {
                me->CastSpell(me, SPELL_DRUID_FORM, true);
                me->RemoveAurasDueToSpell(SPELL_CROW_FORM);
                me->SetFlying(false);
                me->SetUInt32Value(UNIT_NPC_EMOTESTATE, EMOTE_STATE_STAND);
            }
            me->DespawnOrUnsummon(60000);
        }

    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_evergrove_druidAI(creature);
    }
};

#define GOSSIP_ITEM_DRUID_SIGNAL "He perdido mi Se�al para druida."

class npc_antelarion_gossip : public CreatureScript
{
public:
    npc_antelarion_gossip() : CreatureScript("npc_antelarion_gossip") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (creature->isQuestGiver())
            player->PrepareQuestMenu(creature->GetGUID());

        if ((player->GetQuestStatus(QUEST_DEATH_DOOR) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_DEATH_DOOR) == QUEST_STATUS_REWARDED) && !player->HasItemCount(ITEM_DRUID_SIGNAL, 1))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_DRUID_SIGNAL, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->CLOSE_GOSSIP_MENU();
        player->AddItem(ITEM_DRUID_SIGNAL, 1);
        return true;
    }
};

enum woodlandsWalkerData
{
    HOSTILE_FACTION = 16,
    QUEST_STRENGTH_ANCIENTS_A = 12092,
    QUEST_STRENGTH_ANCIENTS_H = 12096,
    ITEM_BARK_WALKERS = 36786,
};

#define GOSSIP_ITEM_BARK "Necesito un poco de tu corteza para reponer a los ancestros."

class npc_woodlands_walker : public CreatureScript
{
public:
    npc_woodlands_walker() : CreatureScript("npc_woodlands_walker") { }

    bool OnGossipHello(Player* player, Creature* creature)
    {
        if (player->GetQuestStatus(QUEST_STRENGTH_ANCIENTS_A) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(QUEST_STRENGTH_ANCIENTS_H) == QUEST_STATUS_INCOMPLETE)
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_BARK, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(creature), creature->GetGUID());

        return true;
    }

    bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 uiAction)
    {
        player->CLOSE_GOSSIP_MENU();

        switch (urand (1,2))
        {
            case 1:
                player->AddItem(ITEM_BARK_WALKERS, 1);
                creature->DisappearAndDie();
                break;
            case 2:
                creature->setFaction(HOSTILE_FACTION);
                creature->AI()->AttackStart(player);
                break;
        }

        return true;
    }
};

/*######
## Quest 12557: Lab Work
######*/

enum LabWorkReagents
{
    QUEST_LAB_WORK                          = 12557,
    SPELL_WIRHERED_BATWING_KILL_CREDIT      = 51226,
    SPELL_MUDDY_MIRE_MAGGOT_KILL_CREDIT     = 51227,
    SPELL_AMBERSEED_KILL_CREDIT             = 51228,
    SPELL_CHILLED_SERPENT_MUCUS_KILL_CREDIT = 51229,
    GO_AMBERSEED                            = 190459,
    GO_CHILLED_SERPENT_MUCUS                = 190462,
    GO_WITHERED_BATWING                     = 190473,
    GO_MUDDY_MIRE_MAGGOTS                   = 190478
};

class go_lab_work_reagents : public GameObjectScript
{
public:

    go_lab_work_reagents() : GameObjectScript("go_lab_work_reagents") { }

    bool OnGossipHello(Player* player, GameObject* go)
    {
        if (player->GetQuestStatus(QUEST_LAB_WORK) == QUEST_STATUS_INCOMPLETE)
        {
            uint32 CreditSpell = 0;
            switch (go->GetEntry())
            {
                case GO_AMBERSEED:              CreditSpell = SPELL_AMBERSEED_KILL_CREDIT; break;
                case GO_CHILLED_SERPENT_MUCUS:  CreditSpell = SPELL_CHILLED_SERPENT_MUCUS_KILL_CREDIT; break;
                case GO_WITHERED_BATWING:       CreditSpell = SPELL_WIRHERED_BATWING_KILL_CREDIT; break;
                case GO_MUDDY_MIRE_MAGGOTS:     CreditSpell = SPELL_MUDDY_MIRE_MAGGOT_KILL_CREDIT; break;
            }

            if (CreditSpell)
                player->CastSpell(player, CreditSpell, true);
        }
        return false;
    }
};

/*###################################
# spell_gen_ribbon_pole_dancer_check
####################################*/

enum RibbonPoleData
{
    SPELL_HAS_FULL_MIDSUMMER_SET        = 58933,
    SPELL_BURNING_HOT_POLE_DANCE        = 58934,
    SPELL_RIBBON_DANCE                  = 29175,
    GO_RIBBON_POLE                      = 181605
};

class spell_gen_ribbon_pole_dancer_check : public SpellScriptLoader
{
    public:
        spell_gen_ribbon_pole_dancer_check() : SpellScriptLoader("spell_gen_ribbon_pole_dancer_check") { }

        class spell_gen_ribbon_pole_dancer_check_AuraScript : public AuraScript
        {
            PrepareAuraScript(spell_gen_ribbon_pole_dancer_check_AuraScript);

            bool Validate(SpellInfo const* /*spell*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_HAS_FULL_MIDSUMMER_SET))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_BURNING_HOT_POLE_DANCE))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_RIBBON_DANCE))
                    return false;
                return true;
            }

            void PeriodicTick(AuraEffect const* /*aurEff*/)
            {
                Unit* target = GetTarget();

                if (!target)
                    return;

                // check if aura needs to be removed
                if (!target->FindNearestGameObject(GO_RIBBON_POLE, 20.0f) || !target->HasUnitState(UNIT_STAT_CASTING))
                {
                    target->InterruptNonMeleeSpells(false);
                    target->RemoveAurasDueToSpell(GetId());
                    return;
                }

                // set xp buff duration
                if (Aura* aur = target->GetAura(SPELL_RIBBON_DANCE))
                {
                    aur->SetMaxDuration(aur->GetMaxDuration() >= 3600000 ? 3600000 : aur->GetMaxDuration() + 180000);
                    aur->RefreshDuration();

                    // reward achievement criteria
                    if (aur->GetMaxDuration() == 3600000 && target->HasAura(SPELL_HAS_FULL_MIDSUMMER_SET))
                        target->CastSpell(target, SPELL_BURNING_HOT_POLE_DANCE, true);
                }
                else
                    target->AddAura(SPELL_RIBBON_DANCE, target);
            }

            void Register()
            {
                OnEffectPeriodic += AuraEffectPeriodicFn(spell_gen_ribbon_pole_dancer_check_AuraScript::PeriodicTick, EFFECT_0, SPELL_AURA_PERIODIC_DUMMY);
            }
        };

        AuraScript* GetAuraScript() const
        {
            return new spell_gen_ribbon_pole_dancer_check_AuraScript();
        }
};

/*######
## npc_torch_tossing_bunny
######*/

enum
{
    SPELL_TORCH_TOSSING_COMPLETE_A = 45719,
    SPELL_TORCH_TOSSING_COMPLETE_H = 46651,
    SPELL_TORCH_TOSSING_TRAINING   = 45716,
    SPELL_TORCH_TOSSING_PRACTICE   = 46630,
    SPELL_TORCH_TOSS               = 46054,
    SPELL_TARGET_INDICATOR         = 45723,
    SPELL_BRAZIERS_HIT             = 45724
};

class npc_torch_tossing_bunny : public CreatureScript
{
    public:
        npc_torch_tossing_bunny() : CreatureScript("npc_torch_tossing_bunny") { }

        struct npc_torch_tossing_bunnyAI : public ScriptedAI
        {
            npc_torch_tossing_bunnyAI(Creature* creature) : ScriptedAI(creature) { }

            void Reset()
            {
                _targetTimer = urand(5000, 20000);
            }

            void SpellHit(Unit* caster, SpellInfo const* spell)
            {
                if (spell->Id == SPELL_TORCH_TOSS && me->HasAura(SPELL_TARGET_INDICATOR))
                {
                    uint8 neededHits;

                    if (caster->HasAura(SPELL_TORCH_TOSSING_TRAINING))
                        neededHits = 8;
                    else if (caster->HasAura(SPELL_TORCH_TOSSING_PRACTICE))
                        neededHits = 20;
                    else
                        return;

                    DoCast(me, SPELL_BRAZIERS_HIT, true);
                    caster->AddAura(SPELL_BRAZIERS_HIT, caster);

                    if (caster->GetAuraCount(SPELL_BRAZIERS_HIT) >= neededHits)
                    {
                        // complete quest
                        caster->CastSpell(caster, SPELL_TORCH_TOSSING_COMPLETE_A, true);
                        caster->CastSpell(caster, SPELL_TORCH_TOSSING_COMPLETE_H, true);
                        caster->RemoveAurasDueToSpell(SPELL_BRAZIERS_HIT);
                        caster->RemoveAurasDueToSpell(neededHits == 8 ? SPELL_TORCH_TOSSING_TRAINING : SPELL_TORCH_TOSSING_PRACTICE);
                    }
                }
            }

            void UpdateAI(uint32 const diff)
            {
                if (_targetTimer <= diff)
                {
                    DoCast(SPELL_TARGET_INDICATOR);
                    _targetTimer = urand(10000, 20000);
                }
                else
                    _targetTimer -= diff;
            }

        private:
            uint32 _targetTimer;
        };

        CreatureAI* GetAI(Creature* creature) const
        {
            return new npc_torch_tossing_bunnyAI(creature);
        }
};

enum TorchCatchingData
{
    SPELL_FLING_TORCH_MISSILE     = 45669,
    SPELL_TOSS_TORCH_SHADOW       = 46105,
    SPELL_TORCH_TARGET_PICKER     = 45907,
    SPELL_TORCHES_COUGHT          = 45693,
    SPELL_JUGGLE_TORCH_MISSED     = 45676,
    SPELL_TORCH_CATCHING_SUCCESS  = 46081,
    SPELL_TORCH_DAILY_SUCCESS     = 46654,
    NPC_JUGGLE_TARGET             = 25515,
    QUEST_TORCH_CATCHING_A        = 11657,
    QUEST_TORCH_CATCHING_H        = 11923,
    QUEST_MORE_TORCH_CATCHING_A   = 11924,
    QUEST_MORE_TORCH_CATCHING_H   = 11925
};

class spell_gen_torch_target_picker : public SpellScriptLoader
{
    public:
        spell_gen_torch_target_picker() : SpellScriptLoader("spell_gen_torch_target_picker") {}

        class spell_gen_torch_target_picker_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_torch_target_picker_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_FLING_TORCH_MISSILE))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_TOSS_TORCH_SHADOW))
                    return false;
                return true;
            }

            void FilterTargets(std::list<Unit*>& unitList)
            {
                Unit* caster = GetCaster();

                if (!caster)
                    return;

                std::list<Creature*> juggleList;
                caster->GetCreatureListWithEntryInGrid(juggleList, NPC_JUGGLE_TARGET, 10.0f);

                if (!juggleList.empty())
                    for (std::list<Creature*>::iterator iter = juggleList.begin(); iter != juggleList.end(); ++iter)
                        unitList.remove(*iter);

                if (unitList.empty())
                    return;

                std::list<Unit*>::iterator itr = unitList.begin();
                std::advance(itr, urand(0, unitList.size() - 1));

                Unit* target = *itr;
                unitList.clear();
                unitList.push_back(target);
            }

            void HandleDummy(SpellEffIndex /*effIndex*/)
            {
                Unit* caster = GetCaster();
                Unit* target = GetHitUnit();

                if (!caster || !target)
                    return;

                caster->CastSpell(target, SPELL_FLING_TORCH_MISSILE, true);
                caster->CastSpell(target, SPELL_TOSS_TORCH_SHADOW, true);
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_gen_torch_target_picker_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_SRC_AREA_ENTRY);
                OnEffectHitTarget += SpellEffectFn(spell_gen_torch_target_picker_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_torch_target_picker_SpellScript();
        }
};

class spell_gen_juggle_torch_catch : public SpellScriptLoader
{
    public:
        spell_gen_juggle_torch_catch() : SpellScriptLoader("spell_gen_juggle_torch_catch") {}

        class spell_gen_juggle_torch_catch_SpellScript : public SpellScript
        {
            PrepareSpellScript(spell_gen_juggle_torch_catch_SpellScript);

            bool Validate(SpellInfo const* /*spellInfo*/)
            {
                if (!sSpellMgr->GetSpellInfo(SPELL_TORCH_TARGET_PICKER))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_TORCHES_COUGHT))
                    return false;
                if (!sSpellMgr->GetSpellInfo(SPELL_JUGGLE_TORCH_MISSED))
                    return false;
                return true;
            }

            void FilterTargets(std::list<Unit*>& unitList)
            {
                Unit* caster = GetCaster();
                Unit* juggleTarget = NULL;
                bool missed = true;

                if (unitList.empty() || !caster || !caster->ToPlayer())
                     return;

                for (std::list<Unit*>::iterator iter = unitList.begin(); iter != unitList.end(); ++iter)
                {
                    if (*iter == caster)
                        missed = false;

                    if ((*iter)->ToCreature())
                        juggleTarget = *iter;
                }

                if (missed)
                {
                    if (juggleTarget)
                        juggleTarget->CastSpell(juggleTarget, SPELL_JUGGLE_TORCH_MISSED, true);
                    caster->RemoveAurasDueToSpell(SPELL_TORCHES_COUGHT);
                }
                else
                {
                    uint8 neededCatches;

                    if (caster->ToPlayer()->GetQuestStatus(QUEST_TORCH_CATCHING_A) == QUEST_STATUS_INCOMPLETE
                        || caster->ToPlayer()->GetQuestStatus(QUEST_TORCH_CATCHING_H) == QUEST_STATUS_INCOMPLETE)
                    {
                        neededCatches = 4;
                    }
                    else if (caster->ToPlayer()->GetQuestStatus(QUEST_MORE_TORCH_CATCHING_A) == QUEST_STATUS_INCOMPLETE
                        || caster->ToPlayer()->GetQuestStatus(QUEST_MORE_TORCH_CATCHING_H) == QUEST_STATUS_INCOMPLETE)
                    {
                        neededCatches = 10;
                    }
                    else
                    {
                        caster->RemoveAurasDueToSpell(SPELL_TORCHES_COUGHT);
                        return;
                    }

                    caster->CastSpell(caster, SPELL_TORCH_TARGET_PICKER, true);
                    caster->CastSpell(caster, SPELL_TORCHES_COUGHT, true);

                    // reward quest
                    if (caster->GetAuraCount(SPELL_TORCHES_COUGHT) >= neededCatches)
                    {
                        caster->CastSpell(caster, SPELL_TORCH_CATCHING_SUCCESS, true);
                        caster->CastSpell(caster, SPELL_TORCH_DAILY_SUCCESS, true);
                        caster->RemoveAurasDueToSpell(SPELL_TORCHES_COUGHT);
                    }
                }
            }

            void Register()
            {
                OnUnitTargetSelect += SpellUnitTargetFn(spell_gen_juggle_torch_catch_SpellScript::FilterTargets, EFFECT_0, TARGET_UNIT_DEST_AREA_ENTRY);
            }
        };

        SpellScript* GetSpellScript() const
        {
            return new spell_gen_juggle_torch_catch_SpellScript();
        }
};

/*###########
# boss_ahune
############*/

enum SlavePensMidsummerData
{
    SPELL_ICE_SPIKE_TRIGG = 50097,
    SPELL_AHUNE_SHIELD = 45954,
    SPELL_AHUNE_LOOT = 45939,
    SPELL_ICE_AURA_VISUAL = 45945,
    SPELL_REMERGE = 46402,
    SPELL_GHOST = 46786,

    // unused
    SPELL_SUMMON_AHUNE = 45926,
    SPELL_AHUNE_ACHIEV = 62043,
    SPELL_CRYSTAL_SPIKE = 47944,
    SPELL_CRYSTAL_SPIKE_H = 57067,
    SPELL_SUMMON_SPIKE = 47955,

    // hailstone
    SPELL_CHILLING_AURA = 46542,
    SPELL_HAILSTONE_CHILL = 46458,
    SPELL_PULVERIZE = 2676,

    // coldwave
    SPELL_BITTER_BLAST = 46406,

    // frostwind
    SPELL_LIGHTNING_SHIELD = 12550,
    SPELL_WIND_BUFFET = 46568,

    // skarthis
    SPELL_CRASHING_WAVE = 55909,
    SPELL_FROST_NOVA = 11831,
    SPELL_FROSTBOLT_S = 15043,

    // Creatures
    NPC_AHUNE = 25740,
    NPC_FROZEN_CORE = 25865,
    NPC_GHOST_OF_AHUNE = 26239,
    NPC_SKARTHIS_SUMMONER = 40446,
    NPC_AHUNITE_HAILSTONE = 25755,
    NPC_AHUNITE_COLDWAVE  = 25756,
    NPC_AHUNITE_FROSTWIND = 25757
};

class boss_ahune : public CreatureScript
{
public:
    boss_ahune() : CreatureScript("boss_ahune") { }

    struct boss_ahuneAI : public Scripted_NoMovementAI
    {
        boss_ahuneAI(Creature* creature) : Scripted_NoMovementAI(creature)
        {
            Reset();
        }

        uint32 uiPhase;
        uint32 uiPhaseChangeTimer;
        uint32 uiSpikeTimer;
        uint32 uiAddsTimer;
        bool IsFirstPhase;
        uint8 uiWaveCount;

        uint64 uiCoreGUID;
        uint32 uiCoreHp;

        uint32 uiDieTimer;
        bool MustDie;

        void Reset()
        {
            uiPhase = 0;
            uiPhaseChangeTimer = 90000;
            uiSpikeTimer = 7000;
            uiAddsTimer = 10000;
            IsFirstPhase = true;
            uiWaveCount = 1;
            uiCoreGUID = 0;
            uiCoreHp = 0;
            uiDieTimer = 1000;
            MustDie = false;
            DoCast(me, SPELL_AHUNE_SHIELD);
        }

        void EnterCombat(Unit* who)
        {
            me->SetInCombatWithZone();
            DoCast(me, SPELL_ICE_AURA_VISUAL);
            uiPhase = 1;
        }

        void JustReachedHome()
        {
            me->ForcedDespawn();
            if(Creature* temp = me->GetMap()->GetCreature(uiCoreGUID))
                temp->ForcedDespawn();
        };

        void JustDied(Unit* /*killer*/)
        {
            DoCast(me, SPELL_AHUNE_ACHIEV);
        }

        void JustSummoned(Creature* summon)
        {
            summon->SetInCombatWithZone();
        }

        void DamageTaken(Unit* pDoneBy, uint32 &uiDamage)
        {
            if(me->GetHealth() < uiDamage)
            {
                uiDamage = 0;
                DoSummonLoot();
            }
        }

        void DoSummonLoot()
        {
            DoCast(me, SPELL_AHUNE_LOOT);
            uiDieTimer = 100;
            MustDie = true;
        }

        void UpdateAI(const uint32 uiDiff)
        {

            //Return since we have no target
            if (!UpdateVictim())
                return;

            if(uiSpikeTimer < uiDiff)
            {
                if (Unit *target = SelectTarget(SELECT_TARGET_RANDOM, 0, 10000, true))
                    DoCast(target, SPELL_ICE_SPIKE_TRIGG);
                    uiSpikeTimer = urand(10000, 15000);
            }
            else uiSpikeTimer -= uiDiff;

            if(uiDieTimer <= uiDiff && MustDie)
                me->DealDamage(me, me->GetHealth(), NULL, DIRECT_DAMAGE, SPELL_SCHOOL_MASK_NORMAL, NULL, false);
            else uiDieTimer -= uiDiff;

            if(uiPhase == 1)
            {
                if(!me->HasAura(SPELL_AHUNE_SHIELD))
                    DoCast(me, SPELL_AHUNE_SHIELD);

                if(uiAddsTimer < uiDiff)
                {
                    for(uint8 i = 0; i < uiWaveCount; i++)
                    {
                        float angle = (float) rand()*360/RAND_MAX + 1;
                        float homeX = me->GetPositionX() + 10*cos(angle*(M_PI/180));
                        float homeY = me->GetPositionY() + 10*sin(angle*(M_PI/180));
                        for(uint8 j = 0; j < 2; j++)
                            me->SummonCreature(NPC_AHUNITE_COLDWAVE, homeX + urand(0, 2), homeY + urand(0, 2), me->GetPositionZ(), 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                    }
                    if(!IsFirstPhase)
                    {
                        for(uint8 i = 0; i < uiWaveCount - 1; i++)
                            me->SummonCreature(NPC_AHUNITE_FROSTWIND, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                    }
                    uiAddsTimer = urand(5000, 7000);
                }
                else uiAddsTimer -= uiDiff;

                if(uiPhaseChangeTimer < uiDiff)
                {
                    if(IsFirstPhase)
                    {
                        if(Creature* temp = me->SummonCreature(NPC_FROZEN_CORE, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000))
                        {
                            uiCoreGUID = temp->GetGUID();
                            temp->setFaction(14);
                        }
                    }
                    else
                    {
                        if(Creature* core = me->GetMap()->GetCreature(uiCoreGUID))
                        {
                            core->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                            core->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                            core->SetVisible(true);
                        }
                    }
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->RemoveAurasDueToSpell(SPELL_AHUNE_SHIELD);
                    DoCast(me, SPELL_GHOST);
                    uiPhase = 2;
                    uiWaveCount += 1;
                    uiPhaseChangeTimer = 40000;
                }
                else uiPhaseChangeTimer -= uiDiff;

                DoMeleeAttackIfReady();
            }
            else if(uiPhase == 2)
            {
                if(uiPhaseChangeTimer < uiDiff)
                {
                    if(Creature* core = me->GetMap()->GetCreature(uiCoreGUID))
                    {
                        core->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                        core->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                        core->SetVisible(false);
                    }
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_SELECTABLE);
                    me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_NON_ATTACKABLE);
                    me->SummonCreature(NPC_AHUNITE_HAILSTONE, 0, 0, 0, 0, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 30000);
                    me->RemoveAurasDueToSpell(SPELL_GHOST);
                    DoCast(me, SPELL_REMERGE);
                    IsFirstPhase = false;
                    uiPhase = 1;
                    uiPhaseChangeTimer = 90000;
                }
                else uiPhaseChangeTimer -= uiDiff;
            }
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new  boss_ahuneAI(creature);
    }

};
class mob_frozen_core : public CreatureScript
{
public:
    mob_frozen_core() : CreatureScript("mob_frozen_core") { }

    struct mob_frozen_coreAI : public ScriptedAI
    {
        mob_frozen_coreAI (Creature* creature) : ScriptedAI(creature)
        {
            Reset();
        }

        void Reset()
        {
        }

        void UpdateAI(const uint32 uiDiff)
        {
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new  mob_frozen_coreAI(creature);
    }
};

/*#############
# ahune's adds
##############*/

class mob_ahunite_hailstone : public CreatureScript
{
public:
    mob_ahunite_hailstone() : CreatureScript("mob_ahunite_hailstone") { }

    struct mob_ahunite_hailstoneAI  : public ScriptedAI
    {
        mob_ahunite_hailstoneAI (Creature* creature) : ScriptedAI(creature)
        {
            Reset();
        }

        uint32 uiChillTimer;
        uint32 uiPulverizeTimer;

        void Reset()
        {
            DoCast(me, SPELL_CHILLING_AURA);
            uiPulverizeTimer = 7000;
            uiChillTimer = 5000;
        }

        void UpdateAI(const uint32 uiDiff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if(uiChillTimer < uiDiff)
            {
                if (Unit* target = SelectTarget(SELECT_TARGET_RANDOM,0))
                    DoCast(target, SPELL_HAILSTONE_CHILL);
                uiChillTimer = 5000;
            }
            else uiChillTimer -= uiDiff;

            if(uiPulverizeTimer < uiDiff)
            {
                DoCast(me->getVictim(), SPELL_PULVERIZE);
                uiPulverizeTimer = 10000;
            }
            else uiPulverizeTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new  mob_ahunite_hailstoneAI(creature);
    }
};

class mob_ahunite_coldwave : public CreatureScript
{
public:
    mob_ahunite_coldwave() : CreatureScript("mob_ahunite_coldwave") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new  mob_ahunite_coldwaveAI(creature);
    }

    struct mob_ahunite_coldwaveAI : public ScriptedAI
    {
        mob_ahunite_coldwaveAI (Creature* creature) : ScriptedAI(creature)
        {
            Reset();
        }

        uint32 uiBlastTimer;

        void Reset()
        {
            uiBlastTimer = 5000;
        }

        void UpdateAI(const uint32 uiDiff)
        {
            //Return since we have no target
            if (!UpdateVictim())
                return;

            if(uiBlastTimer < uiDiff)
            {
                DoCast(me->getVictim(), SPELL_BITTER_BLAST);
                uiBlastTimer = urand(4000, 7000);
            }
            else uiBlastTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };
};

class mob_ahunite_frostwind : public CreatureScript
{
public:
    mob_ahunite_frostwind() : CreatureScript("mob_ahunite_frostwind") { }

    struct mob_ahunite_frostwindAI : public ScriptedAI
    {
        mob_ahunite_frostwindAI (Creature* creature) : ScriptedAI(creature)
        {
            Reset();
        }

        uint32 uiBuffedTimer;

        void Reset()
        {
            DoCast(me, SPELL_LIGHTNING_SHIELD);
            uiBuffedTimer = 6000;
        }

        void UpdateAI(const uint32 uiDiff)
        {

            if(uiBuffedTimer < uiDiff)
            {
                DoCast(me->getVictim(), SPELL_WIND_BUFFET);
                uiBuffedTimer = 6000;
            }
            else uiBuffedTimer -= uiDiff;

            DoMeleeAttackIfReady();
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new  mob_ahunite_frostwindAI(creature);
    }
};

/*######################
# go_ice_stone_midsummer
########################*/
#define GOSSIP_SENDER_CHECK 547
class go_ice_stone_midsummer : public GameObjectScript
{
public:
    go_ice_stone_midsummer() : GameObjectScript("go_ice_stone_midsummer") { }

    bool OnGossipHello(Player* player, GameObject* go)
    {
        switch (go->GetGUIDLow())
        {
            case 801054:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Volver a la entrada del Recinto de los Esclavos",GOSSIP_SENDER_CHECK,GOSSIP_ACTION_INFO_DEF+1);
                break;
            case 801055:
                player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT,"Ir a la zona de Ahune",GOSSIP_SENDER_CHECK,GOSSIP_ACTION_INFO_DEF+2);
                break;
        }
        player->SEND_GOSSIP_MENU(DEFAULT_GOSSIP_MESSAGE,go->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, GameObject* /*go*/, uint32 /*uiSender*/, uint32 uiAction)
    {
        switch (uiAction)
        {
            case GOSSIP_ACTION_INFO_DEF+1:
                player->TeleportTo(547,127.881317f,-123.708618f,-1.590554f,4.317789f);
                break;
            case GOSSIP_ACTION_INFO_DEF+2:
                player->TeleportTo(547,-102.074738f,-128.866577f,-1.633126f,4.723298f);
                break;
        }
        player->CLOSE_GOSSIP_MENU();
        return true;
    }
};

/*######
## at_nats_landing
######*/
enum
{
    QUEST_NATS_BARGAIN = 11209,
    SPELL_FISH_PASTE   = 42644,
    NPC_LURKING_SHARK  = 23928
};

class at_nats_landing : public AreaTriggerScript
{
    public:
        at_nats_landing() : AreaTriggerScript("at_nats_landing") {}

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*at*/)
        {
            if (player->GetQuestStatus(QUEST_NATS_BARGAIN) == QUEST_STATUS_INCOMPLETE && player->HasAura(SPELL_FISH_PASTE))
            {
            Creature* shark = GetClosestCreatureWithEntry(player, NPC_LURKING_SHARK, 20.0f);

            if (!shark)
                shark = player->SummonCreature(NPC_LURKING_SHARK, -4246.243f, -3922.356f, -7.488f, 5.0f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 100000);
                shark->AI()->AttackStart(player);
                return false;
            }
            return true;
        }
};

/*########################
# at_legion_hold_smvalley
#########################*/

enum ToLegionHold
{
    SPELL_CRATE_DISGUISE = 37097,
    NPC_RAZUUN = 21502,
    NPC_JOVAAN = 21633
};

class at_legion_hold_smvalley : public AreaTriggerScript
{
    public:

        at_legion_hold_smvalley() : AreaTriggerScript("at_legion_hold_smvalley") {}

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
        {
            if (player->HasAura(SPELL_CRATE_DISGUISE))
            {
                player->SummonCreature(NPC_RAZUUN, -3302.599854f, 2928.284424f, 170.921692f, 2.609816f, TEMPSUMMON_TIMED_DESPAWN, 40000);
                player->SummonCreature(NPC_JOVAAN, -3306.278076f, 2932.812500f, 170.923660f, 5.535417f, TEMPSUMMON_TIMED_DESPAWN, 40000);
                player->KilledMonsterCredit(21502,0);
                return true;
            }
        }
};

/*######
## npc_feknut_bunny
######*/

enum FeknutBunnyData
{
    NPC_DARKCLAW_BAT        = 23959,
    SPELL_SUMMON_GUANO      = 43307

};

class npc_feknut_bunny : public CreatureScript
{
public:
    npc_feknut_bunny() : CreatureScript("npc_feknut_bunny") {}

    struct npc_feknut_bunnyAI : public ScriptedAI
    {
        npc_feknut_bunnyAI (Creature* creature) : ScriptedAI(creature) {}

        uint32 CheckTimer;
        bool Checked;

        void Reset()
        {
            CheckTimer = 1000;
            Checked = false;
        }

        void UpdateAI(const uint32 diff)
        {
            if(!Checked)
            {
                if (CheckTimer <= diff)
                {
                    if(Creature* bat = GetClosestCreatureWithEntry(me, NPC_DARKCLAW_BAT, 35.0f))
                    {
                        if(bat->isAlive())
                        {
                            bat->CastSpell(me, SPELL_SUMMON_GUANO, false);
                            Checked = true;
                        }
                        if(Player* player = me->GetOwner()->ToPlayer())
                        {
                            bat->Attack(player, true);
                            bat->GetMotionMaster()->MoveChase(player);
                        }
                    }
                    me->DespawnOrUnsummon();
                } else CheckTimer -= diff;
            }
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_feknut_bunnyAI(creature);
    }
};

/*###########################
# at_azure_dragons_sanctuary
############################*/

class at_azure_dragons_sanctuary : public AreaTriggerScript
{
    public:

        at_azure_dragons_sanctuary() : AreaTriggerScript("at_azure_dragons_sanctuary") {}

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
        {
            if (player->GetQuestStatus(12107) == QUEST_STATUS_INCOMPLETE || player->GetQuestStatus(12110) == QUEST_STATUS_INCOMPLETE)
            {
                player->KilledMonsterCredit(26889,0);
                return true;
            }
        }
};

/*############
# Quest 10226
#############*/

enum ElementalPowerExtractorData
{
    NPC_WARP_ABERRATION = 18865,
    NPC_SUNDERED_RUMBLER = 18881,
    SPELL_ARCANE_SHIELD = 36640,
    SPELL_WARP_STORM = 36577,
    SPELL_SUMMON_SUNDERED_SHARD = 35310,
    SPELL_ELEMENTAL_POWER_EXTRACTOR = 34520,
    SPELL_CREATE_ELEMENTAL_POWER = 34524,
    GO_ELEMENTAL_POWER = 183933
};

class npc_ele_power_extractor : public CreatureScript
{
public:
    npc_ele_power_extractor() : CreatureScript("npc_ele_power_extractor") { }

    struct npc_ele_power_extractorAI : public ScriptedAI
    {
        npc_ele_power_extractorAI(Creature* creature) : ScriptedAI(creature) {}

        bool hit;
        bool add;
        bool shield;
        uint32 WarpStormTimer;
        uint32 ArcaneShieldTimer;

        void Reset()
        {
            hit = false;
            add = false;
            shield = false;
            WarpStormTimer = urand(2500,3000);
            ArcaneShieldTimer = urand(500,1500);
        }

        void UpdateAI (const uint32 diff)
        {
            if (!UpdateVictim())
                return;

            if (me->GetEntry() == NPC_WARP_ABERRATION)
            {
                if (ArcaneShieldTimer <= diff && !shield)
                {
                    DoCast(me, SPELL_ARCANE_SHIELD, true);
                    shield = true;
                } else ArcaneShieldTimer -= diff;

                if (WarpStormTimer <= diff)
                {
                    if (Unit* target = me->getVictim())
                    {
                        DoCast(target, SPELL_WARP_STORM, true);
                        WarpStormTimer = urand(20000,25000);
                    }
                } else WarpStormTimer -= diff;
            }

            if (me->GetEntry() == NPC_SUNDERED_RUMBLER)
            {
                if (HealthBelowPct(15) && !add)
                {
                    DoCast(me, SPELL_SUMMON_SUNDERED_SHARD, true);
                    add = true;
                }
            }
            DoMeleeAttackIfReady();
        }

        void SpellHit (Unit* /*caster*/, const SpellInfo* spell)
        {
           if (spell->Id == SPELL_ELEMENTAL_POWER_EXTRACTOR)
               hit = true;
        }

        void JustDied (Unit* /*killer*/)
        {
            if (hit)
                me->SummonGameObject(GO_ELEMENTAL_POWER, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ()+2, 0.0f, 0, 0, 0, 0 ,25000);
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_ele_power_extractorAI(creature);
    }
};

/*##########
# Quest 4506
###########*/

enum CorruptedSabersData
{
    NPC_COMMON_KITTEN = 9937,
    NPC_CORRUPTED_SABER = 10657,
    NPC_WINNA_HAZZARD = 9996,
    ZONE_FELWOOD = 361,
    QUEST_CORRUPTED_SABERS = 4506,
};

class npc_q4506_cat : public CreatureScript
{
public:
    npc_q4506_cat() : CreatureScript("npc_q4506_cat") { }

    struct npc_q4506_catAI : public ScriptedAI
    {
        npc_q4506_catAI(Creature* creature) : ScriptedAI(creature) {}

        uint32 CorruptionTime;
        bool QuestGiverFound;

        void Reset()
        {
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE);
            CorruptionTime = 5000;
            QuestGiverFound = false;
        }

        void UpdateAI (const uint32 diff)
        {
            if (CorruptionTime <= diff && me->GetEntry() == NPC_COMMON_KITTEN)
            {
                me->UpdateEntry(NPC_CORRUPTED_SABER);
                if (Unit* owner = me->GetOwner())
                {
                    me->SetLevel(me->GetOwner()->getLevel());
                    me->SetFullHealth();
                }
            }
            else CorruptionTime -= diff;

            if (Creature* Winna = me->FindNearestCreature(NPC_WINNA_HAZZARD, 10.0f, true))
                QuestGiverFound = true;

            if (me->GetEntry() == NPC_CORRUPTED_SABER)
            {
                if (Unit* owner = me->GetOwner())
                {
                    if (owner->GetTypeId() != TYPEID_PLAYER || !owner->isAlive() || owner->ToPlayer()->GetZoneId() != ZONE_FELWOOD)
                        me->DespawnOrUnsummon();
                }
            }

            if (QuestGiverFound)
            {
                if (Player* owner = me->GetOwner()->ToPlayer())
                {
                    owner->ToPlayer()->GroupEventHappens(QUEST_CORRUPTED_SABERS, me);
                    me->DespawnOrUnsummon(3000);
                }
            }
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_q4506_catAI(creature);
    }
};

/*########################
# spell_song_of_cleansing
#########################*/

enum CleansingSongData
{
    SPELL_SONG_OF_CLEANSING = 52941,
    NPC_SPIRIT_OF_ATHA = 29033,
    NPC_SPIRIT_OF_HA_KHALAN = 29018,
    NPC_SPIRIT_OF_KOOSU = 29034,
    AREA_RIVERS_HEART = 4290,
    AREA_LAKE = 4385,
    AREA_WINTERGRASP_RIVER = 4388
};

class spell_song_of_cleansing : public SpellScriptLoader
{
public:
    spell_song_of_cleansing() : SpellScriptLoader("spell_song_of_cleansing") {}

    class spell_song_of_cleansing_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_song_of_cleansing_SpellScript)

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_SONG_OF_CLEANSING))
                return false;
            return true;
        }

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            if (Unit* caster = GetCaster())
                if (Player* player = caster->ToPlayer())
                    switch (player->GetAreaId())
                    {
                       case AREA_RIVERS_HEART: caster->SummonCreature(NPC_SPIRIT_OF_HA_KHALAN, caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000); break;
                       case AREA_LAKE: caster->SummonCreature(NPC_SPIRIT_OF_ATHA, caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000); break;
                       case AREA_WINTERGRASP_RIVER: caster->SummonCreature(NPC_SPIRIT_OF_KOOSU, caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_OR_DEAD_DESPAWN, 60000); break;
                    }
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_song_of_cleansing_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_song_of_cleansing_SpellScript();
    }
};

/*####################
# spell_contact_brann
#####################*/

enum ContactBrannData
{
    SPELL_CONTACT_BRANN = 61122,
    NPC_BRANN = 29579,
    ZONE_STORM_PEAKS = 67
};

class spell_contact_brann : public SpellScriptLoader
{
public:
    spell_contact_brann() : SpellScriptLoader("spell_contact_brann") {}

    class spell_contact_brann_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_contact_brann_SpellScript)

        bool Validate(SpellInfo const* /*spell*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_CONTACT_BRANN))
                return false;
            return true;
        }

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            PreventHitDefaultEffect(effIndex);
            if (Unit* caster = GetCaster())
               if (caster->ToPlayer())
                   if (caster->ToPlayer()->GetZoneId() == ZONE_STORM_PEAKS)
                       caster->SummonCreature(NPC_BRANN, caster->GetPositionX(), caster->GetPositionY(), caster->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 120000);
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_contact_brann_SpellScript::HandleScriptEffect, EFFECT_0, SPELL_EFFECT_SCRIPT_EFFECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_contact_brann_SpellScript();
    }
};

/*##########
# npc_moodle
###########*/

enum MoodleData
{
    AREA_MOSSWALKER_VILLAGE = 4297,
    QUEST_THE_ANGRY_GORLOC = 12578
};

class npc_moodle : public CreatureScript
{
public:
    npc_moodle() : CreatureScript("npc_moodle") { }

    struct npc_moodleAI : public ScriptedAI
    {
        npc_moodleAI(Creature* creature) : ScriptedAI(creature) {}

        void Reset()
        {
            if (Unit* owner = me->GetOwner())
                if (owner->GetTypeId() == TYPEID_PLAYER)
                    if (owner->ToPlayer()->GetAreaId() == AREA_MOSSWALKER_VILLAGE && owner->ToPlayer()->GetQuestStatus(QUEST_THE_ANGRY_GORLOC) == QUEST_STATUS_INCOMPLETE)
                        owner->ToPlayer()->GroupEventHappens(QUEST_THE_ANGRY_GORLOC, me);
        }
    };
    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_moodleAI(creature);
    }
};

/*##########################
# spell_construct_barricade
###########################*/

enum BarricadeData
{
    SPELL_CONSTRUCT_BARRICADE = 59925,
    SPELL_SUMMON_BARRICADE_A = 59922,
    SPELL_SUMMON_BARRICADE_B = 59923,
    SPELL_SUMMON_BARRICADE_C = 59924,
    NPC_EBON_BLADE_MARKER = 31887
};

class spell_construct_barricade : public SpellScriptLoader
{
public:
    spell_construct_barricade() : SpellScriptLoader("spell_construct_barricade") {}

    class spell_construct_barricade_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_construct_barricade_SpellScript)

        bool Validate(SpellInfo const * /*spellInfo*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_CONSTRUCT_BARRICADE))
                return false;
            return true;
        }

        void HandleDummy(SpellEffIndex /*effIndex*/)
        {
            if (Unit* caster = GetCaster())
                if (Player* player = caster->ToPlayer())
                {
                    uint32 SummonBarricadeSpell = 0;
                    switch (urand(1,3))
                    {
                        case 1: SummonBarricadeSpell = SPELL_SUMMON_BARRICADE_A; break;
                        case 2: SummonBarricadeSpell = SPELL_SUMMON_BARRICADE_B; break;
                        case 3: SummonBarricadeSpell = SPELL_SUMMON_BARRICADE_C; break;
                    }
                    player->CastSpell(player, SummonBarricadeSpell, true);
                    player->KilledMonsterCredit(NPC_EBON_BLADE_MARKER, 0);
                }
        }
        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_construct_barricade_SpellScript::HandleDummy, EFFECT_0, SPELL_EFFECT_DUMMY);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_construct_barricade_SpellScript();
    }
};

/*###########################
# npc_dragonforged_blades_giver (Quel'Delar quest chain)
############################*/

enum DragonforgedBlades
{
    QUEST_MEETING_ARCANIST   = 24451, // Quest Horde side
    QUEST_MEETING_MAGISTER   = 20439, // Quest Alliance side
    SPELL_SUNREAVER_DISGUISE = 69672, // Casted on Alliance players
    SPELL_SILVER_DISGUISE    = 69672, // Casted on Horde players
    AURA_SUNREAVER_DIS_F     = 70973, // Aura on Alliance palyers (male)
    AURA_SUNREAVER_DIS_M     = 70974, // Aura on Alliance palyers (female)
    AURA_SILVER_DIS_F        = 70971, // Aura on Horde palyers (male)
    AURA_SILVER_DIS_M        = 70972, // Aura on Horde palyers (female)
    NPC_MAGISTER_HATHOREL    = 36670, // Horde npc
    NPC_ARCANIST_TYBALIN     = 36669, // Alliance npc
    ITEM_DRAGONFORGED_BLADES = 49698,
    SPELL_CREATE_BOOK        = 69722,

    TEXT_ID_GIVE_BOOK_ALLI   = 537554,
    TEXT_ID_GIVE_BOOK_HORDE  = 537555,

    SAY_1_COMMON             = -1380573,
    SAY_2_HORDE              = -1380574,
    SAY_2_ALLIANCE           = -1380575,
    SAY_3_HORDE              = -1380576,
    SAY_3_ALLIANCE           = -1380577,
};

#define GOSSIP_ITEM_TABARD    "Necesito volver a encantar mi tabardo."
#define GOSSIP_ITEM_REQUEST_A "Entregar� el tomo a nuestros contactos en Corona de hielo, arcanista."
#define GOSSIP_ITEM_REQUEST_H "Entregar� el tomo a nuestros contactos en Corona de hielo, magister."

class npc_dragonforged_blades_giver : public CreatureScript
{
public:
    npc_dragonforged_blades_giver() : CreatureScript("npc_dragonforged_blades_giver") { }

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_dragonforged_blades_giverAI(creature);
    }

        bool OnGossipHello(Player* player, Creature* creature)
        {
            uint32 menuTextId = player->GetGossipTextId(creature);

            if(creature->isQuestGiver())
                player->PrepareQuestMenu( creature->GetGUID());

            switch(creature->GetEntry())
            {
                case NPC_ARCANIST_TYBALIN:
                    switch(player->GetTeam())
                    {
                        case ALLIANCE:
                            if(player->GetQuestStatus(QUEST_MEETING_MAGISTER) == QUEST_STATUS_INCOMPLETE)
                                if(!player->HasAura(AURA_SUNREAVER_DIS_M) || !player->HasAura(AURA_SUNREAVER_DIS_F))
                                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, GOSSIP_ITEM_TABARD, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
                            break;
                        case HORDE:
                            if(player->GetQuestStatus(QUEST_MEETING_ARCANIST) == QUEST_STATUS_INCOMPLETE)
                            {
                                menuTextId = TEXT_ID_GIVE_BOOK_ALLI;
                                if(!player->HasItemCount(ITEM_DRAGONFORGED_BLADES, 1, true))
                                    if(player->HasAura(AURA_SILVER_DIS_M) || player->HasAura(AURA_SILVER_DIS_F))
                                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_REQUEST_A, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                            }
                            break;
                    }
                    break;
                case NPC_MAGISTER_HATHOREL:
                    switch(player->GetTeam())
                    {
                        case HORDE:
                            if(player->GetQuestStatus(QUEST_MEETING_ARCANIST) == QUEST_STATUS_INCOMPLETE)
                                if(!player->HasAura(AURA_SILVER_DIS_M) || !player->HasAura(AURA_SILVER_DIS_F))
                                    player->ADD_GOSSIP_ITEM(GOSSIP_ICON_TABARD, GOSSIP_ITEM_TABARD, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
                            break;
                        case ALLIANCE:
                            if(player->GetQuestStatus(QUEST_MEETING_MAGISTER) == QUEST_STATUS_INCOMPLETE)
                            {
                                menuTextId = TEXT_ID_GIVE_BOOK_HORDE;
                                if(!player->HasItemCount(ITEM_DRAGONFORGED_BLADES, 1, true))
                                    if(player->HasAura(AURA_SUNREAVER_DIS_M) || player->HasAura(AURA_SUNREAVER_DIS_F))
                                        player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_ITEM_REQUEST_A, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF+1);
                            }
                            break;
                    }
                    break;
            }
            player->SEND_GOSSIP_MENU(menuTextId, creature->GetGUID());
            return true;
        }

        bool OnGossipSelect(Player* player, Creature* creature, uint32 /*uiSender*/, uint32 action)
        {
            player->PlayerTalkClass->ClearMenus();

            switch(action)
            {
                case GOSSIP_ACTION_INFO_DEF:
                    switch(creature->GetEntry())
                    {
                        case NPC_ARCANIST_TYBALIN: creature->CastSpell(player, SPELL_SUNREAVER_DISGUISE, true); break;
                        case NPC_MAGISTER_HATHOREL: creature->CastSpell(player, SPELL_SILVER_DISGUISE, true); break;
                    }
                    break;
                case GOSSIP_ACTION_INFO_DEF+1:
                    creature->AI()->SetGUID(player->GetGUID(), 1);
                    creature->RemoveFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
                    break;
            }

            player->CLOSE_GOSSIP_MENU();
            return true;
        }

    struct npc_dragonforged_blades_giverAI : public ScriptedAI
    {
        npc_dragonforged_blades_giverAI(Creature* c) : ScriptedAI(c) {}

        EventMap events;
        uint64 playerGUID;
        bool inProgress;

        void Reset()
        {
            me->SetTarget(0);
            me->SetFlag(UNIT_NPC_FLAGS, UNIT_NPC_FLAG_GOSSIP);
            inProgress = false;
            events.Reset();
            playerGUID = 0;
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            switch(events.ExecuteEvent())
            {
                case 1:
                    DoScriptText(SAY_1_COMMON, me);
                    events.ScheduleEvent(2, 5000);
                    break;
                case 2:
                    DoScriptText(me->GetEntry() == NPC_ARCANIST_TYBALIN ? SAY_2_HORDE : SAY_2_ALLIANCE, me);
                    if(Player* player = me->GetPlayer(*me, playerGUID))
                        DoCast(player, SPELL_CREATE_BOOK, true);
                    events.ScheduleEvent(3, 6000);
                    break;
                case 3:
                    DoScriptText(me->GetEntry() == NPC_ARCANIST_TYBALIN ? SAY_3_HORDE : SAY_3_ALLIANCE, me);
                    events.ScheduleEvent(4, 3000);
                    break;
                case 4:
                    Reset();
                    break;
            }
        }

        void SetGUID(const uint64 guid, int32 id)
        {
            if(inProgress)
                return;

            if(id == 1)
            {
                events.ScheduleEvent(1, 0);
                playerGUID = guid;
                me->SetTarget(playerGUID);
                inProgress = true;
            }
        }
    };

};

// Dark Iron Guzzler in the Brewfest achievement 'Down With The Dark Iron'
enum DarkIronGuzzler
{
    NPC_DARK_IRON_GUZZLER       = 23709,
    NPC_DARK_IRON_HERALD        = 24536,
    NPC_DARK_IRON_SPAWN_BUNNY   = 23894,

    NPC_FESTIVE_KEG_1           = 23702, // Thunderbrew Festive Keg
    NPC_FESTIVE_KEG_2           = 23700, // Barleybrew Festive Keg
    NPC_FESTIVE_KEG_3           = 23706, // Gordok Festive Keg
    NPC_FESTIVE_KEG_4           = 24373, // T'chalis's Festive Keg
    NPC_FESTIVE_KEG_5           = 24372, // Drohn's Festive Keg

    SPELL_GO_TO_NEW_TARGET      = 42498,
    SPELL_ATTACK_KEG            = 42393,
    SPELL_RETREAT               = 42341,
    SPELL_DRINK                 = 42436,

    SAY_RANDOM              = 0,
};

class npc_dark_iron_guzzler : public CreatureScript
{
public:
    npc_dark_iron_guzzler() : CreatureScript("npc_dark_iron_guzzler") { }

    CreatureAI *GetAI(Creature* creature) const
    {
        return new npc_dark_iron_guzzlerAI(creature);
    }

    struct npc_dark_iron_guzzlerAI : public ScriptedAI
    {
        npc_dark_iron_guzzlerAI(Creature* creature) : ScriptedAI(creature) { }

        bool atKeg;
        bool playersLost;
        bool barleyAlive;
        bool thunderAlive;
        bool gordokAlive;
        bool drohnAlive;
        bool tchaliAlive;

        uint32 AttackKegTimer;
        uint32 TalkTimer;

        void Reset()
        {
            AttackKegTimer = 5000;
            TalkTimer = (urand(1000, 120000));
            me->AddUnitMovementFlag(MOVEMENTFLAG_WALKING);
        }

        void IsSummonedBy(Unit* summoner)
        {
            // Only cast the spell on spawn
            DoCast(me, SPELL_GO_TO_NEW_TARGET);
        }

        // These values are set through SAI - when a Festive Keg dies it will set data to all Dark Iron Guzzlers within 3 yards (the killers)
        void SetData(uint32 type, uint32 data)
        {
            if (type == 10 && data == 10)
            {
                DoCast(me, SPELL_GO_TO_NEW_TARGET);
                thunderAlive = false;
            }

            if (type == 11 && data == 11)
            {
                DoCast(me, SPELL_GO_TO_NEW_TARGET);
                barleyAlive = false;
            }

            if (type == 12 && data == 12)
            {
                DoCast(me, SPELL_GO_TO_NEW_TARGET);
                gordokAlive = false;
            }

            if (type == 13 && data == 13)
            {
                DoCast(me, SPELL_GO_TO_NEW_TARGET);
                drohnAlive = false;
            }

            if (type == 14 && data == 14)
            {
                DoCast(me, SPELL_GO_TO_NEW_TARGET);
                tchaliAlive = false;
            }
        }

        // As you can see here we do not have to use a spellscript for this
        void SpellHit(Unit* caster, const SpellInfo* spell)
        {
            if (spell->Id == SPELL_DRINK)
            {
                // Fake death - it's only visual!
                me->SetUInt32Value(UNIT_FIELD_BYTES_1, UNIT_STAND_STATE_DEAD);
                me->StopMoving();

                // Time based on information from videos
                me->ForcedDespawn(7000);
            }

            // Retreat - run back
            if (spell->Id == SPELL_RETREAT)
            {
                // Remove walking flag so we start running
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_WALKING);

                if (me->GetAreaId() == 1296)
                {
                    me->GetMotionMaster()->MovePoint(1, 1197.63f, -4293.571f, 21.243f);
                }
                else if (me->GetAreaId() == 1)
                {
                     me->GetMotionMaster()->MovePoint(2, -5152.3f, -603.529f, 398.356f);
                }
            }

            if (spell->Id == SPELL_GO_TO_NEW_TARGET)
            {
                // If we're at Durotar we target different kegs if we are at at Dun Morogh
                if (me->GetAreaId() == 1296)
                {
                    if (drohnAlive && gordokAlive && tchaliAlive)
                    {
                        switch (urand(0, 2))
                        {
                            case 0: // Gordok Festive Keg
                                me->GetMotionMaster()->MovePoint(4, 1220.86f, -4297.37f, 21.192f);
                                break;
                            case 1: // Drohn's Festive Keg
                                me->GetMotionMaster()->MovePoint(5, 1185.98f, -4312.98f, 21.294f);
                                break;
                            case 2: // Ti'chali's Festive Keg
                                me->GetMotionMaster()->MovePoint(6, 1184.12f, -4275.21f, 21.191f);
                                break;
                        }
                    }
                    else if (!drohnAlive)
                    {
                        switch (urand(0, 1))
                        {
                            case 0: // Gordok Festive Keg
                                me->GetMotionMaster()->MovePoint(4, 1220.86f, -4297.37f, 21.192f);
                                break;
                            case 1: // Ti'chali's Festive Keg
                                me->GetMotionMaster()->MovePoint(6, 1184.12f, -4275.21f, 21.191f);
                                break;
                        }
                    }
                    else if (!gordokAlive)
                    {
                        switch (urand(0, 1))
                        {
                            case 0: // Drohn's Festive Keg
                                me->GetMotionMaster()->MovePoint(5, 1185.98f, -4312.98f, 21.294f);
                                break;
                            case 1: // Ti'chali's Festive Keg
                                me->GetMotionMaster()->MovePoint(6, 1184.12f, -4275.21f, 21.191f);
                                break;
                        }
                    }
                    else if (!tchaliAlive)
                    {
                        switch (urand(0, 1))
                        {
                            case 0: // Gordok Festive Keg
                                me->GetMotionMaster()->MovePoint(4, 1220.86f, -4297.37f, 21.192f);
                                break;
                            case 1: // Drohn's Festive Keg
                                me->GetMotionMaster()->MovePoint(5, 1185.98f, -4312.98f, 21.294f);
                                break;
                        }
                    }
                }
                // If we're at Dun Morogh we target different kegs if we are at Durotar
                else if (me->GetAreaId() == 1)
                {
                    if (barleyAlive && gordokAlive && thunderAlive)
                    {
                        switch (urand(0, 2))
                        {
                            case 0: // Barleybrew Festive Keg
                                me->GetMotionMaster()->MovePoint(7, -5183.67f, -599.58f, 397.177f);
                                break;
                            case 1: // Thunderbrew Festive Keg
                                me->GetMotionMaster()->MovePoint(8, -5159.53f, -629.52f, 397.213f);
                                break;
                            case 2: // Gordok Festive Keg
                                me->GetMotionMaster()->MovePoint(9, -5148.01f, -578.34f, 397.177f);
                                break;
                        }
                    }
                    else if (!barleyAlive)
                    {
                        switch (urand(0, 1))
                        {
                            case 0: // Thunderbrew Festive Keg
                                me->GetMotionMaster()->MovePoint(8, -5159.53f, -629.52f, 397.213f);
                                break;
                            case 1: // Gordok Festive Keg
                                me->GetMotionMaster()->MovePoint(9, -5148.01f, -578.34f, 397.177f);
                                break;
                        }
                    }
                    else if (!gordokAlive)
                    {
                        switch (urand(0, 1))
                        {
                            case 0: // Barleybrew Festive Keg
                                me->GetMotionMaster()->MovePoint(7, -5183.67f, -599.58f, 397.177f);
                                break;
                            case 1: // Thunderbrew Festive Keg
                                me->GetMotionMaster()->MovePoint(8, -5159.53f, -629.52f, 397.213f);
                                break;
                        }
                    }
                    else if (!thunderAlive)
                    {
                        switch (urand(0, 1))
                        {
                            case 0: // Barleybrew Festive Keg
                                me->GetMotionMaster()->MovePoint(7, -5183.67f, -599.58f, 397.177f);
                                break;
                            case 1: // Gordok Festive Keg
                                me->GetMotionMaster()->MovePoint(9, -5148.01f, -578.34f, 397.177f);
                                break;
                        }
                    }
                }
                atKeg = false;
            }
        }

        void MovementInform(uint32 Type, uint32 PointId)
        {
            if (Type != POINT_MOTION_TYPE)
                return;

            // Arrived at the retreat spot, we should despawn
            if (PointId == 1 || PointId == 2)
                me->ForcedDespawn(1000);

            // Arrived at the new keg - the spell has conditions in database
            if (PointId == 4 || PointId == 5 || PointId == 6 || PointId == 7 || PointId == 8 || PointId == 9)
            {
                DoCast(SPELL_ATTACK_KEG);
                me->SetByteFlag(UNIT_FIELD_BYTES_1, 1, 0x01); // Sit down
                atKeg = true;
            }
        }

        void UpdateAI(const uint32 diff)
        {
            if (!IsHolidayActive(HOLIDAY_BREWFEST))
                return;

            // If all kegs are dead we should retreat because we have won
            if ((!gordokAlive && !thunderAlive && !barleyAlive) || (!gordokAlive && !drohnAlive && !tchaliAlive))
            {
                DoCast(me, SPELL_RETREAT);

                // We are doing this because we'll have to reset our scripts when we won
                if (Creature* herald = me->FindNearestCreature(NPC_DARK_IRON_HERALD, 100.0f))
                    herald->AI()->SetData(20, 20);

                // Despawn all summon bunnies so they will stop summoning guzzlers
                if (Creature* spawnbunny = me->FindNearestCreature(NPC_DARK_IRON_SPAWN_BUNNY, 100.0f))
                    spawnbunny->ForcedDespawn();
            }

            if (TalkTimer <= diff)
            {
                me->AI()->Talk(SAY_RANDOM);
                TalkTimer = (urand(44000, 120000));
            } else TalkTimer -= diff;

            // Only happens if we're at keg
            if (atKeg)
            {
                if (AttackKegTimer <= diff)
                {
                    DoCast(SPELL_ATTACK_KEG);
                    AttackKegTimer = 5000;
                } else AttackKegTimer -= diff;
            }
        }
    };
};

/*######
## at_brewfest
######*/

enum Brewfest
{
        NPC_TAPPER_SWINDLEKEG     = 24711,
        NPC_IPFELKOFER_IRONKEG    = 24710,

        AT_BREWFEST_DUROTAR       = 4829,
        AT_BREWFEST_DUN_MOROGH    = 4820,

        SAY_WELCOME               = 4,

        AREATRIGGER_TALK_COOLDOWN = 5, // in seconds
};

class at_brewfest : public AreaTriggerScript
{
        public:
                at_brewfest() : AreaTriggerScript("at_brewfest")
                {
                        // Initialize for cooldown
                        _triggerTimes[AT_BREWFEST_DUROTAR] = _triggerTimes[AT_BREWFEST_DUN_MOROGH] = 0;
                }

                bool OnTrigger(Player* player, AreaTriggerEntry const* trigger)
                {
                        uint32 triggerId = trigger->id;
                        // Second trigger happened too early after first, skip for now
                        if (sWorld->GetGameTime() - _triggerTimes[triggerId] < AREATRIGGER_TALK_COOLDOWN)
                                return false;

                        switch (triggerId)
                        {
                                case AT_BREWFEST_DUROTAR:
                                        if (Creature* tapper = player->FindNearestCreature(NPC_TAPPER_SWINDLEKEG, 20.0f))
                                                tapper->AI()->Talk(SAY_WELCOME, player->GetGUID());
                                        break;
                                case AT_BREWFEST_DUN_MOROGH:
                                        if (Creature* ipfelkofer = player->FindNearestCreature(NPC_IPFELKOFER_IRONKEG, 20.0f))
                                                ipfelkofer->AI()->Talk(SAY_WELCOME, player->GetGUID());
                                        break;
                                default:
                                        break;
                        }

                        _triggerTimes[triggerId] = sWorld->GetGameTime();
                        return false;
                }

        private:
                std::map<uint32, time_t> _triggerTimes;
};

enum mistwhisperTreasure
{
    QUEST_LOST_MISTWHISPER_TREASURE = 12575,
    NPC_WARLORD_TARTEK              = 28105,
    ITEM_MISTWHISPER_TREASURE       = 38601,
};

class go_mistwhisper_treasure : public GameObjectScript
{
public:
    go_mistwhisper_treasure() : GameObjectScript("go_mistwhisper_treasure") { }

    bool OnGossipHello(Player* player, GameObject* go)
    {
        if (player->HasItemCount(ITEM_MISTWHISPER_TREASURE, 1) && player->GetQuestStatus(QUEST_LOST_MISTWHISPER_TREASURE) == QUEST_STATUS_INCOMPLETE)
        {
            if (tartekGUID)
                if (Creature* tartek = player->GetCreature(*player, tartekGUID))
                    if (tartek->isAlive())
                        return false;
                    else
                        tartek->DespawnOrUnsummon();

            if (Creature*  tartek = go->SummonCreature(NPC_WARLORD_TARTEK, 6708.30f, 5147.15f, 0.92712f, 4.9878f, TEMPSUMMON_TIMED_DESPAWN_OUT_OF_COMBAT, 10000))
            {
                tartekGUID = tartek->GetGUID();
                tartek->GetMotionMaster()->MovePoint(0, player->GetPositionX(), player->GetPositionY(), player->GetPositionZ());
            }
        }
        return true;
    }
private:
    uint64 tartekGUID;
};

enum HalloweenData
{
    NPC_STINKY_BOMB_CREDIT = 15415,

    GO_STINKY_BOMB_FLASK   = 180449,
    GO_STINKY_BOMB_CLOUD   = 180450,

    QUEST_CRASHING_WICKERMAN_FESTIVAL = 1658,

    SPELL_PIRATE_COSTUME_MALE           = 24708,
    SPELL_PIRATE_COSTUME_FEMALE         = 24709,
    SPELL_NINJA_COSTUME_MALE            = 24711,
    SPELL_NINJA_COSTUME_FEMALE          = 24710,
    SPELL_LEPER_GNOME_COSTUME_MALE      = 24712,
    SPELL_LEPER_GNOME_COSTUME_FEMALE    = 24713,
    SPELL_GHOST_COSTUME_MALE            = 24735,
    SPELL_GHOST_COSTUME_FEMALE          = 24736,


    SPELL_HALLOWEEN_WAND_PIRATE         = 24717,
    SPELL_HALLOWEEN_WAND_NINJA          = 24718,
    SPELL_HALLOWEEN_WAND_LEPER_GNOME    = 24719,
    SPELL_HALLOWEEN_WAND_RANDOM         = 24720,
    SPELL_HALLOWEEN_WAND_SKELETON       = 24724,
    SPELL_HALLOWEEN_WAND_WISP           = 24733,
    SPELL_HALLOWEEN_WAND_GHOST          = 24737,
    SPELL_HALLOWEEN_WAND_BAT            = 24741,

    SPELL_GRIM_VISAGE                   = 24705,

    // Headless Horseman fire event
    NPC_HEADLESS_FIRE                   = 23537,
    NPC_FIRE_DUMMY                      = 23686,
    NPC_SHADE_HORSEMAN                  = 23543,
    GO_FIRE_EFFIGY                      = 186720,
    GO_LARGE_JACK_O_LANTERN             = 186887,


    SPELL_FIRE_CREATE_NODE              = 42118,
    SPELL_WATER_SPOUT_VISUAL            = 42348,
    SPELL_FIRE_VISUAL_BUFF              = 42074,
    SPELL_FIRE_SIZE_STACK               = 42091,
    SPELL_FIRE_STARTING_SIZE            = 42096,
    SPELL_QUESTS_CREDITS                = 42242,
    SPELL_CREATE_WATER_BUCKET           = 42349,

    SPELL_HORSEMAN_CONFLAGRATION_1      = 42380,
    SPELL_HORSEMAN_CONFLAGRATION_2      = 42869,
    SPELL_HORSEMAN_JACK_O_LANTERN       = 44185,
    SPELL_HORSEMAN_CLEAVE               = 15496,

    SAY_HORSEMAN_SPAWN                  = 1,
    SAY_HORSEMAN_FIRES_OUT              = 2,
    SAY_HORSEMAN_FIRES_FAIL             = 3,
    SAY_HORSEMAN_LAUGHS                 = 4,

    QUEST_LET_THE_FIRES_COME_A          = 12135,
    QUEST_LET_THE_FIRES_COME_H          = 12139,
    QUEST_STOP_FIRES_A                  = 11131,
    QUEST_STOP_FIRES_H                  = 11219,
};

#define FIRE_NODES_PER_AREA  13

const Position FireNodesGoldShire[FIRE_NODES_PER_AREA + 1] =
{
    {-9459.41f, 43.90f, 64.23f, 0.00f},
    {-9472.57f, 41.11f, 64.17f, 0.00f},
    {-9467.22f, 85.86f, 66.20f, 0.00f},
    {-9472.94f, 93.84f, 69.20f, 0.00f},
    {-9462.50f, 103.90f, 68.51f, 0.00f},
    {-9467.84f, 100.69f, 66.12f, 0.00f},
    {-9456.91f, 112.81f, 66.12f, 0.00f},
    {-9478.22f, 41.65f, 69.85f, 0.00f},
    {-9481.30f, 24.87f, 69.08f, 0.00f},
    {-9482.69f, 14.39f, 62.94f, 0.00f},
    {-9471.16f, -6.65f, 70.76f, 0.00f},
    {-9451.26f, 38.83f, 68.02f, 0.00f},
    {-9450.13f, 89.46f, 66.22f, 0.00f},
    {-9464.28f,68.1982f,56.2331f,0.0f}, // Center of Town
};

const Position FireNodesRazorHill[FIRE_NODES_PER_AREA+1] =
{
    {372.70f, -4714.64f, 23.11f, 0.00f},
    {343.11f, -4708.87f, 29.19f, 0.00f},
    {332.06f, -4703.21f, 24.52f, 0.00f},
    {317.20f, -4694.22f, 16.78f, 0.00f},
    {326.30f, -4693.24f, 34.59f, 0.00f},
    {281.18f, -4705.37f, 22.38f, 0.00f},
    {293.32f, -4773.45f, 25.03f, 0.00f},
    {280.17f, -4831.90f, 22.25f, 0.00f},
    {319.04f, -4770.23f, 31.47f, 0.00f},
    {362.50f, -4676.11f, 28.63f, 0.00f},
    {348.71f, -4805.08f, 32.23f, 0.00f},
    {342.88f, -4837.07f, 26.29f, 0.00f},
    {361.80f, -4769.27f, 18.49f, 0.00f},
    {317.837f,-4734.06f,9.76272f,0.0f}, // Center of Town
};

enum HalloweenFireEvents
{
    EVENT_FIRE_NONE,
    EVENT_FIRE_HIT_BY_BUCKET,
    EVENT_FIRE_VISUAL_WATER,
    EVENT_FIRE_GROW_FIRE,
    EVENT_HORSEMAN_CONFLAGRATION,
    EVENT_HORSEMAN_CLEAVE,
    EVENT_HORSEMAN_LAUGHS,
    EVENT_FIRE_FINISH,
    EVENT_FIRE_FAIL,
};

class spell_halloween_wand : public SpellScriptLoader
{
public:
    spell_halloween_wand() : SpellScriptLoader("spell_halloween_wand") {}

    class spell_halloween_wand_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_halloween_wand_SpellScript)

        bool Validate(SpellInfo const* /*spellEntry*/)
        {
            if (!sSpellMgr->GetSpellInfo(SPELL_PIRATE_COSTUME_MALE))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_PIRATE_COSTUME_FEMALE))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_NINJA_COSTUME_MALE))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_NINJA_COSTUME_FEMALE))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_LEPER_GNOME_COSTUME_MALE))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_LEPER_GNOME_COSTUME_FEMALE))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_GHOST_COSTUME_MALE))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_GHOST_COSTUME_FEMALE))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_HALLOWEEN_WAND_PIRATE))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_HALLOWEEN_WAND_NINJA))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_HALLOWEEN_WAND_LEPER_GNOME))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_HALLOWEEN_WAND_RANDOM))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_HALLOWEEN_WAND_SKELETON))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_HALLOWEEN_WAND_WISP))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_HALLOWEEN_WAND_GHOST))
                return false;
            if (!sSpellMgr->GetSpellInfo(SPELL_HALLOWEEN_WAND_BAT))
                return false;
            return true;
        }

        void HandleScriptEffect()
        {
            Unit* caster = GetCaster();
            Unit* target = GetHitPlayer();

            if (!caster || !target)
                return;

            uint32 spellId = 0;
            uint8 gender = target->getGender();

            switch (GetSpellInfo()->Id)
            {
                case SPELL_HALLOWEEN_WAND_LEPER_GNOME:
                    spellId = gender ? SPELL_LEPER_GNOME_COSTUME_FEMALE : SPELL_LEPER_GNOME_COSTUME_MALE;
                    break;
                case SPELL_HALLOWEEN_WAND_PIRATE:
                    spellId = gender ? SPELL_PIRATE_COSTUME_FEMALE : SPELL_PIRATE_COSTUME_MALE;
                    break;
                case SPELL_HALLOWEEN_WAND_GHOST:
                    spellId = gender ? SPELL_GHOST_COSTUME_FEMALE : SPELL_GHOST_COSTUME_MALE;
                    break;
                case SPELL_HALLOWEEN_WAND_NINJA:
                    spellId = gender ? SPELL_NINJA_COSTUME_FEMALE : SPELL_NINJA_COSTUME_MALE;
                    break;
                case SPELL_HALLOWEEN_WAND_RANDOM:
                    spellId = RAND(SPELL_HALLOWEEN_WAND_PIRATE, SPELL_HALLOWEEN_WAND_NINJA, SPELL_HALLOWEEN_WAND_LEPER_GNOME, SPELL_HALLOWEEN_WAND_SKELETON, SPELL_HALLOWEEN_WAND_WISP, SPELL_HALLOWEEN_WAND_GHOST, SPELL_HALLOWEEN_WAND_BAT);
                    break;
            }
            caster->CastSpell(target, spellId, true);
        }

        void Register()
        {
            AfterHit += SpellHitFn(spell_halloween_wand_SpellScript::HandleScriptEffect);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_halloween_wand_SpellScript();
    }
};

class spell_toss_stinky_bomb : public SpellScriptLoader
{
public:
    spell_toss_stinky_bomb() : SpellScriptLoader("spell_toss_stinky_bomb") {}

    class spell_toss_stinky_bomb_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_toss_stinky_bomb_SpellScript)

        void HandleScriptEffect(SpellEffIndex effIndex)
        {
            Unit* caster = GetCaster();

            if (caster && caster->GetTypeId() == TYPEID_PLAYER)
                caster->ToPlayer()->KilledMonsterCredit(NPC_STINKY_BOMB_CREDIT, 0);
        }

        void Register()
        {
            OnEffectHit += SpellEffectFn(spell_toss_stinky_bomb_SpellScript::HandleScriptEffect, EFFECT_2, SPELL_EFFECT_SEND_EVENT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_toss_stinky_bomb_SpellScript();
    }
};

class spell_clean_stinky_bomb : public SpellScriptLoader
{
public:
    spell_clean_stinky_bomb() : SpellScriptLoader("spell_clean_stinky_bomb") {}

    class spell_clean_stinky_bomb_SpellScript : public SpellScript
    {
        PrepareSpellScript(spell_clean_stinky_bomb_SpellScript)

        SpellCastResult CheckIfNearBomb()
        {
            Unit* caster = GetCaster();

            if(GameObject* stinky = GetClosestGameObjectWithEntry(caster, GO_STINKY_BOMB_CLOUD, 15.0f))
                return SPELL_CAST_OK;
            else
                return SPELL_FAILED_CANT_DO_THAT_RIGHT_NOW;
        }

        void HandleCleanBombEffect(SpellEffIndex effIndex)
        {
            Unit* caster = GetCaster();

            if (GameObject* stinky = GetClosestGameObjectWithEntry(caster, GO_STINKY_BOMB_CLOUD, 15.0f))
                stinky->RemoveFromWorld();
        }

        void Register()
        {
            OnCheckCast += SpellCheckCastFn(spell_clean_stinky_bomb_SpellScript::CheckIfNearBomb);
            OnEffectHit += SpellEffectFn(spell_clean_stinky_bomb_SpellScript::HandleCleanBombEffect, EFFECT_1, SPELL_EFFECT_ACTIVATE_OBJECT);
        }
    };

    SpellScript* GetSpellScript() const
    {
        return new spell_clean_stinky_bomb_SpellScript();
    }
};

class at_wickerman_festival : public AreaTriggerScript
{
    public:
        at_wickerman_festival() : AreaTriggerScript("at_wickerman_festival") {}

        bool OnTrigger(Player* player, AreaTriggerEntry const* /*trigger*/)
        {
            player->GroupEventHappens(QUEST_CRASHING_WICKERMAN_FESTIVAL, player);
            return true;
        }
};

#define GOSSIP_WICKERMAN_EMBER "Usar las cenizas como pintura de guerra para la cara" //"Smear the ash on my face like war paint!" 

class go_wickerman_ember : public GameObjectScript
{
public:
    go_wickerman_ember() : GameObjectScript("go_wickerman_ember") { }

    bool OnGossipHello(Player* player, GameObject* go)
    {
        if (!player->HasAura(SPELL_GRIM_VISAGE))
            player->ADD_GOSSIP_ITEM(GOSSIP_ICON_CHAT, GOSSIP_WICKERMAN_EMBER, GOSSIP_SENDER_MAIN, GOSSIP_ACTION_INFO_DEF);
        player->SEND_GOSSIP_MENU(player->GetGossipTextId(go), go->GetGUID());
        return true;
    }

    bool OnGossipSelect(Player* player, GameObject* go, uint32 /*sender*/, uint32 action)
    {
        if (action == GOSSIP_ACTION_INFO_DEF)
            go->CastSpell(player, SPELL_GRIM_VISAGE);

        player->PlayerTalkClass->ClearMenus();
        player->CLOSE_GOSSIP_MENU();
        return true;
    }
};

class item_water_bucket : public ItemScript
{
    public:

        item_water_bucket() : ItemScript("item_water_bucket") { }

        bool OnUse(Player* player, Item* item, SpellCastTargets const& targets)
        {
            if (Creature* dummy = player->SummonCreature(NPC_FIRE_DUMMY, targets.GetDst()->GetPositionX(), targets.GetDst()->GetPositionY(), targets.GetDst()->GetPositionZ(), 0.0f, TEMPSUMMON_TIMED_DESPAWN, 500))
            {
                std::list<Creature*> firesList;
                Trinity::AllCreaturesOfEntryInRange checker(dummy, NPC_HEADLESS_FIRE, 3.0f);
                Trinity::CreatureListSearcher<Trinity::AllCreaturesOfEntryInRange> searcher(dummy, firesList, checker);
                player->VisitNearbyObject(3.0f, searcher);

                if (firesList.empty())
                {
                    // Just some extra checks...
                    Creature* fire = dummy->FindNearestCreature(NPC_HEADLESS_FIRE, 3.0f, true);
                    if (fire && fire->isAlive())
                        fire->AI()->SetGUID(player->GetGUID(), EVENT_FIRE_HIT_BY_BUCKET);
                    else if (Player* friendPlr = dummy->SelectNearestPlayer(3.0f))
                    {
                        if (friendPlr->IsFriendlyTo(player) && friendPlr->isAlive())
                            player->CastSpell(friendPlr, SPELL_CREATE_WATER_BUCKET, true);
                    }
                    else
                        return false;
                }

                for (std::list<Creature*>::const_iterator i = firesList.begin(); i != firesList.end(); ++i)
                    if ((*i) && (*i)->isAlive())
                        (*i)->AI()->SetGUID(player->GetGUID(), EVENT_FIRE_HIT_BY_BUCKET);
            }
            return false;
        }
};

class npc_halloween_fire : public CreatureScript
{
public:
    npc_halloween_fire() : CreatureScript("npc_halloween_fire") { }


    struct npc_halloween_fireAI : public ScriptedAI
    {
        npc_halloween_fireAI(Creature* c) : ScriptedAI(c) {}

        bool fireEffigy;
        bool off;
        EventMap events;
        uint64 _playerGUID;

        void Reset()
        {
            off = false;
            fireEffigy = false;
            _playerGUID = 0;
            events.Reset();
            // Mark the npc if is for handling effigy instead of horseman fires
            if(GameObject* effigy = me->FindNearestGameObject(GO_FIRE_EFFIGY, 0.5f))
                fireEffigy = true;
            me->CastSpell(me, SPELL_FIRE_STARTING_SIZE, true);
            events.ScheduleEvent(EVENT_FIRE_GROW_FIRE, 1000);
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            switch(events.ExecuteEvent())
            {
                case EVENT_FIRE_VISUAL_WATER:
                    me->CastSpell(me, SPELL_WATER_SPOUT_VISUAL, true);
                    if (fireEffigy)
                    {
                        if (GameObject* effigy = me->FindNearestGameObject(GO_FIRE_EFFIGY, 0.5f))
                        {
                            effigy->SetGoState(GO_STATE_READY);
                            if (Player* player = me->GetPlayer(*me, _playerGUID))
                                player->KilledMonsterCredit(me->GetEntry(),0);
                            events.ScheduleEvent(EVENT_FIRE_GROW_FIRE, 22000);
                        }
                    } else {
                        if (Aura* fireSize = me->GetAura(SPELL_FIRE_SIZE_STACK))
                        {
                            if (fireSize->GetStackAmount() < 10)
                            {
                                me->RemoveAura(fireSize);
                                me->RemoveAurasDueToSpell(SPELL_FIRE_VISUAL_BUFF);
                                me->DespawnOrUnsummon(1000);
                            } else
                                fireSize->ModStackAmount(-10);
                        }
                    }
                    break;
                case EVENT_FIRE_GROW_FIRE:
                    if (fireEffigy)
                    {
                        if (GameObject* effigy = me->FindNearestGameObject(GO_FIRE_EFFIGY, 0.5f))
                            effigy->SetGoState(GO_STATE_ACTIVE);
                    } else {
                        if (off) break; // This fire have been extinguished

                        if (Aura* fireSize = me->GetAura(SPELL_FIRE_SIZE_STACK)) // This fire have maxium size
                            if(fireSize->GetStackAmount() == 255) break;

                        if (!me->HasAura(SPELL_FIRE_STARTING_SIZE))
                            me->CastSpell(me, SPELL_FIRE_STARTING_SIZE, true);
                        if (!me->HasAura(SPELL_FIRE_VISUAL_BUFF))
                            me->CastSpell(me, SPELL_FIRE_VISUAL_BUFF, true);
                        me->CastSpell(me, SPELL_FIRE_SIZE_STACK, true);
                        events.ScheduleEvent(EVENT_FIRE_GROW_FIRE, urand(1000,2500));
                    }
                    break;
            }
        }

        void SetGUID(uint64 guid, int32 id)
        {
            if (off) return;

            if (id == EVENT_FIRE_HIT_BY_BUCKET)
            {
                _playerGUID = guid;
                if (fireEffigy)
                {
                    if (GameObject* effigy = me->FindNearestGameObject(GO_FIRE_EFFIGY, 0.5f))
                        if (effigy->GetGoState() == GO_STATE_ACTIVE)
                            events.ScheduleEvent(EVENT_FIRE_VISUAL_WATER, 1000);
                } else
                {
                    if (Creature* horseman = me->GetCreature(*me, me->GetCreatorGUID()))
                        horseman->AI()->SetGUID(_playerGUID, EVENT_FIRE_HIT_BY_BUCKET);
                    events.ScheduleEvent(EVENT_FIRE_VISUAL_WATER, 1000);
                }
            } 
        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_halloween_fireAI(creature);
    }
};

/* This should be fixed ASAP, as far as I know, HHman should appear flying on villages and
start casting SPELL_FIRE_CREATE_NODE on their buildings, maybe also son zone warning, also need
to fix the quests, there are 2 aviable now, when only one should be depending if the village is
alreade setted on fire or nor.
*/
class npc_shade_horseman : public CreatureScript
{
public:
    npc_shade_horseman() : CreatureScript("npc_shade_horseman") { }


    struct npc_shade_horsemanAI : public ScriptedAI
    {
        npc_shade_horsemanAI(Creature* c) : ScriptedAI(c), fires(c) {}

        SummonList fires;
        EventMap events;
        bool moving;
        bool pointReached;
        bool allFiresSet;
        bool firesOut;
        uint32 wpCount;
        std::list<uint64> _playerList;

        void Reset()
        {
            moving = true;
            pointReached = true;
            allFiresSet = false;
            firesOut = false;
            wpCount = 0;
            _playerList.clear();
            events.Reset();

            me->Mount(25159);
            me->SetReactState(REACT_PASSIVE);
            me->SetFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_DISABLE_MOVE);
            me->AddUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT | MOVEMENTFLAG_LEVITATING);

            events.ScheduleEvent(EVENT_HORSEMAN_LAUGHS, urand(5000, 10000));
            events.ScheduleEvent(EVENT_HORSEMAN_CONFLAGRATION, urand(7000, 14000));
            events.ScheduleEvent(EVENT_FIRE_FAIL, 600000);
        }

        void JustSummoned(Creature* summon)
        {
            summon->SetCreatorGUID(me->GetGUID());
            fires.Summon(summon);
        }

        void SummonedCreatureDespawn(Creature* summon)
        {
            fires.Despawn(summon);
        }

        void JustDied(Unit* killer)
        {
            if (killer && killer->GetAreaId() == me->GetAreaId())
                killer->SummonGameObject(GO_LARGE_JACK_O_LANTERN, me->GetPositionX(), me->GetPositionY(), killer->GetPositionZ()+1.0f, me->GetOrientation(), 0.0f, 0.0f, 0.0f, 0.0f, 180000);
        }
        void MovementInform(uint32 type, uint32 id)
        {
            if (type != POINT_MOTION_TYPE)
                return;

            if (moving && id < FIRE_NODES_PER_AREA)
            {
                if (id == 0 && !allFiresSet)
                    Talk(SAY_HORSEMAN_SPAWN);

                if (!allFiresSet)
                    me->CastSpell(GetPositionsForArea()[wpCount].GetPositionX(), GetPositionsForArea()[wpCount].GetPositionY(), GetPositionsForArea()[wpCount].GetPositionZ(), SPELL_FIRE_CREATE_NODE, true);

                if (id+1 == FIRE_NODES_PER_AREA)
                {
                    allFiresSet = true;
                    wpCount = 0;
                }
                else
                    ++wpCount;

                pointReached = true;
            } else if (id == FIRE_NODES_PER_AREA && firesOut)
            {
                me->Unmount();
                me->SetReactState(REACT_AGGRESSIVE);
                me->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_OOC_NOT_ATTACKABLE | UNIT_FLAG_DISABLE_MOVE);
                me->RemoveUnitMovementFlag(MOVEMENTFLAG_ONTRANSPORT | MOVEMENTFLAG_LEVITATING);
                events.ScheduleEvent(EVENT_HORSEMAN_CLEAVE, urand(5000, 10000));
            }
        }

        void UpdateAI(const uint32 diff)
        {
            events.Update(diff);

            if (!firesOut)
            {
                if (pointReached)
                {
                    pointReached = false;
                    me->GetMotionMaster()->MovePoint(wpCount, GetPositionsForArea()[wpCount].GetPositionX(), GetPositionsForArea()[wpCount].GetPositionY(), GetZForArea());
                }

                if (allFiresSet && fires.size() <= (uint32)(FIRE_NODES_PER_AREA*0.3f) && wpCount != FIRE_NODES_PER_AREA+1)
                {
                    Talk(SAY_HORSEMAN_FIRES_OUT);
                    wpCount = FIRE_NODES_PER_AREA;
                    me->GetMotionMaster()->MoveIdle();
                    me->GetMotionMaster()->MovePoint(FIRE_NODES_PER_AREA, GetPositionsForArea()[wpCount].GetPositionX(), GetPositionsForArea()[wpCount].GetPositionY(), GetPositionsForArea()[wpCount].GetPositionZ());
                    fires.DespawnAll();
                    firesOut = true;
                    return;
                }

                switch(events.ExecuteEvent())
                {
                    case EVENT_FIRE_FAIL:
                        fires.DespawnAll();
                        Talk(SAY_HORSEMAN_FIRES_FAIL);
                        wpCount = FIRE_NODES_PER_AREA+1;
                        me->GetMotionMaster()->MovePoint(wpCount, GetPositionsForArea()[wpCount].GetPositionX(), GetPositionsForArea()[wpCount].GetPositionY(), GetZForArea());
                        if (!_playerList.empty())
                        {
                            for (std::list<uint64>::const_iterator i = _playerList.begin();i != _playerList.end(); ++i)
                            {
                                Player* player = me->GetPlayer(*me, *i);
                                if (player)
                                    player->FailQuest(player->GetTeam() == ALLIANCE ? QUEST_LET_THE_FIRES_COME_A : QUEST_LET_THE_FIRES_COME_H);
                            }
                        }
                        me->DespawnOrUnsummon(10000);
                        break;
                    case EVENT_HORSEMAN_CONFLAGRATION:
                        if (!_playerList.empty())
                        {
                            for (std::list<uint64>::const_iterator i = _playerList.begin();i != _playerList.end(); ++i)
                            {
                                Player* player = me->GetPlayer(*me, *i);
                                if (player && player->GetAreaId() == me->GetAreaId() && player->GetDistance(me) <= 30.0f)
                                {
                                    me->CastSpell(player, RAND(SPELL_HORSEMAN_CONFLAGRATION_1, SPELL_HORSEMAN_CONFLAGRATION_2, SPELL_HORSEMAN_JACK_O_LANTERN), true);
                                    break;
                                }
                            }
                        }
                        events.ScheduleEvent(EVENT_HORSEMAN_CONFLAGRATION, urand(5000, 10000));
                        break;
                    case EVENT_HORSEMAN_LAUGHS:
                        Talk(SAY_HORSEMAN_LAUGHS);
                        events.ScheduleEvent(EVENT_HORSEMAN_LAUGHS, urand(10000, 25000));
                        break;
                }
            } else {
                switch(events.ExecuteEvent())
                {
                    case EVENT_HORSEMAN_CLEAVE:
                        if (Unit* victim = me->getVictim())
                            me->CastSpell(victim, SPELL_HORSEMAN_CLEAVE, true);
                        events.ScheduleEvent(EVENT_HORSEMAN_CLEAVE, urand(5000, 10000));
                        return;
                    case EVENT_HORSEMAN_CONFLAGRATION:
                        if (!_playerList.empty())
                        {
                            for (std::list<uint64>::const_iterator i = _playerList.begin();i != _playerList.end(); ++i)
                            {
                                Player* player = me->GetPlayer(*me, *i);
                                if (player && player->GetAreaId() == me->GetAreaId() && player->GetDistance(me) <= 30.0f)
                                {
                                    me->CastSpell(player, RAND(SPELL_HORSEMAN_CONFLAGRATION_1, SPELL_HORSEMAN_CONFLAGRATION_2, SPELL_HORSEMAN_JACK_O_LANTERN), true);
                                    break;
                                }
                            }
                        }
                        events.ScheduleEvent(EVENT_HORSEMAN_CONFLAGRATION, urand(5000, 10000));
                        break;
                }
                DoMeleeAttackIfReady();
            }
        }

        const Position* GetPositionsForArea()
        {
            switch (me->GetAreaId())
            {
                case 87: //GoldShire
                    return FireNodesGoldShire;
                case 362: // Razor Hill
                case 2337:
                    return FireNodesRazorHill;
            }
            return NULL;
        }

        float GetZForArea()
        {
            switch (me->GetAreaId())
            {
                case 87: //GoldShire
                    return 77.6f;
                case 362: // Razor Hill
                case 2337:
                    return 40.0f;
            }
            return 0.0f;
        }

        void SetGUID(uint64 guid, int32 id)
        {
            if (id == EVENT_FIRE_HIT_BY_BUCKET)
            {
                _playerList.push_back(guid);
                _playerList.unique();
            }

        }
    };

    CreatureAI* GetAI(Creature* creature) const
    {
        return new npc_shade_horsemanAI(creature);
    }
};

class npc_halloween_orphan_matron : public CreatureScript
{
public:
    npc_halloween_orphan_matron() : CreatureScript("npc_halloween_orphan_matron") { }

    uint64 _headlessHoresemanGUID;

    bool OnGossipHello(Player* player, Creature* me)
    {
        if (Creature* horseman = me->GetCreature(*me, _headlessHoresemanGUID))
        {
            QuestMenu &qm = player->PlayerTalkClass->GetQuestMenu();

            Quest const* quest = sObjectMgr->GetQuestTemplate(player->GetTeam() == ALLIANCE ? QUEST_STOP_FIRES_A : QUEST_STOP_FIRES_H);

            if (quest && player->CanTakeQuest(quest, false))
                qm.AddMenuItem(player->GetTeam() == ALLIANCE ? QUEST_STOP_FIRES_A : QUEST_STOP_FIRES_H, 2);
        }
        else
            player->PrepareQuestMenu(me->GetGUID());

        player->SEND_GOSSIP_MENU(player->GetGossipTextId(me), me->GetGUID());
        return true;
    }

    bool OnQuestAccept(Player* /*player*/, Creature* me, Quest const* quest)
    {
        if (!(me->GetAreaId() == 87 || me->GetAreaId() == 362))
            return true;

        if (quest->GetQuestId() == QUEST_LET_THE_FIRES_COME_A || quest->GetQuestId() == QUEST_LET_THE_FIRES_COME_H)
        {
            Creature* horseman = me->GetCreature(*me, _headlessHoresemanGUID);

            if (!horseman)
            {
                if (Creature* horseman = me->SummonCreature(NPC_SHADE_HORSEMAN, me->GetPositionX(), me->GetPositionY(), me->GetPositionZ() + 20.0f, 0, TEMPSUMMON_DEAD_DESPAWN))
                    _headlessHoresemanGUID = horseman->GetGUID();
            }
        }
        return true;
    }
};

void AddSC_custom_fixes()
{
    new go_not_a_bug;
    new item_alumeths_remains;
    new npc_q13355_q13320_trigger;
    new npc_brokendown_shredders;
    new npc_shredders_taker;
    new item_writhing_mass;
    new npc_rocket_warhead;
    new npc_father_kamaros;
    new npc_spring_rabbit;
    new npc_decomposing_ghoul();
    new npc_irulon_trueblade();
    new npc_banner_q11429();
    new go_ancient_skull_pile();
    new npc_ancient_vrykul();
    new at_ymiron_house();
    new npc_lich_king_hfjord();
    new npc_iron_rune_construct();
    new npc_lebronski();
    new go_iron_rune_construct_workbank();
    new npc_terokk();
    new spell_fumping_39238();
    new spell_fumping_39246();
    new npc_signal_fire();
    new npc_keristrasza_coldarra();
    new npc_evergrove_druid();
    new npc_antelarion_gossip();
    new npc_woodlands_walker();
    new go_lab_work_reagents();
    new spell_gen_ribbon_pole_dancer_check();
    new npc_torch_tossing_bunny();
    new spell_gen_torch_target_picker();
    new spell_gen_juggle_torch_catch();
    new boss_ahune();
    new mob_frozen_core();
    new mob_ahunite_hailstone();
    new mob_ahunite_coldwave();
    new mob_ahunite_frostwind();
    new go_ice_stone_midsummer();
    new at_nats_landing();
    new at_legion_hold_smvalley();
    new npc_feknut_bunny();
    new at_azure_dragons_sanctuary();
    new npc_ele_power_extractor();
    new npc_q4506_cat();
    new spell_song_of_cleansing();
    new spell_contact_brann();
    new npc_moodle();
    new spell_construct_barricade();
    new npc_dragonforged_blades_giver();
    new npc_dark_iron_guzzler();
    new at_brewfest();
    new go_mistwhisper_treasure();
    new spell_toss_stinky_bomb();
    new spell_clean_stinky_bomb();
    new at_wickerman_festival();
    new spell_halloween_wand();
    new go_wickerman_ember();
    new item_water_bucket();
    new npc_halloween_fire();
    new npc_shade_horseman();
    new npc_halloween_orphan_matron();
}
