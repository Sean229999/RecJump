#pragma once
#include<raylib.h>

#include "map.hpp"
#include "player.hpp"
#include "platform.hpp"
#include "camera.hpp"


#define screenColor WHITE
#define playerColor BLUE
#define platColor RED
#define landColor BLACK
#define SIZE 32.0
#define G 2000.0
#define SPEED 1000
class Game{
    
    private:
        static constexpr float GAME_WIDTH = 1280.0f;
        static constexpr float GAME_HEIGHT = 960.0f;


        Map maps;
        Player player;
        playerCamera camera;
        SaveData data;
        SaveSystem save;
        RenderTexture2D target;
        std::vector<Platform>Plat_Land;
        std::vector<Spike>Spikess;
        std::vector<Spike>orignalSpikes;
        std::vector<bool>activatedTrap;
        int current_checkPoint;
        int current_Level;        
        std::vector<std::string>levels = {
            "assets/maps/level1.tmj",
            "assets/maps/level2.tmj",
            "assets/maps/level3.tmj",
            "assets/maps/level4.tmj",
        };
    
        Sound jumpSound;
        Sound deathSound;

    public:
        Game();
        ~Game();
        void Run();

    private:
        void LoadSave();
        void Init();
    
        void LoadLevel(int level);
        void nextLevel();
        void SaveId();

        void Update();

        void Draw();

        void DrawGameWorld();
        void DrawGameScreen();
        void DrawUi();        
        void triggerTrap();
    
        void resetTrap();
    
        void changeLevel();

};

