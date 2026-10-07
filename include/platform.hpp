#pragma once
#include "raylib.h"
#include<vector>
class Platform{
    public:
        std::vector<Rectangle>REC;
        Color pcolor;

        Platform(std::vector<Rectangle>rec,Color color);
        void DrawPlat(); 
        ~Platform();
};
