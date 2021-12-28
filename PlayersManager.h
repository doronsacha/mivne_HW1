#ifndef HW1_DS_H
#define HW1_DS_H
#define NOT_READY -1
#include "library1.h"
#include "player_by_id.h"
#include "key_by_lvl.h"
#include "group.h"
#include "avltree.h"
#include "not_empty_data.h"
#include "player_in_group.h"

class PlayersManager
        {
    AVL<int, Group*> *avl_of_groups;
    AVL<int, Player_by_id*> *avl_of_player_by_id;
    AVL<int, Not_empty_data> *avl_of_no_empty_groups;
    AVL<Key_by_lvl, int> *avl_of_player_by_lvl;
    int max_player_id;
    int max_player_level;
public:
    PlayersManager()
    {
        avl_of_player_by_lvl = new AVL<Key_by_lvl, int>();
        avl_of_groups = new AVL<int, Group *>();
        avl_of_player_by_id = new AVL<int, Player_by_id*>();
        avl_of_no_empty_groups = new AVL<int, Not_empty_data>();
        max_player_id = NOT_READY;
        max_player_level = NOT_READY;
    }

    ~PlayersManager()
    {
        Quit();
    }

    /// Complexity = (3*log(k)+C) ---->>> O(log(k))
    StatusType add_group(int group_id)
    {
        //checks the input
        if (group_id <= 0)
        {
            return INVALID_INPUT;
        }

        //checks the failure
        if (avl_of_groups->findNode(group_id) != nullptr) /// O(log(k))
        {
            return FAILURE;
        }

        //create a new group for the insertion in the avl of groups;
        Group* new_group = new Group();
        avl_of_groups->insert(group_id, new_group); /// O(log(k))
        if (avl_of_groups->findNode(group_id) == nullptr) /// O(log(k))
        {
            delete new_group;
            return ALLOCATION_ERROR;
        }

        //all good :-)
        return SUCCESS;
    }

    /// Complexity = 5log(k)+4log(n)+C --->>> O(log(k) + log(n))
    /*
     * add to avl of player by id: line 110
     * add to avl of player in group: 129
     * add to avl of non empty group: line 85
     * add to avl of player by level: line 105
     */
    StatusType add_player(int player_id, int group_id, int player_level)
    {
        //checking the input
        if (player_level < 0 || group_id < 0 || group_id == 0 || player_id <= 0)
        {
            return INVALID_INPUT;
        }

        //checks the failure
        if (avl_of_groups->findNode(group_id) == nullptr || avl_of_player_by_id->findNode(player_id) != nullptr) /// /// O(log(k))+ O(log(n))
        {
            return FAILURE;
        }

        //insert in non empty tree and update the max player(id and level)
        if (avl_of_no_empty_groups->findNode(group_id) == nullptr) /// O(log(k))
        {
            Not_empty_data data(player_id, player_level);
            avl_of_no_empty_groups->insert(group_id, data); /// O(log(k))
        }

        //changing the max player of the data structure if needed;
        if (player_level > max_player_level)
        {
            max_player_level = player_level;
            max_player_id = player_id;
        }
        if (player_level == max_player_level)
        {
            if (player_id < max_player_id)
            {
                max_player_level = player_level;
                max_player_id = player_id;
            }
        }

        //insert the new player to the avl by level
        Key_by_lvl key(player_level, player_id);
        avl_of_player_by_lvl->insert(key, player_id);///O(log(n))

        //insert the player in the player_by id tree
        Group *gr = avl_of_groups->findNode(group_id)->data; /// O(log(k))
        Player_by_id* new_player =new Player_by_id(player_id, player_level, group_id, gr);
        avl_of_player_by_id->insert(player_id,new_player); ///O(log(n))

        //updating the max player(level and id) in the avl of groups and in the avl of none empty groups
        if (gr->get_max_level() < player_level)
        {
            gr->change_max(player_level, player_id);
            avl_of_no_empty_groups->findNode(group_id)->data.update_player(player_id,player_level);/// O(log(k))
        }
        if (gr->get_max_level() == player_level)
        {
            if (gr->get_max_player() > player_id)
            {
                gr->change_max(player_level, player_id);
                avl_of_no_empty_groups->findNode(group_id)->data.update_player(player_id,player_level);
            }
        }

        //insert the new player in the avl of player in groups
        Player_in_group player(player_id,(avl_of_player_by_id->findNode(player_id)->data));
        gr->get_avl()->insert(key, player); ///O(log(n))

        //all good :-)
        return SUCCESS;
    }

    /// Complexity = 13log(n)+C --->>> O(log(n))
    /*
     * remove from:
     * avl of player by id: line 197
     * avl of player by level: line 164
     * avl of player in group: line 181
     * avl of none empty group(if needed): line 186
     */
    StatusType RemovePlayer(int PlayerID)
    {
        //checking the input
        if (PlayerID <= 0)
        {
            return INVALID_INPUT;
        }

        //checks the failure
        if (avl_of_player_by_id->findNode(PlayerID) == nullptr) ///O(log(n))
        {
            return FAILURE;
        }

        //get the lvl of the player and the group_id of the player
        int lvl = avl_of_player_by_id->findNode(PlayerID)->data->get_player_level(); ///O(log(n))
        int group_id = avl_of_player_by_id->findNode(PlayerID)->data->get_the_group_id(); ///O(log(n))
        Group* group_of_player = avl_of_player_by_id->findNode(PlayerID)->data->get_the_group(); ///O(log(n))

        //using the lvl and player id to remove from the avl of player lvl the player
        Key_by_lvl key(lvl, PlayerID);
        avl_of_player_by_lvl->remove(key); ///O(log(n))

        //then update the max player of the data structure if the data structure empty of player
        if (avl_of_player_by_lvl->find_max_root() == nullptr)///O(log(n))
        {
            max_player_id = NOT_READY;
            max_player_level = NOT_READY;
        }

        //or update the max player of the data structure if by searching the max in the avl of player by level
        else
        {
            max_player_id = avl_of_player_by_lvl->find_max_root()->data; ///O(log(n))
            max_player_level = avl_of_player_by_lvl->find_max_root()->key.getLevel();///O(log(n))
        }

        //remove the player from the avl of player in group
        group_of_player->remove_player(key);///O(log(n))

        //updating the avl of empty group data or delete it if needed and update the max player of the group
        if (group_of_player->is_empty())///O(1)
        {
            avl_of_no_empty_groups->remove(group_id);///O(log(n)) because there is less than n non empty groups
            group_of_player->change_max(NOT_READY, NOT_READY); ///O(1)
        }
        else
        {
            int group_max_lvl = group_of_player->get_avl()->find_max_root()->key.getLevel(); ///O(log(n))
            int group_max_id = group_of_player->get_avl()->find_max_root()->key.getID(); ///O(log(n))
            (avl_of_no_empty_groups->findNode(group_id)->data).update_player(group_max_id, group_max_lvl);///O(log(n)) because there is less than n non empty groups
            group_of_player->change_max(group_max_lvl, group_max_id); ///O(1)
        }
        //delete the player from the avl of player by id
        delete avl_of_player_by_id->findNode(PlayerID)->data;
        avl_of_player_by_id->remove(PlayerID); ///O(log(n))

        //all good :-)
        return SUCCESS;
    }

    /*
     * this function merge 2 pairs of arrays by level
     */
    ///Complexity = O(size_of(sourceKey)+size_of(sourceData))
    static void merge(Key_by_lvl *sourceKey,Player_in_group* sourceData,int sizeSource,Key_by_lvl * destKey, Player_in_group* destData,
               int sizeDest, Key_by_lvl* resKey,Player_in_group* resData)
    {
        int ia,ib,ic;
        ia=ib=ic=0;
        while(ia<sizeSource && ib <sizeDest)
        {
            if(sourceKey[ia] < destKey[ib])
            {
                resKey[ic] = sourceKey[ia];
                resData[ic] = sourceData[ia];
                ia++; ic++;
            }
            else
            {
                resKey[ic] = destKey[ib];
                resData[ic] = destData[ib];
                ib++; ic++;
            }
        }
        while (ia<sizeSource)
        {
            resKey[ic]= sourceKey[ia];
            resData[ic] = sourceData[ia];
            ia++; ic++;
        }
        while (ib<sizeDest)
        {
            resKey[ic]= destKey[ib];
            resData[ic] = destData[ib];
            ib++; ic++;
        }
    }

    /// Complexity = O(log(k) + n_group + n_replacement)
    StatusType ReplaceGroup(int GroupID, int ReplacementID)
    {
        //checking the input
        if(GroupID == ReplacementID || GroupID <=0 || ReplacementID <=0)
        {
            return INVALID_INPUT;
        }

        //checks the failure
        if(avl_of_groups->findNode(GroupID) == nullptr || avl_of_groups->findNode(ReplacementID) == nullptr)
        {
            return FAILURE;
        }

        //getting the size of the avl's for the malloc
        int i=0;
        int sizeSource = avl_of_groups->findNode(GroupID)->data->get_avl()->getSize();///O(log(k)): to find the group
        int sizeDest = avl_of_groups->findNode(ReplacementID)->data->get_avl()->getSize();///O(log(k)): to find the group

        //case there no need to do anything just remove the group from the avl of group
        if(sizeSource==0)
        {
            delete avl_of_groups->findNode(GroupID)->data;
            avl_of_groups->remove(GroupID);///O(log(k))
            return SUCCESS;
        }

        //some allocation before the merge
        Key_by_lvl * sourceKey = (Key_by_lvl*)(malloc(sizeSource *sizeof(Key_by_lvl)));
        if(sourceKey== nullptr)
        {
            return ALLOCATION_ERROR;
        }
        Player_in_group * sourceData = (Player_in_group*)(malloc(sizeSource *sizeof(Player_in_group)));
        if(sourceData== nullptr)
        {
            free(sourceKey);
            return ALLOCATION_ERROR;
        }
        Key_by_lvl * destKey = (Key_by_lvl*)(malloc(sizeDest *sizeof(Key_by_lvl)));
        if(destKey== nullptr)
        {
            free(sourceKey);
            free(sourceData);
            return ALLOCATION_ERROR;
        }
        Player_in_group * destData = (Player_in_group*)(malloc(sizeDest*sizeof(Player_in_group)));
        if(destData== nullptr)
        {
            free(sourceData);
            free(sourceKey);
            free(destKey);
            return ALLOCATION_ERROR;
        }

        //get the pointers to the source group and dest groups
        Group* sourceGroup = avl_of_groups->findNode(GroupID)->data;///O(log(k))
        Group* destGroup = avl_of_groups->findNode(ReplacementID)->data;///O(log(k))

        //transform the avl source and dest to array's
        sourceGroup->get_avl()->transform_to_array_by_inorder_keys(&sourceKey,&i); ///O(n_group)
        i=0;
        destGroup->get_avl()->transform_to_array_by_inorder_keys(&destKey,&i);///O(n_replacement)
        i=0;
        sourceGroup->get_avl()->transform_to_array_by_inorder(&sourceData,&i);///O(n_group)
        i=0;
        destGroup->get_avl()->transform_to_array_by_inorder(&destData,&i);///O(n_replacement)
        i=0;

        //some malloc for the result array
        Key_by_lvl * resKey = (Key_by_lvl*)(malloc(sizeof(Key_by_lvl) * (sizeSource+sizeDest)));
        Player_in_group * resData = (Player_in_group*)(malloc(sizeof(Player_in_group) * (sizeSource+sizeDest)));

        //using the static function helper merge
        merge(sourceKey,sourceData,sizeSource,destKey,destData,sizeDest,resKey,resData);///O(n_group+n_replacement)

        //create an empty tree using the static function helper createEmptyTree and fill it with the res array(after merge)
        AVL<Key_by_lvl,Player_in_group>* resAvl=new AVL<Key_by_lvl,Player_in_group>();
        createEmptyTree(*resAvl, sizeSource+sizeDest);///O(n_group+n_replacement)
        resAvl->fill_tree(&resKey,&resData,&i);///O(n_group+n_replacement)
        i=0;
        AVL<Key_by_lvl,Player_in_group>* avl=avl_of_groups->findNode(GroupID)->data->get_avl();///O(log(k))

        //using the strong function from the avlTree doc
        delete avl_of_groups->findNode(ReplacementID)->data->get_avl();
        change_pointer_of_group(avl,destGroup,ReplacementID);///O(n_group)

        //then delete the group_id from the non empty group
        avl_of_no_empty_groups->remove(GroupID);///O(log(k))

        // and now delete the group_id from the avl of groups
        delete avl_of_groups->findNode(GroupID)->data;
        avl_of_groups->remove(GroupID);///O(log(k))

        //updating the max and the avl of player in group of the replacement group
        avl_of_groups->findNode(ReplacementID)->data->change_max(resKey[sizeDest+sizeSource-1].getLevel(),resKey[sizeDest+sizeSource-1].getID());///O(log(k))
        avl_of_groups->findNode(ReplacementID)->data->change_avl(resAvl);///O(log(k)

        //updating the avl of none empty group if the group was empty of player, and now there is a player
        if(avl_of_no_empty_groups->findNode(ReplacementID) == nullptr /*O(log(k))*/ )
        {
            avl_of_no_empty_groups->insert(ReplacementID,Not_empty_data(resKey[sizeDest+sizeSource-1].getID(),resKey[sizeDest+sizeSource-1].getLevel()));///O(log(k))
        }
        else
        {
            avl_of_no_empty_groups->findNode(ReplacementID)->data.update_player(resKey[sizeDest+sizeSource-1].getID(),resKey[sizeDest+sizeSource-1].getLevel());///O(log(k))
        }

        //free the malloc for the merge
        free(sourceKey);
        free(sourceData);
        free(destKey);
        free(destData);
        free(resKey);
        free(resData);

        //all good :-)
        return SUCCESS;
    }

    /*
     * this function delete the avl of player in group from the previous group
     * updating all the pointer in the avl of player by id from the previous to the replacement group
     * Updating the group_id in the avl of player by id from the previous to the replacement group
     */
    ///Complexity = O(size_of(avl))
    static void change_pointer_of_group(AVL<Key_by_lvl,Player_in_group>* avl,Group* group,int groupID)
    {
        if((*avl).getRoot()== nullptr)
        {
            return ;
        }
        (*avl).getRoot()->data.player->change_groupID(groupID);///O(1)
        (*avl).getRoot()->data.player->change_group(group);///O(1)
        (*avl).remove((*avl).getRoot()->key);///O(1)
        change_pointer_of_group(avl,group,groupID);
    }

    /*
     * this function create an empty tree of size(size)
     */
    ///O(size)
    static void createEmptyTree(AVL<Key_by_lvl,Player_in_group> &resAvl, int size)
    {
        Key_by_lvl key(0,0);
        Player_in_group data(0, nullptr);
        for(int i=0; i<size; i++)
        {
            resAvl.insert(key,data);
        }
    }

    /// Complexity = 10log(n) --->>> O(log(n))
    StatusType IncreaseLevel(int player_id, int level_increase)
    {
        //checking input.
        if (player_id <=0 || level_increase <=0)
        {
            return INVALID_INPUT;
        }
        if(avl_of_player_by_id->findNode(player_id)== nullptr)///O(log(n))
        {
            return FAILURE;
        }
        //changing the values of the player in the player_by_id tree. only changing the lvl to the current one.
        avl_of_player_by_id->findNode(player_id)->data->addToPlayerLvl(level_increase);///O(log(n))

        //getting the player updated lvl and the group of the player
        int current_lvl = avl_of_player_by_id->findNode(player_id)->data->get_player_level();///O(log(n))
        int group_id = avl_of_player_by_id->findNode(player_id)->data->get_the_group_id();///O(log(n))
        Group *gr = avl_of_player_by_id->findNode(player_id)->data->get_the_group();///O(log(n))

        //removing the player from his group(previous level and adding him with the new level.That will also update the group's max information.
        Key_by_lvl prevKey(current_lvl-level_increase,player_id);
        Key_by_lvl currKey(current_lvl,player_id);
        gr->get_avl()->remove(prevKey);///O(log(n))
        gr->add_player(player_id,current_lvl,Player_in_group(player_id,(avl_of_player_by_id->findNode(player_id)->data)));///O(log(n))

        //fixing player_by_level tree.
        avl_of_player_by_lvl->remove(prevKey);///O(log(n))
        avl_of_player_by_lvl->insert(currKey,player_id);///O(log(n))


        //updating the no_empty_group tree on the node of our tree, to the new max. (if the max didn't change, will do nothing).
        avl_of_no_empty_groups->findNode(group_id)->data.update_player(gr->get_max_player(),gr->get_max_level());///O(log(n))

        //fixing system's max if needed.
        if(current_lvl>max_player_level)
        {
            max_player_level=current_lvl;
            max_player_id=player_id;
        }
        if(current_lvl==max_player_level)
        {
            if(player_id<max_player_id)
            {
                max_player_level=current_lvl;
                max_player_id=player_id;
            }
        }

        //all good :-)
        return SUCCESS;
    }

    ///Complexity = O(log(k))
    StatusType GetHighestLevel(int GroupID, int *PlayerID)
    {
        //checking input
        if(GroupID == 0 || PlayerID == nullptr)
        {
            return INVALID_INPUT;
        }
        //if group_id <0 returning the max_player_id of the system.
        if(GroupID<0)
        {
            *PlayerID = max_player_id;
            return SUCCESS;
        }

        //checks if group exist.
        if(avl_of_groups->findNode(GroupID) == nullptr)///O(log(k))
        {
            return FAILURE;
        }
        if(avl_of_groups->findNode(GroupID)->data->get_avl()->getSize()==0)///O(log(k))
        {
            *PlayerID=-1;
            return SUCCESS;
        }
        //returning the max player in the group. get_max_player only returns it without searching, so its good with complexity.
        *PlayerID = avl_of_groups->findNode(GroupID)->data->get_max_player();///O(log(k))

        //all good :-)
        return SUCCESS;
    }

    /// Complexity = O(log(k)+n_group_id)
    StatusType GetAllPlayersByLevel(int GroupID, int **Players, int *numOfPlayers)
    {
        //checking the input
        if(GroupID == 0 || Players == nullptr || numOfPlayers == nullptr)
        {
            return INVALID_INPUT;
        }

        //if the group_id <0 return all the players in the system ordered by level
        if(GroupID <0)
        {
            int num_of_players = avl_of_player_by_id->getSize();
            int* players_array = (int*)malloc(sizeof(int) *num_of_players );
            if (players_array == nullptr)
            {
                return ALLOCATION_ERROR;
            }
            int i=0;
            avl_of_player_by_lvl->transform_to_array_by_order(&players_array,&i);///O(n)
            *numOfPlayers = num_of_players;
            *Players=players_array;

            //all good :-)
            return SUCCESS;
        }

        //if the group exist than print the players of the group by level
        else if(avl_of_groups->findNode(GroupID) != nullptr)
        {
            int num_of_players = avl_of_groups->findNode(GroupID)->data->getNumOfPlayersInGroup();///O(log(k))
            Player_in_group* players_array = (Player_in_group*)malloc(sizeof(Player_in_group) *num_of_players );
            if (players_array == nullptr)
            {
                return ALLOCATION_ERROR;
            }
            int i=0;
            avl_of_groups->findNode(GroupID)->data->get_avl()->transform_to_array_by_order(&players_array,&i);///O(log(k)+n_group)
            int * result = (int*) malloc(sizeof(int)*num_of_players);
            if(result== nullptr)
            {
                free(players_array);
                return ALLOCATION_ERROR;
            }
            for(int i=0; i<num_of_players;i++)
            {
                result[i]=(players_array[i].player_id);
            }
            *Players=result;
            free(players_array);
            *numOfPlayers = num_of_players;

            //all good :-)
            return SUCCESS;
        }
        return FAILURE;
    }

    /// Complexity = O(numOfGroups+log(k))
    StatusType GetGroupsHighestLevel(int numOfGroups, int **Players)
    {
        //checking the input
        if(numOfGroups<1 || Players == nullptr)
        {
            return INVALID_INPUT;
        }

        //checking the failure cases
        if(numOfGroups > avl_of_no_empty_groups->getSize())
        {
            return FAILURE;
        }

        Not_empty_data * players_array = (Not_empty_data*)malloc(sizeof(Not_empty_data)*(numOfGroups+1));
        if (players_array == nullptr)
        {
            return ALLOCATION_ERROR;
        }
        int i=0;
        avl_of_no_empty_groups->transform_to_array_by_inorder_maximum_n(&players_array,&i,numOfGroups);///O(log(k))
        int * result = (int*) malloc(sizeof(int)*numOfGroups);
        if(result== nullptr)
        {
            free(players_array);
            return ALLOCATION_ERROR;
        }
        for(int j=0; j<numOfGroups;j++)///(O(numOfGroups))
        {
            result[j]=(players_array[j].getID());
        }
        free(players_array);
        *Players = result;

        //everything ok :-)
        return SUCCESS;
    }




    ///O(n+k)
    void Quit()
    {
        avl_of_groups->treeDataClear();///O(K)
        avl_of_no_empty_groups->treeClear();///O(K)
        avl_of_player_by_id->treeClear();///O(n)
        avl_of_player_by_lvl->treeClear();///O(n)

        delete avl_of_groups;
        delete avl_of_player_by_lvl;
        delete avl_of_player_by_id;
        delete avl_of_no_empty_groups;
    }

};
#endif //HW1_DS_H