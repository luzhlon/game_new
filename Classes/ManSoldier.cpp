#include "ManSoldier.h"

bool ManSoldier::init_soldier() {
    initWithFile("man/man.c3b");

    _name = "Man";
    _role_id = SOLDIER_TYPE_MAN;

    string path = FileUtils::getInstance()->fullPathForFilename("man/config");
    CC_ASSERT(load_config((char *)path.c_str()));
    //load_actions();
    m_eff_death = "man_death";
    m_eff_aida = "man_aida";
    m_eff_boxing = "man_boxing";
    m_eff_kick = "man_kick";
    m_eff_special = "man_special";

    return true;
}

void ManSoldier::load_actions() {
    m_act_idle = Animate3D::createWithFrames(Animation3D::create("man/animate_man_idle.c3b"), 0, 1000);
    m_act_walk = Animate3D::createWithFrames(Animation3D::create("man/animate_man_walk.c3b"), 0, 30);
    m_act_kick = Animate3D::createWithFrames(Animation3D::create("man/animate_man_kick.c3b"), 0, 119);
    m_act_boxing = Animate3D::createWithFrames(Animation3D::create("man/animate_man_boxing.c3b"), 0, 87);
    m_act_special = Animate3D::createWithFrames(Animation3D::create("man/animate_man_special.c3b"), 0, 100);
}
