
#include "Game.h"

Mix_Music *music = NULL;
Mix_Chunk *hit_sound = NULL;
Mix_Chunk *levelup = NULL;
Mix_Chunk *brickhit = NULL;
Mix_Chunk *welldone = NULL;

Game::Game() {
    window = 0;
    renderer = 0;
}

Game::~Game() {

}

bool Game::Init() {
    bool success = true;
	SDL_Init(SDL_INIT_EVERYTHING);

	window = SDL_CreateWindow("BreakOut",
		SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
		WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
	if (!window) {
		std :: cout << "Error creating window:" << SDL_GetError() << std :: endl;
		success = false;
	}


	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	if (!renderer) {
		std::cout << "Error creating renderer:" << SDL_GetError() << std::endl;
		success = false;
	}

	if (TTF_Init() < 0) {
		std::cout << "TTF could not initialize!" << SDL_GetError() << std::endl;
		success = false;
	}

	if( Mix_OpenAudio( MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 4096 ) < 0 )
    {
        std::cout << "Mixer could not initialize!" << SDL_GetError() << std::endl;
        success = false;
    }


	last_tick = SDL_GetTicks();
	fps_tick = last_tick;
	fps = 0;
	framecount = 0;

    //Sound load
    music = Mix_LoadMUS("Sound//awesomeness.wav");
    if (music == NULL)
		std::cout << "Error loading music: " << SDL_GetError() << std:: endl;

    hit_sound = Mix_LoadWAV("Sound//hit.wav");
    if (hit_sound == NULL )
        std::cout << "Error loading hit sound: " << SDL_GetError() << std:: endl;

    levelup = Mix_LoadWAV("Sound//levelup.wav");
    if (levelup == NULL )
        std::cout << "Error loading hit sound: " << SDL_GetError() << std:: endl;

    brickhit = Mix_LoadWAV("Sound//brickhit.wav");
    if (brickhit == NULL )
        std::cout << "Error loading hit sound: " << SDL_GetError() << std:: endl;

    welldone = Mix_LoadWAV("Sound//WellDone.wav");
    if (welldone == NULL )
        std::cout << "Error loading hit sound: " << SDL_GetError() << std:: endl;

	return success;
}
void Game::Clean() {
    // Clean resources
    SDL_DestroyTexture(texture);

    // Clean renderer and window
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_FreeChunk(hit_sound);
    Mix_FreeMusic(music);
    Mix_FreeChunk(welldone);
    Mix_FreeChunk(levelup);
    Mix_FreeChunk(brickhit);
}

void Game::Run() {
    //framesStart = SDL_GetTicks();
    GameMenu();
    //Sound();
    background = new Background (renderer);
    board = new Board(renderer);
    paddle = new Paddle(renderer);
    ball = new Ball(renderer);

    NewGame();



    // Main loop
    while (1) {

        // Handler events
        SDL_Event e;
        if (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                break;
            }
        }
        /*
        frameTime = SDL_GetTicks() - framesStart;
        float delta = frameTime / 1000.0f;
        if (frameDelay > frameTime)
            SDL_Delay(frameDelay - frameTime);
        */

        // Calculate delta and fps
       unsigned int current_tick = SDL_GetTicks();
        float delta = (current_tick - last_tick) / 1000.0f;
        //std:: cout << current_tick <<  " " << last_tick
        //<< " " << delta << " " << fps_tick << " " << framecount<< std :: endl;
        if (current_tick - fps_tick >= FPS_DELAY) {
            fps = framecount * (1000.0f / (current_tick - fps_tick));
            fps_tick = current_tick;
            framecount = 0;

            char GameTitle[50];
            snprintf(GameTitle, 100, "BreakOut (fps: %u)", fps);
            SDL_SetWindowTitle(window, GameTitle);
        } else {
            framecount++;
        }
        last_tick = current_tick;



        // Update and render the game
        Update(delta);
        Render(delta);

        //Gameover
        if (BrickCount() == 0 || LifeCount == 0) {
            Gameover();
            GameMenu();
            NewGame();

        }

    }
    delete paddle;
    delete ball;
    delete background;
    delete board;

    Clean();

    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    Mix_CloseAudio();
}


