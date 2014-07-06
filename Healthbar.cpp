#include "Healthbar.h"

HealthBar::HealthBar(int maxValue) :
    maxHealth(maxValue),
    currentHealth(maxHealth),
    maxHealthBar(sf::Vector2f(100, 10)),
    currentHealthBar(maxHealthBar),
    MAX_WIDTH(100)
    {
        maxHealthBar.setSize(sf::Vector2f(MAX_WIDTH, 10));
        maxHealthBar.setFillColor(sf::Color::Black);

        currentHealthBar.setFillColor(sf::Color::Green);
    }

void HealthBar::setCurrentHealth(const int& value) {

    currentHealth = value;

    if(currentHealth < 0) {

        currentHealth = 0;
    }

    if(currentHealth > maxHealth) {

        currentHealth = maxHealth;
    }

    resizeHealthBar();
}

const int HealthBar::getCurrentHealth() const {

    return currentHealth;
}

void HealthBar::reduceHealth(const int& value) {

    setCurrentHealth(getCurrentHealth() - value);
}

void HealthBar::setPosition(const sf::Vector2f& position) {

    maxHealthBar.setPosition(position);
    currentHealthBar.setPosition(position);
}

void HealthBar::draw(sf::RenderWindow& window) {

    window.draw(maxHealthBar);
    window.draw(currentHealthBar);
}

void HealthBar::resizeHealthBar() {

    float currentBarWidth = static_cast<float>(currentHealth) / static_cast<float>(maxHealth) * MAX_WIDTH;
    sf::Vector2f size(currentBarWidth, currentHealthBar.getSize().y);

    currentHealthBar.setSize(size);
}
