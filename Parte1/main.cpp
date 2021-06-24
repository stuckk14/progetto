//#include "algo.hpp"
#include <iostream>
#include <vector>
#include <stdexcept>
#include "pandemy1.hpp"
#include "graphics1.hpp"

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
        std::cout << "Inserire il numero di elementi della popolazione" << '\n';
        int N;
        std::cin >> N;
        if (!controllo(0, 2147483647, N))
        {
            throw std::runtime_error("Il numero di elementi della popolazione e' fuori dal range\n");
        }
        std::cout << "Inserire il numero di infetti iniziali\n";
        int I;
        std::cin >> I;
        if (!controllo(0, N, I))
        {
            throw std::runtime_error("Il numero di infetti iniziali e' fuori dal range\n");
        }
        std::cout << "Inserire la durata in giorni della simulazione" << '\n';
        int T;
        std::cin >> T;
        if (!controllo(0, 10000, T))
        {
            throw std::runtime_error("La durata della simulazione e' fuori dal range\n");
        }
        std::cout << "Inserire i parametri beta e gamma relativi alla simulazione" << '\n';
        double beta, gamma;
        std::cin >> beta >> gamma;
        if (!controllo(0., 1., beta))
        {
            throw std::runtime_error("Beta e' fuori dal range\n");
        }
        if (!controllo(0., 1., gamma))
        {
            throw std::runtime_error("Gamma e' fuori dal range\n");
        }
        std::cout << '\n'
                  << "Tabella riassuntiva" << '\n'
                  << std::setw(45) << "STATUS:\n\n";
        Pandemy pan{N, I, T, beta, gamma};

        std::cout << "DAY: " << std::setw(15)
                  << std::setw(21) << "SUSCEPTIBLES:"
                  << std::setw(18) << "INFECTED:"
                  << std::setw(18) << "REMOVED:" << "\t\tTOTAL: " << N << '\n'
                  << "==========================================================================================================" << '\n';
        window(T, pan, N, 1280, 720);
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