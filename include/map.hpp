#include<raylib.h>
#include "player.hpp"
#include<string>
#include<vector>


struct trap{
    int index;
    int limit;
    int direction;
    float speed;
    float trigger_span_top_y;
    float trigger_span_down_y;
    float trigger_span_left_x;
    float trigger_span_right_x;
};


class Map{
    public:
        std::vector<Rectangle> platforms;
        std::vector<trap> traps; 
        std::vector<Spike> spikes;  
        std::vector<CheckPoint> checkpoints;
        
        Rectangle Exit;

        Vector2 spawn;
        Map();

        void Load(const std::string &path);
        void transRecToSpike();
        Spike transRecToSpikeOne(Rectangle rec,int dir);
        void DrawSpike(const Spike&sp);
        void Draw();
        void Clear();

        ~Map();
};

