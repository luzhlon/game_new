#ifndef __MAN2SOLDIER_H__
#define __MAN2SOLDIER_H__

#include "Soldier.h"

class Man2Soldier : public Soldier {
public:
    CREATE_FUNC(Man2Soldier);
    bool init_soldier() override;

    void load_actions();
};

#endif // __MAN2SOLDIER_H__
