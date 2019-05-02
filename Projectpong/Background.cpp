#include "Background.h"

Background :: Background(SDL_Renderer* renderer) : Common(renderer) {
    SDL_Surface* surface = IMG_Load("PNG//background.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    x = 0;
    y = 0;

}

Background :: ~Background() {
    SDL_DestroyTexture(texture);
}

void Background::Update(float delta) {

}

void Background::Render(float delta) {
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = BACKGROUND_WIDTH;
    rect.h = BACKGROUND_HEIGHT;
    SDL_RenderCopy(renderer, texture, 0, &rect);
}

