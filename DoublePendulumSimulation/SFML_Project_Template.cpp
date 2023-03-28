#include <SFML/Graphics.hpp>
#include "math.h"
#include "imgui-sfml/imgui.h"
#include "imgui-sfml/imgui-SFML.h"

float r1 = 200, r2 = 200;
float m1 = 20, m2 = 20;
float a1 = 90, a2 = 90;
float a1_v = 0, a2_v = 0;

float g = 1;

float x_off = 350, y_off = 200;

void draw(sf::RenderWindow& window) {
    sf::Vertex line[2];
    sf::CircleShape circle;

    // first pendulum
    float x1 = x_off + r1 * std::sin(a1);
    float y1 = y_off + r1 * std::cos(a1);

    line[0] = sf::Vertex(sf::Vector2f(x_off, y_off));
    line[1] = sf::Vertex(sf::Vector2f(x1, y1));

    circle.setRadius(m1);
    circle.setPosition(sf::Vector2f(x1 - m1, y1 - m1));
    circle.setFillColor(sf::Color(255, 255, 255));


    window.draw(line, 2, sf::Lines);
    window.draw(circle);

    // second pendulum
    float x2 = x1 + r2 * std::sin(a2);
    float y2 = y1 + r2 * std::cos(a2);

    line[0] = sf::Vertex(sf::Vector2f(x1, y1));
    line[1] = sf::Vertex(sf::Vector2f(x2, y2));

    circle.setRadius(m2);
    circle.setPosition(sf::Vector2f(x2 - m2, y2 - m2));
    circle.setFillColor(sf::Color(255, 255, 255));

    window.draw(line, 2, sf::Lines);
    window.draw(circle);

}

void update() {
    // calculate a1_a
    float num1, num2, num3, num4, den;

    num1 = -g * (2 * m1 + m2) * std::sin(a1);
    num2 = -m2 * g * std::sin(a1 - 2 * a2);
    num3 = -2 * std::sin(a1 - a2) * m2;
    num4 = a2_v * a2_v * r2 + a1_v * a1_v * r1 * std::cos(a1 - a2);
    den = r1 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));

    float a1_a = (num1 + num2 + num3 * num4) / den;

    // calculate a2_a
    num1 = 2 * std::sin(a1 - a2);
    num2 = (a1_v * a1_v * r1 * (m1 + m2));
    num3 = g * (m1 + m2) * std::cos(a1);
    num4 = a2_v * a2_v * r2 * m2 * std::cos(a1 - a2);
    den = r2 * (2 * m1 + m2 - m2 * cos(2 * a1 - 2 * a2));

    float a2_a = (num1 * (num2 + num3 + num4)) / den;

    a1_v += a1_a;
    a2_v += a2_a;
    a1 += a1_v;
    a2 += a2_v;

}

void imgui() {
    ImGui::Begin("Pendulum 1");
    ImGui::SliderFloat("Mass", &m1, 10.0f, 150.0f);
    ImGui::SliderFloat("Rod Length", &r1, 100.0f, 250.0f);
    ImGui::End();

    ImGui::Begin("Pendulum 2");
    ImGui::SliderFloat("Mass", &m2, 10.0f, 150.0f);
    ImGui::SliderFloat("Rod Length", &r2, 100.0f, 250.0f);
    ImGui::End();
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(700, 700), "SFML works!");
    window.setFramerateLimit(60);

    ImGui::SFML::Init(window);

    sf::Clock deltaClock;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);
            switch (event.type) {
            case (sf::Event::Closed):
                    window.close();
            }
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        update();

        // ui
        imgui();

        window.clear();

        draw(window);
        ImGui::SFML::Render(window);

        window.display();
    }

    ImGui::SFML::Shutdown();

    return 0;
}