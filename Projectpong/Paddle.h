#ifndef PADDLE_H_
#define PADDLE_H_

#include "Common.h"

class Paddle: public Common {
public:
    Paddle(SDL_Renderer* renderer);
    ~Paddle();

    void Update(float delta);
    void Render(float delta);
    SDL_Rect frame_clip [3];
    int frame ;
private:
    SDL_Texture* texture;

    float width_frame ;
    float height_frame;




};

#endif