void Game::GameMenu() {
    bool success = true;
    if (Mix_PlayingMusic() == 0 )
		Mix_PlayMusic(music, -1);
	SDL_Surface* gSurface = IMG_Load("PNG//menu.jpg");
	texture = SDL_CreateTextureFromSurface(renderer, gSurface);
	SDL_FreeSurface(gSurface);

    SDL_Rect dRect;

    dRect.x = 0;
    dRect.y = 0;
    dRect.w = WINDOW_WIDTH;
    dRect.h = WINDOW_HEIGHT;
    SDL_RenderCopy(renderer, texture, 0, &dRect);

    SDL_Texture* BreakOut = NULL;
    SDL_Texture* wStart = NULL;
    SDL_Texture* rStart = NULL;
    SDL_Texture* wExit = NULL;
    SDL_Texture* rExit = NULL;
    TTF_Font* TittleFont = NULL;
    TTF_Font* GameFont = NULL;


    //SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
   // SDL_RenderClear(renderer);

    TittleFont = TTF_OpenFont("font//pdark.ttf",800);
    if (TittleFont == NULL)
        std::cout << "Error load font: " << SDL_GetError() << std::endl;

    GameFont = TTF_OpenFont("font//neutro.ttf", 800);
    if (GameFont == NULL)
        std::cout << "Error load font: " << SDL_GetError() << std::endl;

	SDL_Color WHITE = {255,255,255,255};
	SDL_Color RED = {255,0,0,0};

    std :: string game_text = "BreakOut";
    gSurface = TTF_RenderText_Solid(TittleFont, game_text.c_str(), WHITE );
    BreakOut = SDL_CreateTextureFromSurface(renderer, gSurface);
    SDL_FreeSurface(gSurface);


    dRect.x = WINDOW_WIDTH/2 - 250;
    dRect.y = 15;
    dRect.w = 500;
    dRect.h = 190;
    SDL_RenderCopy(renderer, BreakOut, 0, &dRect);

    std:: string start_text = "START";
    gSurface = TTF_RenderText_Solid(GameFont, start_text.c_str(), WHITE);
    wStart = SDL_CreateTextureFromSurface(renderer, gSurface);
    SDL_FreeSurface(gSurface);

    gSurface = TTF_RenderText_Solid(GameFont, start_text.c_str(), RED);
    rStart = SDL_CreateTextureFromSurface(renderer, gSurface);
    SDL_FreeSurface(gSurface);

    SDL_Rect StartRect;
    StartRect.x = WINDOW_WIDTH/2 -100;
    StartRect.y = 300;
    StartRect.w = 200;
    StartRect.h = 100;
    SDL_RenderCopy(renderer, wStart, 0, &StartRect);

    std:: string exit_text = "EXIT";
    gSurface = TTF_RenderText_Solid(GameFont, exit_text.c_str(), WHITE);
    wExit = SDL_CreateTextureFromSurface(renderer, gSurface);
    SDL_FreeSurface(gSurface);

    gSurface = TTF_RenderText_Solid(GameFont, exit_text.c_str(), RED);
    rExit = SDL_CreateTextureFromSurface(renderer, gSurface);
    SDL_FreeSurface(gSurface);

    SDL_Rect ExitRect;
    ExitRect.x = WINDOW_WIDTH/2 -176/2;
    ExitRect.y = 450;
    ExitRect.w = 176;
    ExitRect.h = 95;
    SDL_RenderCopy(renderer, wExit, 0, &ExitRect);


    SDL_RenderPresent(renderer);

    int start = 0 ;
	while (success)
	{
		SDL_Event g_event;
		while (SDL_PollEvent(&g_event)) {
			if (g_event.type == SDL_QUIT) {
				SDL_DestroyWindow(window);
				SDL_Quit();
				exit(0);
				break;
			}
			else if (g_event.type == SDL_KEYDOWN) {
				switch (g_event.key.keysym.sym)
				{
                case SDLK_UP:
                    SDL_RenderCopy(renderer, rStart, 0 , &StartRect);
                    SDL_RenderCopy(renderer, wExit, 0, &ExitRect);
                    SDL_RenderPresent(renderer);
                    start = 1;
                    break;
                case SDLK_DOWN:
                    SDL_RenderCopy(renderer, rExit, 0 , &ExitRect);
                    SDL_RenderCopy(renderer, wStart, 0 , &StartRect);
                    SDL_RenderPresent(renderer);
                    start = 2;
                    break;
                case SDLK_RETURN:
                    if ( start == 1)
                        success = false;
                    else if ( start == 2 ) {
                        SDL_DestroyWindow(window);
                        SDL_Quit();
                        exit(0);
                    }
                    else
                        continue;
                    break;
				}
			}
		}
	}

    SDL_DestroyTexture(wStart);
    SDL_DestroyTexture(BreakOut);
    SDL_DestroyTexture(wExit);
    SDL_DestroyTexture(rStart);
    SDL_DestroyTexture(rExit);
    TTF_CloseFont(TittleFont);
    TTF_CloseFont(GameFont);
    Mix_HaltMusic();

}

void Game::NewGame() {
    paddle->frame = 0;
    explosion_frame = 0;
    LifeCount = 3;
    GameScore = 0;
    hit_times = 0;
    level = 0;
    onetime_1 = true;
    onetime_2 = true;
    onetime_3 = true;

    board->CreateLevel();
    ResetPaddle();
}

int Game::BrickCount() {
    int brickcount = 0;
    for (int i=0; i<BRICK_PER_ROW; i++) {
        for (int j=0; j<BRICK_PER_COL; j++) {
            Brick brick = board->bricks[i][j];
            if (brick.state) {
                brickcount++;
            }
        }
    }

    return brickcount;
}

