#include "UserPlayer.h"
#include "math.h"
#include "Gun.h"

#include <iostream>

using std::vector;
using std::tr1::shared_ptr;
using std::cout;
using std::endl;

UserPlayer::UserPlayer():
    PlayerBase(),
    INVALID_INPUT_ID(-1),
    playerId(0),
    velocities(0, 0),
    inputBuffer(),
    inputsToSend(),
    queuedGunshotRotations(),
    currentInputId(0),
    nextNewInputId(0)
    {

    }

void UserPlayer::handleEvent(sf::Event& event) {

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

void UserPlayer::handleServerUpdate(const State& stateUpdate, const sf::Uint32& lastConfirmedInputId) {

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

    //now load the health from the server update
    setHealth(stateUpdate.health);

    //for whatever reason the queued input with the same id as the last confirmed input doesn't get deleted so delete it here if it's still there
    if(inputBuffer.size() > 0) {

        inputBuffer.erase(inputBuffer.begin());
    }
}

const vector<UserPlayer::Input>& UserPlayer::getInputsToSend() const {

    return inputsToSend;
}

const vector<float>& UserPlayer::getGunshotsToSend() const {

    return queuedGunshotRotations;
}

void UserPlayer::clearGunshotQueue() {

    queuedGunshotRotations.clear();
}

void UserPlayer::clearInputsToSend() {

    inputsToSend.clear();
}

void UserPlayer::setRotation(const float& newRotation) {

    destinationRotation = newRotation;
    currentRotation = newRotation;
    pastRotation = newRotation;

    updateHitboxRotation();
    gun->updateRotation(currentHitBox.getPosition(), currentRotation);
}

void UserPlayer::fireGun(const float& angle) {

    gun->fire(angle);
}

void UserPlayer::handleClientInput(Input& clientInput) {

    //input is valid, process it
    processInput(clientInput);
}

void UserPlayer::update(const float& delta, const sf::Vector2f& screenSize) {

    pastHitBox.setPosition(destinationHitBox.getPosition());

    //set the destination hitbox to the new position so the current hitbox and interpolate towards it over time
    destinationHitBox.move(velocities.x * delta, velocities.y * delta);

    updateGun(delta);
}

void UserPlayer::updateRotation(const sf::Vector2f& mousePosition) {

    setRotation(calculateAngle(currentHitBox.getPosition(), mousePosition));
}

void UserPlayer::drawGun(sf::RenderWindow& window) {

    gun->drawAll(window);
}

const float UserPlayer::getHorizontalVelocity() {

    return 200;
}

const float UserPlayer::getVerticalVelocity() {

    return 200;
}

UserPlayer::Input UserPlayer::getInputToProcess() {

    //search for the current input that is being processed
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

void UserPlayer::processInput(const Input& inputToProcess) {

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
}

void UserPlayer::placeIntoQueue(Input inputToQueue) {

    inputBuffer.push_back(inputToQueue);
    inputsToSend.push_back(inputToQueue);
}

void UserPlayer::removeConfirmedInputs(const sf::Uint32& lastConfirmedInputId) {

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

UserPlayer::Input UserPlayer::createInput(const Action& playerAction) {

    Input input;
    input.action = playerAction;
    input.inputId = nextNewInputId;

    nextNewInputId++;

    return input;
}

void UserPlayer::fireGun() {

    float angle = gun->fire();

    //queue the rotation of the gun when it was fired
    queuedGunshotRotations.push_back(angle);
}