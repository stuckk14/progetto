#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <fstream>
#include "automa_mouse.hpp"
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
    int world_size, nDays;
    double beta, gamma, deathRate, lockdownLimit;
    int daysToDeath, nVaccinati;
    std::ifstream file;
    file.open("input.dat");
    if(!file.is_open())
        std::cerr << "Errore nell'apertura del file";
    std::string bin;
    file >> bin >> world_size >> bin >> nDays >> bin >> beta >> bin >> gamma >> bin >> deathRate
         >> bin >> lockdownLimit >> bin >> daysToDeath >> bin
         >> nVaccinati;
    file.close();
    if(!controllo (0., 1., beta)){
        std::cerr << "Beta e' fuori dal range";
        return -1;
    }
    if(!controllo (0., 1., gamma)){
        std::cerr << "Gamma e' fuori dal range";
        return -1;
    }
    if(!controllo (0, 10000, nDays)){
        std::cerr << "Il numero dei giorni della simulazione e' fuori dal range";
        return -1;
    }
    if(!controllo (0, 60, world_size)){
        std::cerr << "Il lato della griglia e' fuori dal range";
        return -1;
    }
    if(!controllo (0., 1., deathRate)){
        std::cerr << "Il tasso di letalita' e' fuori dal range";
        return -1;
    }
    if(!controllo (0., 1., lockdownLimit)){
        std::cerr << "La soglia di attivazione del lockdown e' fuori dal range";
        return -1;
    }
    if(!controllo (0, 10000, daysToDeath)){
        std::cerr << "Il tempo di risoluzione e' fuori dal range";
        return -1;
    }
    if(!controllo (0, world_size * world_size, nVaccinati)){
        std::cerr << "Il numero di vaccinabili e' fuori dal range";
        return -1;
    }
    World world(world_size, beta, gamma, deathRate, lockdownLimit, daysToDeath, nVaccinati);
    std::cerr<<"lockdown limit: " << world.getLockdownLimit();
    Window(nDays, world, world_size, 900, 900);
}