void Game :: Gameover() {
    if ( Mix_PlayChannel(-1,welldone,0) == -1)
                        std:: cout << "Error playing gameover sound: " << Mix_GetError << std::endl;
    SDL_Surface* gSurface= IMG_Load("PNG//gameover.png");
    SDL_Texture* gameovertexture = SDL_CreateTextureFromSurface(renderer, gSurface);
    SDL_FreeSurface(gSurface);

    SDL_Rect drect;
    drect.x = WINDOWGAME_WIDTH/2 - GAMEOVER_WIDTH/2;
    drect.y = WINDOWGAME_HEIGHT/2 - GAMEOVER_HEIGHT/2 -100;
    drect.w = GAMEOVER_WIDTH;
    drect.h = GAMEOVER_HEIGHT;
    SDL_RenderCopy(renderer, gameovertexture, 0 , &drect);

    TTF_Font* text = TTF_OpenFont("font//arcade.ttf", 60);
    if (text == NULL){
        std::cout << "Error load font: " << SDL_GetError() << std::endl;
    }
    std:: string Game = "Press Space to replay";
    SDL_Color ORANGE = {255,128,0};

    gSurface = TTF_RenderText_Solid(text, Game.c_str(),ORANGE);
    SDL_Texture* gTexture = SDL_CreateTextureFromSurface(renderer, gSurface);
    SDL_FreeSurface(gSurface);

    SDL_Rect gRect;
    gRect.x = WINDOWGAME_WIDTH/2 - 630/2;
    gRect.y = 400;
    gRect.w = 630;
    gRect.h = 60;
    SDL_RenderCopy(renderer, gTexture, 0 , &gRect);

    SDL_RenderPresent(renderer);
	bool success = true;
	while (success)
	{
		SDL_RenderClear(renderer);
		SDL_Event g_event;
		while (SDL_PollEvent(&g_event)) {
			if (g_event.type == SDL_QUIT) {
				SDL_DestroyWindow(window);
				SDL_Quit();
				exit(0);
				break;
			}
			else if (g_event.key.keysym.sym == SDLK_SPACE) {
				success = false;
				break;
			}
		}
    }
    SDL_DestroyTexture(gameovertexture);
    SDL_DestroyTexture(gTexture);
    TTF_CloseFont(text);
}

void Game::ResetPaddle() {
    paddlestick = true;
    ResetBall();
    hit_times = 0;
}

void Game::ResetBall() {
    ball->x = paddle->x + paddle->width/2 - ball->width/2;
    ball->y = paddle->y - ball->height;
}

void Game::Update(float delta) {

    int mouseX, mouseY;
    Uint8 mouse_state = SDL_GetMouseState(&mouseX, &mouseY);
    SetPaddleX(mouseX - paddle->width/2.0f);

    if (mouse_state&SDL_BUTTON(1)) {   // Click left-mouse to start
        if (paddlestick) {
            paddlestick = false;
            ball->SetDirection(1, 1);
        }
    }

    if (paddlestick) {
        ResetBall();
    }

    isBoardCollides();
    isPaddleCollides();
    isBrickCollides2();


    board->Update(delta);
    //Explosion();
    paddle->Update(delta);


    if (!paddlestick)
        ball->Update(delta);

}

void Game::SetPaddleX(float x) {
    float newx;
    if (x < board->x) {
        // Upper bound
        newx = board->x;
    } else if (x + paddle->width > board->x + board->width) {
        // Lower bound
        newx = board->x + board->width - paddle->width;
    } else {
        newx = x;
    }
    paddle->x = newx;
}

void Game::isBoardCollides() {
    // Top and bottom collisions
    if (ball->y < board->y) {
        // Top
        ball->y = board->y;
        ball->diry *= -1;
    } else if (ball->y > board->y + board->height) {
        // Bottom

        // Ball lost
        LifeCount--;
        board -> hearts[LifeCount].state = false;
        ResetPaddle();
    }

    // Left and right collision
    if (ball->x <= board->x) {
        // Left
        ball->x = board->x;
        ball->dirx *= -1;
    } else if (ball->x + ball->width >= board->x + board->width) {
        // Right
        ball->x = board->x + board->width - ball->width;
        ball->dirx *= -1;
    }
}

float Game::GetReflection(float hitx) {
    //  hitx within the width of the paddle
    if (hitx < 0) {
        hitx = 0;
    } else if (hitx > paddle->width) {
        hitx = paddle->width;
    }

    // Everything to the left of the center of the paddle is reflected to the left
    // while everything right of the center is reflected to the right
    hitx -= paddle->width / 2.0f;

    // Scale the reflection ranging from -2.0f to 2.0f
    return 2.0f * (hitx / (paddle->width / 2.0f));
}


void Game::isPaddleCollides() {
    // Get the center x-coordinate of the ball
    float ballcenterx = ball->x + ball->width / 2.0f;

    // Check paddle collision
    if (ball->IsCollides(paddle)) {
        hit_times = 0;
        ball->y = paddle->y - ball->height;
        ball->SetDirection(GetReflection(ballcenterx - paddle->x), -1);
        if ( Mix_PlayChannel(-1,hit_sound,0) == -1)
                        std:: cout << "Error playing sound: " << Mix_GetError << std::endl;
        //ball->SetDirection(0, -1);
    }
}

