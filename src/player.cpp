#include "player.hpp"

Player::Player(Rectangle rec,float G,Color color):REC(rec),RESET(rec),pcolor(color),gravity(G),jumpCount(0){}

void Player::VCollision(float oldTop,float nowTop,float oldBottom,float nowBottom,std::vector<Platform> &plats){
    float x = this->REC.x;
    float w = this->REC.width;
        
    onGround = false;

    //is on platforms or lands
    for(int i = 0;i<(int)plats.size();i++){
        for(int j = 0;j<(int)plats[i].REC.size();j++){
            float p_x = plats[i].REC[j].x;
            float platTop = plats[i].REC[j].y;
            float p_w = plats[i].REC[j].width;
            float p_h = plats[i].REC[j].height;
            

            bool overLeap_x = false;
            bool hitTop = false;
            bool hitBottom = false;
            if(x + w > p_x && x < p_x + p_w){
                overLeap_x = true;
            }    
            
            if(velocity_y > 0){   

                if(oldBottom <= platTop && nowBottom >= platTop){
                    hitTop = true;
                }

                //On plats or lands
                if(overLeap_x && hitTop){
                    this->REC.y = platTop - REC.height;
                    this->velocity_y = 0;
                    this->onGround = true;
                    jumpCount = 0;
                    break;
                }

            }
            else {
                float platBottom = platTop + p_h;
                if(oldTop >= platBottom && nowTop <= platBottom){
                    hitBottom = true; 
                }

                if(overLeap_x && hitBottom){
                    this->REC.y = platBottom;
                    velocity_y = 0;
                    break;
                }
            }

        }
    }
}

void Player::HCollision(float dx,std::vector<Platform>&plats){

    for(auto P : plats){
        for(auto p : P.REC){
            if(!CheckCollisionRecs(this->REC,p)) continue;
            
            if(dx < 0){
                this->REC.x = p.x + p.width;
            }
            if(dx > 0){
                this->REC.x = p.x - REC.width;
            }
        }
    }
}

void Player::DrawPlayer(){
    DrawRectangleRec(REC,pcolor);
}


// Reset Player
void Player::Reset(std::vector<CheckPoint>&checkpoints,int &current_checkPoint){
    if(current_checkPoint != -1){
        this->REC.x = checkpoints[current_checkPoint].area.x;
        this->REC.y = checkpoints[current_checkPoint].area.y;
    }
    else {
        this->REC = this->RESET;
    }
    velocity_y = 0;
    jumpCount = 0;
    onGround = false;
    deathTimer = 0.0f;
    state = PlayerState::Normal;
}


// If dead due to fall
void Player::UpdateDeadFall(){
    
    deathTimer += dt;
    velocity_y += dt * gravity;
    REC.y += velocity_y * dt;

    if(deathTimer >= deathDuration){
        this->state = PlayerState::Respwaning;
    }
}


// Is Player dead
void Player::Die(PlayerState deathType){
    
    if(state != PlayerState::Normal) return;
    
    state = deathType;
    deathTimer = 0.0f;
    if(state == PlayerState::DeadFall){
        velocity_y = -500.0f;
    }
    else if(state == PlayerState::DeadSpike){
        velocity_y = -600.0f;
    }

}

void Player::Update(std::vector<Platform> &plats,std::vector<Spike>&Sp,Camera2D &camera,std::vector<CheckPoint>&checkpoints,int &current_checkPoint){ 

    dt = GetFrameTime();
    if(IsKeyPressed(KEY_R)){
        Reset(checkpoints,current_checkPoint);
    }

    // check if dead of alive
    if(state == PlayerState::DeadFall){
        UpdateDeadFall();
        return;
    }
    else if(state == PlayerState::DeadSpike){
        UpdateDeadFall();
        return;
    }
    
    float dx = 0;
    if(IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)){
        dx -= this->speed * this->dt;
    }

    if(IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)){ 
        dx += this->speed * this->dt;
    }

    REC.x += dx;

    //Horizontal collision detect
    HCollision(dx,plats);


    // Jump !!! 
    if((IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) && jumpCount < 2){
        velocity_y = jumpSpeed; 
        jumpCount++;
        onGround = false;
    }
   
    // For Collision
    float oldBottom = REC.y + REC.height;
    float oldTop = REC.y;

    velocity_y += gravity * dt;
    this->REC.y += velocity_y * dt;
   
    // Same as the old one
    float nowBottom = REC.y + REC.height;
    float nowTop = REC.y;        

    //Vertical collision detect
    VCollision(oldTop,nowTop,oldBottom,nowBottom,plats);
   
    // If fall out the world
    
    Vector2 screenPos = GetWorldToScreen2D({REC.x,REC.y},camera);
    if(screenPos.y >= 960){
        Die(PlayerState::DeadFall);
    }   

    
    
    // If hit the spikes
    for(auto &sp : Sp){
        if(CheckCollisionRecs(REC,sp.hitbox)){
            Die(PlayerState::DeadSpike);
            break;
        }
    }
}
Player::~Player(){}

