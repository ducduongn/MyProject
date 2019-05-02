#ifndef BOARD_H_
#define BOARD_H_


#include "Common.h"
#include <stdlib.h>
#include <time.h>


// Define the dimensions of the board and bricks
#define BRICK_PER_ROW 15
#define BRICK_PER_COL 16
#define BOARD_BRWIDTH 64
#define BOARD_BRHEIGHT 24

#define HEARTCOUNT 3
#define HEART_WIDTH 50
#define HEART_HEIGHT 50;

#define SCORE_WIDTH 50
#define SCORE_HEIGHT 100
#define HIT_WIDTH 25
#define HIT_HEIGHT 60

#define WINDOWGAME_HEIGHT 650
#define WINDOWGAME_WIDTH 992

#define GAMEOVER_WIDTH 650
#define GAMEOVER_HEIGHT 460

class Score {
public:
    bool state;
    int type;
};

class Heart {
public:
    bool state;
};

class Brick {
public:
    int type;
    bool state;
    int status;

};

class Board: public Common {
public:
    Board(SDL_Renderer* renderer);
    ~Board();

    void Update(float delta);
    void Render(float delta);
    void CreateLevel();

    float brickoffsetx, brickoffsety;

    // Define the two-dimensional array of bricks
    Brick bricks[BRICK_PER_ROW][BRICK_PER_COL];
    //Define the array of hearts
    Heart hearts [HEARTCOUNT];


private:
    SDL_Texture* bricktexture;
    SDL_Texture* brokenbricktexture;
    SDL_Texture* sidetexture;
    SDL_Texture* logotexture;
    SDL_Texture* hearttexture;



};

#endif

