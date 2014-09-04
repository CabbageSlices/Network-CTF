#ifndef PLAYERINFODISPLAY_H_INCLUDED
#define PLAYERINFODISPLAY_H_INCLUDED

#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#include <vector>
#include <tr1/memory>

class PlayerBase;

//determines if player is trying to look at the information about all players connected
//displays each player's statistics, i.e their kills, deaths, flag captures, flag returns
class PlayerInfoDisplay {

    private:

        //list of information that is displayed for each player
        //the stats are positioned according to the image used for the information display
        struct InfoToDisplay {

            sf::Text name;
            sf::Text kills;
            sf::Text deaths;
            sf::Text captures;
            sf::Text returns;
        };

        sf::Font font;

        //the key player needs to press in order to bring up the display
        const sf::Keyboard::Key TOGGLE_DISPLAY_KEY;

        bool drawDisplay;

        sf::Texture infoTexture;
        sf::Sprite infoSprite;

        //info about the players that exist on both teams
        //a vector is used to seperate players into the two teams that way when drawing their stats you can use the index in the vector to determine the position
        //that the information should be drawn on
        std::vector<std::tr1::shared_ptr<InfoToDisplay> > redTeam;
        std::vector<std::tr1::shared_ptr<InfoToDisplay> > blueTeam;

        //the info created for the given class is not positioned so it has to be positioned
        template<class Player>
        std::tr1::shared_ptr<InfoToDisplay> createInfo(std::tr1::shared_ptr<Player>& player);

        void clearTeams();

        void updateInfoPosition();

        //draw the given container of infos
        void drawTeamInfo(sf::RenderWindow& window, const std::vector<std::tr1::shared_ptr<InfoToDisplay> >& teamInfo);

        //draw the given info object
        void drawInfo(sf::RenderWindow& window, shared_ptr<InfoToDisplay> toDisplay);

    public:

        PlayerInfoDisplay();

        void handleEvents(sf::Event& event);

        void setDisplayCenter(const sf::Vector2f& center);

        bool canDisplayInfo();

        //update the list of players on each team, erases previous list of players
        //can't convert a vector<shared_ptr<derived> > to vector<shared_ptr<base> > implicitly
        //so use a template to allow a vector for any type of players
        template<class Player>
        void updatePlayerList(std::vector<std::tr1::shared_ptr<Player> >& players);

        //add the given player to the bottom of the list of his respective team, this player's information is displayed at the bottom
        template<class Player>
        void addPlayerInfo(std::tr1::shared_ptr<Player>& player);

        //setup the positions of each of the info for the players

        void draw(sf::RenderWindow& window);
};

#endif // PLAYERINFODISPLAY_H_INCLUDED
