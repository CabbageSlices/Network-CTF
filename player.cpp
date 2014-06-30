#include "player.h"
#include "Lerp.h"
#include "math.h"

#include <iostream>

using std::vector;
using std::tr1::shared_ptr;
using std::cout;
using std::endl;

#include "Gun.h"

Player::Player():
    INVALID_INPUT_ID(-1),
    playerId(0),
    pastHitBox(sf::Vector2f(50, 50)),
    currentHitBox(pastHitBox),
    destinationHitBox(currentHitBox),
    velocities(0, 0),
    pastRotation(0),
    currentRotation(0),
    destinationRotation(0),
    gun(new Gun),
    inputBuffer(),
    inputsToSend(),
    queuedGunshotRotations(),
    currentInputId(0),
    nextNewInputId(0)
    {
        //set the origin of the hit boxes to the center because player needs to rotate around the center
        pastHitBox.setOrigin(calculateCenter(pastHitBox.getGlobalBounds() ));
        currentHitBox = pastHitBox;
        destinationHitBox = pastHitBox;
    }

void Player::handleEvent(sf::Event& event) {

    //convert all events to an input and place them in the input queue to be handled later, and create an input to send to the server
    if(event.type == sf::Event::KeyPressed) {

        if(event.key.code == sf::Keyboard::Left) {

            placeIntoQueue(createInput(PRESS_LEFT));
        }

        if(event.key.code == sf::Keyboard::Right) {

            placeIntoQueue(createInput(PRESS_RIGHT));
        }

        if(event.key.code == sf::Keyboard::Up) {

            placeIntoQueue(createInput(PRESS_UP));
        }

        if(event.key.code == sf::Keyboard::Down) {

            placeIntoQueue(createInput(PRESS_DOWN));
        }
    }

    if(event.type == sf::Event::MouseButtonPressed) {

        if(event.mouseButton.button == sf::Mouse::Left) {

            fireGun();
        }
    }

    if(event.type == sf::Event::KeyReleased) {

        if(event.key.code == sf::Keyboard::Left) {

            placeIntoQueue(createInput(RELEASE_LEFT));
        }

        if(event.key.code == sf::Keyboard::Right) {

            placeIntoQueue(createInput(RELEASE_RIGHT));
        }

        if(event.key.code == sf::Keyboard::Up) {

            placeIntoQueue(createInput(RELEASE_UP));
        }

        if(event.key.code == sf::Keyboard::Down) {

            placeIntoQueue(createInput(RELEASE_DOWN));
        }
    }

    //now get the next input in the queue and handle it
    processInput(getInputToProcess());
}

void Player::handleServerUpdate(const State& stateUpdate, const sf::Uint32& lastConfirmedInputId) {

    //if the first input in queue has an id that is greater than the last confirmed input id it means
    //that the current server update is from the past and should be ignored because the first input in queue is the first
    //input sent to the server that wasn't confirmed and any id less than that is data from the past
    if(inputBuffer.size() > 0 && inputBuffer[0].inputId > lastConfirmedInputId) {

        return;
    }

    //first remove all the old inputs, including the confirmed one because the new position of hte player given by the server
    //is the position of the player after the given input was processed so no need to process it again
    removeConfirmedInputs(lastConfirmedInputId);

    //now that the old inputs have been removed, you have to handle all of the inputs starting from the last input that was processed because the player might be at a different position
    //so the player has to reprocess the inputs to calculate its new state

    currentInputId = lastConfirmedInputId + 1;

    setInterpolationPosition(stateUpdate.position);

    //for whatever reason if the queued input with the same id as the last confirmed input doesn't get deleted so delete it here if it's still there
    if(inputBuffer.size() > 0) {

        ///cout << lastConfirmedInputId << "  " << inputBuffer[0].inputId << endl;
        inputBuffer.erase(inputBuffer.begin());
    }

}

const vector<Player::Input>& Player::getInputsToSend() const {

    return inputsToSend;
}

const vector<float>& Player::getGunshotsToSend() const {

    return queuedGunshotRotations;
}

vector<shared_ptr<Bullet> > Player::getBullets() {

    return gun->getBullets();
}

void Player::clearGunshotQueue() {

    queuedGunshotRotations.clear();
}

void Player::clearInputsToSend() {

    inputsToSend.clear();
}

int Player::getId() {

    return playerId;
}

void Player::setId(const int& id) {

    playerId = id;
}

const float Player::getRotation() const {

    return currentRotation;
}

void Player::setRotation(const float& newRotation) {

    destinationRotation = newRotation;
    currentRotation = newRotation;
    pastRotation = newRotation;

    updateHitboxRotation();
    gun->updateRotation(currentHitBox.getPosition(), currentRotation);
}

void Player::setInterpolationRotation(const float& newRotation) {

    pastRotation = destinationRotation;
    destinationRotation = newRotation;
}

void Player::fireGun(const float& angle) {

    gun->fire(angle);
}

void Player::fireGun(const sf::Vector2f& bulletBegin, const sf::Vector2f& bulletEnd) {

    gun->fire(bulletBegin, bulletEnd);
}

void Player::handleClientInput(Player::Input& clientInput) {

    //input is valid, process it
    processInput(clientInput);
}

void Player::update(const float& delta, const sf::Vector2f& screenSize) {

    pastHitBox.setPosition(destinationHitBox.getPosition());

    //set the destination hitbox to the new position so the current hitbox and interpolate towards it over time
    destinationHitBox.move(velocities.x * delta, velocities.y * delta);

    updateGun(delta);
}

