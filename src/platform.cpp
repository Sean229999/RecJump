#include "platform.hpp"

Platform::Platform(std::vector<Rectangle>rec,Color color):REC(rec),pcolor(color){}

void Platform::DrawPlat(){
    for(int i = 0;i<(int)REC.size();i++){
        DrawRectangleRec(REC[i],pcolor);
    }
}

Platform::~Platform(){}
