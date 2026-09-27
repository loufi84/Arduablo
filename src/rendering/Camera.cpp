#include "rendering/Camera.h"

#include "Config.h"

void Camera::follow(Position target) {
    int16_t newX =
        static_cast<int16_t>(target.x) - VIEW_WIDTH / 2;

    int16_t newY =
        static_cast<int16_t>(target.y) - VIEW_HEIGHT / 2;

    if (newX < 0) {
        newX = 0;
    }

    if (newY < 0) {
        newY = 0;
    }

    const int16_t maxX = MAP_WIDTH - VIEW_WIDTH;
    const int16_t maxY = MAP_HEIGHT - VIEW_HEIGHT;

    if (newX > maxX) {
        newX = maxX;
    }

    if (newY > maxY) {
        newY = maxY;
    }

    x = static_cast<uint8_t>(newX);
    y = static_cast<uint8_t>(newY);
}

uint8_t Camera::getX() const {
    return x;
}

uint8_t Camera::getY() const {
    return y;
}
