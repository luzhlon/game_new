#include <string>
#include "cocos2d.h"
#include "../Server/message.h"
#include "Soldier.h"

USING_NS_CC;

struct Skill;

class GoodsBase;

namespace NetRoom {
    extern int         _self_id;                       // 自己的ID
    extern int         _master_id;                     // 房主ID
    extern bool        _game_over;                     // 游戏结束否
    extern std::string _room_name;                     // 房间名称
    extern room_member _members[MAX_ROOM_MEMBERS];     // 房间成员
    extern net_pkg     _pkg;
    extern Vec2        _house_pos;                     // 营地位置

    void init();
    void create_soldiers();                            // 创建玩家实例
    void register_handlers();                          // 注册消息处理器

    /*****************************      以下动作均在房间内进行      ****************************/
    void action_move(Vec2&);                           // 移动
    void action_stop();                                // 停止移动

    void set_state(Soldier::State state);
    void set_position(Vec2&);
    void set_angle(float angle);
    void set_blood(float blood);
    void set_speed(float speed);
    void set_grass(int count, int room_id = _self_id);
    void add_goods(GoodsBase *);
    void dec_goods(int index);

    int  get_team_grass(int room_id = _self_id);       // 获取队伍的Grass数量

    void declare_win(int team);                        // 宣布胜利

    int get_near_enemy(float distance = 50.f);         // 获得附近的敌人的ID，没有返回-1

    void do_skill(Skill::Type);                            // 执行某个技能
}