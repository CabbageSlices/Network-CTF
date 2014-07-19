#include "Camera.h"
#include "math.h"

Camera::Camera():
    view(sf::FloatRect(0, 0, 1, 1)),
    defaultCameraSize(1, 1),
    zoomLevel(0),
    ZOOM_FACTOR(0.9)
    {

    }

void Camera::applyCamera(sf::RenderWindow& window) const {

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
    setZoomLevel(0);
}

void Camera::zoomIn() {

    setZoomLevel(static_cast<short>(zoomLevel + 1));
}

void Camera::zoomOut() {

   setZoomLevel(static_cast<signed short>(zoomLevel) - 1);
}

void Camera::setZoomLevel(short zoom) {

    if(zoom < 0) {

        zoom = 0;
    }

    zoomLevel = zoom;
    applyZoom();
}

void Camera::resetZoom() {

    setZoomLevel(0);
}

void Camera::applyZoom() {

    //the zoom level is just the default view size multiplied by a factor
    sf::Vector2f viewSize = defaultCameraSize;

    //multiply the view sizes by the zoomfactor ^ zoomlevel to get the current view size at this zoom level
    for(unsigned multiples = 0; multiples < zoomLevel; multiples++) {

        viewSize.x *= ZOOM_FACTOR;
        viewSize.y *= ZOOM_FACTOR;
    }

    view.setSize(viewSize);
}
