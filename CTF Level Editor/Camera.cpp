#include "Camera.h"
#include "math.h"

#include <iostream>

using std::cout;
using std::endl;

Camera::Camera():
    view(sf::FloatRect(0, 0, 1, 1)),
    defaultCameraSize(1, 1),
    currentCameraSize(defaultCameraSize),
    destinationCameraSize(currentCameraSize),
    transitionTimer(),
    ZOOM_FACTOR(0.8)
    {

    }

void Camera::applyCamera(sf::RenderWindow& window) {

    transitionZoom();

    //update the views size
    view.setSize(currentCameraSize);

    window.setView(view);
}

void Camera::applyDefaultCamera(sf::RenderWindow& window) const {

    //construct a view based on the current default view size
    window.setView(sf::View(sf::FloatRect(0, 0, defaultCameraSize.x, defaultCameraSize.y) ));
}

void Camera::setCameraCenter(const sf::Vector2f& targetCenter, const sf::FloatRect& worldBounds) {

    //calculate the new bounds of the camera given the new center
    sf::FloatRect cameraBounds(0, 0, 0, 0);
    cameraBounds.left = targetCenter.x - view.getSize().x / 2;
    cameraBounds.top = targetCenter.y - view.getSize().y / 2;
    cameraBounds.width = view.getSize().x;
    cameraBounds.height = view.getSize().y;

    //make sure camera is inside the world bounds
    if(cameraBounds.left < worldBounds.left) {

        cameraBounds.left = worldBounds.left;
    }

    if(cameraBounds.top < worldBounds.top) {

        cameraBounds.top = worldBounds.top;
    }

    if(cameraBounds.left + cameraBounds.width > worldBounds.left + worldBounds.width) {

        cameraBounds.left = worldBounds.left + worldBounds.width - cameraBounds.width;
    }

    if(cameraBounds.top + cameraBounds.height > worldBounds.top + worldBounds.height) {

        cameraBounds.top = worldBounds.top + worldBounds.height - cameraBounds.height;
    }

    //camera will be within world bounds so set the camera properties
    view.setCenter(calculateCenter(cameraBounds));
}

void Camera::setDefaultSize(sf::RenderWindow& window) {

    defaultCameraSize.x = window.getSize().x;
    defaultCameraSize.y = window.getSize().y;

    //since default size has been changed it means you should reset zoom
    resetAll();
}

void Camera::zoomIn() {

    //calculate the destination zoom level
    destinationCameraSize.x *= ZOOM_FACTOR;
    destinationCameraSize.y *= ZOOM_FACTOR;
}

void Camera::zoomOut() {

   destinationCameraSize.x /= ZOOM_FACTOR;
   destinationCameraSize.y /= ZOOM_FACTOR;
}

void Camera::resetZoom() {

    destinationCameraSize = defaultCameraSize;
}

const sf::Vector2f Camera::calculateZoomVelocity() const {

    //i want the camera to be able to zoom to its default size in 1.5 seconds so calculate the speed based on that requirement
    //the x and y velocities will be different because of different aspect ratios and different resolutions
    sf::Vector2f zoomVelocity(defaultCameraSize.x / sf::seconds(2).asSeconds(), defaultCameraSize.y / sf::seconds(2).asSeconds() );

    //determine if the current size should increase or decrease to reach destination size
    if(static_cast<int>(currentCameraSize.x) > static_cast<int>(destinationCameraSize.x)) {

        //size should decrease so velocity should become negative instead of positive
        zoomVelocity.x *= -1;
        zoomVelocity.y *= -1;
    }

    return zoomVelocity;
}

void Camera::transitionZoom() {

    //if the camera already reached the destination zoom level then there is nothing to transition
    if(static_cast<int> (currentCameraSize.x) == static_cast<int> (destinationCameraSize.x)) {

        //restart the timer so there isn't a ton of delta the next time this function is called
        transitionTimer.restart();
        return;
    }

    sf::Vector2f currentZoomVelocity = calculateZoomVelocity();

    float delta = transitionTimer.restart().asSeconds();

    currentCameraSize.x += currentZoomVelocity.x * delta;
    currentCameraSize.y += currentZoomVelocity.y * delta;

    //determine if the current size exceeded the destination size
    //first you have to determine whether the camera was shrinking or enlarging
    if((currentZoomVelocity.x < 0 && currentCameraSize.x < destinationCameraSize.x) ||
       (currentZoomVelocity.x > 0 && currentCameraSize.x > destinationCameraSize.x)) {

        //camera is too big or too small
        currentCameraSize = destinationCameraSize;
    }
}

void Camera::resetAll() {

    destinationCameraSize = defaultCameraSize;
    currentCameraSize = defaultCameraSize;

    transitionTimer.restart();
}
