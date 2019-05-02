#include "Common.h"

Common::Common(SDL_Renderer* renderer) {
    this->renderer = renderer;

    x = 0;
    y = 0;
    width = 1;
    height = 1;
}

Common::~Common() {
}

void Common::Update(float delta) {
}

void Common::Render(float delta) {
}

bool Common::IsCollides(Common* other) {
    if (x + width > other->x && x < other->x + other->width &&
        y + height > other->y && y < other->y + other->height) {
        return true;
    }
    return false;
}

