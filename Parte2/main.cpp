#include <SFML/Graphics.hpp>
#include <fstream>
#include <iostream>
#include <limits>
#include <random>
#include <stdexcept>

#include "graphics2.hpp"
#include "pandemy2.hpp"

template <class T>
bool control(T min, T max, T val) {
  return val <= max && val >= min;
}

int main() {
  try {
    int nDays, daysToDeath, nVaccinated;
    double beta, gamma, deathRate, lockdownLimit;
    std::ifstream file{"../input.dat"};
    if (!file.is_open()) {
      std::cerr << "Errore nell'apertura del file";
    }
    std::string bin;
    sf::Image mondo;
    // Caricamento immagine mondo
    if (!mondo.loadFromFile("../Mondo_grande_porti.psd")) {
      std::cerr << "Errore caricamento immagine";
    }
    sf::Vector2u dim{mondo.getSize()};

    file >> bin >> nDays >> bin >> beta >> bin >> gamma >> bin >> deathRate >>
        bin >> lockdownLimit >> bin >> daysToDeath >> bin >> nVaccinated;
    file.close();
    if (!control<double>(0., 1., beta)) {
      std::cerr << "Beta e' fuori dal range";
      return 1;
    }
    if (!control<double>(0., 1., gamma)) {
      std::cerr << "Gamma e' fuori dal range";
      return 1;
    }
    if (!control<int>(0, 10000, nDays)) {
      std::cerr << "Il numero dei giorni della simulazione e' fuori dal range";
      return 1;
    }
    if (!control<double>(0., 1., deathRate)) {
      std::cerr << "Il tasso di letalita' e' fuori dal range";
      return 1;
    }
    if (!control<double>(0., 1., lockdownLimit)) {
      std::cerr << "La soglia di attivazione del lockdown e' fuori dal range";
      return 1;
    }
    if (!control<int>(0, 10000, daysToDeath)) {
      std::cerr << "Il tempo di risoluzione e' fuori dal range";
      return 1;
    }
    if (!control<int>(0, 60000, nVaccinated)) {
      std::cerr << "Il numero di vaccinabili e' fuori dal range";
      return 1;
    }
    int width, height;
    std::cout
        << "Inserire dimensioni finestra (consigliate maggiori di 1660x890)";
    std::cin >> width >> height;

    if (!control<int>(0, INT16_MAX, width)) {
      std::cerr << "La larghezza della finestra e' fuori dal range";
      return 1;
    }
    if (!control<int>(0, INT16_MAX, height)) {
      std::cerr << "L'altezza della finestra e' fuori dal range";
      return 1;
    }
    World world(dim.y, dim.x, beta, gamma, deathRate, lockdownLimit,
                daysToDeath, nVaccinated);
    Window(nDays, world, mondo, width, height);
  } catch (std::runtime_error const& e) {
    std::cerr << e.what();
  } catch (...) {
    std::cerr << "\n\nUnknown exception\n";
  }
}
