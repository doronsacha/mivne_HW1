#ifndef HW1_KEY_BY_LVL_H
#define HW1_KEY_BY_LVL_H

#include <cassert>
#include <iostream>
#include <memory>
#include "player_by_id.h"

using namespace std;

class Key_by_lvl
{
private:
    int player_id;
    int level;
public:
    Key_by_lvl()=default;
    Key_by_lvl(int lvl, int player):player_id(player),level(lvl){};
    ~Key_by_lvl()=default;
    Key_by_lvl& operator =(const Key_by_lvl& other)=default;

    bool operator <= (Key_by_lvl key) const
    {
        if (level<=key.level)
        {
            return true;
        }
        if(level==key.level)
        {
            if(player_id >= key.player_id)
            {
                return true;
            }
        }
        return false;
    }
    bool operator < (Key_by_lvl key) const
    {
        if (level<key.level)
        {
            return true;
        }
        if(level==key.level)
        {
            if(player_id> key.player_id)
            {
                return true;
            }
        }
        return false;
    }

    bool operator == (Key_by_lvl key) const
    {
        if(level==key.level)
        {
            return true;
        }
    }
    int getLevel() const
    {
        return level;
    }
    int getID() const
    {
        return player_id;
    }
};

#endif