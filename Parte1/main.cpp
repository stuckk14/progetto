#include <iostream>
#include <vector>
#include <stdexcept>
#include "pandemy1.hpp"
#include "graphics1.hpp"

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
        std::cout << "Inserire il numero di elementi della popolazione" << '\n';
        int N;
        std::cin >> N;
        if (!control(0, 2147483647, N))
        {
            throw std::runtime_error("Il numero di elementi della popolazione e' fuori dal range\n");
        }
        std::cout << "Inserire il numero di infetti iniziali\n";
        int I;
        std::cin >> I;
        if (!control(0, N, I))
        {
            throw std::runtime_error("Il numero di infetti iniziali e' fuori dal range\n");
        }
        std::cout << "Inserire la durata in giorni della simulazione" << '\n';
        int T;
        std::cin >> T;
        if (!control(0, 10000, T))
        {
            throw std::runtime_error("La durata della simulazione e' fuori dal range\n");
        }
        std::cout << "Inserire i parametri beta e gamma relativi alla simulazione\n";
        double beta, gamma;
        std::cin >> beta >> gamma;
        if (!control(0., 1., beta))
        {
            throw std::runtime_error("Beta e' fuori dal range\n");
        }
        if (!control(0., 1., gamma))
        {
            throw std::runtime_error("Gamma e' fuori dal range\n");
        }
        std::cout << "Inserire le dimensioni della finestra del grafico\n";
        short width, height;
        std::cin >> width >> height;

        if (!control(0, 32767, width))
        {
            throw std::runtime_error("La larghezza della finestra e' fuori dal range");
        }
        if (!control(0, 32767, height))
        {
            throw std::runtime_error("L'altezza della finestra e' fuori dal range");
        }

        Pandemy pan{N, I, T, beta, gamma};
        window(T, pan, N, width, height);
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