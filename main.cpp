#include "algo.hpp"
#include<iostream>
#include<vector>
#include<iomanip>


void print ( Pandemy& pan ) 
{    
    Population pop = pan.GetState();
    std::cout << std::setw(15) <<  pop.S;
    std::cout << std::setw(14);
    std::cout<< pop.I;
    std::cout << std::setw(14);
    std::cout << pop.R << std::setw(20) << pop.S + pop.I +pop.R <<  '\n';
    std::cout << "-----------------------------------------------------------------------------------------------" << '\n';
}

int main () 
{
  std::cout << "Inserire il numero di elementi della popolazione" << '\n';
  int N;
  std::cin >> N ;
  std::cout << "Inserire il numero di infetti iniziali" << '\n';
  int I;
  std::cin >> I;
  std::cout << "Inserire la durata in giorni della simulazione" << '\n';
  int T;
  std::cin >>  T;
  std::cout << "Inserire i parametri beta e gamma relativi alla simulazione" << '\n';
  double beta, gamma;
  std::cin >> beta >> gamma; 
  std::cout << '\n' <<  "Tabella riassuntiva" << '\n' << '\n';
  Pandemy pan{N, I, T, beta, gamma};
  Window(T, pan, N, 1280, 720);
  std::cout << "  STATUS:     " << "SUSCEPTIBLES:    " << "INFECTED:    " << "REMOVED:     " <<'\n'
          << "==========================================================================================================" << '\n'; 
 for (int a = 1; a <= T; ++a) {
     std::cout << a << ":           ";
     pan.evolve();
     print(pan);
 }
}