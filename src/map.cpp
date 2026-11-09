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
                float x = obj["x"].get<float>();
                float y = obj["y"].get<float>();
                float width = obj["width"].get<float>();
                float height = obj["height"].get<float>();
                int dir = 0;
                for(auto &prop : obj["properties"]){
                    if(prop["name"] == "direction"){
                        dir = prop["value"].get<int>();
                    }
                }
                auto sp = transRecToSpikeOne({x,y,width,height},dir);
                spikes.push_back(sp);


                //find traps by opacity
                if(obj["opacity"].get<float>()*100 == 99){  
                    int limit = 0;
                   
                    float top_y,down_y,right_x,left_x;
                    
                    if(dir == 0){
                        top_y = y - height * 2;
                        down_y = y + height;
                        left_x = x + width/2;
                        right_x = x + width;
                        limit = y - height * 3;
                    }
                    if(dir == 1){
                        top_y = y;
                        down_y = y + height;
                        left_x = x;
                        right_x = x + width * 2;
                        limit = x + width * 3;
                    }
                    if(dir == 2){
                        top_y = y;
                        down_y = y + height;
                        left_x = x - width * 2;
                        right_x = x + width;
                        limit = x - width * 3;
                    }
                    if(dir == 3){
                        top_y = y;
                        down_y = y + height * 2;
                        left_x = x;
                        right_x = x + width;
                        limit = y + height * 3;
                    }

                    for(auto &prop : obj["properties"]){
                        if(prop["name"] == "limit"){
                            limit = prop["value"].get<int>();
                        }
                        if(prop["name"] == "downY"){
                            down_y = prop["value"].get<int>();
                            //TraceLog(LOG_INFO,"found Down");
                        }
                        if(prop["name"] == "topY"){
                            top_y = prop["value"].get<int>();
                            //TraceLog(LOG_INFO,"found Top");
                        }
                        if(prop["name"] == "rightX"){
                            right_x = prop["value"].get<int>();
                        }
                        if(prop["name"] == "leftX"){
                            left_x = prop["value"].get<int>();
                        }
                    }
                    traps.push_back({idx,limit,dir,114514,top_y,down_y,left_x,right_x});
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

/*
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
*/

Spike Map::transRecToSpikeOne(Rectangle rec,int dir){    

        Spike sp;
        auto [x,y,w,h] = rec;

        sp.direction = dir;
        sp.bound = { x, y, w, h };
       
        /*
        if(dir == 0){
            sp.hitbox = {
                x + w * 0.15f,
                y + h * 0.15f,
                w * 0.7f,
                h * 0.85f
            };
        }
        else if(dir == 1){
            sp.hitbox = {

            }
        }
        else if(dir == 2){
            
        }
        else if(dir == 3){

        }
        */
        sp.hitbox = {
            x + w * 0.15f,
            y + h * 0.15f,
            w * 0.7f,
            h * 0.85f
        };

        return sp;
}

void Map::DrawSpike(const Spike&sp){
    const Rectangle &rec = sp.bound;
    
    int dir = sp.direction;
    Vector2 Top,Left,Right;
    if(dir == 0){
        Top = {
            rec.x + rec.width/2,
            rec.y
        };

        Left = {
            rec.x,
            rec.y + rec.height
        };

        Right = {
            rec.x + rec.width,
            rec.y + rec.height
        };

    }
    else if(dir == 1){
        Top = {
            rec.x + rec.width,
            rec.y + rec.height / 2
        };

        Left = {
            rec.x,
            rec.y
        };

        Right = {
            rec.x,
            rec.y + rec.height
        };
    }
    else if(dir == 2){
        Top = {
            rec.x,
            rec.y + rec.height / 2
        };

        Left = {
            rec.x + rec.width,
            rec.y + rec.height
        };

        Right = {
            rec.x + rec.width,
            rec.y
        };

    }
    else if(dir == 3){
        Top = {
            rec.x + rec.width / 2,
            rec.y + rec.height
        };

        Left = {
            rec.x + rec.width,
            rec.y
        };

        Right = {
            rec.x,
            rec.y
        };

    }

    DrawTriangle(Left,Right,Top,RED);
}

void Map::Draw(){

    for(auto &sp : spikes){
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
    spikes.clear();
    checkpoints.clear();
    spawn = {0,0};
    traps.clear();     
    Exit = {};
}


Map::~Map(){}
