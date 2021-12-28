#ifndef HW1_NOT_EMPTY_DATA_H
#define HW1_NOT_EMPTY_DATA_H

class Not_empty_data
{
private:
    int max_player_id;
    int max_player_level;
public:
    Not_empty_data()=default;
    ~Not_empty_data()=default;
    Not_empty_data(int id, int level):max_player_id(id), max_player_level(level){};
    void update_player(int player_id, int player_level)
    {
        max_player_id = player_id;
        max_player_level = player_level;
    }
    int getID() const
    {
        return max_player_id;
    }
};


#endif //HW1_NOT_EMPTY_DATA_H

