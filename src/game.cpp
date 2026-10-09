#include "game.hpp"

Game::Game():player({0,600,SIZE,SIZE},G,playerColor),camera(player,GAME_WIDTH,GAME_HEIGHT),target(),current_checkPoint(-1){Init();}


void Game::Init(){
     // Make window resizable
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    //init window
    InitWindow(GAME_WIDTH,GAME_HEIGHT,"RecJump");     
    //set Fps
    SetTargetFPS(60);
    // create canvas
    target = LoadRenderTexture(GAME_WIDTH,GAME_HEIGHT);

    //Load Map 
    maps.Load("assets/maps/map.tmj");
    maps.transRecToSpike(); 
    
    Plat_Land = {{maps.platforms,GRAY}};
    Spikess = maps.Sp;

    LoadSave();

}

void Game::Run(){
    while(!WindowShouldClose()){
        Update();
        Draw();
    }
}

void Game::LoadSave(){
    if(save.Load(data)){
        current_checkPoint = data.checkPointId;
        player.Reset(maps.checkpoints,current_checkPoint);
    }
}

void Game::Update(){
    if(IsKeyPressed(KEY_F11)){
        ToggleFullscreen();
    }

    player.Update(
        Plat_Land,
        Spikess,
        camera.camera,
        maps.checkpoints,
        current_checkPoint
            );
    
    camera.updateCamera(player);
}

void Game::DrawGameWorld(){
    BeginTextureMode(target);
    ClearBackground(screenColor);

    camera.Begin();
        maps.Draw();
        player.DrawPlayer();
    camera.End();

    EndTextureMode();
}


void Game::DrawGameScreen(){

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

}

void Game::Draw(){
    DrawGameWorld();
    DrawGameScreen();
}

Game::~Game(){
    UnloadRenderTexture(target); 
    CloseWindow();
}
