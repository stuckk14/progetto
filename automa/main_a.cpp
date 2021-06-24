#include "pandemy.hpp"
#include "graphics.hpp"
#include <iostream>
#include <random>
#include <fstream>
#include <stdexcept>
#include <SFML/Graphics.hpp>

bool controllo(double min, double max, double val)
{
    return val <= max && val >= min;
}
bool controllo(int min, int max, int val)
{
    return val <= max && val >= min;
}
int main()
{
    try
    {
        int nDays;
        double beta, gamma, deathRate, lockdownLimit;
        int daysToDeath, nVaccinati;
        std::ifstream file;
        file.open("input.dat");
        if (!file.is_open())
            std::cerr << "Errore nell'apertura del file";
        std::string bin;
        sf::Image mondo;

        if (!mondo.loadFromFile("Mondo_grande_porti.psd"))
            std::cerr << "Errore caricamento immagine";
        sf::Vector2u dim{mondo.getSize()};

        file >> bin >> nDays >> bin >> beta >> bin >> gamma >> bin >> deathRate >> bin >> lockdownLimit >> bin >> daysToDeath >> bin >> nVaccinati;
        file.close();
        if (!controllo(0., 1., beta))
        {
            std::cerr << "Beta e' fuori dal range";
            return 1;
        }
        if (!controllo(0., 1., gamma))
        {
            std::cerr << "Gamma e' fuori dal range";
            return 1;
        }
        if (!controllo(0, 10000, nDays))
        {
            std::cerr << "Il numero dei giorni della simulazione e' fuori dal range";
            return 1;
        }
        if (!controllo(0., 1., deathRate))
        {
            std::cerr << "Il tasso di letalita' e' fuori dal range";
            return 1;
        }
        if (!controllo(0., 1., lockdownLimit))
        {
            std::cerr << "La soglia di attivazione del lockdown e' fuori dal range";
            return 1;
        }
        if (!controllo(0, 10000, daysToDeath))
        {
            std::cerr << "Il tempo di risoluzione e' fuori dal range";
            return 1;
        }
        if (!controllo(0, 60000, nVaccinati))
        {
            std::cerr << "Il numero di vaccinabili e' fuori dal range";
            return 1;
        }
        World world(dim.y, dim.x, beta, gamma, deathRate, lockdownLimit, daysToDeath, nVaccinati);
        std::cerr << "lockdown limit: " << world.getLockdownLimit();
        Window(nDays, world, mondo, 1850, 1000);
    }
    catch (std::runtime_error &e)
    {
        std::cerr << e.what();
    }
    catch (...)
    {
        std::cerr << "\n\nUnknown exception\n";
    }
}
