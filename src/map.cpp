#include "map.hpp"

#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

Map::Map(){}


void Map::Load(const std::string&path){
    std::ifstream file(path);
    if(!file.is_open()){
        TraceLog(LOG_TRACE,"Could not open map");
        return;
    }

    json data;
    file >> data;

    for(auto&layer :data["layers"]){
        if(layer["name"] == "Platform"){
            for(auto &obj : layer["objects"]){
                platforms.push_back({
                    obj["x"].get<float>(),
                    obj["y"].get<float>(),
                    obj["width"].get<float>(),
                    obj["height"].get<float>()
                
                });
            }
        }
        else if(layer["name"] == "Spike"){
            int idx = 0;
            for(auto &obj : layer["objects"]){
                spikes.push_back({
                    obj["x"].get<float>(),
                    obj["y"].get<float>(),
                    obj["width"].get<float>(),
                    obj["height"].get<float>()
                });

                //find traps by opacity
                if(obj["opacity"].get<float>()*100 == 99){  
                    //traps.push_back({idx,-1});
                    int limit = 0;
                    for(auto &prop : obj["properties"]){
                        if(prop["name"] == "limit"){
                            limit = prop["value"].get<int>();
                            break;
                        }
                    }
                    traps.push_back({idx,limit});
                }
                idx++;
            }
        }
        else if(layer["name"] == "CheckPoint"){
            int id = 0;
            for(auto &obj : layer["objects"]){
                checkpoints.push_back({
                    id,
                    {
                        obj["x"].get<float>(),
                        obj["y"].get<float>(),
                        obj["width"].get<float>(),
                        obj["height"].get<float>()
                    },

                    {   obj["x"].get<float>(),
                        obj["y"].get<float>(),
                    },
                    
                    false,
                    YELLOW
                });
                id++;
            }
        }
        else if(layer["name"] == "Spawn"){
            for(auto &obj : layer["objects"]){
                this->spawn.x = obj["x"].get<float>();
                this->spawn.y = obj["y"].get<float>();
            }
        }
        else if(layer["name"] == "ExitArea"){
            for(auto &obj : layer["objects"]){
                this->Exit.x = obj["x"].get<float>();
                this->Exit.y = obj["y"].get<float>();
                this->Exit.height = obj["height"].get<float>();
                this->Exit.width = obj["width"].get<float>();
            }
        }


    }
}

void Map::transRecToSpike(){
 
    for(auto &rec : spikes){
        
        Spike sp;
        auto [x,y,w,h] = rec;
        sp.bound = { x, y, w, h };

        sp.hitbox = {
            x + w * 0.15f,
            y + h * 0.15f,
            w * 0.7f,
            h * 0.85f
        };
        Sp.push_back(sp);
    }
}
/*
void Map::transRecToSpikeOne(){    

        Spike sp;
        auto [x,y,w,h] = rec;
        sp.bound = { x, y, w, h };

        sp.hitbox = {
            x + w * 0.15f,
            y + h * 0.15f,
            w * 0.7f,
            h * 0.85f
        };
       
}
*/
void Map::DrawSpike(const Spike&sp){
    const Rectangle &rec = sp.bound;
    
    Vector2 Top = {
        rec.x + rec.width/2,
        rec.y
    };

    Vector2 Left = {
        rec.x,
        rec.y + rec.height
    };

    Vector2 Right = {
        rec.x + rec.width,
        rec.y + rec.height
    };

    DrawTriangle(Left,Right,Top,RED);
}

void Map::Draw(){

    for(auto &sp : Sp){
        DrawSpike(sp);
    }
    for(int i = 0;i<(int)platforms.size();i++){
        DrawRectangleRec(platforms[i],GRAY);
    }
    for(auto &cp : checkpoints){
        if(cp.activated)cp.checkColor = GREEN;
        DrawRectangleRec(cp.area,cp.checkColor);
    }
    DrawRectangleRec(Exit,RED);
}

void Map::Clear(){
    platforms.clear();
    Sp.clear();
    spikes.clear();
    checkpoints.clear();
    spawn = {0,0};
    traps.clear();     
}


Map::~Map(){}
