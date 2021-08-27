#include "graphics2.hpp"

#include <iostream>
#include <thread>

int Graphics::getPanY() const { return panY; }

void Graphics::loadImage(World &pan) {
  for (int row = 0; row < immHeight; ++row) {
    for (int col = 0; col < immWidth; ++col) {
      if (image.getPixel(col, row).r == 255 && image.getPixel(col, row).g < 230)
        pan.condition(row, col) = Condition::Port;

      else if (image.getPixel(col, row).g > 250 &&
               image.getPixel(col, row).r < 230)
        pan.condition(row, col) = Condition::Airport;

      else if (image.getPixel(col, row).b < 20 &&
               image.getPixel(col, row).r > 230)
        pan.condition(row, col) = Condition::Desert;

      else if (image.getPixel(col, row).r < 100)
        pan.condition(row, col) = Condition::Susceptible;

      else
        pan.condition(row, col) = Condition::Wall;
    }
  }
}
SIR Graphics::createArray(World const &pan) {
  float side = 1.f, spacing = 2.f;
  int r = 0, immHeight = image.getSize().y, immWidth = image.getSize().x;
  SIR state;
  for (int row = 0; row < immHeight; ++row) {
    int c = 0;
    for (int col = 0; col < immWidth; ++col) {
      c += 4;
      int index{r * immWidth * 4 + c};
      mappa[index].position =
          sf::Vector2f(col * 2 * side + panX, row * 2 * side + panY);
      mappa[index + 1].position =
          sf::Vector2f(col * 2 * side + panX, row * 2 * side + panY + spacing);
      mappa[index + 2].position = sf::Vector2f(col * 2 * side + panX + spacing,
                                               row * 2 * side + panY + spacing);
      mappa[index + 3].position =
          sf::Vector2f(col * 2 * side + panX + spacing, row * 2 * side + panY);

      mappa[index].color = sf::Color::Black;
      mappa[index + 1].color = sf::Color::Black;
      mappa[index + 2].color = sf::Color::Black;
      mappa[index + 3].color = sf::Color::Black;

      switch (pan.condition(row, col)) {
        case Condition::Susceptible: {
          mappa[index].color = sf::Color::Blue;
          mappa[index + 1].color = sf::Color::Blue;
          mappa[index + 2].color = sf::Color::Blue;
          mappa[index + 3].color = sf::Color::Blue;
          ++state.S;
          break;
        }
        case Condition::Infected: {
          mappa[index].color = sf::Color::Red;
          mappa[index + 1].color = sf::Color::Red;
          mappa[index + 2].color = sf::Color::Red;
          mappa[index + 3].color = sf::Color::Red;
          ++state.I;
          break;
        }
        case Condition::Healed: {
          mappa[index].color = sf::Color::Green;
          mappa[index + 1].color = sf::Color::Green;
          mappa[index + 2].color = sf::Color::Green;
          mappa[index + 3].color = sf::Color::Green;
          ++state.H;
          break;
        }
        case Condition::Dead: {
          mappa[index].color = sf::Color{220, 200, 0, 255};
          mappa[index + 1].color = sf::Color{220, 200, 0, 255};
          mappa[index + 2].color = sf::Color{220, 200, 0, 255};
          mappa[index + 3].color = sf::Color{220, 200, 0, 255};
          ++state.D;
          break;
        }
        case Condition::Port: {
          mappa[index].color = sf::Color{185, 185, 185, 255};
          mappa[index + 1].color = sf::Color{185, 185, 185, 255};
          mappa[index + 2].color = sf::Color{185, 185, 185, 255};
          mappa[index + 3].color = sf::Color{185, 185, 185, 255};
          break;
        }
        case Condition::Airport: {
          mappa[index].color = sf::Color::White;
          mappa[index + 1].color = sf::Color::White;
          mappa[index + 2].color = sf::Color::White;
          mappa[index + 3].color = sf::Color::White;
          break;
        }
        case Condition::Desert: {
          mappa[index].color = sf::Color{153, 153, 153, 255};
          mappa[index + 1].color = sf::Color{153, 153, 153, 255};
          mappa[index + 2].color = sf::Color{153, 153, 153, 255};
          mappa[index + 3].color = sf::Color{153, 153, 153, 255};
          break;
        }
        default: {
          break;
        }
      }
    }
    ++r;
  }
  return state;
}
void Graphics::drawArray() {
  window.clear(sf::Color::Black);
  window.draw(mappa);
}
void Graphics::writeText(const std::string &string, int posX, int posY) {
  sf::Font font;
  if (!font.loadFromFile("../../arial.ttf"))
    std::cerr << "\n\nError loading font\n\n";
  sf::Text text;
  text.setFont(font);
  text.setCharacterSize(20);
  text.setFillColor(sf::Color::White);
  text.setString(string);
  float textW = text.getGlobalBounds().width;
  float textH = text.getGlobalBounds().height;
  text.setPosition(posX - textW / 2, (posY - textH));
  window.draw(text);
}
void Graphics::chooseMouse(World &pan) {
  sf::Event event;
  while (true) {
    while (true) {
      window.pollEvent(event);
      if (event.type == sf::Event::KeyPressed) return;
      if (event.type == sf::Event::MouseButtonPressed) break;
    }
    sf::Vector2f pos{sf::Mouse::getPosition(window)};
    int posX = std::round((pos.x - panX) / 2.);
    int posY = std::round((pos.y - panY) / 2.);
    Condition state = wToConst(pan).condition(posY, posX);
    if (state != Condition::Susceptible && state != Condition::Infected &&
        state != Condition::Empty)
      continue;
    char stateI = static_cast<char>(state);
    state = static_cast<Condition>((stateI + 1) % 3);
    pan.condition(posY, posX) = state;

    createArray(pan);
    drawArray();
    writeText("Press any key to start the simulation", width / 2,
              height - panY);
    window.display();
  }
}

