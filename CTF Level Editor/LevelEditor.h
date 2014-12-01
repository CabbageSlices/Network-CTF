#ifndef LEVELEDITOR_H_INCLUDED
#define LEVELEDITOR_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "LargeImage.h"
#include "Camera.h"
#include "TeamManager.h"
#include "Flag.h"
#include "Floor.h"

#include <string>
#include <vector>
#include <tr1/memory>
#include <map>

class Block;
class GunGiver;
class Portal;
class ForegroundObject;

//a class that lets you create, save, and load levels, until this is made just use the simple garbage in main to test level editing
class LevelEditor {

    private:

        //types of object being created
        const std::string BLOCK;
        const std::string PASSTHROUGH_BLOCK;
        const std::string SPAWN_ZONE;
        const std::string FLAG_SPAWN;
        const std::string FOREGROUND_OBJECT;
        const std::string GUN_GIVER;
        const std::string PORTAL_ENRANCE;
        const std::string PORTAL_EXIT;
        const std::string NOTHING;

        //images and sprites of each part of the level editor
        struct Menus {

            //main level editor menu
            sf::Texture editorMenu;

            //what is drawn when the level editor is hidden
            sf::Texture editorMenuHidden;

            //other menus
            sf::Texture objectSelectionMenu;
            sf::Texture teamSelectMenu;
            sf::Texture gunGiverMenu;
            sf::Texture levelNameBar;

            sf::Sprite editor;
            sf::Sprite editorHidden;
            sf::Sprite objectSelection;
            sf::Sprite teamSelect;
            sf::Sprite gunGiver;
            sf::Sprite levelName;

        } menus;

        //bounding boxes for all buttons that can be pressed in the menu
        struct Buttons {

            sf::FloatRect showEditor;
            sf::FloatRect hideEditor;

            sf::FloatRect changeName;
            sf::FloatRect previousFloor;
            sf::FloatRect nextFloor;
            sf::FloatRect enableMirroring;
            sf::FloatRect setBackground;
            sf::FloatRect clearBackground;
            sf::FloatRect createCollisionBoxes;
            sf::FloatRect clearCollisionBoxes;
            sf::FloatRect createObjects;
            sf::FloatRect clearObjects;
            sf::FloatRect clearLevel;
            sf::FloatRect saveLevel;
            sf::FloatRect loadLevel;

            sf::FloatRect teamSpawnArea;
            sf::FloatRect flags;
            sf::FloatRect foregroundObjects;
            sf::FloatRect portals;
            sf::FloatRect weapons;
            sf::FloatRect cancelSelectObjects;

            sf::FloatRect redTeam;
            sf::FloatRect blueTeam;
            sf::FloatRect cancelSelectTeam;

            sf::FloatRect pistolGiver;
            sf::FloatRect shotgunGiver;
            sf::FloatRect sniperGiver;

        } buttons;

        Flag redFlag;
        Flag blueFlag;

        //current floor the level editor is working on
        unsigned currentFloor;

        std::map<unsigned, std::tr1::shared_ptr<Floor> > floors;

        std::string levelName;

        //file path for all resources
        const std::string filePath;

        //spawn area for each team
        sf::RectangleShape redSpawnArea;
        sf::RectangleShape blueSpawnArea;

        sf::Font font;

        //text to represent level name
        sf::Text levelNameText;

        //certain menus and other things are only drawn and interacted with if they are open
        bool isEditorOpen;
        bool isObjectSelectOpen;
        bool isTeamSelectOpen;
        bool isWeaponSelectOpen;

        //text to indicate what floor the game is on
        sf::Text floorText;

        //message to display to player indicating what object he is currently creating
        sf::Text creationMessage;

        //message to display beside the mirror option in order to indicate mirroring is in effect
        sf::Text mirrorText;

        //whether the user is creating rectangles on screen by clicking and dragging the mouse across the screen, like in the windows desktop
        bool creatingRectangle;

        //beginning mouse click position in order to create rectangle
        sf::Vector2f rectangleStart;

        //end point of rectangle
        sf::Vector2f rectangleEnd;

        //the rectangle formed when the user clicks and drags the mouse on screen
        sf::RectangleShape dragArea;

        sf::Vector2f levelSize;

        //whether player is creating blocks
        bool creatingBlocks;

