#ifndef GUN_H_INCLUDED
#define GUN_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

#include <tr1/memory>
#include <vector>

class LineSegment;
class Bullet;

//gun for player, creates a ray bullet when fired
//when idle gun will show direction player is aiming
class Gun {

    private:

        //line used to show direction of player's aim
        sf::VertexArray lineOfSight;

        //texture for the line of sight to give it graphics
        sf::Texture lineTexture;

        std::vector<std::tr1::shared_ptr<Bullet> > bullets;

        //maximum distance the gun can shoot in order to draw the line of sight and determine bullet's start and end points
        //that way players can't shoot accross the map
        const float MAX_DISTANCE_FIRED;

        //angle of player's rotation in degrees
        //this is the angle in standard position
        float rotation;

        //calculate the end point of the bullet once fired
        sf::Vector2f calculateEndPoint(const sf::Vector2f& beginPoint) const;

        //set the new position of the line of sight using the player's origin and rotation
        void updateLineOfSight(const sf::Vector2f& origin, const float& rotation);

        //creates a bullet using the guns current state
        void createBullet();

    public:

        Gun();

        void updateBullets(const sf::Time& delta);

        void updateRotation(const sf::Vector2f& playerPosition, const float& playerRotation);

        void draw(sf::RenderWindow& window);

        std::vector<std::tr1::shared_ptr<Bullet> > getBullets();

        //returns the angle the gun shot the bullet at because guns could have different accuracies, this can be used to determine where the gun was shot
        float fire();

        //fire the gun at the given angle, does not change angle of gun
        void fire(const float& angle);
};

#endif // GUN_H_INCLUDED
