#include "Healthbar.h"

HealthBar::HealthBar(const float barWidth, const float barHeight, float outlineThickness) :
    maxHealth(100),
    currentHealth(maxHealth),
    maxHealthBar(sf::Vector2f(100, 10)),
    currentHealthBar(maxHealthBar),
    MAX_WIDTH(barWidth),
    MAX_HEIGHT(barHeight)
    {
        maxHealthBar.setSize(sf::Vector2f(MAX_WIDTH, MAX_HEIGHT));
        maxHealthBar.setFillColor(sf::Color(255, 0, 0, 175));

        currentHealthBar.setSize(sf::Vector2f(MAX_WIDTH, MAX_HEIGHT));
        currentHealthBar.setFillColor(sf::Color(0, 255, 0, 175));

        maxHealthBar.setOutlineThickness(outlineThickness);
        maxHealthBar.setOutlineColor(sf::Color::Black);
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

void HealthBar::refillHealth() {

    setCurrentHealth(maxHealth);
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
