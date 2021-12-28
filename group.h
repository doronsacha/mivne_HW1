#ifndef HW1_GROUP_H
#define HW1_GROUP_H

#include <cassert>
#include <iostream>
#include <memory>
#include "player_in_group.h"
#include "key_by_lvl.h"
#include "avltree.h"
#include "not_empty_data.h"

using namespace std;

class Group
{
private:
    AVL<Key_by_lvl,Player_in_group>* avl_of_player_in_group;
    int max_player_id;
    int max_player_level;
public:
    Group(AVL<Key_by_lvl, Player_in_group>* avl_of_player, int player_id, int player_level)
    {
        avl_of_player_in_group= avl_of_player;
        max_player_id=player_id;
        max_player_level=player_level;
    }
    ~Group ()
    {
        avl_of_player_in_group->treeSpecialDataClear();
        delete avl_of_player_in_group;
    }
    Group()
    {
        max_player_id=NOT_READY;
        max_player_level=NOT_READY;
        avl_of_player_in_group= new AVL<Key_by_lvl,Player_in_group>();
    }

    void changePlayersTree(AVL<Key_by_lvl,Player_in_group>* newTree, int maxPlayerID, int maxLvlId)
    {
        avl_of_player_in_group = newTree;
        max_player_level = maxLvlId;
        max_player_id = maxPlayerID;
    }

    void add_player(int player_id, int player_level, Player_in_group player)
    {
        Key_by_lvl key=Key_by_lvl(player_level, player_id);
        if(player_level>max_player_level)
        {
            max_player_level=player_level;
            max_player_id=player_id;
        }
        if(player_level==max_player_level)
        {
            if(player_id<max_player_id)
            {
                max_player_level=player_level;
                max_player_id=player_id;
            }
        }
        avl_of_player_in_group->insert(key,player);
    }
    void change_avl(AVL<Key_by_lvl,Player_in_group>* new_avl)
    {
        avl_of_player_in_group=new_avl;
    }
    void change_max(int level, int player)
    {
        max_player_id=player;
        max_player_level=level;
    }

    void remove_player(Key_by_lvl key)
    {
        avl_of_player_in_group->remove(key);
    }
    int get_max_player()
    {
        return max_player_id;
    }


    bool is_empty()
    {

        if(avl_of_player_in_group->getRoot() == nullptr)
        {
            return true;
        }
        return false;
    }
    int get_max_level()
    {
        return max_player_level;
    }
        int getNumOfPlayersInGroup() const
    {
        return avl_of_player_in_group->getSize();
    }
    AVL<Key_by_lvl,Player_in_group>* get_avl()
    {
        return avl_of_player_in_group;
    }
};

#endif