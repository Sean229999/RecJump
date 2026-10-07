#include<raylib.h>
#include "player.hpp"
#include<string>
#include<vector>

/*
struct Spike{
    Rectangle bound,hitbox;
};
*/

class Map{
    public:
        std::vector<Rectangle> platforms;
        std::vector<Rectangle> spikes;     
        std::vector<Spike> Sp; 
    
        std::vector<CheckPoint> checkpoints;

        void Load(const std::string &path);
        void transRecToSpike();
        void DrawSpike(const Spike&sp);
        void Draw();
};

