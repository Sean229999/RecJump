#include<raylib.h>
#include "player.hpp"
#include<string>
#include<vector>

/*
struct Spike{
    Rectangle bound,hitbox;
};
*/

struct trap{
    int index;
    int limit;
};


class Map{
    public:
        std::vector<Rectangle> platforms;
        std::vector<Rectangle> spikes;     
        std::vector<trap> traps; 
        std::vector<Spike> Sp;  
        std::vector<CheckPoint> checkpoints;
        
        Rectangle Exit;

        Vector2 spawn;
        Map();

        void Load(const std::string &path);
        void transRecToSpike();
        void transRecToSpikeOne();
        void DrawSpike(const Spike&sp);
        void Draw();
        void Clear();

        ~Map();
};