void Game::isBrickCollides() {
    for (int i=0; i<BRICK_PER_ROW; i++) {
        for (int j=0; j<BRICK_PER_COL; j++) {
            Brick brick = board->bricks[i][j];

            // Check if brick is present
            if (brick.state) {
                // Brick x and y coordinates
                float brickx = board->brickoffsetx + board->x + i*BOARD_BRWIDTH;
                float bricky = board->brickoffsety + board->y + j*BOARD_BRHEIGHT;

                // Check ball-brick collision

                float w = 0.5f * (ball->width + BOARD_BRWIDTH);
                float h = 0.5f * (ball->height + BOARD_BRHEIGHT);
                float dx = (ball->x + 0.5f*ball->width) - (brickx + 0.5f*BOARD_BRWIDTH);
                float dy = (ball->y + 0.5f*ball->height) - (bricky + 0.5f*BOARD_BRHEIGHT);

                if (fabs(dx) <= w && fabs(dy) <= h) {
                    // Collision confirmed
                    board->bricks[i][j].state = false;

                    float wy = w * dy;
                    float hx = h * dx;

                    if (wy > hx) {
                        if (wy > -hx) {
                            BallBrickResponse(3); // Bottom
                        } else {
                            BallBrickResponse(0);  // Left
                        }
                    } else {
                        if (wy > -hx) {
                            BallBrickResponse(2); // Right

                        } else {
                            BallBrickResponse(1); // Top
                        }
                    }
                    return;
                }
            }
        }
    }
}

void Game::isBrickCollides2() {
    /*
    SDL_Surface* surface = IMG_Load("PNG//explosion.png");
    texture = SDL_CreateTextureFromSurface(renderer , surface);
    SDL_FreeSurface(surface);
    set_clips();
    SDL_Rect desRect;
*/

    for (int i=0; i<BRICK_PER_ROW; i++) {
        for (int j=0; j<BRICK_PER_COL; j++) {
            Brick brick = board->bricks[i][j];

            // Check brick's state
            if (brick.state) {
                // Brick x and y coordinates
                float brickx = board->brickoffsetx + board->x + i*BOARD_BRWIDTH;
                float bricky = board->brickoffsety + board->y + j*BOARD_BRHEIGHT;

                // Center of the ball x and y coordinates
                float ballcenterx = ball->x + 0.5f*ball->width;
                float ballcentery = ball->y + 0.5f*ball->height;

                // Center of the brick x and y coordinates
                float brickcenterx = brickx + 0.5f*BOARD_BRWIDTH;
                float brickcentery = bricky + 0.5f*BOARD_BRHEIGHT;

                if (ball->x <= brickx + BOARD_BRWIDTH && ball->x+ball->width >= brickx
                    && ball->y <= bricky + BOARD_BRHEIGHT && ball->y + ball->height >= bricky) {
                    // // Collision confirmed, delete bricks

                    if ( Mix_PlayChannel(-1,brickhit,0) == -1)
                        std:: cout << "Error playing brick sound: " << Mix_GetError << std::endl;
                    board -> bricks[i][j].status--; // Update broken bricks
                    hit_times++;

                    if ( board -> bricks[i][j].status == 0 ){
                        board->bricks[i][j].state = false;
                        GameScore += hit_times * SCORE_PER_BRICK;

                    }


                    // Asume the ball goes slow enough to not skip through the bricks

                    // Calculate ysize
                    float ymin = 0;
                    if (bricky > ball->y) {
                        ymin = bricky;
                    } else {
                        ymin = ball->y;
                    }

                    float ymax = 0;
                    if (bricky+BOARD_BRHEIGHT < ball->y+ball->height) {
                        ymax = bricky+BOARD_BRHEIGHT;
                    } else {
                        ymax = ball->y+ball->height;
                    }

                    float ysize = ymax - ymin;

                    // Calculate xsize
                    float xmin = 0;
                    if (brickx > ball->x) {
                        xmin = brickx;
                    } else {
                        xmin = ball->x;
                    }

                    float xmax = 0;
                    if (brickx+BOARD_BRWIDTH < ball->x+ball->width) {
                        xmax = brickx+BOARD_BRWIDTH;
                    } else {
                        xmax = ball->x+ball->width;
                    }

                    float xsize = xmax - xmin;

                    // The origin is at the top-left corner of the screen
                    // Set collision response
                    if (xsize > ysize) {
                        if (ballcentery > brickcentery) {
                            // Bottom
                            ball->y += ysize + 0.01f; // Move out of collision
                            BallBrickResponse(3);
                        } else {
                            // Top
                            ball->y -= ysize + 0.01f; // Move out of collision
                            BallBrickResponse(1);
                        }
                    } else {
                        if (ballcenterx < brickcenterx) {
                            // Left
                            ball->x -= xsize + 0.01f; // Move out of collision
                            BallBrickResponse(0);
                        } else {
                            // Right
                            ball->x += xsize + 0.01f; // Move out of collision
                            BallBrickResponse(2);
                        }
                    }

                    return;
                }
            }
        }
    }
}


