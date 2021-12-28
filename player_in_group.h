#ifndef HW1_PLAYER_IN_GROUP_H
#define HW1_PLAYER_IN_GROUP_H
#include "player_by_id.h"
#include "group.h"

class Player_by_id;

class Player_in_group
{
public:
    Player_in_group(int player_id, Player_by_id* player):player_id(player_id), player(player){};
    Player_in_group()=default;
    ~Player_in_group() = default;
    Player_in_group& operator=(const Player_in_group& other)
    {
        player = other.player;
        player_id = other.player_id;
        return *this;
    }
    int player_id;
    Player_by_id* player;
};
#endif //HW1_PLAYER_IN_GROUP_H


