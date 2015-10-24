#ifndef __WOMANSOLDIER_H__
#define __WOMANSOLDIER_H__

#include "Soldier.h"

class WomanSoldier : public Soldier
{
public:
    CREATE_FUNC(WomanSoldier);
    bool init_soldier() override;
    //void action_boxing() override;

	void load_actions();
};

#endif // __WOMANSOLDIER_H__
