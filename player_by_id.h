#ifndef PLAYER_H
#define PLAYER_H


#include "group.h"

class Player_by_id
        {
private:
    Group* my_group; // this is the pointer to the group of the player_by_id
    int player_id;
    int player_level;
    int group_id;
public:
    Player_by_id(int playerID,int playerLevel,int groupId,Group* my): my_group(my), player_id(playerID),player_level(playerLevel),
    group_id(groupId){}
    ~Player_by_id() = default;
    Player_by_id()=default;
    void change_group(Group* new_group)
    {
        my_group=new_group;
    }
    int get_player_level()
    {
        return player_level;
    }
    Group* get_the_group()
    {
        return my_group;
    }
    int get_the_group_id()
    {
        return group_id;
    }
    void addToPlayerLvl(int extra)
    {
        player_level += extra;
    }
    int get_the_player_id()
    {
        return player_id;
    }
    void change_groupID(int group)
    {
        group_id=group;
    }
};

#endif

