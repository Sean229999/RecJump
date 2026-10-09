#include<raylib.h>

struct SaveData{
    int checkPointId;
};

class SaveSystem{
    public:
    
        SaveSystem();

        bool Save(const SaveData& data);
        bool Load(SaveData& data);

        ~SaveSystem();
};
