#include "Paddle.h"

Paddle::Paddle(SDL_Renderer* renderer): Common(renderer) {
    SDL_Surface* surface = IMG_Load("PNG//paddle.png");
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);

    width = 128;
    height = 32;

    width_frame = width;
    height_frame = height;

    y = 610;
}

Paddle::~Paddle() {
    // Clean resources
    SDL_DestroyTexture(texture);
}


void Paddle::Update(float delta) {

}


void Paddle::Render(float delta) {

    if ( width_frame> 0 && height_frame > 0 ) {

        frame_clip[0].x = 0;
        frame_clip[0].y = 0;
        frame_clip[0].w = width_frame;
        frame_clip[0].h = height_frame;

        frame_clip[1].x = 0;
        frame_clip[1].y = height_frame;
        frame_clip[1].w = width_frame;
        frame_clip[1].h = height_frame;

        frame_clip[2].x = 0;
        frame_clip[2].y = 2 * height_frame;
        frame_clip[2].w = width_frame;
        frame_clip[2].h = height_frame;
    }

    frame++;
    if (frame == 3) frame = 0;

    SDL_Rect drect;
    drect.x = (int)(x + 0.5f);
    drect.y = (int)(y + 0.5f);
    drect.w = width;
    drect.h = height;

    SDL_RenderCopy(renderer, texture, &frame_clip[frame], &drect);
}