void Player::updateGun(const float& delta) {

    gun->updateBullets(sf::seconds(delta));
}

void Player::updateRotation(const sf::Vector2f& mousePosition) {

    //rotation of player is determined from the current position the user see the player at
    setRotation(calculateAngle(currentHitBox.getPosition(), mousePosition));
}

void Player::forceUpdate(const float& delta, const sf::Vector2f& screenSize) {

    //calculate the new position and just set all positions to the new position
    destinationHitBox.move(velocities.x * delta, velocities.y * delta);

    setInterpolationPosition(destinationHitBox.getPosition());
}

void Player::interpolate(const float& deltaFraction) {

    //calculate how far player should in between the past and destination hit box and set his position to that
    const float& horizontalDistance = ::interpolate(pastHitBox.getPosition().x, destinationHitBox.getPosition().x, deltaFraction);
    const float& verticalDistance = ::interpolate(pastHitBox.getPosition().y, destinationHitBox.getPosition().y, deltaFraction);

    currentHitBox.setPosition(pastHitBox.getPosition().x + horizontalDistance, pastHitBox.getPosition().y + verticalDistance);

    //interpolate the player's rotation
    float rotationDelta = ::interpolate(pastRotation, destinationRotation, deltaFraction);
    currentRotation = pastRotation + rotationDelta;

    updateHitboxRotation();
    gun->updateRotation(currentHitBox.getPosition(), currentRotation);
}

void Player::draw(sf::RenderWindow& window) {

    window.draw(currentHitBox);

    gun->draw(window);
}

const float Player::getHorizontalVelocity() {

    return 200;
}

const float Player::getVerticalVelocity() {

    return 200;
}

Player::Input Player::getInputToProcess() {

    for(unsigned index = 0; index < inputBuffer.size(); index++)
    {
        if(inputBuffer[index].inputId == currentInputId)
        {
            //input found so handle the input and move onto the next one so next time another input is handled
            currentInputId++;
            return inputBuffer[index];
        }
    }

    //input doesn't exist so create a blank input with an invalid input Id
    Input invalidInput;
    invalidInput.inputId = INVALID_INPUT_ID;
    return invalidInput;
}

void Player::processInput(const Input& inputToProcess) {

    //if the given input is invalid then don't process
    if(inputToProcess.inputId == INVALID_INPUT_ID) {

        return;
    }

    Action action = inputToProcess.action;

    if(action == PRESS_LEFT) {

        velocities.x = -getHorizontalVelocity();

    }else if(action == PRESS_RIGHT) {

        velocities.x = getHorizontalVelocity();

    }else if(action == PRESS_UP) {

        velocities.y = -getVerticalVelocity();

    }else if(action == PRESS_DOWN) {

        velocities.y = getVerticalVelocity();

    }

    //if the player releases a key and he is moving in the direction of the key press, then stop moving
    //otherwise he was moving in the opposite direction and releasing left shouldn't stop him from moving right
    if(action == RELEASE_LEFT && velocities.x < 0) {

        velocities.x = 0;

    }else if(action == RELEASE_RIGHT && velocities.x > 0) {

        velocities.x = 0;

    }else if(action == RELEASE_UP && velocities.y < 0) {

        velocities.y = 0;

    }else if(action == RELEASE_DOWN && velocities.y > 0) {

        velocities.y = 0;

    }

    cout << velocities.x << "   " << velocities.y << endl;
}

void Player::placeIntoQueue(Input inputToQueue) {

    inputBuffer.push_back(inputToQueue);
    inputsToSend.push_back(inputToQueue);
}

void Player::removeConfirmedInputs(const sf::Uint32& lastConfirmedInputId) {

    //find the input with the given id and remove all inputs up to and including the given input
    for(unsigned index = 0; index < inputBuffer.size(); index++) {

        if(inputBuffer[index].inputId == lastConfirmedInputId) {

            //erase removes all objects from begin to end, but not including end
            //however the inputBuffer.begin() + index moves the iterator 1 past the range you want to remove
            //so all objects that you want to remove are removed so there is no need to move the iterator again
            inputBuffer.erase(inputBuffer.begin(), inputBuffer.begin() + index);

            //container size changed, index is now invalid
            return;
        }
    }
}

void Player::setInterpolationPosition(const sf::Vector2f& position) {

    ///pastHitBox.setPosition(position);
    ///currentHitBox.setPosition(position);
    ///destinationHitBox.setPosition(position);

    ///return;

    currentHitBox.setPosition(pastHitBox.getPosition());
    pastHitBox.setPosition(destinationHitBox.getPosition());
    destinationHitBox.setPosition(position);
}

Player::Input Player::createInput(const Action& playerAction) {

    Input input;
    input.action = playerAction;
    input.inputId = nextNewInputId;

    nextNewInputId++;

    return input;
}

void Player::updateHitboxRotation() {

    currentHitBox.setRotation(-currentRotation);
    pastHitBox.setRotation(-currentRotation);
    destinationHitBox.setRotation(-currentRotation);
}

void Player::fireGun() {

    float angle = gun->fire();

    //queue the rotation of the gun when it was fired
    queuedGunshotRotations.push_back(angle);
}

const sf::Vector2f& Player::getPosition() const {

    return destinationHitBox.getPosition();
}
