#include "graphics1.hpp"
void Graphics::drawAxis()
{
    sf::RectangleShape line(sf::Vector2f(width * 0.8, 3));
    line.setFillColor(sf::Color::Black);
    line.setPosition(pan_x, pan_y);
    window.draw(line);
    line.scale((float)height / width, 1);
    line.rotate(270);
    window.draw(line);
}

void Graphics::WriteTextGraph(const std::string &string, short pos_x, short pos_y)
{
    sf::Font font;
    if (!font.loadFromFile("../arial.ttf"))
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
void Graphics::drawPoint(Population &state, const int i, const int duration)
{
    sf::CircleShape point(3.f);
    const int N = std::round(state.S + state.I + state.R);
    const float y_s = state.S / N * (height * 0.8);
    const float y_i = state.I / N * (height * 0.8);
    const float y_r = state.R / N * (height * 0.8);
    point.setPosition(pan_x + i * (width * .8f / duration), -y_s + pan_y);
    point.setFillColor(sf::Color::Blue);
    window.draw(point);
    point.setPosition(pan_x + i * (width * .8f / duration), -y_i + pan_y);
    point.setFillColor(sf::Color::Red);
    window.draw(point);
    point.setPosition(pan_x + i * (width * .8f / duration), -y_r + pan_y);
    point.setFillColor(sf::Color{0, 200, 0, 255});
    window.draw(point);
}
void printSIR(Population &state, const int day)
{
  const int N = std::round(state.S + state.I + state.R);
  std::cout << day << '.' << std::setw(18 - static_cast<int>(log10(day))) << std::round(state.S);
  std::cout << std::setw(19);
  std::cout << std::round(state.I);
  std::cout << std::setw(19);
  std::cout << N - std::round(state.S) - std::round(state.I) << '\n';
  std::cout << "-----------------------------------------------------------------------------------------------" << '\n';
}

void window(int T, Pandemy &pan, int N, short width, short height)
{
  sf::RenderWindow window(sf::VideoMode(width, height), "Andamento S, I e R");
  window.clear(sf::Color::White);

  const float pan_x = 0.1 * width, pan_y = 0.9 * height;
  Graphics graph{window, width, height, pan_x, pan_y};
  graph.drawAxis();
  Population state;
  for (int i = 0; i < T; ++i)
  {
    pan.evolve();
    state = pan.GetState();
    assert(std::round(state.S + state.I + state.R) == N);
    graph.drawPoint(state, i, T);
    printSIR(state, i + 1);
  }

  graph.WriteTextGraph({"People   " + std::to_string(N)}, pan_x - 3.f, height - pan_y); //Y max
  graph.WriteTextGraph({std::to_string(T) + "    Days"}, width - pan_x, pan_y);        //X max
  graph.WriteTextGraph(std::to_string(N / 2), pan_x - 3.f, height / 2); //Y max / 2
  graph.WriteTextGraph(std::to_string(T / 2), width / 2, pan_y);        //Y max / 2
  window.display();
  sf::Event chiusura;
  while (window.waitEvent(chiusura))
    if (chiusura.type == sf::Event::Closed)
      window.close();
}