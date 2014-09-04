#include "PlayerInfoDisplay.h"
#include "PlayerBase.h"
#include "TeamManager.h"
#include "Conversion.h"

using std::vector;
using std::tr1::shared_ptr;
using std::tr1::weak_ptr;

PlayerInfoDisplay::PlayerInfoDisplay():
    font(),
    TOGGLE_DISPLAY_KEY(sf::Keyboard::Tab),
    drawDisplay(false),
    infoTexture(),
    infoSprite(),
    redTeam(),
    blueTeam()
    {
        font.loadFromFile("font.ttf");
    }

void PlayerInfoDisplay::handleEvents(sf::Event& event) {

    if(event.type == sf::Event::KeyPressed) {

        if(event.key.code == TOGGLE_DISPLAY_KEY) {

            drawDisplay = !drawDisplay;
        }
    }
}

void PlayerInfoDisplay::setDisplayCenter(const sf::Vector2f& center) {

    infoSprite.setPosition(center.x - infoSprite.getSize().x, center.y - infoSprite.getSize().y);
}

bool PlayerInfoDisplay::canDisplayInfo() {

    return drawDisplay;
}

template<class Player>
void PlayerInfoDisplay::updatePlayerList(vector<shared_ptr<Player> >& players) {

    clearTeams();

    //add each player to his respective team so that his info will be drawn later
    for(auto& player : players) {

        addPlayerInfo(player);
    }
}

template<class Player>
void PlayerInfoDisplay::addPlayerInfo(shared_ptr<Player>& player) {

    //figure out his team and add him to the correct team container
    shared_ptr<InfoToDisplay> newInfo = createInfo(player);

    if(player->getTeam() == TEAM_A_ID) {

        redTeam.push_back(newInfo);

    } else {

        blueTeam.push_back(newInfo);
    }
}

void PlayerInfoDisplay::draw(sf::RenderWindow& window) {

    window.draw(infoSprite);

    drawTeamInfo(window, redTeam);
    drawTeamInfo(window, blueTeam);
}

template<class Player>
shared_ptr<PlayerInfoDisplay::InfoToDisplay> PlayerInfoDisplay::createInfo(shared_ptr<Player>& player) {

    shared_ptr<InfoToDisplay> newInfo(new InfoToDisplay);

    newInfo->name.setFont(font);
    newInfo->kills.setFont(font);
    newInfo->deaths.setFont(font);
    newInfo->captures.setFont(font);
    newInfo->returns.setFont(font);

    newInfo->name.setString(player->getName());
    newInfo->kills.setString(toString(player->getKills() ));
    newInfo->deaths.setString(toString(player->getDeaths() ));
    newInfo->captures.setString(toString(player->getCaptures() ));
    newInfo->returns.setString(toString(player->getReturns() ));

    return newInfo;
}

void PlayerInfoDisplay::clearTeams() {

    redTeam.clear();
    blueTeam.clear();
}

void PlayerInfoDisplay::drawTeamInfo(sf::RenderWindow& window, const vector<shared_ptr<InfoToDisplay> >& teamInfo) {

    for(auto infoDisplayed : teamInfo) {

        drawInfo(window, infoDisplayed);
    }
}

void PlayerInfoDisplay::drawInfo(sF::RenderWindow& window, shared_ptr<InfoToDisplay> toDisplay) {

    window.draw(toDisplay->name);
    window.draw(toDisplay->kills);
    window.draw(toDisplay->deaths);
    window.draw(toDisplay->captures);
    window.draw(toDisplay->returns);
}
