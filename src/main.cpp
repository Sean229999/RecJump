#include<raylib.h>

#include "map.hpp"
#include "player.hpp"
#include "platform.hpp"
#include "camera.hpp"


#define GAME_WIDTH 1280.00
#define GAME_HEIGHT 960.00
#define screenColor WHITE
#define playerColor BLUE
#define platColor RED
#define landColor BLACK
#define SIZE 50.0
#define G 1200.0

int current_checkPoint = -1;

int main(){
 
    
    //Load Map 
    Map maps;
    maps.Load("assets/maps/map.tmj");
    maps.transRecToSpike(); 
    Platform plats(maps.platforms,GRAY);
        
    std::vector<Platform>Plat_land = {plats};
    std::vector<Spike>Spikess = maps.Sp;
    //Player initialization
    Player player({0,600,SIZE,SIZE},G,playerColor); 
    //Camera initialization
    playerCamera camera(player,GAME_WIDTH,GAME_HEIGHT);

    //SaveSystem initialization
    SaveData data;
    SaveSystem save;


    if(save.Load(data)){

        current_checkPoint = data.checkPointId;
        player.Reset(maps.checkpoints,current_checkPoint);
    }

    // Make window resizable
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    //init window
    InitWindow(GAME_WIDTH,GAME_HEIGHT,"RecJump");     
    //set Fps
    SetTargetFPS(60);
    // create canvas
    RenderTexture2D target = LoadRenderTexture(GAME_WIDTH,GAME_HEIGHT);

    //Main loop
    while(!WindowShouldClose()){

        // ----------- Main logic ----------------
        
                     
        if(IsKeyPressed(KEY_F11)){
            ToggleFullscreen();
        }
        player.Update(Plat_land,Spikess,camera.camera,maps.checkpoints,current_checkPoint);
        camera.updateCamera(player);


        // ---------------------------------------


        //---------------Draw the screen of game----------
        
        BeginTextureMode(target);

            //set background color
            ClearBackground(screenColor);

            //set camera
            camera.Begin();
                maps.Draw();
                player.DrawPlayer();
                //DrawTriangle({576,800},{640,800},{608,704},RED);
            camera.End();

        EndTextureMode();

        // a bunch of changes of window size 
        float windowWidth = GetScreenWidth();
        float windowHeight = GetScreenHeight();
        
        float scaleX = windowWidth / GAME_WIDTH;
        float scaleY = windowHeight / GAME_HEIGHT;
        float scale = std::min(scaleX,scaleY);
        
        float drawWidth = GAME_WIDTH * scale;
        float drawHeight = GAME_HEIGHT * scale;

        float offsetX = (windowWidth - drawWidth) / 2.0f;
        float offsetY = (windowHeight - drawHeight) / 2.0f;
       

        // Draw game 
        BeginDrawing();

        ClearBackground(BLACK);

        Rectangle source = {
            0,0,GAME_WIDTH,-GAME_HEIGHT
        };
        
        Rectangle dest = {
            offsetX,offsetY,drawWidth,drawHeight
        };

        DrawTexturePro(
            target.texture,
            source,
            dest,
            {0,0},
            0.0f,
            WHITE
        );
        

        EndDrawing();
        //-------------------------------------------------
    }

    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}
