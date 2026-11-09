#include "game.hpp"
Game::Game():player({0,600,SIZE,SIZE},G,playerColor),camera(player,GAME_WIDTH,GAME_HEIGHT),target(),current_checkPoint(-1),current_Level(0){Init();}


// initialization
void Game::Init(){

    // Make window resizable
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    //init window
    InitWindow(GAME_WIDTH,GAME_HEIGHT,"RecJump");     

    //init audio
    InitAudioDevice();
    jumpSound = LoadSound("assets/audio/jumpAudio.wav");
    deathSound = LoadSound("assets/audio/deathAudio.wav"); 

    //set Fps
    SetTargetFPS(180);
    // create canvas
    target = LoadRenderTexture(GAME_WIDTH,GAME_HEIGHT);
    //ToggleFullscreen();
     

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


// running the game
void Game::Run(){
    while(!WindowShouldClose()){
        Update();
        Draw();
    }
}


// load data
void Game::LoadSave(){
    if(save.Load(data)){
        current_checkPoint = data.checkPointId;
        current_Level = data.currentLevelId;
    }
}

void Game::changeLevel(){
    if(IsKeyPressed(KEY_ONE)){
        current_checkPoint = -1;
        current_Level = 0;
    }
    else if(IsKeyPressed(KEY_TWO)){
        current_checkPoint = -1;
        current_Level = 1;
    }
    else if(IsKeyPressed(KEY_THREE)){
        current_checkPoint = -1;
        current_Level = 2;
    }
    else {
        return;
    }
    LoadLevel(current_Level);
}



// main update
void Game::Update(){

    //toggle full screen
    if(IsKeyPressed(KEY_F11)){
        ToggleFullscreen();
    }

    changeLevel();

    //player update 
    player.Update(
        Plat_Land,
        Spikess,
        camera.camera,
        maps.checkpoints,
        current_checkPoint
    );
     
    if(player.jumpEvent){
        PlaySound(jumpSound);
        player.jumpEvent = false;
    }
    
    
    if(player.deathEvent){
        PlaySound(deathSound);
        player.deathEvent = false;
    }
    

    triggerTrap();

    // reset the trap after the death animation done
    if(player.state == PlayerState::Respwaning){
        resetTrap();
        player.Reset(maps.checkpoints,current_checkPoint);
        player.state = PlayerState::Normal;
    }
    
    // update the camera
    camera.updateCamera(player);
    // save when hit the checkpoints
    SaveId();
    
    // next level
    if(CheckCollisionRecs(player.REC,maps.Exit)){
        nextLevel();
    }
}

void Game::DrawUi(){
    DrawFPS(10,10);

    DrawText("FullScreen: F11",10,40,45,RED);
    DrawText("Exit Game : esc",10,100,45,RED);


}

void Game::DrawGameWorld(){
    BeginTextureMode(target);
    ClearBackground(screenColor);

    camera.Begin();
    maps.Draw();
    player.DrawPlayer();
    camera.End();
    
    DrawUi();

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
    Spikess.clear();
    orignalSpikes.clear();
    activatedTrap.clear();
    
    //Load Map 
    maps.Load(levels[level]);
    //maps.transRecToSpike(); 

    player.REC.x = maps.spawn.x;
    player.REC.y = maps.spawn.y;
    player.RESET.x = maps.spawn.x;
    player.RESET.y = maps.spawn.y;

    Plat_Land.clear();
    Platform plats(maps.platforms,GRAY);
    Plat_Land.push_back(plats);
    Spikess = maps.spikes;
    orignalSpikes = Spikess;
    activatedTrap.assign(Spikess.size(),false);
    resetTrap();
     
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


// Draw the game
void Game::Draw(){
    DrawGameWorld();
    DrawGameScreen();
}

// active the trap
void Game::triggerTrap(){

    float dt = GetFrameTime();
    float spd = SPEED * 2.0;
      
    for(int i = 0;i < (int)maps.traps.size();i++){
        int idx = maps.traps[i].index;
        float player_center_x = player.REC.x + SIZE/2;
        float player_center_y = player.REC.y + SIZE/2;
        auto &spike1 = maps.spikes[idx];
        auto &spike2 = Spikess[idx];

        int limit = maps.traps[i].limit;
        int trap_speed = maps.traps[i].speed;
        int dir = maps.traps[i].direction;
       

        //trigger span
        float top_y = maps.traps[i].trigger_span_top_y;
        float down_y = maps.traps[i].trigger_span_down_y;
        float right_x = maps.traps[i].trigger_span_right_x;
        float left_x = maps.traps[i].trigger_span_left_x;

        //set speed of traps 
        spd = trap_speed == 114514 ? SPEED * 4.0 : trap_speed;  

         
        if(player_center_x >= left_x &&
            player_center_x <= right_x && 
            player_center_y <= down_y &&
            player_center_y >= top_y
            ){
            activatedTrap[idx] = true;
        }
        if(activatedTrap[idx]){
            if(dir == 0){ 
                if(spike1.bound.y > limit){
                    spike1.bound.y -= spd * dt;
                    spike1.hitbox.y -= spd * dt;

                    spike2.bound.y -= spd * dt;
                    spike2.hitbox.y -= spd * dt;
                }
                else {
                    spike1.bound.y = limit;
                    spike1.hitbox.y = limit;
                    
                    spike2.bound.y = limit;
                    spike2.hitbox.y = limit;
                }

            }
            else if(dir == 1){ 
                if(spike1.bound.x < limit){
                    spike1.bound.x += spd * dt;
                    spike1.hitbox.x += spd * dt;

                    spike2.bound.x += spd * dt;
                    spike2.hitbox.x += spd * dt;
                }
                else {
                    spike1.bound.x = limit;
                    spike1.hitbox.x = limit;
                    
                    spike2.bound.x = limit;
                    spike2.hitbox.x = limit;
                }
            }
            else if(dir == 2){
                if(spike1.bound.x > limit){
                    spike1.bound.x -= spd * dt;
                    spike1.hitbox.x -= spd * dt;

                    spike2.bound.x -= spd * dt;
                    spike2.hitbox.x -= spd * dt;
                }
                else {
                    spike1.bound.x = limit;
                    spike1.hitbox.x = limit;
                    
                    spike2.bound.x = limit;
                    spike2.hitbox.x = limit;
                }
            }
            else if(dir == 3){
 
                if(spike1.bound.y < limit){
                    spike1.bound.y += spd * dt;
                    spike1.hitbox.y += spd * dt;

                    spike2.bound.y += spd * dt;
                    spike2.hitbox.y += spd * dt;
                }
                else {
                    spike1.bound.y = limit;
                    spike1.hitbox.y = limit;
                    
                    spike2.bound.y = limit;
                    spike2.hitbox.y = limit;
                }


            }
        }
    }


}

// reset trap
void Game::resetTrap(){
    for(int i = 0;i<(int)maps.traps.size();i++){
        int idx = maps.traps[i].index;
        auto &spike1 = maps.spikes[idx];
        auto &spike2 = Spikess[idx];
        spike1 = spike2 = orignalSpikes[idx];
        activatedTrap[idx] = false;
    }
}

Game::~Game(){
    
    UnloadSound(jumpSound);
    UnloadSound(deathSound);
    CloseAudioDevice();
    UnloadRenderTexture(target); 
    CloseWindow();
}
