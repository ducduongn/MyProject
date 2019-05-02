#ifndef Common_H_
#define Common_H_

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <SDL_mixer.h>

class Common {
public:
    Common(SDL_Renderer* renderer);
    virtual ~Common();

    float x, y, width, height;

    virtual void Update(float delta);
    virtual void Render(float delta);

    bool IsCollides(Common* other);
protected:
    SDL_Renderer* renderer;

};

#endif
