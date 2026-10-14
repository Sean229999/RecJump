#include<raylib.h>

struct SaveData{
    int checkPointId;
    int currentLevelId;
};

class SaveSystem{
    public:
    
        SaveSystem();

        bool Save(const SaveData& data);
        bool Load(SaveData& data);

        ~SaveSystem();
};
