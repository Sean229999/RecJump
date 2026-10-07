#pragma  once
#include<raylib.h>
#include"player.hpp"

class playerCamera {
public:
    Camera2D camera;
    float leftLimit;
    float rightLimit;
    playerCamera(Player &player,float width,float height);
    void Begin();
    void updateCamera(Player &player);
    void End();
    ~playerCamera();
};
