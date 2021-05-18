#ifndef ALGO_HPP
#define ALGO_HPP

#include <vector>
#include <cassert>
#include <SFML/Graphics.hpp>
#include <iostream>

struct Population
{
    double S{}, I{}, R{};
};

class Pandemy
{
    int N{};
    int T;
    double gamma = 0., beta = 0.;
    Population pop;

public:
    Pandemy(int N_in, int I, int T_in, double beta_in, double gamma_in) : N{N_in}, T{T_in}, gamma{gamma_in}, beta{beta_in},
                                                                                                     pop{static_cast<double>(N_in - I), static_cast<double>(I), 0} {}

    bool invariant()
    {
        return pop.S + pop.I + pop.R == N && pop.S >= 0 && pop.I >= 0 && pop.R >= 0;
    }

    Population GetState() const
    {
        return pop;
    }

    void evolve()
    {
        //assert(invariant());
        auto const coeff = beta * (pop.S / N);
        auto const I_old = pop.I;
        pop.R = pop.R + gamma * I_old;
        pop.I = I_old + coeff * I_old - gamma * I_old;
        pop.S = pop.S - coeff * I_old;
        ++T;
        //assert(invariant());
    }
    //result.push_back(pop);
};
void WriteText(sf::RenderWindow &window, const std::string &string, short pos_x, short pos_y)
{
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        std::cerr << "\n\nError loading font\n\n";
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(15);
    text.setFillColor(sf::Color::Black);
    text.setString(string);
    float text_w = text.getGlobalBounds().width;
    text.setPosition(pos_x - text_w, pos_y);
    window.draw(text);
}
/*class Graph{
    private:
    short width, height;
    
    public:
    void drawAxis()
    {
        sf::RectangleShape line(sf::Vector2f(width * 0.8, 3));
        line.setFillColor(sf::Color::Black);
        line.setPosition(pan_x, pan_y);
        window.draw(line);
        line.scale((float)height / width, 1);
        line.rotate(270);
        window.draw(line);
    }
    void WriteText(sf::RenderWindow& window, const std::string& string, short pos_x, short pos_y)
    {
        sf::Font font;
        if (!font.loadFromFile("arial.ttf"))
            std::cerr << "\n\nError loading font\n\n";
        sf::Text text;
        text.setFont(font);
        text.setCharacterSize(15);
        text.setFillColor(sf::Color::Black);
        text.setString(string);
        float text_w = text.getGlobalBounds().width;
        text.setPosition(pos_x - text_w, pos_y);
        window.draw(text);
    }
};*/
void Window(int T, Pandemy &pan, int N, short width = 800, short height = 600)
{
    sf::RenderWindow window(sf::VideoMode(width, height), "Andamento S, I e R");
    //sf::RectangleShape point(sf::Vector2f(3.f, 3.f));
    sf::CircleShape point(3.f);
    sf::RectangleShape background(sf::Vector2f(width, height));
    background.setFillColor(sf::Color::White);
    window.draw(background);
    sf::RectangleShape line(sf::Vector2f(width * 0.8, 3));
    line.setFillColor(sf::Color::Black);
    const float pan_x = 0.1 * width, pan_y = 0.9 * height;
    line.setPosition(pan_x, pan_y);
    window.draw(line);
    line.scale((float)height / width, 1);
    line.rotate(270);
    window.draw(line);
    Population state;
    for (int i = 0; i < T; ++i)
    {
        pan.evolve();
        state = pan.GetState();
        float y_s = state.S / N * (height * 0.8);
        float y_i = state.I / N * (height * 0.8);
        float y_r = state.R / N * (height * 0.8);
        point.setPosition(pan_x + i * (width * .8f / T), -y_s + pan_y);
        point.setFillColor(sf::Color::Blue);
        window.draw(point);
        point.setPosition(pan_x + i * (width * .8f / T), -y_i + pan_y);
        point.setFillColor(sf::Color::Red);
        window.draw(point);
        point.setPosition(pan_x + i * (width * .8f / T), -y_r + pan_y);
        point.setFillColor(sf::Color{0,200,0,255});
        window.draw(point);
    }
    WriteText(window, std::to_string(N), pan_x - 3.f, height - pan_y); //Y max
    WriteText(window, std::to_string(T), width - pan_x, pan_y);        //X max
    WriteText(window, std::to_string(N / 2), pan_x - 3.f, height / 2); //Y max / 2
    WriteText(window, std::to_string(T / 2), width / 2, pan_y);        //Y max / 2
    window.display();
    sf::Event chiusura;
    while (window.waitEvent(chiusura))
        if (chiusura.type == sf::Event::Closed)
            window.close();
}
#endif