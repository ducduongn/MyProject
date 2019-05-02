#include "Board.h"

Board::Board(SDL_Renderer* renderer): Common(renderer) {

    SDL_Surface* surface = IMG_Load("PNG//bricks.png");
    bricktexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("PNG//brokenbricks.png");
    brokenbricktexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("PNG//side.png");
    sidetexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("PNG//logo.png");
    logotexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    surface = IMG_Load("PNG//heart.png");
    hearttexture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    srand(time(0));

    x = 16;
    y = 0;
    width = 960;
    height = 800;

    brickoffsetx = 0;
    brickoffsety = 0;
}

Board::~Board() {
    // Clean resources
    SDL_DestroyTexture(bricktexture);
    SDL_DestroyTexture(sidetexture);
    SDL_DestroyTexture(brokenbricktexture);

}

void Board::Update(float delta) {

}

void Board::Render(float delta) {
    // Render bricks
    for (int i=0; i<BRICK_PER_ROW; i++) {
        for (int j=0; j<BRICK_PER_COL; j++) {
            Brick brick = bricks[i][j];

            // Check if the brick exists
            if (!brick.state)
                continue;

            SDL_Rect srcrect;

            srcrect.x = (brick.type % 2) * BOARD_BRWIDTH;
            srcrect.y = (brick.type / 2) * BOARD_BRHEIGHT;
            srcrect.w = BOARD_BRWIDTH;
            srcrect.h = BOARD_BRHEIGHT;

            SDL_Rect dstrect;
            dstrect.x = brickoffsetx + x + i * BOARD_BRWIDTH;
            dstrect.y = brickoffsety + y + j * BOARD_BRHEIGHT;
            dstrect.w = BOARD_BRWIDTH;
            dstrect.h = BOARD_BRHEIGHT;

            if (brick.status == 1 )
                SDL_RenderCopy(renderer, brokenbricktexture, &srcrect, &dstrect);
            else
                SDL_RenderCopy(renderer, bricktexture, &srcrect, &dstrect);
        }
    }

    // Render sides
    SDL_Rect dstrect;
    dstrect.x = 0;
    dstrect.y = 0;
    dstrect.w = 16;
    dstrect.h = 650;
    SDL_RenderCopy(renderer, sidetexture, 0, &dstrect);

    dstrect.x = 992 - 16;
    dstrect.y = 0;
    dstrect.w = 16;
    dstrect.h = 650;
    SDL_RenderCopy(renderer, sidetexture, 0, &dstrect);

    // Render logo
    SDL_Rect drect;
    drect.x = 992;
    drect.y = 30;
    drect.w = 250;
    drect.h = 250;
    SDL_RenderCopy(renderer, logotexture, 0 , &drect);

    // Render heart
    for ( int i = 0 ; i < HEARTCOUNT; i++ ) {
        Heart heart = hearts [i];

        if ( !heart.state)
            continue;

        drect.x = 1010 + i* (HEART_WIDTH + 30) ;
        drect.y = 350;
        drect.w = HEART_WIDTH;
        drect.h = HEART_HEIGHT;
        SDL_RenderCopy(renderer, hearttexture, 0 , &drect);
    }

}

void Board::CreateLevel() {
    //Create bricks
    for (int i=0; i<BRICK_PER_ROW; i++) {
        for (int j=0; j<BRICK_PER_COL; j++) {
            Brick brick;

            brick.state = true;
            if ( j == 7 || j == 8)
                brick.state = false;
            if ( j >= 9 && j <= 15) {
                if ( i % 2 == 0 && j % 2 ==0)
                    brick.state = false;
            }
            if ( j == 9 ) {
                brick.type = 5;
                brick.status = 4;
            }

            else {
                brick.type = rand() % (4 - 0 + 1) + 0;
                brick.status = 2;
            }
            bricks[i][j] = brick;

        }
    }

    //Create Heart
    for ( int i=0; i<HEARTCOUNT; i++) {
        Heart heart;
        heart.state = true;
        hearts[i] = heart;
    }


}
