#include "NetRoom.h"
#include "GameScene.h"
#include "World.h"
#include "Dialog.h"
#include "Skill.h"
#include "Player.h"
#include "Client.h"
#include "Soldier.h"

using namespace NetRoom;

extern World *g_world;
extern Player *g_player;
extern Client *g_client;
extern Soldier *g_soldiers[MAX_ROOM_MEMBERS];

namespace NetRoom {
    int         _self_id = -1;
    int         _master_id = -1;
    bool        _game_over = false;
    std::string _room_name;
    room_member _members[MAX_ROOM_MEMBERS];
    net_pkg     _pkg;
    Vec2        _house_pos;
}

void NetRoom::init() {
    _game_over = false;

    register_handlers();
    create_soldiers();

    g_world->on_gen_goods([](GoodsBase *goods) {
        add_goods(goods);
    });
}

void NetRoom::register_handlers() {
    HANDLER(update_state) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->switch_state((Soldier::State)pkg->arg2);
    });
    HANDLER(update_blood) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->set_blood(*(float *)pkg->data);
    });
    HANDLER(update_speed) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->speed(*(float *)pkg->data);
    });
    HANDLER(update_angle) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->angle(*(float *)pkg->data);
    });
    HANDLER(update_position) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        auto v2 = (Vec2 *)(pkg->data);

        g_world->set_position(sol, *v2);

        if(pkg->arg1 == _self_id) 
            GameScene::set_small_direction(sol->angle()); // 小地图方向箭头
    });
    HANDLER(action_move) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->move_to(*(Vec2 *)pkg->data);

        if(pkg->arg1 == _self_id) 
            GameScene::set_small_direction(sol->angle()); // 小地图方向箭头
    });
    HANDLER(action_stop) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        sol->move_stop();
    });
    HANDLER(do_skill) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        Skill::Type skill = (Skill::Type)pkg->arg2;
        sol->do_skill(skill);
    });
    HANDLER(on_skill) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1]; //Soldier be attacked
        Skill::Type skill = (Skill::Type)pkg->arg2;

        if (sol->death()) return;

        if (pkg->arg1 == _self_id) {
            g_player->on_skill(skill);
        } else {
            sol->on_skill(skill);
            sol->show_blood_decline(Skill::get_skill(skill)._blood);
        }
    });
    HANDLER(update_grass) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1]; 
        if (pkg->arg1 == _self_id) g_player->set_grass(pkg->arg2);
        else  sol->grass(pkg->arg2);
        // 统计两队Grass
        int score_red = 0,
            score_blue = 0;
        score_red = NetRoom::get_team_grass(0);
        score_blue = NetRoom::get_team_grass(1);

        GameScene::set_score_red(score_red);
        GameScene::set_score_blue(score_blue);
        //判断获胜
        int self_score = NetRoom::get_team_grass();
        if (self_score <= 3) {
            NetRoom::declare_win(NetRoom::_self_id + 1);
            return;
        }
        if (self_score >= 100) {
            NetRoom::declare_win(NetRoom::_self_id);
            return;
        }
    });
    HANDLER(add_goods) = Client::handler([](net_pkg *pkg) {
        g_world->add_goods((GoodsBase *)pkg->data);
    });
    HANDLER(dec_goods) = Client::handler([](net_pkg *pkg) {
        g_world->dec_goods(pkg->arg1);
    });

    HANDLER(set_master) = Client::handler([](net_pkg *pkg) {
        _master_id = pkg->arg1;
        // 房主负责在世界里产生物品
        if (_master_id == _self_id) {
            g_world->begin_gen_goods();
        }
    });

    HANDLER(quit_room) = Client::handler([](net_pkg *pkg) {
        auto sol = g_soldiers[pkg->arg1];
        if (sol) sol->removeFromParent();

        g_soldiers[pkg->arg1] = nullptr;
    });

    HANDLER(game_over) = Client::handler([](net_pkg *pkg) {
        static bool end = false; // 游戏结束
        if (end) return;
        end = true;
        Dialog::getInstance()->setCallback([](Dialog *dlg, bool ok) {
            //Director::getInstance()->popScene();
            Director::getInstance()->end();
        });
        Dialog::getInstance()->Popup_t(GameScene::Instance, "Game Over",
                                            pkg->arg1 ? "蓝方胜" : "红方胜");
    });
}

void NetRoom::create_soldiers() {
    Vec2 base_pos_red(246.f, 484.f);
    Vec2 base_pos_blue(103.f, -503.f);

    for (int i = 0; i < MAX_ROOM_MEMBERS; i++) {
        room_member *meb = &NetRoom::_members[i];
        if (meb->is_empty()) {
            g_soldiers[i] = nullptr;
            continue;
        }
        g_soldiers[i] = Soldier::create(meb); // 创建
        g_soldiers[i]->name_text()->setString(meb->m_name); // 玩家名称
        g_soldiers[i]->name_text()->setColor( i % 2 ?
                                                Color3B(0, 0, 255) :  // 蓝队
                                                Color3B(255, 0, 0));  // 红队
        g_world->add_thing(g_soldiers[i]); // 添加到世界中
        g_world->set_position(g_soldiers[i], i % 2 ?
                                            base_pos_blue :
                                            base_pos_red);
        // 房主负责在世界里产生物品
        if (_master_id == _self_id) {
            g_world->begin_gen_goods();
        }
    }

    if (_self_id % 2) {  // 蓝队
        _house_pos = base_pos_blue;
    } else { // 红队
        _house_pos = base_pos_red;
    }

    auto self = g_soldiers[NetRoom::_self_id];
    g_player = Player::getInstance(self); // 初始化Player
    g_world->addChild(g_player); // 加到景中才能触发定时器
    set_grass(self->grass());
}