void Game::BallBrickResponse(int dirindex) {
    // Define statistic 0: Left, 1: Top, 2: Right, 3: Bottom

    // Direction factors
    int mulx = 1;
    int muly = 1;

    if (ball->dirx > 0) {
        // Ball => + x direction
        if (ball->diry > 0) {
            // Ball => + y direction

            if (dirindex == 0 || dirindex == 3) {
                mulx = -1;
            } else {
                muly = -1;
            }
        } else if (ball->diry < 0) {
            // Ball => - y direction

            if (dirindex == 0 || dirindex == 1) {
                mulx = -1;
            } else {
                muly = -1;
            }
        }
    } else if (ball->dirx < 0) {
        // Ball => - x direction
        if (ball->diry > 0) {
            // Ball => + y direction

            if (dirindex == 2 || dirindex == 3) {
                mulx = -1;
            } else {
                muly = -1;
            }
        } else if (ball->diry < 0) {
            // Ball => - y direction

            if (dirindex == 1 || dirindex == 2) {
                mulx = -1;
            } else {
                muly = -1;
            }
        }
    }

    // Ball's new direction = old direction * determined direction factors
    ball->SetDirection(mulx*ball->dirx, muly*ball->diry);
}


void Game::Render(float delta) {
    SDL_RenderClear(renderer);

    background -> Render(delta);
    board->Render(delta);
    paddle->Render(delta);
    ball->Render(delta);

    ScoreUpdate(GameScore, hit_times);

    SDL_RenderPresent(renderer);
}


void Game:: ScoreUpdate(int score, int hit) {
    SDL_Texture* wScore = NULL;
    SDL_Texture* yScore = NULL;
    SDL_Texture* oScore = NULL;
    SDL_Texture* rScore = NULL;
    SDL_Texture* Hit_textute = NULL;
    SDL_Texture* ComboTexture = NULL;
    SDL_Texture* xTexture = NULL;

    int length, hit_length;
    int combo_length = 5*HIT_WIDTH;
    if (score < 10) {
        length = 1*SCORE_WIDTH;
    }
    else if (score >=10 && score < 100){
        length = 2*SCORE_WIDTH;
    }
    else if (score >=100 && score < 1000){
        length = 3*SCORE_WIDTH;
    }

    else if ( score >=1000 && score < 10000) {
        length = 4*SCORE_WIDTH;
    }
    else{
        length = 5*SCORE_WIDTH;
    }

    //Levelup sound
        if ( score >= 100 && score< 500)
            level = 1;
        else if ( score >= 500 && score < 1000)
            level = 2;
        else if ( score >= 1000 && score < 10000)
            level = 3;

        if (level == 1 && onetime_1 == true) {
            if ( Mix_PlayChannel(-1,levelup,1) == -1)
                std:: cout << "Error playing sound: " << Mix_GetError << std::endl;
            onetime_1 = false;
        }
        else if (level == 2 && onetime_2 == true) {
            if ( Mix_PlayChannel(-1,levelup,1) == -1)
                std:: cout << "Error playing sound: " << Mix_GetError << std::endl;
            onetime_2 = false;
        }
        else if (level == 3 && onetime_3 == true) {
            if ( Mix_PlayChannel(-1,levelup,1) == -1)
                std:: cout << "Error playing sound: " << Mix_GetError << std::endl;
            onetime_3 = false;
        }

    TTF_Font* score_font = TTF_OpenFont("font//arcade.ttf", 60);
    if ( score_font == NULL) {
        std::cout << "Error load font: " << SDL_GetError() << std::endl;
    }

    SDL_Color YELLOW = {255,255,0,255};
    SDL_Color WHITE = {255,255,255,255};
    SDL_Color ORANGE = {255,128,0};
    SDL_Color RED = {255,0,0,0};
    SDL_Color GREEN = {0,255,0};

    std::string score_text = std :: to_string (score);
    SDL_Surface* surface = TTF_RenderText_Solid(score_font, score_text.c_str(), WHITE);
    wScore = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderText_Solid(score_font, score_text.c_str(), YELLOW);
    yScore = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderText_Solid(score_font, score_text.c_str(), ORANGE);
    oScore = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderText_Solid(score_font, score_text.c_str(), RED);
    rScore = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    SDL_Rect score_rect;
    score_rect.x = WINDOWGAME_WIDTH + 250/2 - length/2;
    score_rect.y = 430;
    score_rect.w = length;
    score_rect.h = SCORE_HEIGHT;
    int times = 0;

    if (score < 100){
        SDL_RenderCopy(renderer, wScore, 0 , &score_rect);
    }
    else if (score >=100 && score < 500) {
        SDL_RenderCopy(renderer, yScore, 0 , &score_rect);
    }
    else if (score >=500 && score < 1000) {
        SDL_RenderCopy(renderer, oScore, 0 , &score_rect);
    }
    else if ( score >= 1000 ) {
        SDL_RenderCopy(renderer, rScore, 0 , &score_rect);
    }

    std::string combo = "COMBO";
    std::string hit_text = std::to_string(hit);
    std::string x = "x";

    surface = TTF_RenderText_Solid(score_font, hit_text.c_str(), GREEN);
    Hit_textute = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderText_Solid(score_font, combo.c_str(), GREEN);
    ComboTexture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    surface = TTF_RenderText_Solid(score_font, x.c_str(), GREEN);
    xTexture = SDL_CreateTextureFromSurface(renderer,surface);
    SDL_FreeSurface(surface);

    SDL_Rect HitRect;
    if ( hit < 10) {
        hit_length = 1*HIT_WIDTH;
        SDL_Rect HitRect;
        HitRect.x = WINDOWGAME_WIDTH + 250/2 - hit_length/2;
        HitRect.y = 590;
        HitRect.w = hit_length;
        HitRect.h = HIT_HEIGHT;
        SDL_RenderCopy(renderer, Hit_textute, 0, &HitRect);
    } else if ( hit >=10 && hit < 1000) {
        hit_length = 2*HIT_WIDTH;
        HitRect.x = WINDOWGAME_WIDTH + 250/2 - hit_length/2;
        HitRect.y = 590;
        HitRect.w = hit_length;
        HitRect.h = HIT_HEIGHT;
        SDL_RenderCopy(renderer, Hit_textute, 0, &HitRect);
    } else if ( hit > 1000){
        hit_length = 3*HIT_WIDTH;
        HitRect.x = WINDOWGAME_WIDTH + 250/2 - hit_length/2;
        HitRect.y = 590;
        HitRect.w = hit_length;
        HitRect.h = HIT_HEIGHT;
        SDL_RenderCopy(renderer, Hit_textute, 0, &HitRect);
    }

    SDL_Rect ComboRect;
    ComboRect.x = WINDOWGAME_WIDTH + 250/2 - combo_length/2;
    ComboRect.y = 520;
    ComboRect.w = combo_length;
    ComboRect.h = HIT_HEIGHT;
    SDL_RenderCopy(renderer, ComboTexture, 0, &ComboRect);

    SDL_Rect xRect;
    xRect.x = WINDOWGAME_WIDTH + 250/2 - 40/2;
    xRect.y = 550;
    xRect.w = 40;
    xRect.h = 60;
    SDL_RenderCopy(renderer, xTexture, 0, &xRect);

    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(wScore);
    SDL_DestroyTexture(yScore);
    SDL_DestroyTexture(oScore);
    SDL_DestroyTexture(rScore);
    SDL_DestroyTexture(Hit_textute);
    SDL_DestroyTexture(ComboTexture);
    SDL_DestroyTexture(xTexture);

    TTF_CloseFont(score_font);
}
/*
void Game :: Sound() {

    Mix_Music *music = NULL;

    music = Mix_LoadMUS("Sound//ObservingTheStar.wav");

	if (music == NULL)
		std::cout << "Error loading music: " << SDL_GetError() << std:: endl;
    //if ( Mix_PlayChannel(-1, wave, 0) == -1 )
    if (Mix_PlayingMusic() == 0 )
		Mix_PlayMusic(music, -1);

    Mix_FreeMusic(music);
}
*/

