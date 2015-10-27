#include "Skill.h"
#include "Soldier.h"
#include "NetRoom.h"

extern Soldier *g_self;

Skill Skill::s_skill[NUMBER_SKILL] = {
    //blood,   magic,   delay, distance, angle, cool
    { -20.f,   0.f,     0.f,   70.f,     45.f,  3.f },            // Boxing
    { -30.f,   0.f,     0.f,   60.f,     45.f,  4.f },            // Kick
    { -50.f,   -15.f,   0.f,   50.f,     45.f,  5.f },            // Special
    { 0.f,     -10.f,   0.f,   0.f,      45.f,  8.f }             // Speed
};
