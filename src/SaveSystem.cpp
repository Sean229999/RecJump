#include "SaveSystem.hpp"
#include "nlohmann/json.hpp"
#include <fstream>

SaveSystem::SaveSystem(){}

using json = nlohmann::json;

bool SaveSystem::Save(const SaveData& data){
    json j;

    j["checkPointId"] = data.checkPointId;
    j["currentLevelId"] = data.currentLevelId; 
    std::ofstream file("save.json");

    if(!file.is_open()){
        return false;
    }

    file << j.dump(4);
    
    return true;

}


bool SaveSystem::Load(SaveData&data){

    std::ifstream file("save.json");
    if(!file.is_open()){
        return false;
    }
    else {
        try{
            json j;
            file >> j;
            data.checkPointId = j.value("checkPointId",0);
            data.currentLevelId = j.value("currentLevelId",0);
            return true;
        }
        catch(...){
            return false;
        }
    }

}

SaveSystem::~SaveSystem(){}