void Window(int duration, World &pan, sf::Image &image, int width, int height) {
  sf::RenderWindow window(sf::VideoMode(width, height),
                          "Simulazione pandemia globale");
  window.clear(sf::Color::Black);

  Graphics graph(window, image, width, height);
  const float panY = graph.getPanY();

  graph.loadImage(pan);
  SIR state, oldState = graph.createArray(pan);
  graph.drawArray();
  window.display();
  graph.chooseMouse(pan);

  World oldPan = pan;
  bool lockdown = false, vaccinazioni = false;
  int deaths = 0, totVaccinated = 0;
  // Evoluzione
  for (int i = 1; i <= duration; ++i) {
    // Vaccinazioni
    std::default_random_engine eng{std::random_device{}()};
    std::uniform_int_distribution<int> distRows{0, oldPan.getNRows() - 1},
        distCol{0, oldPan.getNCol() - 1};
    int vaccinatedPerDay = std::round(oldPan.getNVaccinated() / 10.);
    for (int j = 0; j != vaccinatedPerDay && i >= duration / 5 &&
                    i < ((duration / 5) + 10);
         ++j) {
      auto r = distRows(eng);
      auto c = distCol(eng);
      if (totVaccinated != state.S) {
        while (wToConst(pan).condition(r, c) != Condition::Susceptible) {
          r = distRows(eng);
          c = distCol(eng);
        }
        pan.condition(r, c) = Condition::Healed;
        ++totVaccinated;
        vaccinazioni = true;
      } else
        break;
    }
    if (i == (duration / 5 + 10)) vaccinazioni = false;

    // Lockdown
    double percentageInfected = static_cast<double>(oldState.I) / oldState.S;
    if (lockdown == false && percentageInfected >= oldPan.getLockdownLimit()) {
      pan.setLockdown(true);
      lockdown = true;
    } else if (lockdown == true &&
               percentageInfected <= (oldPan.getLockdownLimit()) * 0.9) {
      pan.setLockdown(false);
      lockdown = false;
    }

    std::thread second(evolve, std::ref(pan), i);
    state = graph.createArray(oldPan);
    graph.drawArray();
    deaths += state.D;
    std::string dati{"Day: "};
    dati = dati + std::to_string(i) +
           "   Susceptibles: " + std::to_string(state.S) +
           "   Infected: " + std::to_string(state.I) +
           "   Healed: " + std::to_string(state.H) +
           "   Dead: " + std::to_string(deaths);
    graph.writeText(dati, width / 2, height - panY);
    dati = "";
    if (lockdown) dati += "Lockdown in atto   ";
    if (vaccinazioni) dati += "Vaccinazioni in corso";
    graph.writeText(dati, width / 2, height - panY - 25);

    window.display();
    second.join();
    oldPan = pan;
    oldState = state;
  }
  sf::Event chiusura;
  while (window.waitEvent(chiusura))
    if (chiusura.type == sf::Event::Closed) window.close();
}