void NetRoom::action_move(Vec2& pos) {
    if (g_soldiers[_self_id]->death()) return;

    _pkg.msg = MESSAGE::action_move;
    _pkg.arg1 = _self_id;
    memcpy(_pkg.data, &pos, sizeof(pos));
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(pos));
}

void NetRoom::action_stop() {
    g_client->sendMsg(MESSAGE::action_stop, _self_id);
}

void NetRoom::set_angle(float angle) {
    _pkg.msg = MESSAGE::update_angle;
    _pkg.arg1 = _self_id;
    *(float *)_pkg.data = angle;
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(float));
}

void NetRoom::set_blood(float blood) {
    _pkg.msg = MESSAGE::update_blood;
    _pkg.arg1 = _self_id;
    *(float *)_pkg.data = blood;
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(blood));
}

void NetRoom::set_speed(float speed) {
    _pkg.msg = MESSAGE::update_blood;
    _pkg.arg1 = _self_id;
    *(float *)_pkg.data = speed;
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(speed));
}

void NetRoom::set_position(Vec2& pos) {
    _pkg.msg = MESSAGE::update_position;
    _pkg.arg1 = _self_id;
    memcpy(_pkg.data, &pos, sizeof(pos));
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(pos));
}

void NetRoom::do_skill(Skill::Type skill) {
    _pkg.msg = MESSAGE::do_skill;
    _pkg.arg1 = _self_id;
    _pkg.arg2 = skill;
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg));

    _pkg.msg = MESSAGE::on_skill;
    //从对方队伍里寻找离自己近的人
    for (int i = (_self_id % 2 + 1) % 2; i < MAX_ROOM_MEMBERS; i += 2) {
        auto s = g_soldiers[i];
        if (!s) continue;
        if (s->member()->is_empty()) continue;

        if (s->death()) continue; // 对方已经死亡

        auto self = g_soldiers[_self_id];
        auto delta3 = s->getPosition3D() - self->getPosition3D();
        Vec2 delta(delta3.x, delta3.z);

        auto distance = delta.length(); // 相差距离
        auto angle = CC_RADIANS_TO_DEGREES(delta.getAngle()) - self->angle(); 
        angle = fabs(angle); // 相差角度

        if (distance <= Skill::get_skill(skill)._distance &&
            angle <= Skill::get_skill(skill)._angle) { //在技能的攻击范围内
            _pkg.arg1 = i;
            _pkg.arg2 = skill;
            g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)); // 发动攻击
        }
    }
}

void NetRoom::set_state(Soldier::State state) {
    _pkg.msg = MESSAGE::update_state;
    _pkg.arg1 = _self_id;
    _pkg.arg2 = state;
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg));
}

void NetRoom::set_grass(int count, int room_id) {
    _pkg.msg = MESSAGE::update_grass;
    _pkg.arg1 = room_id;
    _pkg.arg2 = count;
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg));
}

void NetRoom::add_goods(GoodsBase *good) {
    _pkg.msg = MESSAGE::add_goods;
    _pkg.arg1 = _self_id;
    memcpy(_pkg.data, good, sizeof(GoodsBase));
    g_client->sendMsg(&_pkg, sizeof(mini_net_pkg)+sizeof(GoodsBase));
}

void NetRoom::dec_goods(int index) {
    g_client->sendMsg(MESSAGE::dec_goods, index);
}

int NetRoom::get_near_enemy(float distance) {
    auto self = g_soldiers[_self_id];
    //从对方队伍里寻找离自己近的人
    for (int i = (_self_id % 2 + 1) % 2;
        i < MAX_ROOM_MEMBERS; i += 2) {
        auto s = g_soldiers[i];
        if (!s) continue;

        //if (s->member()->is_empty()) continue;
        if (s->death()) continue; // 对方已经死亡

        auto delta3 = s->getPosition3D() - self->getPosition3D();
        Vec2 delta(delta3.x, delta3.z);

        if(delta.length() < distance) return i; // 相差距离
    }
    // 给定距离内没有敌人
    return -1;
}

int NetRoom::get_team_grass(int room_id) {
    int score = 0;
    for (int i = room_id % 2; i < MAX_ROOM_MEMBERS; i += 2) {
        auto sol = g_soldiers[i];
        if (sol) score += sol->grass();
    }
    return score;
}

void NetRoom::declare_win(int team) {
    g_client->sendMsg(MESSAGE::game_over, team % 2);
}
