#ifndef HEALTHBAR_H_INCLUDED
#define HEALTHBAR_H_INCLUDED

#include "SFML/System.hpp"
#include "SFML/Graphics.hpp"

class HealthBar {

    private:

        int maxHealth;
        int currentHealth;

        sf::RectangleShape maxHealthBar;
        sf::RectangleShape currentHealthBar;

        const float MAX_WIDTH;
        const float MAX_HEIGHT;

        //resize the health bars so that the current healtbar is inside the max healthbar
        void resizeHealthBar();

    public:

        HealthBar(const float barWidth = 100, const float barHeight = 10, float outlineThickness = 0);

        bool isFull() {

            return currentHealth == maxHealth;
        }

        void setCurrentHealth(const int& value);
        const int getCurrentHealth() const;

        void reduceHealth(const int& value);
        void refillHealth();

        void setPosition(const sf::Vector2f& position);

        void draw(sf::RenderWindow& window);
};

#endif // HEALTHBAR_H_INCLUDED
