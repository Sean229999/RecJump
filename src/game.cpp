#include "game.hpp"

Game::Game():player({0,600,SIZE,SIZE},G,playerColor),camera(player,GAME_WIDTH,GAME_HEIGHT),target(),current_checkPoint(-1),current_Level(0){Init();}


void Game::Init(){
    // Make window resizable
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    //init window
    InitWindow(GAME_WIDTH,GAME_HEIGHT,"RecJump");     
    //set Fps
    SetTargetFPS(180);
    // create canvas
    target = LoadRenderTexture(GAME_WIDTH,GAME_HEIGHT);

    LoadSave();
    LoadLevel(current_Level);

    for(auto &cp : maps.checkpoints){
        if(cp.activated || cp.id <= current_checkPoint){
            cp.checkColor = GREEN;
            cp.checkColor = GREEN;
        }
    }
    player.Reset(maps.checkpoints,current_checkPoint);
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
        current_Level = data.currentLevelId;
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
    SaveId();

    if(CheckCollisionRecs(player.REC,maps.Exit)){
        nextLevel();
    }
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

void Game::LoadLevel(int level){

    if(level < 0 || level >= (int)levels.size())return;

    maps.Clear();

    //Load Map 
    maps.Load(levels[level]);
    maps.transRecToSpike(); 

    player.REC.x = maps.spawn.x;
    player.REC.y = maps.spawn.y;
    player.RESET.x = maps.spawn.x;
    player.RESET.y = maps.spawn.y;

    Plat_Land.clear();
    Platform plats(maps.platforms,GRAY);
    Plat_Land.push_back(plats);

    Spikess = maps.Sp;
    

}

void Game::nextLevel(){
    if(current_Level + 1 >= (int)levels.size()){
        return;
    }

    current_Level++;
    current_checkPoint = -1;

    data.currentLevelId = current_Level;
    data.checkPointId = current_checkPoint;
    
    save.Save(data);

    LoadLevel(current_Level);
}


void Game::SaveId(){
    for(auto& cp : maps.checkpoints){
        if (!CheckCollisionRecs(player.REC, cp.area))
            continue;

        if (cp.id <= current_checkPoint){
            continue;
        }

        cp.activated = true;
        current_checkPoint = cp.id;

        data.checkPointId = current_checkPoint;
        data.currentLevelId = current_Level;

        save.Save(data);

        break;
    }
 

}


void Game::Draw(){
    DrawGameWorld();
    DrawGameScreen();
}

Game::~Game(){
    UnloadRenderTexture(target); 
    CloseWindow();
}
