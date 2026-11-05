#include "camera.hpp"


playerCamera::playerCamera(Player &player,float width,float height){
    camera.target = {player.REC.x + 25,player.REC.y + 25};
    camera.offset = {width * 0.5f,height * 0.5f};
    camera.rotation = 0.0f;
    camera.zoom = 0.75f;

    rightLimit = width * 0.5f;
    leftLimit = width * 0.3f;
    

}

void playerCamera::Begin(){
    BeginMode2D(camera);
}

void playerCamera::updateCamera(Player &player){
    Vector2 screenPos = GetWorldToScreen2D({player.REC.x + 25,player.REC.y+25},camera);
    if(screenPos.x >= rightLimit){
        camera.target.x += screenPos.x - rightLimit;
    }
    if(screenPos.x <= leftLimit){ 
        camera.target.x += screenPos.x - leftLimit;
    }
}

void playerCamera::End(){
    EndMode2D();
}

playerCamera::~playerCamera(){}

