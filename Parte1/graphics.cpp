#include "graphics1.hpp"

#include <array>
void Graphics::drawAxis() {
  sf::RectangleShape line(sf::Vector2f(width * 0.8, 3));
  line.setFillColor(sf::Color::Black);
  line.setPosition(panX, panY);
  window.draw(line);
  line.scale(static_cast<float>(height) / width, 1);
  line.rotate(270);
  window.draw(line);
}

void Graphics::writeTextGraph(const std::string &string, int posX, int posY,
                              int FontDim = 15, bool XLeft = false) {
  sf::Font font;
  if (!font.loadFromFile("../../arial.ttf"))
    std::cerr << "\n\nError loading font\n\n";
  sf::Text text;
  text.setFont(font);
  text.setCharacterSize(FontDim);
  text.setFillColor(sf::Color::Black);
  text.setString(string);
  float textW = text.getGlobalBounds().width;
  if (XLeft == false) {
    text.setPosition(posX - textW, posY);
  } else {
    text.setPosition(posX, posY);
  }

  window.draw(text);
}
void Graphics::drawGraph(Population &state, const int i, const int duration) {
  sf::CircleShape point(3.f);
  const int N = std::round(state.S + state.I + state.R);
  const float yS = state.S / N * (height * 0.8);
  const float yI = state.I / N * (height * 0.8);
  const float yR = state.R / N * (height * 0.8);
  point.setPosition(panX + i * (width * .8f / duration), -yS + panY);
  point.setFillColor(sf::Color::Blue);
  window.draw(point);
  point.setPosition(panX + i * (width * .8f / duration), -yI + panY);
  point.setFillColor(sf::Color::Red);
  window.draw(point);
  point.setPosition(panX + i * (width * .8f / duration), -yR + panY);
  point.setFillColor(sf::Color{0, 200, 0, 255});
  window.draw(point);
}

void Graphics::drawLegend() {
  std::array<sf::Color, 3> colors{sf::Color::Blue, sf::Color::Red,
                                  sf::Color{0, 200, 0, 255}};
  std::array<std::string, 3> categories{"Suscettibili", "Infetti", "Rimossi"};
  const float pointY = .05f * height;
  sf::CircleShape point{4.f};
  for (int i = 0; i < 3; ++i) {
    point.setPosition(.5 * width + i * 0.15 * width, pointY);
    point.setFillColor(colors[i]);
    window.draw(point);
    writeTextGraph(categories[i], .52 * width + i * 0.15 * width,
                   pointY - .015 * height, 15, true);
  }
}

void printSIR(Population &state, const int day) {
  const int N = std::round(state.S + state.I + state.R);
  std::cout << day << '.' << std::setw(18 - static_cast<int>(log10(day)))
            << std::round(state.S);
  std::cout << std::setw(19);
  std::cout << std::round(state.I);
  std::cout << std::setw(19);
  std::cout << N - std::round(state.S) - std::round(state.I) << '\n';
  std::cout << "---------------------------------------------------------------"
               "--------------------------------"
            << '\n';
}

void window(int T, Pandemy &pan, int N, int width = 800, int height = 600) {
  std::cout << '\n'
            << "Tabella riassuntiva" << '\n'
            << std::setw(45) << "STATUS:\n\n"
            << "DAY: " << std::setw(15) << std::setw(21)
            << "SUSCEPTIBLES:" << std::setw(18) << "INFECTED:" << std::setw(18)
            << "REMOVED:"
            << "\t\tTOTAL: " << N
            << "\n============================================================="
               "=============================================\n";
  sf::RenderWindow window(sf::VideoMode(width, height), "Andamento S, I e R");
  window.clear(sf::Color::White);
  const float panX = 0.1 * width, panY = 0.9 * height;
  Graphics graph{window, width, height, panX, panY};
  graph.drawAxis();
  Population state;
  for (int i = 0; i < T; ++i) {
    pan.evolve();
    state = pan.getState();
    assert(std::round(state.S + state.I + state.R) == N);
    graph.drawGraph(state, i, T);
    printSIR(state, i + 1);
  }

  graph.writeTextGraph(std::to_string(N), panX - 3.f, height - panY);  // Y max
  graph.writeTextGraph(std::to_string(T), width - panX, panY);         // X max
  graph.writeTextGraph(std::to_string(N / 2), panX - 3.f,
                       height / 2);                              // Y max / 2
  graph.writeTextGraph(std::to_string(T / 2), width / 2, panY);  // Y max / 2
  graph.writeTextGraph("Andamento S,I,R", panX + width * 0.2, 0.04 * height,
                       22);
  graph.drawLegend();
  window.display();
  sf::Event chiusura;
  while (window.waitEvent(chiusura))
    if (chiusura.type == sf::Event::Closed) window.close();
}
