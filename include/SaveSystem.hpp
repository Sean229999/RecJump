#include<raylib.h>

struct SaveData{
    int checkPointId;
};

class SaveSystem{
    public:
        bool Save(const SaveData& data);
        bool Load(SaveData& data);
};
