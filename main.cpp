
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <iostream>
#include <thread>
#include <chrono>

class Target
{
public:
    float distance;
    float angle;
    sf::CircleShape shape;

    Target(float d, float a)
    {
        distance = d;
        angle = a;
        shape.setRadius(10);
        shape.setFillColor(sf::Color::Red);
        shape.setOrigin(sf::Vector2f(10, 10));
    }

    void update_position(float center_x, float center_y)
    {
        shape.setPosition(sf::Vector2f(center_x + distance * cos(angle),
                                       center_y + distance * sin(angle)));
    }
};

class Missile
{
public:
    sf::CircleShape shape;
    float speed = 5.0f;
    float angle;
    bool launched = false;

    Missile(float x, float y, float a) : angle(a)
    {
        shape.setRadius(5);
        shape.setFillColor(sf::Color::Green);
        shape.setOrigin(sf::Vector2f(5, 5));
        shape.setPosition(sf::Vector2f(x, y));
    }

    void move()
    {
        if (launched)
            shape.move(sf::Vector2f(speed * cos(angle), speed * sin(angle)));
    }

    void launch(float x, float y, float a)
    {
        shape.setPosition(sf::Vector2f(x, y));
        angle = a;
        launched = true;
    }
};

class Radar
{
public:
    sf::CircleShape radar_shape;
    std::vector<Target> targets;

    Radar()
    {
        radar_shape.setRadius(200);
        radar_shape.setFillColor(sf::Color(0, 255, 0, 50));
        radar_shape.setOutlineColor(sf::Color::Green);
        radar_shape.setOutlineThickness(5);
        radar_shape.setOrigin(sf::Vector2f(200, 200));
    }

    void detect_targets()
    {
        targets.clear();
        for (int i = 0; i < 5; ++i) {
            float distance = rand() % 150 + 50;
            float angle = rand() % 360 * (3.14159265 / 180);
            targets.push_back(Target(distance, angle));
        }
    }

    void draw(sf::RenderWindow &window)
    {
        radar_shape.setPosition(sf::Vector2f(400, 300));
        window.draw(radar_shape);
        for (auto &target : targets) {
            target.update_position(radar_shape.getPosition().x, radar_shape.getPosition().y);
            window.draw(target.shape);
        }
    }
};

class ControlCenter
{
public:
    Radar radar;
    Missile missile;

    ControlCenter() : missile(400, 300, 0) {}

    void simulate()
    {
        radar.detect_targets();
        missile.move();
    }

    void launch_missile(float target_angle)
    {
        missile.launch(radar.radar_shape.getPosition().x, radar.radar_shape.getPosition().y, target_angle);
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(800, 600)), "Weapon System Simulation");

    ControlCenter control;
    bool simulation_running = false;

    sf::Font font;
    if (!font.openFromFile("ViperDisplay-Regular.ttf")) {
        std::cerr << "Error loading font!" << std::endl;
        return -1;
    }

    sf::Text control_text(font);
    control_text.setFont(font);
    control_text.setCharacterSize(18);
    control_text.setFillColor(sf::Color::White);

    std::string controls = "Controls:\n";
    controls += "Space - Start/Stop Simulation\n";
    controls += "L - Launch Missile\n";
    controls += "Esc - Quit";

    control_text.setString(controls);
    control_text.setPosition({600, 50});

    while (window.isOpen()) {
        while (const std::optional<sf::Event> event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
            simulation_running = !simulation_running;
            if (simulation_running)
                control.simulate();
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L)) {
            float target_angle = rand() % 360 * (3.14159265 / 180);
            control.launch_missile(target_angle);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) {
            window.close();
        }

        window.clear(sf::Color::Black);

        control.radar.draw(window);
        window.draw(control.missile.shape);

        window.draw(control_text);

        window.display();

        std::this_thread::sleep_for(std::chrono::milliseconds(33));
    }

    return 0;
}
