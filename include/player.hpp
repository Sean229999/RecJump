#pragma once
#include<raylib.h>
#include"platform.hpp"
#include"SaveSystem.hpp"

struct Spike{
    Rectangle bound,hitbox;
};

struct CheckPoint{
    int id;
    Rectangle area;
    Vector2 respwanPos;
    bool activated = false;
};

enum class PlayerState {
    Normal,
    DeadFall,
    DeadSpike,
    DeadEnemy,
    Respwaning
};

class Player{ 
public:
    Rectangle REC;
    Rectangle RESET;
    Color pcolor;
    float velocity_y = 0;
    float jumpSpeed = -750;
    float speed = 600;
    float gravity;
    float dt;
    bool onGround = false;
    
    SaveSystem save;
    float deathTimer = 0.0f;
    float deathDuration = 1.0f;
    PlayerState state = PlayerState::Normal;


    Player(Rectangle rec,float G,Color color); 

    void DrawPlayer();
    void Update(std::vector<Platform>&plats,std::vector<Spike>&Sp,Camera2D&camera,std::vector<CheckPoint>&checkpoints,int &current_checkPoint); 
    void VCollision(float oldTop,float nowTop,float oldBottom,float nowBottom,std::vector<Platform>&plats);
    void HCollision(float dx,std::vector<Platform>&plats);
    void Reset(std::vector<CheckPoint>&checkpoints,int &current_checkPoint);
    void UpdateDeadFall(std::vector<CheckPoint>&checkpoints,int &current_checkPoint);
    void Die(PlayerState state);

    ~Player();

};
