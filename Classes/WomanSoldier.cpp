#include "WomanSoldier.h"
#include "AppDelegate.h"

bool WomanSoldier::init_soldier() {

    string path = FileUtils::getInstance()->fullPathForFilename("girl/config");
    CC_ASSERT(load_config((char *)path.c_str()));

    _name = "Woman";
    _role_id = SOLDIER_TYPE_WOMAN;

    m_eff_death = "girl_death";
    m_eff_aida = "girl_aida";
    m_eff_boxing = "girl_boxing";
    m_eff_kick = "girl_kick";
    m_eff_special = "girl_special";

    //load_actions();

    return true;
}

void WomanSoldier::load_actions() {
    m_act_idle = Animate3D::createWithFrames(Animation3D::create("girl/animate_girl_idle.c3b"), 0, 500);
    m_act_walk = Animate3D::createWithFrames(Animation3D::create("girl/animate_girl_walk.c3b"), 0, 30);
    m_act_kick = Animate3D::createWithFrames(Animation3D::create("girl/animate_girl_kick.c3b"), 0, 119);
    m_act_boxing = Animate3D::createWithFrames(Animation3D::create("girl/animate_girl_boxing.c3b"), 0, 119);
    m_act_special = Animate3D::createWithFrames(Animation3D::create("girl/animate_girl_special.c3b"), 0, 80);
}
