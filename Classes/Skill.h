#ifndef __SKILL_H__
#define __SKILL_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

USING_NS_CC;
using namespace ui;

struct Skill {
    enum Type {
        SKILL_BOXING = 0,
        SKILL_KICK,
        SKILL_SPECIAL,
        SKILL_SPEED,

        NUMBER_SKILL
    };

    static Skill s_skill[NUMBER_SKILL];
    inline
        static
        const Skill&
        get_skill(Skill::Type skill) {
            return s_skill[skill];
        }

    Skill(float blood, float magic, float delay_time,
        float distance, float angle, float cool_time)
        :_blood(blood), _magic(magic), _delay_time(delay_time),
        _distance(distance), _angle(angle), _cool_time(cool_time) { }

    //Type    _type = (Type)(-1);
    float   _blood = 0.f;            // 对血量的影响
    float   _magic = 0.f;            // 魔力值消耗
    float   _delay_time = 0.f;       // 伤害延时
    float   _distance = 0.f;         // 造成伤害的距离范围
    float   _angle = 45.f;           // 造成伤害的角度范围
    float   _cool_time = 0.f;        // 冷却时间
};

#endif /* __SKILL_H__ */