/*
void Game:: set_clips() {
    if ( EXPLODE_FRAME_HEIGHT > 0 && EXPLODE_FRAME_WIDTH > 0) {
                frame_clip[0].x = 0;
                frame_clip[0].y = 0;
                frame_clip[0].w = EXPLODE_FRAME_WIDTH;
                frame_clip[0].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[1].x = EXPLODE_FRAME_WIDTH;
                frame_clip[1].y = 0;
                frame_clip[1].w = EXPLODE_FRAME_WIDTH;
                frame_clip[1].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[2].x = 2*EXPLODE_FRAME_WIDTH;
                frame_clip[2].y = 0;
                frame_clip[2].w = EXPLODE_FRAME_WIDTH;
                frame_clip[2].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[3].x = 3 * EXPLODE_FRAME_WIDTH;
                frame_clip[3].y = 0;
                frame_clip[3].w = EXPLODE_FRAME_WIDTH;
                frame_clip[3].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[4].x = 4*EXPLODE_FRAME_WIDTH;
                frame_clip[4].y = 0;
                frame_clip[4].w = EXPLODE_FRAME_WIDTH;
                frame_clip[4].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[5].x = 5 * EXPLODE_FRAME_WIDTH;
                frame_clip[5].y = 0;
                frame_clip[5].w = EXPLODE_FRAME_WIDTH;
                frame_clip[5].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[6].x = 6 * EXPLODE_FRAME_WIDTH;
                frame_clip[6].y = 0;
                frame_clip[6].w = EXPLODE_FRAME_WIDTH;
                frame_clip[6].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[7].x = 7 * EXPLODE_FRAME_WIDTH;
                frame_clip[7].y = 0;
                frame_clip[7].w = EXPLODE_FRAME_WIDTH;
                frame_clip[7].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[8].x = 0;
                frame_clip[8].y = EXPLODE_FRAME_HEIGHT;
                frame_clip[8].w = EXPLODE_FRAME_WIDTH;
                frame_clip[8].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[9].x = EXPLODE_FRAME_WIDTH;
                frame_clip[9].y = EXPLODE_FRAME_HEIGHT;
                frame_clip[9].w = EXPLODE_FRAME_WIDTH;
                frame_clip[9].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[10].x = 2*EXPLODE_FRAME_WIDTH;
                frame_clip[10].y = EXPLODE_FRAME_HEIGHT;
                frame_clip[10].w = EXPLODE_FRAME_WIDTH;
                frame_clip[10].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[11].x = 3 * EXPLODE_FRAME_WIDTH;
                frame_clip[11].y = EXPLODE_FRAME_HEIGHT;
                frame_clip[11].w = EXPLODE_FRAME_WIDTH;
                frame_clip[11].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[12].x = 4*EXPLODE_FRAME_WIDTH;
                frame_clip[12].y = EXPLODE_FRAME_HEIGHT;
                frame_clip[12].w = EXPLODE_FRAME_WIDTH;
                frame_clip[12].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[13].x = 5 * EXPLODE_FRAME_WIDTH;
                frame_clip[13].y = EXPLODE_FRAME_HEIGHT;
                frame_clip[13].w = EXPLODE_FRAME_WIDTH;
                frame_clip[13].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[14].x = 6 * EXPLODE_FRAME_WIDTH;
                frame_clip[14].y = EXPLODE_FRAME_HEIGHT;
                frame_clip[14].w = EXPLODE_FRAME_WIDTH;
                frame_clip[14].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[15].x = 7 * EXPLODE_FRAME_WIDTH;
                frame_clip[15].y = EXPLODE_FRAME_HEIGHT;
                frame_clip[15].w = EXPLODE_FRAME_WIDTH;
                frame_clip[15].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[16].x = 0;
                frame_clip[16].y = 2*EXPLODE_FRAME_HEIGHT;
                frame_clip[16].w = EXPLODE_FRAME_WIDTH;
                frame_clip[16].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[17].x = EXPLODE_FRAME_WIDTH;
                frame_clip[17].y = 2*EXPLODE_FRAME_HEIGHT;
                frame_clip[17].w = EXPLODE_FRAME_WIDTH;
                frame_clip[17].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[18].x = 2*EXPLODE_FRAME_WIDTH;
                frame_clip[18].y = 2*EXPLODE_FRAME_HEIGHT;
                frame_clip[18].w = EXPLODE_FRAME_WIDTH;
                frame_clip[18].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[19].x = 3 * EXPLODE_FRAME_WIDTH;
                frame_clip[19].y = 2*EXPLODE_FRAME_HEIGHT;
                frame_clip[19].w = EXPLODE_FRAME_WIDTH;
                frame_clip[19].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[20].x = 4*EXPLODE_FRAME_WIDTH;
                frame_clip[20].y = 2*EXPLODE_FRAME_HEIGHT;
                frame_clip[20].w = EXPLODE_FRAME_WIDTH;
                frame_clip[20].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[21].x = 5 * EXPLODE_FRAME_WIDTH;
                frame_clip[21].y = 2*EXPLODE_FRAME_HEIGHT;
                frame_clip[21].w = EXPLODE_FRAME_WIDTH;
                frame_clip[21].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[22].x = 6 * EXPLODE_FRAME_WIDTH;
                frame_clip[22].y = 2*EXPLODE_FRAME_HEIGHT;
                frame_clip[22].w = EXPLODE_FRAME_WIDTH;
                frame_clip[22].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[23].x = 7 * EXPLODE_FRAME_WIDTH;
                frame_clip[23].y = 2*EXPLODE_FRAME_HEIGHT;
                frame_clip[23].w = EXPLODE_FRAME_WIDTH;
                frame_clip[23].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[24].x = 0;
                frame_clip[24].y = 3*EXPLODE_FRAME_HEIGHT;
                frame_clip[24].w = EXPLODE_FRAME_WIDTH;
                frame_clip[24].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[25].x = EXPLODE_FRAME_WIDTH;
                frame_clip[25].y = 3*EXPLODE_FRAME_HEIGHT;
                frame_clip[25].w = EXPLODE_FRAME_WIDTH;
                frame_clip[25].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[26].x = 2*EXPLODE_FRAME_WIDTH;
                frame_clip[26].y = 3*EXPLODE_FRAME_HEIGHT;
                frame_clip[26].w = EXPLODE_FRAME_WIDTH;
                frame_clip[26].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[27].x = 3 * EXPLODE_FRAME_WIDTH;
                frame_clip[27].y = 3*EXPLODE_FRAME_HEIGHT;
                frame_clip[27].w = EXPLODE_FRAME_WIDTH;
                frame_clip[27].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[28].x = 4*EXPLODE_FRAME_WIDTH;
                frame_clip[28].y = 3*EXPLODE_FRAME_HEIGHT;
                frame_clip[28].w = EXPLODE_FRAME_WIDTH;
                frame_clip[28].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[29].x = 5 * EXPLODE_FRAME_WIDTH;
                frame_clip[29].y = 3*EXPLODE_FRAME_HEIGHT;
                frame_clip[29].w = EXPLODE_FRAME_WIDTH;
                frame_clip[29].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[30].x = 6 * EXPLODE_FRAME_WIDTH;
                frame_clip[30].y = 3*EXPLODE_FRAME_HEIGHT;
                frame_clip[30].w = EXPLODE_FRAME_WIDTH;
                frame_clip[30].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[31].x = 7 * EXPLODE_FRAME_WIDTH;
                frame_clip[31].y = 3*EXPLODE_FRAME_HEIGHT;
                frame_clip[31].w = EXPLODE_FRAME_WIDTH;
                frame_clip[31].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[32].x = 0;
                frame_clip[32].y = 4*EXPLODE_FRAME_HEIGHT;
                frame_clip[32].w = EXPLODE_FRAME_WIDTH;
                frame_clip[32].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[33].x = EXPLODE_FRAME_WIDTH;
                frame_clip[33].y = 4*EXPLODE_FRAME_HEIGHT;
                frame_clip[33].w = EXPLODE_FRAME_WIDTH;
                frame_clip[33].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[34].x = 2*EXPLODE_FRAME_WIDTH;
                frame_clip[34].y = 4*EXPLODE_FRAME_HEIGHT;
                frame_clip[34].w = EXPLODE_FRAME_WIDTH;
                frame_clip[34].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[35].x = 3 * EXPLODE_FRAME_WIDTH;
                frame_clip[35].y = 4*EXPLODE_FRAME_HEIGHT;
                frame_clip[35].w = EXPLODE_FRAME_WIDTH;
                frame_clip[35].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[36].x = 4*EXPLODE_FRAME_WIDTH;
                frame_clip[36].y = 4*EXPLODE_FRAME_HEIGHT;
                frame_clip[36].w = EXPLODE_FRAME_WIDTH;
                frame_clip[36].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[37].x = 5 * EXPLODE_FRAME_WIDTH;
                frame_clip[37].y = 4*EXPLODE_FRAME_HEIGHT;
                frame_clip[37].w = EXPLODE_FRAME_WIDTH;
                frame_clip[37].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[38].x = 6 * EXPLODE_FRAME_WIDTH;
                frame_clip[38].y = 4*EXPLODE_FRAME_HEIGHT;
                frame_clip[38].w = EXPLODE_FRAME_WIDTH;
                frame_clip[38].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[39].x = 7 * EXPLODE_FRAME_WIDTH;
                frame_clip[39].y = 4*EXPLODE_FRAME_HEIGHT;
                frame_clip[39].w = EXPLODE_FRAME_WIDTH;
                frame_clip[39].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[40].x = 0;
                frame_clip[40].y = 5*EXPLODE_FRAME_HEIGHT;
                frame_clip[40].w = EXPLODE_FRAME_WIDTH;
                frame_clip[40].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[41].x = EXPLODE_FRAME_WIDTH;
                frame_clip[41].y = 5*EXPLODE_FRAME_HEIGHT;
                frame_clip[41].w = EXPLODE_FRAME_WIDTH;
                frame_clip[41].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[42].x = 2*EXPLODE_FRAME_WIDTH;
                frame_clip[42].y = 5*EXPLODE_FRAME_HEIGHT;
                frame_clip[42].w = EXPLODE_FRAME_WIDTH;
                frame_clip[42].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[43].x = 3 * EXPLODE_FRAME_WIDTH;
                frame_clip[43].y = 5*EXPLODE_FRAME_HEIGHT;
                frame_clip[43].w = EXPLODE_FRAME_WIDTH;
                frame_clip[43].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[44].x = 4*EXPLODE_FRAME_WIDTH;
                frame_clip[44].y = 5*EXPLODE_FRAME_HEIGHT;
                frame_clip[44].w = EXPLODE_FRAME_WIDTH;
                frame_clip[44].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[45].x = 5 * EXPLODE_FRAME_WIDTH;
                frame_clip[45].y = 5*EXPLODE_FRAME_HEIGHT;
                frame_clip[45].w = EXPLODE_FRAME_WIDTH;
                frame_clip[45].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[46].x = 6 * EXPLODE_FRAME_WIDTH;
                frame_clip[46].y = 5*EXPLODE_FRAME_HEIGHT;
                frame_clip[46].w = EXPLODE_FRAME_WIDTH;
                frame_clip[46].h = EXPLODE_FRAME_HEIGHT;

                frame_clip[47].x = 7 * EXPLODE_FRAME_WIDTH;
                frame_clip[47].y = 5*EXPLODE_FRAME_HEIGHT;
                frame_clip[47].w = EXPLODE_FRAME_WIDTH;
                frame_clip[47].h = EXPLODE_FRAME_HEIGHT;
    }
}

*/