        //whether player is creating special blocks that let bullets pass through
        bool creatingPassThrough;

        //whether player is creating team spawn area
        bool creatingSpawn;

        //while creating portal you are either creating an entrance or exit, once you create an entrance the next thing to be created is an exit
        bool creatingPortal;
        bool creatingPortalEntrance;
        bool creatingPortalExit;

        //portal currently being created, if user cancles in the middle of a creation this is discarded, if he completes the portal then it is discarded
        std::tr1::shared_ptr<Portal> portal;

        //the floor where the portal originiated
        unsigned portalOriginFloor;

        //creating a foreground object
        bool creatingForeground;

        //the foreground object being created that way user can see where he is placing the objects
        std::tr1::shared_ptr<ForegroundObject> foregroundCreated;

        bool creatingGunGiver;

        std::tr1::shared_ptr<GunGiver> gunGiverCreated;

        bool settingFlag;

        //what team any team related object is going to be created for
        //that is, if you create a spawn area, it will be associated with the team the user is creating for
        unsigned team;

        //whehter objects being created will be mirrored along the halfway point of the screen
        bool isMirrored;

        //center position of the camera which can move
        sf::Vector2f cameraCenter;

        sf::Vector2f velocities;

        //camera to move around the world with in order to draw things
        Camera camera;

        void setupMenus(sf::Vector2u windowSize);
        void loadTextures();
        void setupSprites(sf::Vector2u windowSize);
        void setupButtons();

        //sets the title of the level onto the leveltext and positions it at the top of the screen in the center
        void setupTitle();

        //receives input and if the input is valid places the new input into the destination string, otherwise string is unchanged
        //returns true if changed
        bool setInput(std::string& inputDestination, sf::RenderWindow& window);

        //set the creating message to the given type
        void setCreationMessage(const std::string& objectCreated);

        void startPortalEntranceCreation();
        void startPortalExitCreation();

        //stops creating whatever object was being created before
        void stopCreation();

        void clearLevel();
        void clearBlocks();
        void clearObjects();
        void clearBackground();

        void saveLevel();
        void loadLevel(const std::string& levelName);

        //changes the floor message to indicate the current floor
        void resetFloorText();
        void resetLevelSize();

        void setupMirrorText();

        //create a mirror of the given type of object and place it in the container
        std::tr1::shared_ptr<Block> createMirror(std::tr1::shared_ptr<Block>& objToMirror);

        //find the mirrored position
        sf::Vector2f createMirror(const sf::Vector2f& position);

        //destory the first object that collides with the given point
        template<class Object>
        void destoryObject(std::vector<std::tr1::shared_ptr<Object> >& container, const sf::Vector2f& point);

        //check if a button is pressed and handle accordingly
        //only handles events related to menu buttons being pressed
        //returns true if a button was clicked, needs this in order to make objects aren't created when a menu is pressed
        bool buttonPressEvents(sf::Vector2f& mousePosition, sf::RenderWindow& window);
        void leftClickEvents(sf::Vector2f& mousePosition, sf::RenderWindow& window);
        void leftReleaseEvents(sf::Vector2f& mousePosition, sf::RenderWindow& window);
        void rightClickEvents(sf::Vector2f& mousePosition);
        void rightReleaseEvents(sf::Vector2f& mousePosition, sf::RenderWindow& window);
        void resizedEvent(sf::Event& event, sf::RenderWindow& window);
        void keyPressEvent(sf::Event& event);

        void keyboardState();
        void mouseState(sf::RenderWindow& window);

        void drawMenus(sf::RenderWindow& window);
        void drawBackgrounds(sf::RenderWindow& window);
        void drawBlocks(sf::RenderWindow& window);
        void drawPortals(sf::RenderWindow& window);
        void drawForeground(sf::RenderWindow& window);
        void drawGunGivers(sf::RenderWindow& window);
        void drawFlags(sf::RenderWindow& window);

        const float getVelocity() const;

    public:

        LevelEditor(sf::RenderWindow& window);

        void handleEvents(sf::Event& event, sf::RenderWindow& window);
        void update(const float& delta, sf::RenderWindow& window);
        void draw(sf::RenderWindow& window);

};

#endif // LEVELEDITOR_H_INCLUDED
