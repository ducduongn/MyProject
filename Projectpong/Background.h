#ifndef BACKGROUND_H_
#define BACKGROUND_H_

#include "Common.h"

#define BACKGROUND_WIDTH 1442
#define BACKGROUND_HEIGHT 800

class Background : public Common {
public:
    Background(SDL_Renderer* renderer);
    ~Background();

    void Update(float delta);
    void Render(float delta);

private:
    SDL_Texture* texture;

};

#endif
