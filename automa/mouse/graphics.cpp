#include <iostream>
#include "pandemy.hpp"
#include "graphics.hpp"

void Graphics::loadImage(World &pan)
{

    for (unsigned short row = 0; row < imm_height; ++row)
    {
        for (unsigned short col = 0; col < imm_width; ++col)
        {
            if (image.getPixel(col, row).r == 255 && image.getPixel(col, row).g < 230)
                pan.setCondition(row, col) = Condition::Port;

            else if (image.getPixel(col, row).g > 250 && image.getPixel(col, row).r < 230)
                pan.setCondition(row, col) = Condition::Airport;

            else if (image.getPixel(col, row).b < 20 && image.getPixel(col, row).r > 230)
                pan.setCondition(row, col) = Condition::Desert;

            else if (image.getPixel(col, row).r < 100)
                pan.setCondition(row, col) = Condition::Susceptible;

            else
                pan.setCondition(row, col) = Condition::Wall;
        }
    }
}
void Graphics::createArray(const World &pan)
{
    unsigned short r{};
    float lato = 1.f, spaziatura = 2.f;
    unsigned short imm_height = image.getSize().y, imm_width = image.getSize().x;
    //std::cerr << "imm_height: " << imm_height << "\timm_width: " << imm_width << "\n\n";
    for (unsigned short row = 0; row < imm_height; ++row)
    {
        unsigned short c = 0;
        for (unsigned short col = 0; col < imm_width; ++col)
        {
            c += 4;
            int index{r * imm_width * 4 + c};
            mappa[index].position = sf::Vector2f(col * 2 * lato + pan_x, row * 2 * lato + pan_y);
            mappa[index + 1].position = sf::Vector2f(col * 2 * lato + pan_x, row * 2 * lato + pan_y + spaziatura);
            mappa[index + 2].position = sf::Vector2f(col * 2 * lato + pan_x + spaziatura, row * 2 * lato + pan_y + spaziatura);
            mappa[index + 3].position = sf::Vector2f(col * 2 * lato + pan_x + spaziatura, row * 2 * lato + pan_y);

            mappa[index].color = sf::Color::Black;
            mappa[index + 1].color = sf::Color::Black;
            mappa[index + 2].color = sf::Color::Black;
            mappa[index + 3].color = sf::Color::Black;

            switch (pan.getCondition(row, col))
            {
            case Condition::Susceptible:
            {
                mappa[index].color = sf::Color::Blue;
                mappa[index + 1].color = sf::Color::Blue;
                mappa[index + 2].color = sf::Color::Blue;
                mappa[index + 3].color = sf::Color::Blue;
                break;
            }
            case Condition::Infected:
            {
                mappa[index].color = sf::Color::Red;
                mappa[index + 1].color = sf::Color::Red;
                mappa[index + 2].color = sf::Color::Red;
                mappa[index + 3].color = sf::Color::Red;
                break;
            }
            case Condition::Healed:
            {
                mappa[index].color = sf::Color::Green;
                mappa[index + 1].color = sf::Color::Green;
                mappa[index + 2].color = sf::Color::Green;
                mappa[index + 3].color = sf::Color::Green;
                break;
            }
            case Condition::Dead:
            {
                mappa[index].color = sf::Color{220, 200, 0, 255};
                mappa[index + 1].color = sf::Color{220, 200, 0, 255};
                mappa[index + 2].color = sf::Color{220, 200, 0, 255};
                mappa[index + 3].color = sf::Color{220, 200, 0, 255};
                break;
            }
            case Condition::Port:
            {
                mappa[index].color = sf::Color{185, 185, 185, 255};
                mappa[index + 1].color = sf::Color{185, 185, 185, 255};
                mappa[index + 2].color = sf::Color{185, 185, 185, 255};
                mappa[index + 3].color = sf::Color{185, 185, 185, 255};
                break;
            }
            case Condition::Airport:
            {
                mappa[index].color = sf::Color::White;
                mappa[index + 1].color = sf::Color::White;
                mappa[index + 2].color = sf::Color::White;
                mappa[index + 3].color = sf::Color::White;
                break;
            }
            case Condition::Desert:
            {
                mappa[index].color = sf::Color{153, 153, 153, 255};
                mappa[index + 1].color = sf::Color{153, 153, 153, 255};
                mappa[index + 2].color = sf::Color{153, 153, 153, 255};
                mappa[index + 3].color = sf::Color{153, 153, 153, 255};
                break;
            }
            default:
            {
                break;
            }
            }
        }
        ++r;
    }
}
void Graphics::drawArray()
{
    window.clear(sf::Color::Black);
    window.draw(mappa);
}
void Graphics::WriteText(const std::string &string, short pos_x, short pos_y)
{
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        std::cerr << "\n\nError loading font\n\n";
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    text.setString(string);
    float text_w = text.getGlobalBounds().width;
    float text_h = text.getGlobalBounds().height;
    text.setPosition(pos_x - text_w / 2, (pos_y - text_h));
    window.draw(text);
}

