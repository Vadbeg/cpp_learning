#include <SFML/Graphics.hpp>
#include "include/physical_circle.hpp"


int main() {
    sf::RenderWindow window(sf::VideoMode(640, 480), "SFML window");

    PhysicalCircle physicalCircle(100, 0, 0);

    // Large green circle
    sf::CircleShape shape(100.f);  // Radius is 100
    shape.setOrigin(shape.getRadius(), shape.getRadius()); // Set origin to the center
    shape.setPosition(0, 0); // Set position to radius to align top-left to (0,0)
    shape.setFillColor(sf::Color(255,255,255,128)); 
    shape.setOutlineColor(sf::Color::Red); // Set outline color to red

    // Smaller red circle
    sf::CircleShape shape2(50.f);  // Radius is 50
    shape2.setOrigin(shape2.getRadius(), shape2.getRadius()); // Set origin to the center
    shape2.setPosition(150, 150); // Set position to radius to align top-left to (0,0)
    shape2.setFillColor(sf::Color(255,255,255,128)); 
    shape2.setOutlineColor(sf::Color::Red); // Set outline color to red

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.draw(shape2);
        window.display();
    }

    return 0;
}
