
#include "pandemy2.hpp"
#include "graphics2.hpp"
#include <iostream>
#include <random>
#include <fstream>
#include <stdexcept>
#include <SFML/Graphics.hpp>

bool control(double min, double max, double val)
{
    return val <= max && val >= min;
}
bool control(int min, int max, int val)
{
    return val <= max && val >= min;
}
int main()
{
    try
    {
        int nDays;
        double beta, gamma, deathRate, lockdownLimit;

        int daysToDeath, nVaccinated;
        //Input da file
        std::ifstream file;
        file.open("../input.dat");
        if (!file.is_open())
            std::cerr << "Errore nell'apertura del file";
        std::string bin;
        sf::Image mondo;
        //Caricamento immagine mondo
        if (!mondo.loadFromFile("../Mondo_grande_porti.psd"))
            std::cerr << "Errore caricamento immagine";
        sf::Vector2u dim{mondo.getSize()};

        file >> bin >> nDays >> bin >> beta >> bin >> gamma >> bin >> deathRate >> bin >> lockdownLimit >> bin >> daysToDeath >> bin >> nVaccinated;
        file.close();
        if (!control(0., 1., beta))
        {
            std::cerr << "Beta e' fuori dal range";
            return 1;
        }
        if (!control(0., 1., gamma))
        {
            std::cerr << "Gamma e' fuori dal range";
            return 1;
        }
        if (!control(0, 10000, nDays))
        {
            std::cerr << "Il numero dei giorni della simulazione e' fuori dal range";
            return 1;
        }
        if (!control(0., 1., deathRate))
        {
            std::cerr << "Il tasso di letalita' e' fuori dal range";
            return 1;
        }
        if (!control(0., 1., lockdownLimit))
        {
            std::cerr << "La soglia di attivazione del lockdown e' fuori dal range";
            return 1;
        }
        if (!control(0, 10000, daysToDeath))
        {
            std::cerr << "Il tempo di risoluzione e' fuori dal range";
            return 1;
        }
        if (!control(0, 60000, nVaccinated))
        {
            std::cerr << "Il numero di vaccinabili e' fuori dal range";
            return 1;
        }
        short width, height;
        std::cout << "Inserire dimensioni finestra (consigliate maggiori di 1660x870)";
        std::cin >> width >> height;
        
        if (!control(0, 32767, width))
        {
            std::cerr << "La larghezza della finestra e' fuori dal range";
            return 1;
        }
        if (!control(0, 32767, height))
        {
            std::cerr << "L'altezza della finestra e' fuori dal range";
            return 1;
        }
        World world(dim.y, dim.x, beta, gamma, deathRate, lockdownLimit, daysToDeath, nVaccinated);
        Window(nDays, world, mondo, width, height);
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