void Window(int T, World &pan, sf::Image image, short width, short height)
{
    sf::RenderWindow window(sf::VideoMode(width, height), "Simulazione pandemia globale");
    sf::RectangleShape background(sf::Vector2f(width, height));
    background.setFillColor(sf::Color::Black);
    window.draw(background);

    Graphics graph(window, image, image.getSize().x, image.getSize().y);
    sf::Vector2u dim = image.getSize();
    short imm_height = dim.x, imm_width = dim.y;
    const float pan_x = 0.025 * imm_width, pan_y = 0.025 * imm_height;
    //const float width_p = .95 * width / imm_height, height_p = .95 * height / imm_width;
    /*float pixel_ratio = width_p;
  if (width_p > height_p)
    pixel_ratio = height_p;*/
    graph.loadImage(pan);
    graph.createArray(pan);
    graph.drawArray();
    window.display();
    bool flagIn = true;
    while (flagIn)
    {
        sf::Event event;
        //bool infected = true;
        while (1)
        {
            window.pollEvent(event);
            if (event.type == sf::Event::KeyPressed)
            {
                flagIn = false;
                break;
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                //if (event.mouseButton.button == sf::Mouse::Button::Right)
                //infected = false;
                break;
            }
        }
        if (flagIn)
        {
            sf::Vector2f pos{sf::Mouse::getPosition(window)};
            int pos_x = std::round(pos.x / 2.);
            int pos_y = std::round(pos.y / 2.);
            std::cerr << "x: " << pos.x << "    col: " << pos_x << "\ty: " << pos.y << "   row:" << pos_y << "\n\n";
            Condition state = pan.getCondition(pos_y, pos_x);
            if (state != Condition::Susceptible && state != Condition::Infected && state != Condition::Empty)
                continue;
            char state_i = static_cast<char>(state);
            state = static_cast<Condition>((state_i + 1) % 3);
            pan.setCondition(pos_y, pos_x) = state;

            graph.createArray(pan);
            graph.drawArray();
            graph.WriteText("Press any key to start the simulation", width / 2, height - pan_y);
            window.display();
            //std::this_thread::sleep_for(std::chrono::milliseconds(250));
        }
    }
    Condition state;
    bool lockdown = false;
    int deaths = 0;
    for (int i = 1; i <= T; ++i) //Evoluzione
    {
        graph.createArray(pan);
        graph.drawArray();

        //Conteggi
        int infected = 0, healed = 0, empty = 0, suscep = 0;
        for (int r = 0; r < pan.getNRighe(); r++)
        {
            for (int c = 0; c < pan.getNCol(); ++c)
            {
                state = pan.getCondition(r, c);
                if (state == Condition::Infected)
                    ++infected;
                else if (state == Condition::Healed)
                    ++healed;
                else if (state == Condition::Empty)
                    ++empty;
                else if (state == Condition::Susceptible)
                    ++suscep;
                else if (state == Condition::Dead)
                    ++deaths;
            }
        }
        std::string dati{"Day: "};
        dati = dati + std::to_string(i) + "   Susceptibles: " + std::to_string(suscep) + "   Infected: " + std::to_string(infected) + "   Healed: " + std::to_string(healed) + "   Dead: " + std::to_string(deaths);
        graph.WriteText(dati, width / 2, height - pan_y);

        //Vaccinazioni
        std::default_random_engine eng{std::random_device{}()};
        std::uniform_int_distribution<int> dist_righe{0, pan.getNRighe() - 1}, dist_col{0, pan.getNCol() - 1};
        int vaccinatiPerGiorno = std::round(pan.getNVaccinati() / 10.);
        int totVaccinati = 0;
        for (int j = 0; j != vaccinatiPerGiorno && i >= T / 5 && i < ((T / 5) + 10); ++j)
        {
            auto r = dist_righe(eng);
            auto c = dist_col(eng);
            if (totVaccinati != suscep)
            {
                while (pan.getCondition(r, c) != Condition::Susceptible)
                {
                    r = dist_righe(eng);
                    c = dist_col(eng);
                }
                pan.setCondition(r, c) = Condition::Healed;
                ++totVaccinati;
            }
            else
                break;
        }

        //Lockdown
        double percentageInfected = static_cast<double>(infected) / suscep;
        if (lockdown == false && percentageInfected >= pan.getLockdownLimit())
        {
            std::cout << "Inizio lockdown!";
            pan.setLockdown(true);
            lockdown = true;
        }
        else if (lockdown == true && percentageInfected <= (pan.getLockdownLimit()) * 0.9)
        {
            std::cout << "Fine lockdown!";
            pan.setLockdown(false);
            lockdown = false;
        }
        std::cerr << "\nDays: " << i << "\tInfected: " << infected << "\tHealed: " << healed << "\tEmpty: " << empty
                  << "Percentuale infetti: " << (percentageInfected * 100) << "% \n\n";

        window.display();
        pan = evolve(pan, i);
    }
    sf::Event chiusura;
    while (window.waitEvent(chiusura))
        if (chiusura.type == sf::Event::Closed)
            window.close();
}
