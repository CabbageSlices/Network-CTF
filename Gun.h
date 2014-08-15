#ifndef GUN_H_INCLUDED
#define GUN_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"
#include "GunTypes.h"

#include <tr1/memory>
#include <vector>
#include <iostream>

using std::cout;
using std::endl;

class LineSegment;
class Bullet;

//gun for player, creates a ray bullet when fired
//when idle gun will show direction player is aiming
class Gun {

    protected:

        //line used to show direction of player's aim
        ///the position of the first point of the line of sight is also the position of the gun
        ///and the origin of all bullets
        sf::VertexArray lineOfSight;

        //texture for the line of sight to give it graphics
        sf::Texture lineTexture;

        //bullets that have been fired
        std::vector<std::tr1::shared_ptr<Bullet> > bullets;

        //bullets whose data needs to be sent to the clients that way server can keep track of which data it has sent and which ones it hasn't
        /// FOR SERVER ONLY
        std::vector<std::tr1::shared_ptr<Bullet> > bulletsForClients;

        //maximum distance the gun can shoot in order to draw the line of sight and determine bullet's start and end points
        //that way players can't shoot accross the map
        const float MAX_DISTANCE_FIRED;

        //angle of player's rotation in degrees
        //this is the angle in standard position
        float rotation;

        //range of values to add or subtract to the gun's rotation while firing
        //in order to change the trajectory of the bullet to have the guns firing with different accuracies
        //range is from 0 to accuracy modifier
        //all in degrees
        ///always positive
        float accuracyModifier;

        //container containing the rotation of the gun when the gun was fired
        //these rotations can be sent to the server in order to replicate the gunfire on the server
        //should be cleared when data is sent to server
        std::vector<float> queuedRotations;

        //flag to determine if the gun is being fired
        bool fired;

        //timer to keep track of how long it has been since the gun was last fired
        //time is accumulated through the delta time sent by update loop
        sf::Time timeSinceFired;

        sf::Time fireDelay;

        //current floor
        unsigned floor;

        int bulletDamage;

        //maximum size of the current magazine upon reloading
        int maxCurrentMagazine;

        //max amount of ammo gun can have in reserve to reload
        int maxTotalMagazine;

        //ammo for the gun
        //ammo left
        int currentMagazine;

        //total ammo is how many rounds of magazines the gun has
        int totalMagazine;

        bool reloading;

        sf::Clock animationTimer;

        //time to move from one frame of animation to then ext
        sf::Time frameTime;

        //current frame of animtion
        int frame;

        ///temporary counters, number of frames for reloading animation in order to reload and have psuedo animations
        const int reloadingFrameCount;

        //calculate the end point of the bullet once fired
        sf::Vector2f calculateEndPoint(const sf::Vector2f& beginPoint, const float& angle) const;

        //set the new position of the line of sight using the player's origin and rotation
        virtual void updateLineOfSight(const sf::Vector2f& origin);

        //creates a bullet using the guns current state
        void createBullet(const sf::Vector2f& bulletBegin, const sf::Vector2f& bulletEnd, const unsigned& floor);

        //get the accuracy modifier to use when firing the gun
        //different guns might use different accuracy modifier than the default one
        virtual const float getAccuracyModifier() const;

        bool canFireGun() const;

        //complete the reloading action, actually refills the ammo
        void finishReloading();

        //reset all drawing state to being a new animation
        void resetAnimation();

        //different guns might handle ammo use differetly
        virtual void useAmmo();

    public:

        Gun(const int& damage = 10, const float& maxDist = 1200.f, const sf::Time& fireDelay = sf::milliseconds(1000), const float& accuracyMod = 4);
        virtual ~Gun(){}

        //handle a player's mouselick when he tries to fire a gun
        //have this function in order to have guns that handle button presses differently
        virtual void handleButtonPress();
        virtual void handleButtonRelease();

        //determines if it is time to fire again and fires
        virtual void updateGunfire(const sf::Time& delta);

        void updateBullets(const sf::Time& delta);

        void updateRotation(const sf::Vector2f& playerPosition, const float& playerRotation);

        //attempt to reload, returns true if reloading started, that is the gun has ammo left to reload so it can begin to reload
        //does not complete reloading, that happens once the animation finishes
        bool reload();

        bool canReload() const;

        //if gun is empty it must absolutely reload
        bool mustReload() const;

        void animate();

        void drawAll(sf::RenderWindow& window);

        //draw gun's line of sight
        virtual void drawSight(sf::RenderWindow& window);

        //draw the bullets that have already checked for collision with other entities
        void drawBullets(sf::RenderWindow& window);

        std::vector<std::tr1::shared_ptr<Bullet> >& getBullets();
        std::vector<std::tr1::shared_ptr<Bullet> >& getBulletsForClients();

        const std::vector<float>& getQueuedRotations() const;

        void clearRotations();

        void setFloor(const unsigned& floor);

        void setCurrentAmmo(const int& amount);

        const int getCurrentAmmo() const;

        void setTotalAmmo(const int& amount);
        const int getTotalAmmo() const;

        void refillTotalAmmo() {

            totalMagazine = maxTotalMagazine;
            ///cout << "refillled" << endl;
        }

        virtual GunTypes getGunType() const;

        //returns the angle the gun shot the bullet at because guns could have different accuracies, this can be used to determine where the gun was shot
        float fire();

        //fire the gun at the given angle, does not change angle of gun
        void fire(const float& angle);

        //fire a gun and place the bullet at the given location
        void fire(const sf::Vector2f& bulletBegin, const sf::Vector2f& bulletEnd, const unsigned& floor);

};

#endif // GUN_H_INCLUDED
