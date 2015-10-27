#include "Man2Soldier.h"

bool Man2Soldier::init_soldier() {
    //initWithFile("man2/man2.c3b");
    string path = FileUtils::getInstance()->fullPathForFilename("man2/config");
    CC_ASSERT(load_config((char *)path.c_str()));

    _name = "Man2";
    _role_id = SOLDIER_TYPE_MAN2;

    m_eff_death = "man2_death";
    m_eff_aida = "man2_aida";
    m_eff_boxing = "man2_boxing";
    m_eff_kick = "man2_kick";
    m_eff_special = "man2_special";

    return true;
}

void Man2Soldier::load_actions() {
    m_act_idle = Animate3D::createWithFrames(Animation3D::create("man2/animate_man2_idle.c3b"), 0, 1000);
    m_act_walk = Animate3D::createWithFrames(Animation3D::create("man2/animate_man2_walk.c3b"), 0, 30);
    m_act_kick = Animate3D::createWithFrames(Animation3D::create("man2/animate_man2_kick.c3b"), 0, 119);
    m_act_boxing = Animate3D::createWithFrames(Animation3D::create("man2/animate_man2_boxing.c3b"), 0, 87);
    m_act_special = Animate3D::createWithFrames(Animation3D::create("man2/animate_man2_special.c3b"), 0, 100);
}
